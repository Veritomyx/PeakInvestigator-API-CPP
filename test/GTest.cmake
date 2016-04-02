# Modified using the following as a guide:
# https://raw.githubusercontent.com/kaizouman/gtest-cmake-example/master/test/CMakeLists.txt

find_package(Threads REQUIRED)

# Enable ExternalProject CMake module
include(ExternalProject)

SET(GTEST_CMAKE_ARGS -DCMAKE_INSTALL_PREFIX=${CMAKE_BINARY_DIR}/gtest)
IF(WIN32)
	SET(GTEST_CMAKE_ARGS ${GTEST_CMAKE_ARGS} -DBUILD_SHARED_LIBS=ON)
ENDIF()

# Download and install GoogleTest
ExternalProject_Add(
    gtest
    GIT_REPOSITORY https://github.com/google/googletest.git
    PREFIX ${CMAKE_CURRENT_BINARY_DIR}/gtest
    # Disable install step
    CMAKE_ARGS ${GTEST_CMAKE_ARGS}
)

# Create a libgtest target to be used as a dependency by test programs
add_library(libgtest IMPORTED STATIC GLOBAL)
add_dependencies(libgtest gtest)

# Set gtest properties
set_target_properties(libgtest PROPERTIES
    "IMPORTED_LOCATION" "${CMAKE_BINARY_DIR}/gtest/lib/${CMAKE_STATIC_LIBBRARY_PREFIX}gtest${CMAKE_STATIC_LIBRARY_SUFFIX}"
    "IMPORTED_LINK_INTERFACE_LIBRARIES" "${CMAKE_THREAD_LIBS_INIT}"
)

# Create a libgtest target to be used as a dependency by test programs
add_library(libgmock IMPORTED STATIC GLOBAL)
add_dependencies(libgmock gtest)

# Set gtest properties
set_target_properties(libgmock PROPERTIES
    "IMPORTED_LOCATION" "${CMAKE_BINARY_DIR}/gtest/lib/${CMAKE_STATIC_LIBBRARY_PREFIX}gmock${CMAKE_STATIC_LIBRARY_SUFFIX}"
    "IMPORTED_LINK_INTERFACE_LIBRARIES" "${CMAKE_THREAD_LIBS_INIT}"
)
