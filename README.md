# Japanese OCR

This repository holds the training script to fine tune tesseract for japanese, and a script to OCR text given the traineddata file. Since most Japanese OCR's use default tesseract, they arent very accurate with unclear fonts, very little to no image proccesing, etc. This project aims to build a better OCR with a better fine tuned model.

## Training

To install all the training material, run the `install.sh` shell script first. Then put all the fonts you want inside the fonts folder. Then you want to do the following:

```sh
python3 order_fonts.py
python3 find_fonts.py
```

Which orders all the fonts accordingly and outputs all the pango compatible font names. Use the output and paste it into Line 32 of the training script.
