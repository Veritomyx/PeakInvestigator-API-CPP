SET(ACTION_TEST_SOURCES
    PeakInvestigatorSaaSTest.cpp
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

SET(PEAKINVESTIGATOR_USERNAME "" CACHE STRING "Specify a valid username for testing PeakInvestigatorSaaS")
SET(PEAKINVESTIGATOR_PASSWORD "" CACHE STRING "Specify a valid password for testing PeakInvestigatorSaaS")
SET(PEAKINVESTIGATOR_PROJECT "" CACHE STRING "Specify a valid project for testing PeakInvestigatorSaaS")

ADD_DEFINITIONS(
    -DTEST_USERNAME=\"${PEAKINVESTIGATOR_USERNAME}\"
    -DTEST_PASSWORD=\"${PEAKINVESTIGATOR_PASSWORD}\"
    -DTEST_PROJECT=${PEAKINVESTIGATOR_PROJECT}
)

INCLUDE_DIRECTORIES(${PROJECT_SOURCE_DIR} ${PROJECT_BINARY_DIR}/PeakInvestigator)
ADD_EXECUTABLE(PeakInvestigatorTest ${ACTION_TEST_SOURCES} ${SOURCES})

TARGET_LINK_LIBRARIES(PeakInvestigatorTest PeakInvestigatorSaaS libgtest)

ADD_TEST(NAME PeakInvestigatorTest COMMAND PeakInvestigatorTest)
