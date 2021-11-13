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

These dependencies will need to be installed by your system's package manager:

- Qt5 >= 5.10
- Tesseract >= 4.0.0
- Leptonica >= 1.70

Optional dependencies:

- Qt5X11Extras >= 5.10 (for GUI)

### Install

```sh
git clone --recursive https://github.com/kamui-fin/gazou.git
cd gazou
mkdir build
cd build
cmake .. -DGUI=ON
sudo make install
```

#### Wayland

For wayland users, `gazou` must be compiled with `-DGUI=OFF` and use the equally functional command line interface.
This is due to the fact that Wayland does not support applications registering their own global hotkeys as a security measure.

## GUI

You can run the program by typing `gazou` in your terminal. It runs in the background and should appear in your system tray.
If you click on the app in the tray, a settings dialog should appear.
In here, you can customize the keybinds to your liking. The default hotkeys are:

- `Alt+A`: Vertical OCR
- `Alt+D`: Horizontal OCR
- `Alt+S`: Repeat the previous OCR

When you perform an OCR, the result text gets copied to the clipboard.

## CLI

Gazou also has a command line mode, and this can be useful for integrating it with bash scripts.
To get the resulting text copied to your clipboard, you can use `xclip`, `wl-copy`, or any clipboard utility you prefer.
Available options include:

```
1. gazou
   Run the main application

2. gazou --help
   Display this message

3. gazou prevscan
   Run the OCR on the same coordinates of the previous scan

4. gazou ORIENTATION{-h; -v; horizontal; vertical}
   Interactively run the OCR and print the output to stdout

5. gazou ORIENTATION{-h; -v; horizontal; vertical} IMAGEFILE
   Run the OCR on the given IMAGEFILE with the given ORIENTATION.
```
