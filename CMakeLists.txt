cmake_minimum_required(VERSION 4.0)
project(particle-sim CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_CXX_FLAGS_RELEASE -O3)
set(CMAKE_CXX_FLAGS_DEBUG -g)

include(FetchContent)

FetchContent_Declare(
        SFML
        GIT_REPOSITORY https://github.com/SFML/SFML.git
        GIT_TAG 3.1.0
)
FetchContent_MakeAvailable(SFML)

include_directories(include)

add_executable(sim
        main.cpp
        grid.cpp
        threadpool.cpp
)

target_link_libraries(sim PRIVATE sfml-graphics sfml-window sfml-system)