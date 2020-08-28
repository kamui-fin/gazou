#!/bin/bash 

# This is a bash script to train tesseract with a larger japanese dataset (more fonts, better word list, etc)

# some file paths
FONTS_DIR=fonts
TESSDATA_DIR=tesseract/tessdata
LANG_DATA_DIR=langdata_lstm

# variables to change training flow
MAX_ITERATIONS=1000

# If you want to train japanese vertically, pass in -v
TRAIN_LANG=jpn

getopts ":v" VERT
if [ "${VERT}" = "v" ]; then
    TRAIN_LANG=jpn_vert
fi

# Removes output from previous trained data to start clean
rm jpn.lstm
rm -rf output/*
rm -rf train/*
# generates training data
tesseract/src/training/tesstrain.sh \
        --fonts_dir ${FONTS_DIR} \
        --linedata_only \
        --fontlist "" \ # put all the fonts here
        --noextract_font_properties \
        --lang jpn \
        --langdata_dir ${LANG_DATA_DIR} \
        --tessdata_dir  ${TESSDATA_DIR} \
        --output_dir train \
        --exposures "0"  

# extracts lstm from old traineddata
combine_tessdata -e ${TESSDATA_DIR}/jpn.traineddata jpn.lstm

# finetunes model
OMP_THREAD_LIMIT=8 lstmtraining \
    --continue_from jpn.lstm \
    --model_output output/jfdot \
    --traineddata ${TESSDATA_DIR}/jpn.traineddata \
    --train_listfile train/jpn.training_files.txt \
    --max_iterations 1000

# combines into a .traineddata
lstmtraining --stop_training \
    --continue_from output/jfdot_checkpoint \
    --traineddata ${TESSDATA_DIR}/jpn.traineddata \
    --model_output output/jfdot.traineddata

# evaluate (optional)
getopts ":e" evaluate

if [ "${evaluate}" = "e" ]; then
    lstmeval --model output/jfdot_checkpoint \
    --traineddata tesseract/tessdata/jpn.traineddata \
    --eval_listfile train/jpn.training_files.txt
fi