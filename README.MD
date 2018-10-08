Build and run (Linux/deb)
-------------------------

- Install dependencies
    ```
    sudo apt-get install build-essential cmake git libsdl2-dev libfreetype6-dev libglm-dev
    ```
- Build
    ```
    git clone --recursive git@github.com:cpasjuste/libcross2d.git
    cd libcross2d
    mkdir cmake-build && cd cmake-build
    cmake -DBUILD_SDL2=1 ../
    make
    ./cross2d.elf
    ```


Build and run (Nintendo Switch)
-------------------------------

- Install dependencies
    ```
    wget https://github.com/devkitPro/pacman/releases/download/devkitpro-pacman-1.0.1/devkitpro-pacman.deb
    sudo dpkg -i devkitpro-pacman.deb
    sudo dkp-pacman -S
    ```

- Build
    ```
    cmake -DBUILD_SWITCH=1 ../
    make cross2d.nro
    ```