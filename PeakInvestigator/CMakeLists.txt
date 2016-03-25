INCLUDE(GenerateExportHeader)

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
INCLUDE_DIRECTORIES(${CMAKE_SOURCE_DIR}/thirdparty/jsoncpp ${CURL_INCLUDE_DIRS} ${CMAKE_CURRENT_BINARY_DIR})

TARGET_LINK_LIBRARIES(PeakInvestigatorSaaS PRIVATE jsoncpp libssh2 ${CURL_LIBRARIES})

