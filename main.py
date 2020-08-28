from subprocess import call
import pyscreenshot as ImageGrab
import numpy as np
import pytesseract
import pyperclip
import pathlib
import cv2

VERTICAL = False
LANG = "jpn_vert" if VERTICAL else "jpn"

image_path = pathlib.Path(__file__).parent / "image.png"
image_path = str(image_path)
call(f"maim -s -u {image_path}", shell=True)


img = cv2.imread(image_path)
img = cv2.resize(img, None, fx=1.2, fy=1.2,
                 interpolation=cv2.INTER_CUBIC)
img = cv2.cvtColor(img, cv2.COLOR_BGR2GRAY)
kernel = np.ones((1, 1), np.uint8)
img = cv2.dilate(img, kernel, iterations=1)
img = cv2.erode(img, kernel, iterations=1)
cv2.adaptiveThreshold(cv2.medianBlur(img, 3), 255,
                      cv2.ADAPTIVE_THRESH_GAUSSIAN_C, cv2.THRESH_BINARY, 31, 2)

cv2.imwrite(image_path, img)

config = """
-c language_model_ngram_on=0
-c tessedit_enable_dict_correction=1
-c textord_really_old_xheight=1
-c textord_force_make_prop_words=F
-c edges_max_children_per_outline=40
-c tosp_threshold_bias2=1
-c classify_norm_adj_midpoint=96
-c tessedit_class_miss_scale=0.002
-c textord_initialx_ile=1.0
"""

1
text = pytesseract.image_to_string(
    img, config=config, lang="jfdot")

text = text.replace(" ", "").strip()
print(text)
pyperclip.copy(text)
