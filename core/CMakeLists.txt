project(jpocr_core)

add_library(${PROJECT_NAME} src/ocr.cpp)

find_package(PkgConfig)

pkg_check_modules(TESSERACT REQUIRED tesseract>=4.1.1)
pkg_check_modules(LEPTONICA REQUIRED lept>=1.80)

target_link_libraries(${PROJECT_NAME} tesseract lept)
target_include_directories(${PROJECT_NAME} PUBLIC include PRIVATE src)
