# Compilation guide for Windows and Linux. 

## Linux

### Native Linux

```bash
git clone https://github.com/fawtytoo/ManicMiner.git
```

```bash
cd ManicMiner
make
```

The executable file maninminer should now be available run via ./manicminer

### Mingw for Linux

Arch based distro mingw64 SDL2 library
```bash
pacman -S mingw-w64-gcc mingw-w64-sdl2
```

```bash
git clone https://github.com/fawtytoo/ManicMiner.git
```

```bash
cd ManicMiner
make
```
Also required to copy SDL2.dll to same folder as manicminer.exe. Then copy these files to a Windows machine.
On arch based system is
```bash
cp /usr/x86_64-w64-mingw32/bin/SDL2.dll .
```
The executable file maninminer.exe should now be available run.

## Windows 10

### Windows 10 Msys2 64 bit

Install msys2 latest version [https://www.msys2.org/](https://www.msys2.org/)

Run MSYS2 MINGW64

Update base system

```bash
pacman -Syu
```

Install development tools and libraries
```bash
pacman -S git make gcc mingw-w64-x86_64-SDL2 mingw-w64-x86_64-toolchain
git clone https://github.com/fawtytoo/ManicMiner.git
cd ManicMiner
make
```
copy SDL2.dll needed to run the game.
```bash
cp /mingw64/bin/SDL2.dll .
manicminer.exe
```

### Windows 10 Msys2 32 bit

run MSYS2 MINGW32

Update base system

```bash
pacman -Syu
```

Install development tools and libraries
```bash
pacman -S git make gcc mingw-w64-i686-SDL2 mingw-w64-i686-toolchain
git clone https://github.com/fawtytoo/ManicMiner.git
cd ManicMiner
make
```
copy SDL2.dll needed to run the game.
```bash
cp /mingw32/bin/SDL2.dll .
manicminer.exe
```