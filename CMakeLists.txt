cmake_minimum_required(VERSION 3.10)

project(gazou VERSION 0.2.0 LANGUAGES CXX)
set (CMAKE_CXX_STANDARD 11)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

if (WIN32)
    set(Gazou_MODEL_FOLDER models)
else()
    set(Gazou_MODEL_FOLDER ${CMAKE_INSTALL_PREFIX}/share/gazou/models)
endif()
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -DDEBUG")
set(LIBS Qt5::Widgets tesseract lept)
set(SRC 
    src/main.cpp 
    src/selectorwidget.cpp
    src/keydialog.cpp
    src/utils.cpp
    src/ocr.cpp
    src/state.cpp
    resources/resources.qrc
)

if(CMAKE_VERSION VERSION_LESS "3.7.0")
    set(CMAKE_INCLUDE_CURRENT_DIR ON)
endif()

set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -DDEBUG")

configure_file(src/config.h.in config.h)
include_directories(${PROJECT_BINARY_DIR})

if(CMAKE_VERSION VERSION_LESS "3.7.0")
    set(CMAKE_INCLUDE_CURRENT_DIR ON)
endif()

option(GUI "Build GUI support" ON)

configure_file(src/config.h.in config.h)
include_directories(${PROJECT_BINARY_DIR})

find_package(PkgConfig)

pkg_check_modules(tess REQUIRED tesseract)
pkg_check_modules(lept REQUIRED lept)

find_package(Qt5Widgets REQUIRED VERSION 5.10)

if (GUI)
    if (UNIX AND NOT APPLE)
        find_package(Qt5X11Extras)
    endif()
    find_package(QHotkey CONFIG QUIET)
    if (NOT QHotkey_FOUND)
        add_subdirectory(third-party/QHotkey EXCLUDE_FROM_ALL)
    endif()
    list(APPEND LIBS qhotkey)
    list(APPEND SRC src/gui.cpp
                    src/configwindow.cpp
    )
    add_compile_definitions(GUI)
endif()

# Windows target
if (WIN32)
    add_executable(${PROJECT_NAME} WIN32 ${SRC})
else()
    add_executable(${PROJECT_NAME} ${SRC})
endif()

target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra -Wpedantic)
target_link_libraries(${PROJECT_NAME} ${LIBS})

# Install targets
if (UNIX AND NOT APPLE)
    install(TARGETS gazou RUNTIME DESTINATION bin)
    install(DIRECTORY models DESTINATION share/gazou)
    install(FILES resources/gazou.desktop DESTINATION share/applications)
    install(FILES resources/gazou.png DESTINATION share/pixmaps)
endif()

# format entire project
file(GLOB_RECURSE
     ALL_CXX_SOURCE_FILES
     *.[chi]pp *.[chi]xx *.cc *.hh *.ii *.[CHI]
)

add_custom_target(
    format
    COMMAND /usr/lib/llvm/11/bin/clang-format
    -i
    -style=file
    ${ALL_CXX_SOURCE_FILES}
)
