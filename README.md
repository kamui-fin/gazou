# Gazou OCR 

Gazou is a Japanese OCR for Linux written in C++. It utilizes image processing, and other parameters to improve the accuracy. I plan on finetuning the models in the future with a wide variety of fonts for better recognition. All contributions are welcome.

## Dependencies

These dependencies will need to be installed by your system's package manager. 

- Qt5 >= 5.10
- Qt5X11Extras >= 5.10
- Tesseract >= 4.1.1
- Leptonica >= 1.80

On Arch Linux, you can simply run
```sh
sudo pacman -S qt5-base tesseract leptonica qt5-x11extras 
```

## Install

```sh
git clone --recursive https://github.com/kamui-7/Gazou-OCR.git
cd Gazou-OCR
mkdir build
cd build
cmake .. 
sudo make install
```

## Usage

This app runs in the background and will appear in your system tray. If you click on the app in the tray, a settings dialog should appear. In here, you can customize the keybinds to your liking. The default hotkeys are

- `Alt+A`: Vertical OCR
- `Alt+D`: Horizontal OCR

When you perform an OCR, the result text gets copied to the clipboard.