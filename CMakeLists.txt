cmake_minimum_required (VERSION 3.1)
project(markdown2image LANGUAGES CXX C)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -g")
set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -DDEBUG")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -DDEBUG")

add_subdirectory(third-party/md4c)

set(CMAKE_AUTOMOC ON)

find_package(Qt5 COMPONENTS Gui Widgets WebKitWidgets REQUIRED)
find_package(gflags)

add_custom_target(moc_converter COMMAND moc-qt5 ${CMAKE_SOURCE_DIR}/converter.h)

include_directories(${CMAKE_SOURCE_DIR}/third-party/md4c/src)

add_executable(${PROJECT_NAME} ${CMAKE_SOURCE_DIR}/src/main.cc ${CMAKE_SOURCE_DIR}/src/converter.cc)
target_link_libraries(${PROJECT_NAME} md4c-html Qt5::Gui Qt5::Widgets Qt5::WebKitWidgets gflags)

