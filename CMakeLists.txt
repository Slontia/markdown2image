cmake_minimum_required (VERSION 3.1)
project(markdown2image LANGUAGES CXX C)

# set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -g -shared -fpic -pie  -Wl,-E,-emain")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -g")
set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -DDEBUG")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -DDEBUG")

add_subdirectory(third-party/md4c)

set(CMAKE_AUTOMOC ON)

find_package(Qt5 COMPONENTS Gui Widgets WebKitWidgets Test REQUIRED)
find_package(gflags)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/third-party/md4c/src)

add_executable(${PROJECT_NAME} ${CMAKE_CURRENT_SOURCE_DIR}/src/main.cc)
target_link_libraries(${PROJECT_NAME} md4c-html Qt5::Gui Qt5::Widgets Qt5::WebKitWidgets gflags)

