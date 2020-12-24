cmake_minimum_required(VERSION 3.10)

project(JP_OCR VERSION 0.0.1 LANGUAGES CXX)
set (CMAKE_CXX_STANDARD 11)

include(GNUInstallDirs)


add_subdirectory(app)
add_subdirectory(core)

install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/models DESTINATION ${CMAKE_INSTALL_DATADIR}/JP_OCR)