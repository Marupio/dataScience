# DS application


include_directories(
    ${CMAKE_SOURCE_DIR}/src/core
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

