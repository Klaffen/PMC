cmake_minimum_required(VERSION 3.20)
project(DAT220_project LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


set(CMAKE_POLICY_DEFAULT_CMP0048 NEW)

include(FetchContent)

set(SFML_DEPENDENCIES_INSTALL_PREFIX "${CMAKE_BINARY_DIR}" CACHE STRING "" FORCE)

FetchContent_Declare(
        SFML
        GIT_REPOSITORY https://github.com/SFML/SFML.git
        GIT_TAG 2.6.2
)

FetchContent_MakeAvailable(SFML)


set(SOURCES "${CMAKE_SOURCE_DIR}/main.cpp")


file(GLOB_RECURSE GAME_SOURCES "${CMAKE_SOURCE_DIR}/Game/*.cpp")
list(APPEND SOURCES ${GAME_SOURCES})


file(GLOB STATEMACHINE_SOURCES "${CMAKE_SOURCE_DIR}/StateMachine/*.cpp")
file(GLOB SUPPORT_SOURCES "${CMAKE_SOURCE_DIR}/Support/*.cpp")
file(GLOB NETWORKING_SOURCES "${CMAKE_SOURCE_DIR}/Networking/*.cpp")
file(GLOB SQUADBUILDER_SOURCES "${CMAKE_SOURCE_DIR}/SquadBuilder/*.cpp")

list(APPEND SOURCES
        ${STATEMACHINE_SOURCES}
        ${SUPPORT_SOURCES}
        ${NETWORKING_SOURCES}
        ${SQUADBUILDER_SOURCES}
)

add_executable(DAT220_project ${SOURCES})

target_link_libraries(DAT220_project
        sfml-graphics
        sfml-window
        sfml-system
        sfml-network
        sfml-audio
)