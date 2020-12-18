project(core)

find_package(PkgConfig REQUIRED)

link_directories(${TESSERACT_LIBRARY_DIRS})
link_directories(${LEPTONICA_LIBRARY_DIRS})

add_library(${PROJECT_NAME} src/ocr.cpp)

target_link_libraries(${PROJECT_NAME} tesseract lept)
target_include_directories(${PROJECT_NAME} PUBLIC include PRIVATE src)
