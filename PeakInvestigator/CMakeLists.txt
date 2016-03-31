INCLUDE(GenerateExportHeader)
INCLUDE(CheckIncludeFiles)

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
                ${PEAKINVESTIGATOR_SOURCES} ${PEAKINVESTIGATOR_HEADERS}
)
GENERATE_EXPORT_HEADER(PeakInvestigatorSaaS EXPORT_FILE_NAME PeakInvestigatorSaaS_export.h)
INCLUDE_DIRECTORIES(${CMAKE_SOURCE_DIR}/thirdparty/jsoncpp ${CMAKE_CURRENT_BINARY_DIR} ${CURL_INCLUDE_DIRS} ${LIBSSH2_INCLUDE_DIRS})

TARGET_LINK_LIBRARIES(PeakInvestigatorSaaS PRIVATE jsoncpp ${CURL_LIBRARIES} ${LIBSSH2_LIBRARIES})

LIST(APPEND PEAKINVESTIGATOR_HEADERS "${CMAKE_CURRENT_BINARY_DIR}/PeakInvestigatorSaaS_export.h")

INSTALL(FILES ${PEAKINVESTIGATOR_HEADERS} DESTINATION "${INSTALL_INCLUDE_DIR}/PeakInvestigator")
INSTALL(TARGETS PeakInvestigatorSaaS
        EXPORT "PeakInvestigatorSaaSTargets"
        RUNTIME DESTINATION "${INSTALL_RUNTIME_DIR}"
        LIBRARY DESTINATION "${INSTALL_LIBRARY_DIR}"
        ARCHIVE DESTINATION "${INSTALL_ARCHIVE_DIR}")
