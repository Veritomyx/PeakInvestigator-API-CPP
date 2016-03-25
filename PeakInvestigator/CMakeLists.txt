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

INCLUDE_DIRECTORIES(${CMAKE_SOURCE_DIR} ${CMAKE_SOURCE_DIR}/thirdparty/jsoncpp ${CURL_INCLUDE_DIRS})

ADD_LIBRARY(PeakInvestigatorSaaS STATIC
                ${PEAKINVESTIGATOR_SOURCES} ${PEAKINVESTIGATOR_HEADERS}
)

TARGET_LINK_LIBRARIES(PeakInvestigatorSaaS PRIVATE jsoncpp ${CURL_LIBRARIES})

