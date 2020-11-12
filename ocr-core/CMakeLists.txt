cmake_minimum_required(VERSION 3.10)

project(OCR_Core)

include_directories(${OCR_Core_SOURCE_DIR}/include)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${OCR_Core_SOURCE_DIR}/bin)

add_library(ocr_core src/ocr.cpp)