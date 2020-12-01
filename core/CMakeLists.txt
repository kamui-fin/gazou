project(core)

find_package(OpenCV REQUIRED core imgproc imgcodecs highgui)
find_package(PkgConfig REQUIRED)

include_directories(${OpenCV_INCLUDE_DIRS})

link_directories(${TESSERACT_LIBRARY_DIRS})
link_directories(${LEPTONICA_LIBRARY_DIRS})

add_library(${PROJECT_NAME} src/ocr.cpp)

target_link_libraries(${PROJECT_NAME} ${OpenCV_LIBS} tesseract)
target_include_directories(${PROJECT_NAME} PUBLIC include PRIVATE src)
