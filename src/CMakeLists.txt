# DS library

#file(GLOB_RECURSE DS_LIBRARY_SOURCES "src/*.cpp")
#file(GLOB_RECURSE DS_LIBRARY_HEADERS "src/*.h")
#
#set (DS_LIBRARY_INCLUDE_DIRS "")
#foreach (_headerFile ${DS_LIBRARY_INCLUDE_DIRS})
#    get_filename_component(_dir ${_headerFile} PATH)
#    list (APPEND DS_LIBRARY_INCLUDE_DIRS ${_dir})
#endforeach()
#list(REMOVE_DUPLICATES DS_LIBRARY_INCLUDE_DIRS)
#
#include_directories(${DS_LIBRARY_INCLUDE_DIRS})

include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/core
    ${CMAKE_CURRENT_SOURCE_DIR}/gameMechanics
    ${CMAKE_CURRENT_SOURCE_DIR}/algorithm
    ${CMAKE_CURRENT_SOURCE_DIR}/stats
    ${CMAKE_CURRENT_SOURCE_DIR}/importedCode
    ${CMAKE_CURRENT_SOURCE_DIR}/importedCode/mbedtlsInterface
)

add_library(
    core SHARED
    ${CMAKE_CURRENT_SOURCE_DIR}/core/osRelated_POSIX.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/gameMechanics/Blinds.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/gameMechanics/Board.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/gameMechanics/Card.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/gameMechanics/Deck.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/gameMechanics/PktCards.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/gameMechanics/PktVals.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/gameMechanics/Player.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/gameMechanics/Pot.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/gameMechanics/Seats.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/gameMechanics/Suit.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/gameMechanics/Table.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/gameMechanics/ValStats.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/gameMechanics/VecValStats.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/algorithm/DeckMask.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/algorithm/HandRanker.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/algorithm/HandType.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/algorithm/PktMask.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/algorithm/StraightCompleters.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/stats/AllCallPlayer.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/importedCode/mbedtls/aes.c
    ${CMAKE_CURRENT_SOURCE_DIR}/importedCode/mbedtls/aesni.c
    ${CMAKE_CURRENT_SOURCE_DIR}/importedCode/mbedtls/ctr_drbg.c
    ${CMAKE_CURRENT_SOURCE_DIR}/importedCode/mbedtls/entropy.c
    ${CMAKE_CURRENT_SOURCE_DIR}/importedCode/mbedtls/entropy_poll.c
    ${CMAKE_CURRENT_SOURCE_DIR}/importedCode/mbedtls/timing.c
    ${CMAKE_CURRENT_SOURCE_DIR}/importedCode/mbedtls/threading.c
    ${CMAKE_CURRENT_SOURCE_DIR}/importedCode/mbedtls/sha512.c
    ${CMAKE_CURRENT_SOURCE_DIR}/importedCode/mbedtls/sha256.c
    ${CMAKE_CURRENT_SOURCE_DIR}/importedCode/mbedtls/sha512.c
    ${CMAKE_CURRENT_SOURCE_DIR}/importedCode/mbedtlsInterface/Csprng.cpp
)
