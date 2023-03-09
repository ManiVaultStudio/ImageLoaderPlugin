# CI-CD

Linux & Macos (Travis) | Windows (Appveyor)
--- | ---
[![Build Status](https://travis-ci.com/bldrvnlw/conan-ImageLoaderPlugin.svg?branch=master)](https://travis-ci.com/bldrvnlw/conan-ImageLoaderPlugin) | [![Build status](https://ci.appveyor.com/api/projects/status/l5d1vamvwo0aa3jq?svg=true)](https://ci.appveyor.com/project/bldrvnlw/conan-imageloaderplugin)


Pushing to the hdps/ImageLoaderPlugin develop wil trigger the conan CI_CD to start a build via the configured Webhook.

Currently the following build matrix is performed

OS | Architecture | Compiler
--- | --- | ---
Windows | x64 | MSVC 2017
Linux | x86_64 | gcc 9
Macos | x86_64 | clang 10

# ImageLoaderPlugin
Image loader plugin for the HDPS framework

## Local linux build

Tested with Ubuntu 22.10 and gcc 12.2. 
Make sure to install some additional dependencies:
```
apt install qt6-image-formats-plugins libfreeimage-dev
```
