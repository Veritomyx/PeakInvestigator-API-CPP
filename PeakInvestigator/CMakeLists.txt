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

INCLUDE_DIRECTORIES(${CMAKE_SOURCE_DIR} ${CMAKE_SOURCE_DIR}/thirdparty/jsoncpp)

ADD_LIBRARY(PeakInvestigator STATIC
                ${PEAKINVESTIGATOR_SOURCES} ${PEAKINVESTIGATOR_HEADERS}
)

TARGET_LINK_LIBRARIES(PeakInvestigator PRIVATE jsoncpp)

