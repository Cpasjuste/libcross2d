**Build (Linux/SDL2)**

git clone --recursive git@github.com:cpasjuste/libcross2d.git
cd libcross2d
mkdir cmake-build && cd cmake-build
cmake -DBUILD_SDL2=1 -DBUILD_SDL2_GL=1 ../
make
./cross2d.elf


**Build (Switch)**

git clone --recursive git@github.com:cpasjuste/libcross2d.git
cd libcross2d
mkdir cmake-build && cd cmake-build
cmake -DBUILD_NX=1 ../
make cross2d.nro
