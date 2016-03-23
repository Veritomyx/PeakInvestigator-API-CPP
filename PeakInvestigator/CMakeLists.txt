SET(PEAKINVESTIGATOR_SOURCES
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
    Actions/BaseAction.h
    Actions/PiVersionsAction.h
    Actions/InitAction.h
    Actions/SftpAction.h
    Actions/PrepAction.h
    Actions/RunAction.h
    Actions/StatusAction.h
    Actions/DeleteAction.h
)

SET(JSONCPP_SOURCES
    jsoncpp/jsoncpp.cpp
)

SET(JSONCPP_HEADERS
    jsoncpp/json/json-forwards.h
    jsoncpp/json/json.h
)

INCLUDE_DIRECTORIES(${CMAKE_SOURCE_DIR} ${CMAKE_SOURCE_DIR}/PeakInvestigator/jsoncpp)

ADD_LIBRARY(PeakInvestigatorSaaS STATIC 
                ${PEAKINVESTIGATOR_SOURCES} ${PEAKINVESTIGATOR_HEADERS} ${JSONCPP_SOURCES} ${JSONCPP_HEADERS}
)

