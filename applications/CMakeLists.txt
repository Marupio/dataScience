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
    testApp
    ${CMAKE_CURRENT_SOURCE_DIR}/testApp/testApp.cpp
)

target_link_libraries(testApp core)

install_targets(
    ${BIN_DIR}
    testApp
)

