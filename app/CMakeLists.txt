project(app)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

if(CMAKE_VERSION VERSION_LESS "3.7.0")
    set(CMAKE_INCLUDE_CURRENT_DIR ON)
    endif()
    
add_executable(${PROJECT_NAME} src/main.cpp)

find_package(Qt5 COMPONENTS Widgets X11Extras REQUIRED)
find_package(QHotkey CONFIG REQUIRED)

target_link_libraries(${PROJECT_NAME} Qt5::Widgets qhotkey core)

