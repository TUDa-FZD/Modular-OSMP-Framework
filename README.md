[![pipeline status](https://gitlab.com/tuda-fzd/perception-sensor-modeling/modular-osmp-framework/badges/master/pipeline.svg)](https://gitlab.com/tuda-fzd/perception-sensor-modeling/modular-osmp-framework/-/commits/master) 
# Modular OSI Sensor Model Packaging FMU Framework with Example Strategies

<img align="right" src="https://gitlab.com/tuda-fzd/perception-sensor-modeling/object-based-generic-perception-object-model/uploads/17c84e9ec0acf0fac2e35855f038ad0b/fzdlogo.jpg" width="100" />
This is the OSMP FMU Framework provided with example strategies and profiles.
<br>
<img src="https://gitlab.com/tuda-fzd/perception-sensor-modeling/fzd-osmp-model-framework/uploads/172e7820150fb48818bce2c57ce66f85/OSMP_Model_Framework.png" width="800" />

The framework is based on adaptions of OSI Sensor Model Packaging (OSMP) to load individual code modules called strategies and get parameters from profiles into the strategies.
OSMP specifies ways in which models using the [Open Simulation Interface (OSI)](https://github.com/OpenSimulationInterface/open-simulation-interface) are to be packaged for their use in simulation environments using [FMI 2.0](https://fmi-standard.org).

The actual logic of the model is packed in so called strategies.
This is where the magic happens.
The `apply()` function of the strategy is called by the `do_calc()` function of the Framework.
There are two exemplary strategies delivered with this framework:

1. Example strategy: It simply logs some input data to show that it is running.
2. CSV output gt objects strategy: It outputs a .csv file to `CSV_PATH` set via `CMakeLists.txt` containing moving objects from the received ground truth.

## Configuration

### Model name

The model's name (in this case "FrameworkDemoModel") used for CMake-projects and the FMU at the end is defined in file `model_name.conf` located at `src/model`.

### Install path

When building and installing, the framework will build an FMU package into `FMU_INSTALL_DIR`, which can be used with a simulation tool that supports OSI and fills the required fields listed below.

### VariableNamingConvention

The parameter variableNamingConvention for the FMU specified within the modelDescription.xml is taken from file `variableNamingConvention.conf` located at `src/osmp`.
Possible values are "flat" or "structured".

### Profiles for Parameterization

The profiles are parameterized in the files `profile_*.hpp.in`.
The parameters are declared in the files `profile.hpp.in`.
The profiles can be extended by the strategies with additional parameters and values in their respective folders.

The profile to be loaded for simulation is set via a model parameter defined in the `modelDescription.xml` of the FMU.
The first name in `src/model/profiles/profile_list.conf` is taken as default.
If you would like to have a different one or if your simulation master does not support the configuration of model parameters, you have to adapt the *start* value of the parameter `profile` in `src/osmp/modelDescription.in.xml`.

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
   
   bool CFrameworkPackaging::try_load_profile(const std::string &name) {
      if (name == "NAME") {
         profile = model::profile::NAME::generate();
         return true;
      }
      /* TODO add further checks here */
   
      return false;
   }
   ```

## Build Instructions in Windows 10

### Install Dependencies in Windows 10

1. Install cmake from https://github.com/Kitware/CMake/releases/download/v3.20.3/cmake-3.20.3-windows-x86_64.msi
2. Install protobuf for [MSYS-2020](install_protobuf_Win64_MSYS-2020.md) or [Visual Studio 2017](install_protobuf_Win64_VS2017.md)

### Clone with Submodules, Build, and Install in Windows 10

1. Clone this repository <ins>with submodules</ins>:
   ```bash
   $ git clone https://gitlab.com/tuda-fzd/perception-sensor-modeling/modular-osmp-framework.git --recurse-submodules
   ```
2. Build the model in [MSYS-2020](install_model_Win64_MSYS-2020.md) or [Visual Studio 2017](install_model_Win64_VS2017.md)
3. Take FMU from `FMU_INSTALL_DIR`

    (Please note that sources are not packed into the FMU at the moment.)

## Build Instructions in Ubuntu 18.04 / 20.04

### Install Dependencies in Ubuntu 18.04 / 20.04

1. Install cmake 3.12
   * as told in [these install instructions](install_cmake_ubuntu_3-12.md)
2. Install protobuf 3.0.0:
   * Check your version via `protoc --version`. It should output: `libprotoc 3.0.0`
   * If needed, you can install it via `sudo apt-get install libprotobuf-dev protobuf-compiler`
   * or from source:
     * Download it from https://github.com/protocolbuffers/protobuf/releases/tag/v3.0.0 and extract the archive.
     * Try to run `./autogen.sh`, if it failes, download the gmock-1.7.0.zip from https://pkgs.fedoraproject.org/repo/pkgs/gmock/gmock-1.7.0.zip/073b984d8798ea1594f5e44d85b20d66/gmock-1.7.0.zip, extract it into the protobuf folder and rename the gmock-1.7.0 folter to gmock.
     * Proceed with the install with
     ```bash
     $ ./configure
     $ make -j12
     $ sudo make install
     $ sudo ldconfig # refresh shared library cache.
     ```
3. <ins>For ROS-Output-Strategies:</ins> Install ROS melodic (Ubuntu 18.04) / noetic (Ubuntu 20.04)
   * **Ubuntu 18.04: ROS melodic** <br> http://wiki.ros.org/melodic/Installation/Ubuntu (go with `desktop-full`)
   * **Ubuntu 20.04: ROS noetic** <br> http://wiki.ros.org/noetic/Installation/Ubuntu (go with `desktop-full`)

### Clone with Submodules, Build, and Install in Ubuntu 18.04 / 20.04

1. Clone this repository <ins>with submodules</ins>:
    ```bash
    $ git clone https://gitlab.com/tuda-fzd/perception-sensor-modeling/modular-osmp-framework.git --recurse-submodules
    ```
2. <ins>For ROS-Output-Strategies:</ins> Change CMakeLists.txt in every ROS-Strategy according to ROS-version
   * In `src/model/strategies/ros-.../CMakeLists.txt` change the two lines
     - **Ubuntu 18.04: ROS melodic**
       ```CMake
       list(APPEND CMAKE_PREFIX_PATH /opt/ros/melodic/)
       set(PYTHONPATH /opt/ros/melodic/lib/python2.7/dist-packages)
       ```
     - **Ubuntu 20.04: ROS noetic**
       ```CMake
       list(APPEND CMAKE_PREFIX_PATH /opt/ros/noetic/)
       set(PYTHONPATH /opt/ros/melodic/lib/python3/dist-packages)
       ```
3. Build the model by executing in the extracted project root directory:
    ```bash
    $ mkdir cmake-build
    $ cd cmake-build
    # If FMU_INSTALL_DIR is not set, CMAKE_BINARY_DIR is used
    $ cmake -DCMAKE_BUILD_TYPE=Release -DFMU_INSTALL_DIR:PATH=/tmp ..
    $ make -j N_JOBS
    ```
4. Take FMU from `FMU_INSTALL_DIR`

    (Please note that sources are not packed into the FMU at the moment.)

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

This work received funding from the research project 
"[SET Level](https://setlevel.de/)" of the [PEGASUS ](https://pegasus-family.de) project family, promoted by the German Federal Ministry for Economic Affairs and Energy based on a decision of the German Bundestag.
| SET Level                                                                                                | PEGASUS Family                                                                                                       | BMWi                                                                                                                                                                                 |
|----------------------------------------------------------------------------------------------------------|----------------------------------------------------------------------------------------------------------------------|--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------|
| <a href="https://setlevel.de"><img src="https://setlevel.de/assets/logo-setlevel.svg" width="100" /></a> | <a href="https://pegasus-family.de"><img src="https://setlevel.de/assets/logo-pegasus-family.svg" width="100" /></a> | <a href="https://www.bmwi.de/Redaktion/DE/Textsammlungen/Technologie/fahrzeug-und-systemtechnologien.html"><img src="https://setlevel.de/assets/logo-bmwi-en.svg" width="100" /></a> |

Thanks to all contributors of the following libraries:

- [Open Simulation Interface](https://github.com/OpenSimulationInterface/open-simulation-interface), a generic interface based on protocol buffers for the environmental perception of automated driving functions in virtual scenarios
- [FMI Version 2.0: FMI for Model Exchange and Co-Simulation](https://fmi-standard.org/downloads/)
- [Eigen](http://eigen.tuxfamily.org/), a C++ template library for linear algebra: matrices, vectors, numerical solvers, and related algorithms.
