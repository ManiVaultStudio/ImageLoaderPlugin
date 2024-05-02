
# ImageLoaderPlugin ![Build Status](https://github.com/ManiVaultStudio/ImageLoaderPlugin/actions/workflows/build.yml/badge.svg?branch=master)
Image loader plugin for the [ManiVault](https://github.com/ManiVaultStudio/core) framework.

```bash
git clone https://github.com/ManiVaultStudio/ImageLoaderPlugin.git
```

<p align="middle">
  <img src="https://github.com/ManiVaultStudio/ImageLoaderPlugin/assets/58806453/bca56677-847a-493a-a1c2-980f2130c136" align="middle"  width="80%" />
</p>

## Features 

- Multiple image formats: PNG, JPEG, BPM, (single page) TIF and more
- Data layout interpretation:
  - Image sequence: Each (single channel) image in a folder is interpreted as an image channel, i.e., given 100 images of 28x28 pixels a dataset of 28x28=784 data points with 100 dimensions (values per pixel) is loaded. The image dimensions is 28x28 pixel.
  - Image stack: Each pixel of all (single channel) images in a folder are interpreted as an image channel, i.e., given 100 images of 28x28 pixels a dataset of 100 data points with 28x28=784 dimensions is loaded. The image dimensions remain 28x28 pixel.
- Apply data conversion
- Subsample images 

## Building
This project depends on the [freeimage](https://freeimage.sourceforge.io/) library. Ensure that CMake can find freeimage during configuration. 

### Windows
Using a package manager: install freeimage with [vcpkg](https://github.com/microsoft/vcpkg/) or [conan](https://conan.io/) and specify the [cmake toolchain](https://cmake.org/cmake/help/latest/manual/cmake-toolchains.7.html) accordingly.

Manual:
Download [FreeImage 3.18.0](https://freeimage.sourceforge.io/download.html), arrange the files in the following manner in this repo's directory, and set the CMake variable `FREEIMAGE_ROOT_DIR` to `./FreeImage`:
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
