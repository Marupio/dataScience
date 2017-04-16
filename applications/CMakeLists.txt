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
    testHandRankerCompare_Random
    ${CMAKE_CURRENT_SOURCE_DIR}/testApps/testHandRanker/testHandRankerCompare_Random.cpp
)

target_link_libraries(testHandRankerCompare_Random core)

install_targets(
    ${BIN_DIR}
    testHandRankerCompare_Random
)


add_executable(
    testHandRankerCompare2_Random
    ${CMAKE_CURRENT_SOURCE_DIR}/testApps/testHandRanker/testHandRankerCompare2_Random.cpp
)

target_link_libraries(testHandRankerCompare2_Random core)

install_targets(
    ${BIN_DIR}
    testHandRankerCompare2_Random
)


add_executable(
    testHandRankerHandTypes_Input
    ${CMAKE_CURRENT_SOURCE_DIR}/testApps/testHandRanker/testHandRankerHandTypes_Input.cpp
)

target_link_libraries(testHandRankerHandTypes_Input core)

install_targets(
    ${BIN_DIR}
    testHandRankerHandTypes_Input
)


add_executable(
    testHandRankerStraightFinder_Input
    ${CMAKE_CURRENT_SOURCE_DIR}/testApps/testHandRanker/testHandRankerStraightFinder_Input.cpp
)

target_link_libraries(testHandRankerStraightFinder_Input core)

install_targets(
    ${BIN_DIR}
    testHandRankerStraightFinder_Input
)


add_executable(
    testHandRankerRank_Random
    ${CMAKE_CURRENT_SOURCE_DIR}/testApps/testHandRanker/testHandRankerRank_Random.cpp
)

target_link_libraries(testHandRankerRank_Random core)

install_targets(
    ${BIN_DIR}
    testHandRankerRank_Random
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
