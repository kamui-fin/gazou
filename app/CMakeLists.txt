project(app)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

include(FetchContent)

if(CMAKE_VERSION VERSION_LESS "3.7.0")
    set(CMAKE_INCLUDE_CURRENT_DIR ON)
    endif()

if (WIN32)
  set(CMAKE_SHARED_LIBRARY_PREFIX "")
endif ()


add_executable(${PROJECT_NAME} src/main.cpp 
                               src/configwindow.cpp
                               src/selectorwidget.cpp
                               src/keydialog.cpp
                               src/utils.cpp
                               resources/resources.qrc
)

find_package(Qt5 COMPONENTS Widgets REQUIRED)
find_package(QHotkey CONFIG REQUIRED)

if (UNIX)
    find_package(Qt5 COMPONENTS X11Extras REQUIRED)
endif()

target_link_libraries(${PROJECT_NAME} Qt5::Widgets qhotkey core)

