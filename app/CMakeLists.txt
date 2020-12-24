project(jpocr)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

if(CMAKE_VERSION VERSION_LESS "3.7.0")
    set(CMAKE_INCLUDE_CURRENT_DIR ON)
endif()

add_executable(${PROJECT_NAME} src/main.cpp 
                               src/configwindow.cpp
                               src/selectorwidget.cpp
                               src/keydialog.cpp
                               src/utils.cpp
                               resources/resources.qrc
)

find_package(Qt5 COMPONENTS Widgets X11Extras REQUIRED)

find_package(QHotkey CONFIG)
if (NOT QHotkey_FOUND)
    add_subdirectory(third-party/QHotkey EXCLUDE_FROM_ALL)
endif()

target_link_libraries(${PROJECT_NAME} Qt5::Widgets qhotkey jpocr_core)

install(TARGETS jpocr RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})