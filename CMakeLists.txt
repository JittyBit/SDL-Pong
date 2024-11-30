cmake_minimum_required(VERSION 3.29)
project(SDL_Pong C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake_modules)

# will need to be changed depending on the location
# of SDL2
set(SDL2_PATH "C:/SDL2/SDL2-2.30.9/x86_64-w64-mingw32")

find_package(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIR})

add_executable(SDL_Pong main.c)

target_link_libraries(${PROJECT_NAME} ${SDL2_LIBRARY})
