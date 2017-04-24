# Modified using the following as a guide:
# https://raw.githubusercontent.com/kaizouman/gtest-cmake-example/master/test/CMakeLists.txt

# Enable ExternalProject CMake module
include(ExternalProject)

SET(GTEST_CMAKE_ARGS -DCMAKE_INSTALL_PREFIX=${CMAKE_BINARY_DIR}/zlib)

# Download and install GoogleTest
ExternalProject_Add(
    z
    URL http://zlib.net/zlib-1.2.11.tar.gz
    URL_MD5 1c9f62f0778697a09d36121ead88e08e
    PREFIX ${CMAKE_CURRENT_BINARY_DIR}/zlib
    CMAKE_ARGS ${GTEST_CMAKE_ARGS} -DCMAKE_POSITION_INDEPENDENT_CODE=ON
)

# Create a zlib target to be used as a dependency
add_library(zlib IMPORTED STATIC GLOBAL)
add_dependencies(zlib z)

# Set zlib properties
if(MSVC)
	set_target_properties(zlib PROPERTIES
		IMPORTED_LOCATION "${CMAKE_BINARY_DIR}/zlib/lib/zlibstatic.lib"
		IMPORTED_LOCATION_DEBUG "${CMAKE_BINARY_DIR}/zlib/lib/zlibstaticd.lib"
	)
else()
	set_target_properties(zlib PROPERTIES
		"IMPORTED_LOCATION" "${CMAKE_BINARY_DIR}/zlib/lib/${CMAKE_STATIC_LIBRARY_PREFIX}z${CMAKE_STATIC_LIBRARY_SUFFIX}"
	)
endif()

