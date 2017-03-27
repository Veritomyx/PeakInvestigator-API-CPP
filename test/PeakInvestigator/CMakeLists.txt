SET(ACTION_TEST_SOURCES
    Actions/SandboxActionTest.cpp
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

SET(TARFILE_TEST_SOURCES
    TarFileTest.cpp
)

SET(PEAKINVESTIGATOR_USERNAME "" CACHE STRING "Specify a valid username for testing PeakInvestigatorSaaS")
SET(PEAKINVESTIGATOR_PASSWORD "" CACHE STRING "Specify a valid password for testing PeakInvestigatorSaaS")
SET(PEAKINVESTIGATOR_PROJECT "" CACHE STRING "Specify a valid project for testing PeakInvestigatorSaaS")
SET(SFTP_USERNAME "" CACHE STRING "Specify a valid username for testing SFTP transfers")
SET(SFTP_PASSWORD "" CACHE STRING "Specify a valid password for testing SFTP transfers")
SET(SFTP_PORT "" CACHE STRING "Specify a valid password for testing SFTP transfers")
SET(SFTP_HASH "" CACHE STRING "Specify a SSH server hash (lowercase, :-separated)")

IF(PEAKINVESTIGATOR_USERNAME AND PEAKINVESTIGATOR_PASSWORD AND PEAKINVESTIGATOR_PROJECT)
    SET(TEST_PEAKINVESTIGATOR_WEB 1)
ENDIF()

IF(SFTP_USERNAME AND SFTP_PASSWORD AND SFTP_PORT AND SFTP_HASH)
    SET(TEST_PEAKINVESTIGATOR_SFTP 1)
ENDIF()

CONFIGURE_FILE(
    ${CMAKE_CURRENT_SOURCE_DIR}/PeakInvestigatorSaaSTest_config.h.in
    ${CMAKE_CURRENT_BINARY_DIR}/PeakInvestigatorSaaSTest_config.h
)

ADD_DEFINITIONS(
    -DPI_USERNAME=\"${PEAKINVESTIGATOR_USERNAME}\"
    -DPI_PASSWORD=\"${PEAKINVESTIGATOR_PASSWORD}\"
    -DPI_PROJECT=${PEAKINVESTIGATOR_PROJECT}
    -DSFTP_USERNAME=\"${SFTP_USERNAME}\"
    -DSFTP_PASSWORD=\"${SFTP_PASSWORD}\"
    -DSFTP_PORT=${SFTP_PORT}
    -DSFTP_HASH=\"${SFTP_HASH}\"
)

SET(TEST_DATA_DIR ${CMAKE_CURRENT_SOURCE_DIR}/data)
CONFIGURE_FILE(
    ${CMAKE_CURRENT_SOURCE_DIR}/TarFileTest_config.h.in
    ${CMAKE_CURRENT_BINARY_DIR}/TarFileTest_config.h
)

IF(WIN32)
	ADD_DEFINITIONS(-DGTEST_LINKED_AS_SHARED_LIBRARY=1)
ENDIF()

INCLUDE_DIRECTORIES(
    ${PROJECT_SOURCE_DIR}
    ${PROJECT_BINARY_DIR}
    ${PROJECT_BINARY_DIR}/test/PeakInvestigator
    ${PROJECT_BINARY_DIR}/gtest/include
)

ADD_EXECUTABLE(PeakInvestigatorActionsTest ${ACTION_TEST_SOURCES} main.cpp)
TARGET_LINK_LIBRARIES(PeakInvestigatorActionsTest PeakInvestigatorSaaS libgtest libgmock)
ADD_TEST(NAME PeakInvestigatorActionsTest COMMAND PeakInvestigatorActionsTest)

ADD_EXECUTABLE(PeakInvestigatorSaaSTest ${SOURCES} main.cpp)
TARGET_LINK_LIBRARIES(PeakInvestigatorSaaSTest PeakInvestigatorSaaS libgtest libgmock)
ADD_TEST(NAME PeakInvestigatorSaaSTest COMMAND PeakInvestigatorSaaSTest)

ADD_EXECUTABLE(TarFileTest ${TARFILE_TEST_SOURCES} main.cpp)
TARGET_LINK_LIBRARIES(TarFileTest PeakInvestigatorSaaS libgtest libgmock)
ADD_TEST(NAME TarFileTest COMMAND TarFileTest)
