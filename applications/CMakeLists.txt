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
    testHandRanker
    ${CMAKE_CURRENT_SOURCE_DIR}/testApps/testHandRanker/testHandRanker.cpp
)

target_link_libraries(testHandRanker core)

install_targets(
    ${BIN_DIR}
    testHandRanker
)

