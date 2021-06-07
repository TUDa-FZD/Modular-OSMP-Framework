# OSI Sensor Model Packaging FMU Framework with Example Strategies

<img align="right" src="https://gitlab.com/tuda-fzd/perception-sensor-modeling/object-based-generic-perception-object-model/uploads/17c84e9ec0acf0fac2e35855f038ad0b/fzdlogo.jpg" width="100" />
This is the OSMP FMU Framework provided with example strategies and profiles.
<br>
<img src="https://gitlab.com/tuda-fzd/perception-sensor-modeling/fzd-osmp-model-framework/uploads/172e7820150fb48818bce2c57ce66f85/OSMP_Model_Framework.png" width="800" />

The framework is based on adaptions of OSI Sensor Model Packaging (OSMP) to load individual code modules called strategies and get parameters from profiles into the strategies.
OSMP specifies ways in which models (like e.g. environmental effect models, sensor models and logical models) using the [Open Simulation Interface (OSI)](https://github.com/OpenSimulationInterface/open-simulation-interface) are to be packaged for their use in simulation environments using FMI 2.0.

## Important

This work is based on the [`OSMPDummySensor`](https://github.com/OpenSimulationInterface/osi-sensor-model-packaging/tree/master/examples/OSMPDummySensor) example from the [Open Simulation Inferface Sensor Model Packaging project](https://github.com/OpenSimulationInterface/osi-sensor-model-packaging).

Before modifying files, carefully read the files [COPYING](COPYING) and [CONTRIBUTING.md](CONTRIBUTING.md).

### Usage

The actual logic of the model is packed in so called strategies.
The `apply()` function of a strategy is called by the `do_calc()` function of the OSMP Framework.

When building and installing, the framework will build an fmu package into `_FMU_INSTALL_DIR_`, which can be used with a simulation tool like CarMaker, dSpace ASM or others.

## Build Instructions for Ubuntu 18.04 / 20.04

When building and installing, the framework will build an FMU package, which can be used with a simulation tool like CarMaker, dSpace ASM or others.

### Install Dependencies

1. Install cmake 3.12: e.g. with
   ```bash
   $ git clone https://github.com/Kitware/CMake
   $ sudo apt install libssl-dev
   $ cd CMake
   $ ./bootstrap
   $ make -j
   $ sudo make install
   ```
2. Install protobuf 3.0.0:
   * Check your version via `protoc --version`. It should output: `libprotoc 3.0.0`
   * If needed, you can install it via `sudo apt-get install libprotobuf-dev protobuf-compiler`
   * or from source:
     * Download it from https://github.com/protocolbuffers/protobuf/releases/tag/v3.0.0 and extract the archive.
     * Try to run `./autogen.sh`, if it failes, download the gmock-1.7.0.zip from https://pkgs.fedoraproject.org/repo/pkgs/gmock/gmock-1.7.0.zip/073b984d8798ea1594f5e44d85b20d66/gmock-1.7.0.zip, extract it into the protobuf folder and rename the gmock-1.7.0 folter to gmock.
     * Proceed with the install with
     ```bash
     $ make
     $ sudo make install
     $ sudo ldconfig # refresh shared library cache.
     ```

### Clone with Submodules, Build, and Install

1. Clone this repository <ins>with submodules</ins>:
    ```bash
    $ git clone https://gitlab.com/tuda-fzd/perception-sensor-modeling/reflection-based-lidar-object-model.git --recurse-submodules
    ```
    or alternatively initialize submodules after cloning:
    ```
    $ git submodule update --init
    ```
2. Build the model by executing in the extracted project root directory:
    ```bash
    $ mkdir cmake-build
    $ cd cmake-build
    # If FMU_INSTALL_DIR is not set, CMAKE_BINARY_DIR is used
    $ cmake -DCMAKE_BUILD_TYPE=Release -DFMU_INSTALL_DIR:PATH=/opt/osifmu ..
    $ make -j N_JOBS
    ```
3. Take FMU from `FMU_INSTALL_DIR`

    (Please note that sources are not packed into the FMU at the moment.)

## How to use (= extend) this framework

#### … with a different model name

The model name is specified in the file `src/model/model_name.conf`. It is used as the FMU name after build, as well.

#### … with a new strategy

1. Create a new directory structure in `src/model/strategies` for your `model::Strategy` implementation:
   - `strategies/your_strategy/CMakeLists.txt` for registering the strategy with the platform,
   - `strategies/your_strategy/include/your_strategy` containing your headers,
   - `strategies/your_strategy/src` containing your code,
   - (optional) `strategies/your_strategy/profile_struct.hpp.in` containing the profile extensions' structure, and
   - (optional) `strategies/your_strategy/profile_NAME.hpp.in` containing the actual profile extensions of profile *NAME*
2. Implement the interface defined in `src/model/Strategy.hpp`.
3. Reference your strategy by inserting *your_strategy* into `src/model/strategies/sequence.conf`.
4. (optional) Extend a profile with variables your strategy depends on.
   For example, if you need some more flags for your awesome strategy, you have to define the structure extension `profile_struct.hpp.in` like this:
   ```c++
   struct awesome_part {
    bool be_precise;
    u_int precision;
   } awesome;
   ```
   And you have to define the actual values in `profile_NAME.hpp.in` like that:
   ```c++
   NAME.awesome.be_precise = true;
   NAME.awesome.precision = 3;
   ```
   **Remember that you have to define the values for each available profile!**

Have a look at the existing strategies to get an idea about how easy it is to insert a new strategy.

#### … with a new profile

1. Create your `profile_NAME.hpp.in` (replace *NAME* with a name of your choice) in `src/model/profiles`.
   Define a generator that sets all of the structure's fields:
   
   ```c++
   
   // Please mind the matching namespace 'NAME'!
   namespace model::profile::NAME {
   
       Profile generate() {
           Profile NAME;
           NAME.general.range = 4.0;
           NAME.general.number_of_layers = 64;
           NAME.general.irradiation_pattern = {{{1, 2}, {3, 4}, {5, 6}, {7, 8}}};
   ```
2. Make your profile available to CMake by adding it to `src/model/profiles/profile_list.conf` (e.g. "NAME" for `profile_NAME.hpp.in`). 
3. Make your profile available to your strategies by extending `src/model/profiles/profile_list.hpp`:
   ```c++
   /* TODO add further profiles and profile generators here */
   #include <model/profiles/profile_NAME.hpp>
   
   bool CFZDSensorModel::try_load_profile(const std::string &name) {
      if (name == "NAME") {
         profile = model::profile::NAME::generate();
         return true;
      }
      /* TODO add further checks here */
   
      return false;
   }
   ```

**Note:**
The profile to be loaded is set via a model parameter defined in the `modelDescription.xml` of the FMU.
It can be set by the FMU master.
If it is not set by the FMU master, the first name in `src/model/profiles/profile_list.conf` is simply taken as default.

## Licensing

**Please read file [COPYING](COPYING), which is located in the project root, carefully.**

## Credits
C. Linnhoff, P. Rosenberger, M. F. Holder, N. Cianciaruso, and H. Winner: “Highly Parameterizable and Generic Perception Sensor Model Architecture - A Modular Approach for Simulation Based Safety Validation of Automated Driving” in 6. Internationale ATZ-Fachtagung Automated Driving 2020, Wiesbaden, Germany, 2020

If you find our work useful in your research, please consider citing: 

```
@inproceedings{linnhoff_highly_2020,
   title = {Highly {Parameterizable} and {Generic} {Perception} {Sensor} {Model} {Architecture}},
   booktitle = {6. {Internationale} {ATZ}-{Fachtagung} {Automated} {Driving}},
   author = {Linnhoff, Clemens and Rosenberger, Philipp and Holder, Martin Friedrich and Cianciaruso, Nicodemo and Winner, Hermann},
   address = {Wiesbaden},
   year = {2020}
}
```

This work received funding from SET Level and VVM of the PEGASUS project family, promoted by the German Federal Ministry for Economic Affairs and Energy based on a decision of the Deutsche Bundestag.

Thanks to all contributors of the following libraries:

- [Open Simulation Interface](https://github.com/OpenSimulationInterface/open-simulation-interface), a generic interface based on protocol buffers for the environmental perception of automated driving functions in virtual scenarios
- [FMI Version 2.0: FMI for Model Exchange and Co-Simulation](https://fmi-standard.org/downloads/)
- [Eigen](http://eigen.tuxfamily.org/), a C++ template library for linear algebra: matrices, vectors, numerical solvers, and related algorithms.
