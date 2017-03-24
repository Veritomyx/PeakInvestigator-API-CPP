INCLUDE(GenerateExportHeader)
INCLUDE(CheckIncludeFiles)

SET(CMAKE_INCLUDE_CURRENT_DIR_IN_INTERFACE ON)

CHECK_INCLUDE_FILES(winsock2.h HAVE_WINSOCK2_H)
CHECK_INCLUDE_FILES(sys/socket.h HAVE_SYS_SOCKET_H)
CHECK_INCLUDE_FILES(sys/types.h HAVE_SYS_TYPES_H)
CHECK_INCLUDE_FILES(netinet/in.h HAVE_NETINET_IN_H)
CHECK_INCLUDE_FILES(arpa/inet.h HAVE_ARPA_INET_H)
CHECK_INCLUDE_FILES(unistd.h HAVE_UNISTD_H)
CHECK_INCLUDE_FILES(netdb.h HAVE_NETDB_H)

CONFIGURE_FILE(
    ${CMAKE_CURRENT_SOURCE_DIR}/PeakInvestigatorSaaS_config.h.in
    ${CMAKE_CURRENT_BINARY_DIR}/PeakInvestigatorSaaS_config.h
)


SET(PEAKINVESTIGATOR_SOURCES
    PeakInvestigatorSaaS.cpp
    TarFile.cpp
    Actions/BaseAction.cpp
    Actions/PiVersionsAction.cpp
    Actions/InitAction.cpp
    Actions/SftpAction.cpp
    Actions/PrepAction.cpp
    Actions/RunAction.cpp
    Actions/StatusAction.cpp
    Actions/DeleteAction.cpp
)

SET(PEAKINVESTIGATOR_HEADERS
    PeakInvestigatorSaaS.h
    AbstractProgress.h
    TarFile.h
)

SET(PEAKINVESTIGATOR_ACTIONS
    Actions/BaseAction.h
    Actions/PiVersionsAction.h
    Actions/InitAction.h
    Actions/SftpAction.h
    Actions/PrepAction.h
    Actions/RunAction.h
    Actions/StatusAction.h
    Actions/DeleteAction.h
)

SET(CMAKE_CXX_VISIBILITY_PRESET hidden)
SET(CMAKE_VISIBILITY_INLINES_HIDDEN 1)

ADD_LIBRARY(PeakInvestigatorSaaS SHARED
                ${PEAKINVESTIGATOR_SOURCES} ${PEAKINVESTIGATOR_HEADERS} ${PEAKINVESTIGATOR_ACTIONS}
)

GENERATE_EXPORT_HEADER(PeakInvestigatorSaaS EXPORT_FILE_NAME PeakInvestigatorSaaS_export.h)
set_property(TARGET PeakInvestigatorSaaS PROPERTY VERSION ${PeakInvestigatorSaaS_VERSION})
set_property(TARGET PeakInvestigatorSaaS PROPERTY SOVERSION 0)
set_property(TARGET PeakInvestigatorSaaS PROPERTY INTERFACE_PeakInvestigatorSaaS_MAJOR_VERSION 0)
set_property(TARGET PeakInvestigatorSaaS APPEND PROPERTY COMPATIBLE_INTERFACE_STRING PeakInvestigator_MAJOR_VERSION)

INCLUDE_DIRECTORIES(
    ${CMAKE_SOURCE_DIR}/thirdparty/jsoncpp
    ${CMAKE_SOURCE_DIR}/thirdparty/tarball
    ${CMAKE_BINARY_DIR}
    ${CMAKE_CURRENT_BINARY_DIR}
    ${CMAKE_BINARY_DIR}/zlib/include
    ${CURL_INCLUDE_DIRS}
    ${LIBSSH2_INCLUDE_DIR}
)

TARGET_LINK_LIBRARIES(PeakInvestigatorSaaS PRIVATE jsoncpp tarball zlib ${CURL_LIBRARIES} ${LIBSSH2_LIBRARY})

LIST(APPEND PEAKINVESTIGATOR_HEADERS "${CMAKE_CURRENT_BINARY_DIR}/PeakInvestigatorSaaS_export.h")

INSTALL(TARGETS PeakInvestigatorSaaS
        EXPORT "PeakInvestigatorSaaSTargets"
        RUNTIME DESTINATION "${INSTALL_RUNTIME_DIR}"
        LIBRARY DESTINATION "${INSTALL_LIBRARY_DIR}"
        ARCHIVE DESTINATION "${INSTALL_ARCHIVE_DIR}"
        INCLUDES DESTINATION "${INSTALL_INCLUDE_DIR}"
)

INSTALL(FILES ${PEAKINVESTIGATOR_HEADERS} DESTINATION "${INSTALL_INCLUDE_DIR}/PeakInvestigator" COMPONENT Devel)
INSTALL(FILES ${PEAKINVESTIGATOR_ACTIONS} DESTINATION "${INSTALL_INCLUDE_DIR}/PeakInvestigator/Actions" COMPONENT Devel)

include(CMakePackageConfigHelpers)
write_basic_package_version_file(
  "${CMAKE_CURRENT_BINARY_DIR}/PeakInvestigatorSaaS/PeakInvestigatorSaaSConfigVersion.cmake"
  VERSION ${PeakInvestigatorSaaS_VERSION}
  COMPATIBILITY AnyNewerVersion
)
export(EXPORT PeakInvestigatorSaaSTargets
  FILE "${CMAKE_CURRENT_BINARY_DIR}/PeakInvestigatorSaaS/PeakInvestigatorSaaSTargets.cmake"
  NAMESPACE Veritomyx::
)
configure_file(cmake/PeakInvestigatorSaaSConfig.cmake
  "${CMAKE_CURRENT_BINARY_DIR}/PeakInvestigatorSaaS/PeakInvestigatorSaaSConfig.cmake"
  COPYONLY
)

set(ConfigPackageLocation lib/cmake/PeakInvestigatorSaaS)
install(EXPORT PeakInvestigatorSaaSTargets
  FILE
    PeakInvestigatorSaaSTargets.cmake
  NAMESPACE
    Veritomyx::
  DESTINATION
    ${ConfigPackageLocation}
)
install(
  FILES
    cmake/PeakInvestigatorSaaSConfig.cmake
    "${CMAKE_CURRENT_BINARY_DIR}/PeakInvestigatorSaaS/PeakInvestigatorSaaSConfigVersion.cmake"
  DESTINATION
    ${ConfigPackageLocation}
  COMPONENT
    Devel
)
