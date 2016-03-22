SET(PEAKINVESTIGATOR_SOURCES
    PeakInvestigator/Actions/BaseAction.cpp
    PeakInvestigator/Actions/PiVersionsAction.cpp
    PeakInvestigator/Actions/InitAction.cpp
    PeakInvestigator/Actions/SftpAction.cpp
    PeakInvestigator/Actions/PrepAction.cpp
    PeakInvestigator/Actions/RunAction.cpp
    PeakInvestigator/Actions/StatusAction.cpp
    PeakInvestigator/Actions/DeleteAction.cpp
)

SET(JSONCPP_SOURCES
    jsoncpp/jsoncpp.cpp
) 

INCLUDE_DIRECTORIES(${PROJECT_SOURCE_DIR}/include
                    ${PROJECT_SOURCE_DIR}/include/jsoncpp)

ADD_LIBRARY(PeakInvestigatorSaaS STATIC 
                ${PEAKINVESTIGATOR_SOURCES} ${JSONCPP_SOURCES}
)

