
# ImageLoaderPlugin ![Build Status](https://github.com/ManiVaultStudio/ImageLoaderPlugin/actions/workflows/build.yml/badge.svg?branch=master)
Image loader plugin for the [ManiVault](https://github.com/ManiVaultStudio/core) framework.

```bash
git clone https://github.com/ManiVaultStudio/ImageLoaderPlugin.git
```

## Building
This project depends on the freeimage library.

### Windows
Using a package manager: install freeimage with vcpkg or conan and specify the cmake toolchain accordingly.

Manual:
Download [FreeImage 3.18.0](https://freeimage.sourceforge.io/download.html), copy the files to this folder in the following manner, and define the cmake path variable `FREEIMAGE_ROOT_DIR` to `./FreeImage`:
```
.
├── FreeImage
│   ├── bin
│   │   ├── FreeImage.dll
│   ├── include
│   │   ├── FreeImage.h
│   ├── lib
│   │   ├── FreeImage.lib
```

### Linux
```
apt install qt6-image-formats-plugins libfreeimage-dev
```

### Mac
```
brew install freeimage
```