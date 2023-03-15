# Gazou OCR

Gazou is a Japanese and Chinese OCR application written in C++. It utilizes image processing, and other parameters to improve the accuracy. All contributions are welcome.

## Installation

### Windows

You can grab the latest portable zip build from the [releases](https://github.com/kamui-fin/gazou/releases). Simply unzip and execute `gazou.exe` and the app should run.

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

Just to clarify the language option names:

- `jpn`: Japanese
- `chi_sim`: Simplified Chinese
- `chi_trad`: Traditional Chinese

## CLI

Gazou also has a command line mode, and this can be useful for integrating it with bash scripts.
To get the resulting text copied to your clipboard, you can use `xclip`, `wl-copy`, or any clipboard utility you prefer.
Available options include:

```
Usage: gazou [options] imagePath
Launches GUI if no options are provided.

Options:
  -p, --prevscan             Run the OCR on the same coordinates of the
                             previous scan
  -l, --language <language>  Specify OCR language, defaults to jpn. Options:
                             jpn, chi_sim, chi_trad
  -v, --vertical             Switch orientation to vertical. Without this,
                             gazou expects horizontal text.
  --version                  Fetch the version information of gazou
  --help                     View this help menu

Arguments:
  imagePath                  Source image file to OCR
```

### Piping from stdin

You can also run gazou by pipeing an image into the CLI:

```
cat img.png | gazou
```
