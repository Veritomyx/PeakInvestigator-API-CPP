# Modified using the following as a guide:
# https://raw.githubusercontent.com/kaizouman/gtest-cmake-example/master/test/CMakeLists.txt

# Enable ExternalProject CMake module
include(ExternalProject)

SET(GTEST_CMAKE_ARGS -DCMAKE_INSTALL_PREFIX=${CMAKE_BINARY_DIR}/zlib)

# Download and install GoogleTest
ExternalProject_Add(
    z
    URL http://zlib.net/zlib-1.2.8.tar.gz
    URL_MD5 44d667c142d7cda120332623eab69f40
    PREFIX ${CMAKE_CURRENT_BINARY_DIR}/zlib
    CMAKE_ARGS ${GTEST_CMAKE_ARGS}
)

# Create a zlib target to be used as a dependency
add_library(zlib IMPORTED STATIC GLOBAL)
add_dependencies(zlib z)

# Set zlib properties
set_target_properties(zlib PROPERTIES
    "IMPORTED_LOCATION" "${CMAKE_BINARY_DIR}/zlib/lib/${CMAKE_STATIC_LIBRARY_PREFIX}z${CMAKE_STATIC_LIBRARY_SUFFIX}"
)

