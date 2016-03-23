SET(ACTION_TEST_SOURCES
    Actions/PiVersionsActionTest.cpp
    Actions/InitActionTest.cpp
    Actions/SftpActionTest.cpp
    Actions/PrepActionTest.cpp
    Actions/RunActionTest.cpp
    Actions/StatusActionTest.cpp
    Actions/DeleteActionTest.cpp
)

SET(SOURCES
    main.cpp
)

INCLUDE_DIRECTORIES(${PROJECT_SOURCE_DIR}/include ${PROJECT_SOURCE_DIR}/include/jsoncpp)
ADD_EXECUTABLE(PeakInvestigatorTest ${ACTION_TEST_SOURCES} ${SOURCES})

TARGET_LINK_LIBRARIES(PeakInvestigatorTest PeakInvestigatorSaaS libgtest)

ADD_TEST(NAME PeakInvestigatorTest COMMAND PeakInvestigatorTest)
