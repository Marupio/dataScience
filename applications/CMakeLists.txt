# DS application

include_directories(
    ${CMAKE_SOURCE_DIR}/src/greeter
    ${PROJECT_BINARY_DIR}
)

add_executable(
    helloWorld
    ${CMAKE_CURRENT_SOURCE_DIR}/helloWorld/helloWorld.cpp
)

target_link_libraries(helloWorld greeter)

install_targets(
    ${BIN_DIR}
    helloWorld
)

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

