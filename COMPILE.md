# Compilation guide for Windows and Linux. 

## Linux

### Native Linux



```bash
git clone [https://github.com/fawtytoo/ManicMiner.git](https://github.com/fawtytoo/ManicMiner.git)
```

```bash
cd ManicMiner
make
manicminer
```

### Mingw for Linux

Next on the list to document.

## Windows

### Windows 64 bit

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

### Windows 32 bit
