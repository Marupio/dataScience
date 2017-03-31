# DS application


include_directories(
    ${CMAKE_SOURCE_DIR}/src/core
    ${CMAKE_SOURCE_DIR}/src/gameMechanics
    ${CMAKE_SOURCE_DIR}/src/algorithm
    ${PROJECT_BINARY_DIR}
)

add_executable(
    testCard
    ${CMAKE_CURRENT_SOURCE_DIR}/testApps/testCard/testCard.cpp
)

target_link_libraries(testCard core)

install_targets(
    ${BIN_DIR}
    testCard
)

add_executable(
    testPktCard
    ${CMAKE_CURRENT_SOURCE_DIR}/testApps/testPktCard/testPktCard.cpp
)

target_link_libraries(testPktCard core)

install_targets(
    ${BIN_DIR}
    testPktCard
)

add_executable(
    testHandRankerRandom
    ${CMAKE_CURRENT_SOURCE_DIR}/testApps/testHandRanker/testHandRankerRandom.cpp
)

target_link_libraries(testHandRankerRandom core)

install_targets(
    ${BIN_DIR}
    testHandRankerRandom
)

add_executable(
    testHandRankerInput
    ${CMAKE_CURRENT_SOURCE_DIR}/testApps/testHandRanker/testHandRankerInput.cpp
)

target_link_libraries(testHandRankerInput core)

install_targets(
    ${BIN_DIR}
    testHandRankerInput
)

add_executable(
    testBoard
    ${CMAKE_CURRENT_SOURCE_DIR}/testApps/testBoard/testBoard.cpp
)

target_link_libraries(testBoard core)

install_targets(
    ${BIN_DIR}
    testBoard
)
