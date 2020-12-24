project(jpocr_core)

add_library(${PROJECT_NAME} src/ocr.cpp)

target_link_libraries(${PROJECT_NAME} tesseract lept)
target_include_directories(${PROJECT_NAME} PUBLIC include PRIVATE src)
