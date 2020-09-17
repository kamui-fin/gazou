WORDLIST_URL='https://gist.githubusercontent.com/kamui-7/6ee2da389c261b7ed838b6fa6ce4723f/raw/cdc557e087c861469198ea9c63ff780c97f18dff/jpn.wordlist'
TESSERACT_URL='https://github.com/tesseract-ocr/tesseract.git'
LANGDATA_LSTM_URL='https://github.com/tesseract-ocr/langdata_lstm.git' 
ENG_BEST_URL='https://github.com/tesseract-ocr/tessdata_best/raw/master/eng.traineddata'
JPN_BEST_URL='https://github.com/tesseract-ocr/tessdata_best/raw/master/jpn.traineddata'
JPN_VERT_BEST_URL='https://github.com/tesseract-ocr/tessdata_best/raw/master/jpn_vert.traineddata'
TESSTRAIN_UTILS_URL='https://pastebin.com/raw/mz4EQqSJ'


git clone $TESSERACT_URL trainocr/tesseract
git clone $LANGDATA_LSTM_URL trainocr/langdata_lstm

wget $JPN_BEST_URL -O trainocr/tesseract/tessdata/jpn.traineddata
wget $JPN_VERT_BEST_URL -O trainocr/tesseract/tessdata/jpn_vert.traineddata
wget $ENG_BEST_URL -O trainocr/tesseract/tessdata/eng.traineddata

wget $WORDLIST_URL -O trainocr/langdata_lstm/jpn/jpn.wordlist
wget $WORDLIST_URL -O trainocr/langdata_lstm/jpn_vert/jpn_vert.wordlist

sudo apt install python3 python3-pip tesseract libtesseract-dev 
pip3 install numpy pytesseract pyperclip opencv-python