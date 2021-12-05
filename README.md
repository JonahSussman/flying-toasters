# flying-toasters

A screensaver classic, recreated for Windows. Built using C++17 and SFML 2.5.1.

You must be using Windows Vista or later. If you just want the `.scr` file, head on over to the [releases page](https://github.com/JonahSussman/software-renderer/releases). To install, save the folder somewhere, right click on `flying-toasters.scr`, and click "Install."

## 🖼️ Gallery

Fly my toasters, fly!

![Flying toasters!](toasters.gif)

## 💻 Compilation Instructions

Prerequisites:
- A Windows computer
- A Debian-based WSL distro (such as Ubuntu)

Directions
1. Clone the repository.
1. If the `lib` folder does not exist in the repository, go ahead and create it.
1. If you do not have the `mingw-w64` toolchain, install it using `sudo apt-get install -y mingw-w64`.
1. Head on over to [the SFML 2.5.1 downloads page](https://www.sfml-dev.org/download/sfml/2.5.1/) and download the "GCC 7.3.0 MinGW (SEH) - 64-bit" zip file.
1. Copy the `SFML-2.5.1` folder into this project's `lib` folder. 
1. Type `make` to create the screen saver. Type `make clean` to start fresh.

Note: the makefile assumes that mingw is installed in its *default location*, and copies the DLLs from there. If you installed it somewhere else, you will have to modify the makefile.

## 📜 Licence

This software is released under the MIT License. See the LICENSE file for more information.
