# Japanese OCR

This repository holds the training script to fine tune tesseract for japanese, and a script to OCR text given the traineddata file. Since most Japanese OCR's use default tesseract, they arent very accurate with unclear fonts, also contain very little to no image proccesing, etc. This project aims to build a better Japanese OCR with a better fine tuned model, parameters and more image processing to increase accuracy.

## OCR Script

The main OCR script is the `main.py` file and it can be hooked to a keybind on linux systems. Make sure you have trained the model first.
