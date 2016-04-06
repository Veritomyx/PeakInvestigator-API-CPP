# PeakInvestigator-CPP-SDK
A C++ library for interacting with the PeakInvestigator™ public API (https://peakinvestigator.veritomyx.com/api).

## Building
CMake is used to generate files for a naive build on different supported platforms, including Visual Studio 
solutions, Unix-style Makefiles, and XCode. For a full list, see https://cmake.org/cmake/help/v3.0/manual/cmake-generators.7.html.

The general process is to create a build directory and run CMake from there. For example, the following commands
will generate Makefiles on Unix-based platforms:

```
mkdir build
cd build
cmake ..
```

See the CMake documentation for more information.

## Using the PeakInvestigatorSaaS library

### Setting up compile and an linking flags

#### CMake projects

Once the library has been built and installed, the following can be added to the ```CMakeLists.txt``` file of
another CMake project to set-up various compile and linking flags:

```
find_package(PeakInvestigatorSaaS)
target_link_libraries(MyProgram Veritomyx::PeakInvestigatorSaaS)
```

If the package is not found, passing ```-DPeakInvestigatorSaaS_DIR=path_to_cmake_files``` where 
```path_to_cmake_files``` is the directory containing the PeakInvestigator cmake files (e.g. 
```/usr/local/lib/cmake/PeakInvestigatorSaaS``` ).

#### Other projects

For projects that do not use CMake, refer to the documentation for your build environment to specify header 
and library locations.

### Example code

The general use is to create an instance of PeakInvestigatorSaaS, as well as instances of various "Actions" corresponding
to the desired API calls to PeakInvestigator (see https://peakinvestigator.veritomyx.com/api for complete list), and call
```executeAction(BaseAction*)``` on the desired action. For example, making a call to the PI_VERSIONS API can be
accomplished with the following code:

```
#include <PeakInvestigator/PeakInvestigatorSaaS.h>
#include <PeakInvestigator/Actions/PiVersionsAction.h>

...

PiVersionsAction action("username", "password");
PeakInvestigatorSaaS service("peakinvestigator.veritomyx.com");
std::string response = service.executeAction(&action);
action.processResponse(response);

std::string currentVersion = action.getCurrentVersion();

...
```

## Additional information

For more information, including additional help with building and using the PeakInvestigtorSaaS library, contact 
support@veritomyx.com.
