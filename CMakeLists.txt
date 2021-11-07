cmake_minimum_required(VERSION 3.10)

project(gazou VERSION 0.2.0 LANGUAGES CXX)
set (CMAKE_CXX_STANDARD 11)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

set(Gazou_MODEL_FOLDER ${CMAKE_INSTALL_PREFIX}/share/gazou/models)
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

option(GUI "Build X11 GUI support" ON)

configure_file(src/config.h.in config.h)
include_directories(${PROJECT_BINARY_DIR})

find_package(PkgConfig)

pkg_check_modules(TESSERACT REQUIRED tesseract>=4.0.0)
pkg_check_modules(LEPTONICA REQUIRED lept>=1.70)

find_package(Qt5Widgets REQUIRED VERSION 5.10)

if (GUI)
    find_package(Qt5X11Extras)
    find_package(QHotkey CONFIG QUIET)
    if (NOT QHotkey_FOUND)
        add_subdirectory(third-party/QHotkey EXCLUDE_FROM_ALL)
    endif()
    list(APPEND LIBS qhotkey)
    list(APPEND SRC src/gui_x11.cpp
                    src/configwindow.cpp
    )
endif()

add_executable(${PROJECT_NAME} ${SRC})
target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra -Wpedantic)
target_link_libraries(${PROJECT_NAME} Qt5::Widgets tesseract lept)

install(TARGETS gazou RUNTIME DESTINATION bin)
install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/models DESTINATION share/gazou)

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
