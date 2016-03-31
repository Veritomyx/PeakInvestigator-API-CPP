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
    PeakInvestigatorSaaSTest.cpp
)

SET(PEAKINVESTIGATOR_USERNAME "" CACHE STRING "Specify a valid username for testing PeakInvestigatorSaaS")
SET(PEAKINVESTIGATOR_PASSWORD "" CACHE STRING "Specify a valid password for testing PeakInvestigatorSaaS")
SET(PEAKINVESTIGATOR_PROJECT "" CACHE STRING "Specify a valid project for testing PeakInvestigatorSaaS")
SET(SFTP_USERNAME "" CACHE STRING "Specify a valid username for testing SFTP transfers")
SET(SFTP_PASSWORD "" CACHE STRING "Specify a valid password for testing SFTP transfers")
SET(SFTP_PORT "" CACHE STRING "Specify a valid password for testing SFTP transfers")


ADD_DEFINITIONS(
    -DPI_USERNAME=\"${PEAKINVESTIGATOR_USERNAME}\"
    -DPI_PASSWORD=\"${PEAKINVESTIGATOR_PASSWORD}\"
    -DPI_PROJECT=${PEAKINVESTIGATOR_PROJECT}
    -DSFTP_USERNAME=\"${SFTP_USERNAME}\"
    -DSFTP_PASSWORD=\"${SFTP_PASSWORD}\"
    -DSFTP_PORT=${SFTP_PORT}
)

INCLUDE_DIRECTORIES(
    ${PROJECT_SOURCE_DIR}
    ${PROJECT_BINARY_DIR}/PeakInvestigator
    ${PROJECT_BINARY_DIR}/gtest/include
)

ADD_EXECUTABLE(PeakInvestigatorActionsTest ${ACTION_TEST_SOURCES} main.cpp)
TARGET_LINK_LIBRARIES(PeakInvestigatorActionsTest PeakInvestigatorSaaS libgtest libgmock)
ADD_TEST(NAME PeakInvestigatorActionsTest COMMAND PeakInvestigatorActionsTest)

ADD_EXECUTABLE(PeakInvestigatorSaaSTest ${SOURCES} main.cpp)
TARGET_LINK_LIBRARIES(PeakInvestigatorSaaSTest PeakInvestigatorSaaS libgtest libgmock)
ADD_TEST(NAME PeakInvestigatorSaaSTest COMMAND PeakInvestigatorSaaSTest)
