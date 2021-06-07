# OSI Sensor Model Packaging FMU Framework with Example Strategies

<img style="float: right;" src="https://gitlab.com/tuda-fzd/perception-sensor-modeling/object-based-generic-perception-object-model/uploads/17c84e9ec0acf0fac2e35855f038ad0b/fzdlogo.jpg" width="100" />
This is the OSMP FMU Framework provided with example strategies and profiles.

It is based on adaptions of OSI Sensor Model Packaging (OSMP) to load individual code modules called strategies and get parameters from profiles into the strategies.
OSMP specifies ways in which models (like e.g. environmental effect models, sensor models and logical models) using the [Open Simulation Interface (OSI)](https://github.com/OpenSimulationInterface/open-simulation-interface) are to be packaged for their use in simulation environments using FMI 2.0.

## Important

This work is based on the [`OSMPDummySensor`](https://github.com/OpenSimulationInterface/osi-sensor-model-packaging/tree/master/examples/OSMPDummySensor) example from the [Open Simulation Inferface Sensor Model Packaging project](https://github.com/OpenSimulationInterface/osi-sensor-model-packaging).

Before modifying files, carefully read the files `COPYING` and `CONTRIBUTING.md`.

### Usage

The actual logic of the model is packed in so called strategies.
The `apply()` function of a strategy is called by the `do_calc()` function of the OSMP Framework.

When building and installing, the framework will build an fmu package into `_FMU_INSTALL_DIR_`, which can be used with a simulation tool like CarMaker, dSpace ASM or others.

### Installation guide for Ubuntu 18.04.

#### Dependencies

##### Install `cmake` 3.12:

Install `cmake` 3.12 for example as instructed [here](https://cmake.org/install/).

##### Install `protobuf` 3.0.0:

Check your version via `protoc --version`. It should output `libprotoc 3.0.0`

If needed, you can install it via
```bash
$ sudo apt-get install libprotobuf-dev protobuf-compiler
```
or from source by downloading it from https://github.com/protocolbuffers/protobuf/releases/tag/v3.0.0 and install it.


#### Clone, build, and install

1. Clone this repository with submodules (pass `--recurse-submodules` to `git clone`), as OSI is included in this repository as a submodule.
2. Execute in the extracted project root directory:
    ```shell script
    mkdir cmake-build
    cd cmake-build
    # if FMU_INSTALL_DIR is not set, CMAKE_BINARY_DIR is used
    cmake -DCMAKE_BUILD_TYPE=Release -DFMU_INSTALL_DIR:PATH=/opt/osifmu ..
    make -j N_JOBS
    ```
3. Take FMU from `FMU_INSTALL_DIR`

    Please note that sources are not packed into the FMU at the moment.

### How to use (= extend) this framework

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

**Please read file `COPYING`, which is located in the project root, carefully.**

The work (located in `src/model`) created by Institute of Automotive Engineering of Technical University of Darmstadt is licensed under the EUPL-1.2 and can be used/merged and distributed in other works covered by GPL-2.0, GPL-3.0, LGPL, AGPL, CeCILL, OSL, EPL, MPL and other licences listed as compatible in the EUPL Appendix. This applies to the other (combined) work, while the original project stays covered by the EUPL without re-licensing. Alternatively, the work in folder src/osmp may be used under the terms of the MPL-2.0.

The larger work, including the modifications to OSMP dummy sensor, is subject to the terms of MPL-2.0 (but this does not affect the coverage of the incorporated components by their respective licenses).

## Credits

Thanks to all contributors of the following libraries:

- [Open Simulation Interface](https://github.com/OpenSimulationInterface/open-simulation-interface), a generic interface based on protocol buffers for the environmental perception of automated driving functions in virtual scenarios
- [FMI Version 2.0: FMI for Model Exchange and Co-Simulation](https://fmi-standard.org/downloads/)
