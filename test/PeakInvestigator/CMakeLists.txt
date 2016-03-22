SET(ACTION_TEST_SOURCES
    Actions/InitActionTest.cpp
)

SET(SOURCES
    main.cpp
)

INCLUDE_DIRECTORIES(${PROJECT_SOURCE_DIR}/include ${PROJECT_SOURCE_DIR}/include/jsoncpp)
ADD_EXECUTABLE(PeakInvestigatorTest ${ACTION_TEST_SOURCES} ${SOURCES})

TARGET_LINK_LIBRARIES(PeakInvestigatorTest PeakInvestigatorSaaS libgtest)

ADD_TEST(NAME PeakInvestigatorTest COMMAND PeakInvestigatorTest)
