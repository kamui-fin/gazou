#!/bin/bash 

# This is a bash script to train tesseract with a larger japanese dataset (more fonts, better word list, etc)

# some file paths
FONTS_DIR=/home/kamui/Coding/Projects/ocr/trainocr/fonts
TESSDATA_DIR=/home/kamui/Coding/Projects/ocr/trainocr/tesseract/tessdata
LANG_DATA_DIR=/home/kamui/Coding/Projects/ocr/trainocr/langdata_lstm

# variables to change training flow
MAX_ITERATIONS=1000

# If you want to train japanese vertically, pass in -v
TRAIN_LANG=jpn

getopts ":v" VERT
if [ "${VERT}" = "v" ]; then
    TRAIN_LANG=jpn_vert
fi

# generates training data
tesseract/src/training/tesstrain.sh \
        --fonts_dir ${FONTS_DIR} \
        --tessdata_dir  ${TESSDATA_DIR} \
        --langdata_dir ${LANG_DATA_DIR} \
        --linedata_only \
        --lang jpn \
        --fontlist "JF Dot M+H 12" \
        --noextract_font_properties \
        --output_dir train \
        --exposures "0"  

# extracts lstm from old traineddata
combine_tessdata -e ${TESSDATA_DIR}/jpn.traineddata jpn.lstm

# finetunes model
lstmtraining \
    --continue_from jpn.lstm \
    --model_output output/jpn \
    --traineddata ${TESSDATA_DIR}/jpn.traineddata \
    --train_listfile train/jpn.training_files.txt \
    --max_iterations 1000

# combines into a .traineddata
lstmtraining --stop_training \
    --continue_from output/jpn_checkpoint \
    --traineddata ${TESSDATA_DIR}/jpn.traineddata \
    --model_output output/jpn.traineddata

# evaluate (optional)
getopts ":e" evaluate

if [ "${evaluate}" = "e" ]; then
    lstmeval --model output/jpn_checkpoint \
    --traineddata tesseract/tessdata/jpn.traineddata \
    --eval_listfile train/jpn.training_files.txt
fi