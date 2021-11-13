# Gazou OCR

Gazou is a Japanese OCR for Linux written in C++. It utilizes image processing, and other parameters to improve the accuracy. All contributions are welcome.

## Installation

### Gentoo

First, enable my gentoo overlay with `eselect-repository`. If you don't have it already, follow the [guide](https://wiki.gentoo.org/wiki/Eselect/Repository) on the Gentoo wiki first.

```
eselect repository add kamui git https://github.com/kamui-fin/gentoo-overlay.git
emerge --sync kamui
```

Then you can emerge the package normally with:

```
emerge -av gazou
```

### Arch

Since this is packaged in the AUR, you can install it with any AUR helper like:

```
paru -S gazou-git
```

## Building from source

### Dependencies

These dependencies will need to be installed by your system's package manager.

- Qt5 >= 5.10
- Qt5X11Extras >= 5.10
- Tesseract >= 4.0.0
- Leptonica >= 1.70

### Install

```sh
git clone --recursive https://github.com/kamui-fin/gazou.git
cd gazou
mkdir build
cd build
cmake ..
sudo make install
```

## Usage

You can run the program by typing `gazou` in your terminal. It runs in the background and should appear in your system tray. If you click on the app in the tray, a settings dialog should appear. In here, you can customize the keybinds to your liking. The default hotkeys are:

- `Alt+A`: Vertical OCR
- `Alt+D`: Horizontal OCR
- `Alt+S`: Repeat the previous OCR

When you perform an OCR, the result text gets copied to the clipboard.

Gazou also has a command line mode, and this can be useful for integrating it with bash scripts. It takes two different parameters, the image path and the orientation, which is either `vertical (-v)` or `horizontal (-h)`. Here's an example:

```bash
gazou -h page103.png
```

This command scans all the text in the image and prints the resulting text.

For more help, run `gazou --help`.
