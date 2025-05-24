cmake_minimum_required(VERSION 3.30)
project(sfml)

set(CMAKE_CXX_STANDARD 20)

include(FetchContent)
FetchContent_Declare(
        fmt
        GIT_REPOSITORY https://github.com/fmtlib/fmt
        GIT_TAG 11.1.4
)
FetchContent_MakeAvailable(fmt)

FetchContent_Declare(
        SFML
        GIT_REPOSITORY https://github.com/SFML/SFML.git
        GIT_TAG 3.0.0
        GIT_SHALLOW ON
        EXCLUDE_FROM_ALL
        SYSTEM
)
FetchContent_MakeAvailable(SFML)


add_executable(sfml main.cpp
        Menu.cpp
        Menu.h
        Game.cpp
        Game.h
        Word.cpp
        GameSettings.cpp
        GameSettings.h
        GameStats.cpp
        GameStats.h
        EndScreen.cpp
        EndScreen.h
)

target_link_libraries(sfml  PRIVATE SFML::Graphics fmt)



