cmake_minimum_required(VERSION 3.1.0)

project(ocr_desktop)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${ocr_desktop_SOURCE_DIR}/bin)

if(CMAKE_VERSION VERSION_LESS "3.7.0")
    set(CMAKE_INCLUDE_CURRENT_DIR ON)
endif()

find_package(Qt5 COMPONENTS Widgets REQUIRED)

add_executable(ocr_desktop src/main.cpp)

target_link_libraries(ocr_desktop Qt5::Widgets)
