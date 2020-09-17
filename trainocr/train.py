
from subprocess import check_call, check_output
from pathlib import Path
from glob import glob
import os
import shutil

FONTS_DIR="/home/kamui/Coding/Projects/ocr/trainocr/fonts"
TESSDATA_DIR="/home/kamui/Coding/Projects/ocr/trainocr/tesseract/tessdata"
LANG_DATA_DIR="/home/kamui/Coding/Projects/ocr/trainocr/langdata_lstm"
MAX_ITERATIONS=10
TRAIN_LANG="jpn"

def get_font_name(path):
    font_name = check_output('fc-scan --format "%{family} %{style}" "' + path + '"', shell=True).decode("utf-8")
    return font_name.split(",")[0]

def trim_font_name(name):
    return name.replace("", "_")

def gen_train_data(fontname):
    return check_call(f'tesseract/src/training/tesstrain.sh --fonts_dir fonts --tessdata_dir  {TESSDATA_DIR} --langdata_dir {LANG_DATA_DIR} --linedata_only --lang jpn --fontlist "{fontname}" --noextract_font_properties --output_dir train --exposures "0"', shell=True)    

def extract_lstm(traineddata):
    return check_call(f'combine_tessdata -e "{traineddata}" jpn.lstm', shell=True) 

def finetune(old_tdata, name):
    return check_call(f'lstmtraining --continue_from jpn.lstm --model_output "output/{trim_font_name(name)}" --traineddata "{old_tdata}" --train_listfile train/jpn.training_files.txt --max_iterations {MAX_ITERATIONS}', shell=True)

def combine_traineddata(checkpoint, old_traineddata, output_model):
    return check_call(f'lstmtraining --stop_training --continue_from "output/{checkpoint}_checkpoint" --traineddata "{old_traineddata}" --model_output "output/{checkpoint}.traineddata"', shell=True)

def clean_trainfiles():
    shutil.rmtree("train")
    os.mkdir("train")

def duplicate_font(fonts, font):
    for f in fonts:
        if f["fname"] == font:
            return True
    return False

fonts = glob("fonts/*")
trained = []


for font in fonts:
    font_name = get_font_name(font)
    
    if (duplicate_font(trained, font_name)):
        continue

    old_tdata =  f"{TESSDATA_DIR}/jpn.traineddata" if len(trained) == 0 else "output/"+trained[-1]["tdata"] 
    slim_fname = trim_font_name(font_name)

    clean_trainfiles()
    try:
        gen_train_data(font_name)

        extract_lstm(old_tdata)
        finetune(old_tdata, font_name)
        combine_traineddata(slim_fname, old_tdata, slim_fname)
    except:
        with open("train.log", "a", encoding="utf-8") as f:
            f.write(f"Bad font: ${font_name}\n")
        continue
    else:
        trained.append({"fname": font_name, "slim_fname": slim_fname, "tdata": slim_fname+".traineddata"})
        with open("train.log", "a", encoding="utf-8") as f:
            f.write(f"Successfully trained font: ${font_name}\n")

print(trained)