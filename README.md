
# ImageLoaderPlugin
Image loader plugin for the HDPS framework

## Local Windows build

Download [FreeImage 3.18.0](https://freeimage.sourceforge.io/download.html) and copy the files to this folder in the following manner for CMAKE to find it automatically:
```
├── .
│   ├── FreeImage
│   │   ├── bin
│   │   │   ├── FreeImage.dll
│   │   ├── include
│   │   │   ├── FreeImage.h
│   │   ├── lib
│   │   │   ├── FreeImage.lib
```

## Local Linux build

Tested with Ubuntu 22.10 and gcc 12.2. 
Make sure to install some additional dependencies:
```
apt install qt6-image-formats-plugins libfreeimage-dev
```

## CI-CD

Pushing to the hdps/ImageLoaderPlugin develop will trigger the conan CI_CD to start a build via the configured Webhook.

Currently the following build matrix is performed

OS | Architecture | Compiler
--- | --- | ---
Windows | x64 | MSVC 2019
Macos | x86_64 | clang 12
