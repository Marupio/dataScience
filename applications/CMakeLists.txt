# DS application


include_directories(
    ${CMAKE_SOURCE_DIR}/src/core
    ${CMAKE_SOURCE_DIR}/src/gameMechanics
    ${CMAKE_SOURCE_DIR}/src/algorithm
    ${CMAKE_SOURCE_DIR}/src/stats
    ${CMAKE_SOURCE_DIR}/src/importedCode
    ${CMAKE_SOURCE_DIR}/src/importedCode/mbedtlsInterface
    ${PROJECT_BINARY_DIR}
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
    testCsprng
    ${CMAKE_CURRENT_SOURCE_DIR}/testApps/testCsprng/testCsprng.cpp
)
target_link_libraries(testCsprng core)
install_targets(
    ${BIN_DIR}
    testCsprng
)


add_executable(
    testDeck
    ${CMAKE_CURRENT_SOURCE_DIR}/testApps/testDeck/testDeck.cpp
)
target_link_libraries(testDeck core)
install_targets(
    ${BIN_DIR}
    testDeck
)


add_executable(
    testDictionary
    ${CMAKE_CURRENT_SOURCE_DIR}/testApps/testDictionary/testDictionary.cpp
)
target_link_libraries(testDictionary core)
install_targets(
    ${BIN_DIR}
    testDictionary
)


add_executable(
    testHandRankerCompare_Input
    ${CMAKE_CURRENT_SOURCE_DIR}/testApps/testHandRanker/testHandRankerCompare_Input.cpp
)
target_link_libraries(testHandRankerCompare_Input core)
install_targets(
    ${BIN_DIR}
    testHandRankerCompare_Input
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
    testHandRankerHandTypes_Input
    ${CMAKE_CURRENT_SOURCE_DIR}/testApps/testHandRanker/testHandRankerHandTypes_Input.cpp
)
target_link_libraries(testHandRankerHandTypes_Input core)
install_targets(
    ${BIN_DIR}
    testHandRankerHandTypes_Input
)


add_executable(
    testHandRankerRank_Input
    ${CMAKE_CURRENT_SOURCE_DIR}/testApps/testHandRanker/testHandRankerRank_Input.cpp
)
target_link_libraries(testHandRankerRank_Input core)
install_targets(
    ${BIN_DIR}
    testHandRankerRank_Input
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
    testHandRankerRankExtensive_Random
    ${CMAKE_CURRENT_SOURCE_DIR}/testApps/testHandRanker/testHandRankerRankExtensive_Random.cpp
)
target_link_libraries(testHandRankerRankExtensive_Random core)
install_targets(
    ${BIN_DIR}
    testHandRankerRankExtensive_Random
)


add_executable(
    testHandRankerRankInfinitum_Random
    ${CMAKE_CURRENT_SOURCE_DIR}/testApps/testHandRanker/testHandRankerRankInfinitum_Random.cpp
)
target_link_libraries(testHandRankerRankInfinitum_Random core)
install_targets(
    ${BIN_DIR}
    testHandRankerRankInfinitum_Random
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
    testPktCard
    ${CMAKE_CURRENT_SOURCE_DIR}/testApps/testPktCard/testPktCard.cpp
)
target_link_libraries(testPktCard core)
install_targets(
    ${BIN_DIR}
    testPktCard
)


add_executable(
    makePreFlopPredictions
    ${CMAKE_CURRENT_SOURCE_DIR}/utilities/makePreFlopPredictions/makePreFlopPredictions.cpp
)
target_link_libraries(makePreFlopPredictions core)
install_targets(
    ${BIN_DIR}
    makePreFlopPredictions
)


add_executable(
    makeAllCallTables
    ${CMAKE_CURRENT_SOURCE_DIR}/utilities/makeAllCallTables/makeAllCallTables.cpp
)
target_link_libraries(makeAllCallTables core pthread ${PQLIBS})
install_targets(
    ${BIN_DIR}
    makeAllCallTables
)