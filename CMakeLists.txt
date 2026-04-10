cmake_minimum_required (VERSION 3.1)
project(markdown2image LANGUAGES CXX C)

# set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -g -shared -fpic -pie  -Wl,-E,-emain")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -g")
set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -DDEBUG")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -DDEBUG")

add_subdirectory(third-party/md4c)

option(USE_QT5WEBKIT "Use Qt5WebKit instead of Qt6 WebEngine" ON)

if(USE_QT5WEBKIT)
    find_package(Qt5 COMPONENTS Gui Widgets WebKit WebKitWidgets REQUIRED)
    set(QT_LIBS Qt5::Gui Qt5::Widgets Qt5::WebKit Qt5::WebKitWidgets)
    add_compile_definitions(USE_QT5WEBKIT)
else()
    find_package(Qt6 COMPONENTS Gui Widgets WebEngineWidgets REQUIRED)
    set(QT_LIBS Qt6::Gui Qt6::Widgets Qt6::WebEngineWidgets)
endif()

find_package(gflags)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/third-party/md4c/src)

add_executable(${PROJECT_NAME} ${CMAKE_CURRENT_SOURCE_DIR}/src/main.cc)
target_link_libraries(${PROJECT_NAME} md4c-html ${QT_LIBS} gflags)
