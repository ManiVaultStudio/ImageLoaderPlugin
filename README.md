# ImageLoaderPlugin ![Build Status](https://github.com/ManiVaultStudio/ImageLoaderPlugin/actions/workflows/build.yml/badge.svg?branch=master)
Image loader plugin for the [ManiVault](https://github.com/ManiVaultStudio/core) framework.

```bash
git clone https://github.com/ManiVaultStudio/ImageLoaderPlugin.git
```

<p align="middle">
  <img src="https://github.com/ManiVaultStudio/ImageLoaderPlugin/assets/58806453/bca56677-847a-493a-a1c2-980f2130c136" align="middle"  width="80%" />
</p>

## Features 
- Overview of image information for all image files in a given directory:
  - Size, number of points (pixels), channels and more
  - Filter images by name
- Edit meta data like data set and dimension names
- Select which dimensions are to be loaded
- Multiple image formats: PNG, JPEG, BPM and TIF/TIFF
  - RGB images
  - "High-dimensional" images, i.e. more than three values per pixel. All grayscale (single channel) image files in a folder are interpreted as part of one single image, e.g. the files in `./data/images/stack/sun/`
  - Layered Tiff: Dimension (channel) names might be stored in one of several tags. Select "PageName", "ImageDescription" or a custom tag to automatically retrieve them
- Data layout interpretation:
  - Image stack (default): All same-sized, grayscale images in a folder are layered into a single image data set. Pixels at same coordinates from each layer together form a (high-dimensional) data point. Given 100 images of 28x28 pixels this loader creates a dataset of 28x28=784 data points with 100 dimensions (values per pixel). The image dimensions is 28x28 pixel.
  - Image sequence: Each same-size, grayscale image in a folder is considered a data point and each pixel is considered a dimension. Given 100 images of 28x28 pixels this loader creates a dataset of 100 data points with 28x28=784 dimensions.
- Apply data conversion:
  - If a transformation plugin is installed, like the [PointDataConversionPlugin](https://github.com/ManiVaultStudio/PointDataConversionPlugin), you can apply the transformation that it provided upon loading, e.g. `log2` conversions.
- Subsample images:
  - Resize an image with linear, bilinear, B-spline, bicubic, Catmull-Rom or Lanczos interpolation filter
  - Create a image pyramid of user-defined size by downsampling the image on every level 

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
