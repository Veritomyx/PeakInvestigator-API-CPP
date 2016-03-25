# Enable ExternalProject CMake module
INCLUDE(ExternalProject)

# Download and install libssh2
ExternalProject_Add(
    ssh2
    URL https://github.com/libssh2/libssh2/archive/libssh2-1.7.0.zip
    PREFIX ${CMAKE_CURRENT_BINARY_DIR}
    INSTALL_DIR ${CMAKE_BINARY_DIR}/libssh2
    CMAKE_ARGS -DBUILD_EXAMPLES=OFF -DBUILD_TESTING=OFF -DCMAKE_INSTALL_PREFIX=${CMAKE_BINARY_DIR}/libssh2
)

# Create a libssh2 target to be used for PeakInvestigator
ADD_LIBRARY(libssh2 IMPORTED STATIC GLOBAL)
ADD_DEPENDENCIES(libssh2 ssh2)

# Set gtest properties
ExternalProject_Get_Property(ssh2 SOURCE_DIR INSTALL_DIR)
MESSAGE("SSH: ${SOURCE_DIR} ${INSTALL_DIR}")

SET_TARGET_PROPERTIES(libssh2 PROPERTIES
    IMPORTED_LOCATION ${INSTALL_DIR}/lib/libssh2${CMAKE_STATIC_LIBRARY_SUFFIX}
    "IMPORTED_LINK_INTERFACE_LIBRARIES" "${CMAKE_THREAD_LIBS_INIT}"
#    "INTERFACE_INCLUDE_DIRECTORIES" "${source_dir}/include"
)
# I couldn't make it work with INTERFACE_INCLUDE_DIRECTORIES
INCLUDE_DIRECTORIES("${source_dir}/include")
