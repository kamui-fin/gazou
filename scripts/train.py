from random import shuffle
from subprocess import check_call, check_output, call
from pathlib import Path
from glob import glob
from tqdm import tqdm
import os
import shutil

HOME = Path(".")

FONTS_DIR = HOME.joinpath("trainocr/fonts")
TESSDATA_DIR = HOME.joinpath("trainocr/tesseract/tessdata")
LANG_DATA_DIR = HOME.joinpath("trainocr/langdata_lstm")
MAX_ITERATIONS = 1000 
TRAIN_LANG = "jpn"


def get_font_name(path):
    font_name = check_output(
        'fc-scan --format "%{family}" "' + path + '"', shell=True).decode("utf-8")
    return font_name.split(",")[0]


def trim_font_name(name):
    return name.replace("", "_")


def gen_train_data(fontname):
    return check_call(f'{HOME.joinpath("trainocr/tesseract/src/training/tesstrain.sh")} --fonts_dir {HOME.joinpath("trainocr/fonts")} --tessdata_dir  {TESSDATA_DIR} --langdata_dir {LANG_DATA_DIR} --linedata_only --lang jpn --fontlist "{fontname}" --noextract_font_properties --output_dir {HOME.joinpath("trainocr/train")} --exposures "0"', shell=True)


def extract_lstm(traineddata):
    return check_call(f'combine_tessdata -e "{traineddata}" {HOME.joinpath("trainocr/jpn.lstm")}', shell=True)


def finetune(old_tdata, name):
    full_fontname = HOME.joinpath(f"trainocr/output/{trim_font_name(name)}")
    return check_call(f'lstmtraining --continue_from {HOME.joinpath("trainocr/jpn.lstm")} --model_output "{full_fontname}" --traineddata "{old_tdata}" --train_listfile {HOME.joinpath("trainocr/train/jpn.training_files.txt")} --max_iterations {MAX_ITERATIONS}', shell=True)


def combine_traineddata(checkpoint, old_traineddata, output_model):
    print(old_traineddata, output_model, checkpoint)
    return check_call(f'lstmtraining --stop_training --continue_from "{HOME.joinpath(f"trainocr/output/{checkpoint}_checkpoint")}" --traineddata "{old_traineddata}" --model_output "{HOME.joinpath("trainocr/output/"+checkpoint+".traineddata")}"', shell=True)


def clean_trainfiles():
    if (os.path.isdir("./trainocr/train")):
        shutil.rmtree("trainocr/train")
    os.mkdir("trainocr/train")

def duplicate_font(fonts, font):
    for f in fonts:
        if f["fname"] == font:
            return True
    return False


fonts = glob(f'{HOME.joinpath("trainocr/fonts/**/*")}', recursive=True)
trained = []
shuffle(fonts)

for font in tqdm(fonts):
    font_name = ""
    try:
        print(font)
        font_name = get_font_name(font)

        if (duplicate_font(trained, font_name)):
            continue

        old_tdata = f"{TESSDATA_DIR}/jpn.traineddata" if len(
            trained) == 0 else f'{HOME.joinpath("trainocr/output")}'+'/'+trained[-1]["tdata"]
        slim_fname = trim_font_name(font_name)

        clean_trainfiles()
        print("Generating training data...")
        gen_train_data(font_name)
        print("Extracting lstm...")
        extract_lstm(old_tdata)
        print("Finetuning ...")
        finetune(old_tdata, font_name)
        print("Combining traineddata...")
        combine_traineddata(slim_fname, old_tdata, slim_fname)
    except Exception as e:
        with open(f'{HOME.joinpath("trainocr/train.log")}', "a", encoding="utf-8") as f:
            f.write(f"Bad font: ${font_name}\n")
            print(e) 
        continue
    else:
        trained.append({"fname": font_name, "slim_fname": slim_fname,
                        "tdata": slim_fname+".traineddata"})
        with open(f'{HOME.joinpath("trainocr/train.log")}', "a", encoding="utf-8") as f:
            f.write(f"Successfully trained font: ${font_name}\n")
    finally:
        if (glob("/tmp/*")):
            call("rm -r /tmp/*", shell=True)