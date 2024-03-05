from conans import ConanFile
from conan.tools.cmake import CMakeDeps, CMake, CMakeToolchain
from conans.tools import save, load
import os
import shutil
import pathlib
import traceback
import subprocess
import sys
from rules_support import PluginBranchInfo


class ImageLoaderPluginConan(ConanFile):
    """Class to package ImageLoaderPlugin using conan

    Packages both RELEASE and DEBUG.
    Uses rules_support (github.com/ManiVaultStudio/rulessupport) to derive
    versioninfo based on the branch naming convention
    as described in https://github.com/ManiVaultStudio/core/wiki/Branch-naming-rules
    """

    name = "ImageLoaderPlugin"
    description = (
        "A plugin for viewing image data ManiVaultStudio."
    )
    topics = ("hdps", "ManiVaultStudio", "plugin", "image data", "loading")
    url = "https://github.com/ManiVaultStudio/ImageLoaderPlugin"
    author = "B. van Lew b.van_lew@lumc.nl"  # conan recipe author
    license = "MIT"

    short_paths = True
    generators = "CMakeDeps"

    # Options may need to change depending on the packaged library
    settings = {"os": None, "build_type": None, "compiler": None, "arch": None}
    options = {"shared": [True, False], "fPIC": [True, False]}
    default_options = {"shared": True, "fPIC": True}

    # Qt requirement is inherited from hdps-core
    #requires = ("zlib/1.3", "libtiff/4.6.0", "freeimage/3.18.0")
    requires = (
        "zlib/1.3", 
        "zstd/1.5.5", 
        "xz_utils/5.4.5", 
        "libwebp/1.3.2", 
        "libjpeg/9e", 
        "libdeflate/1.18", 
        "jbig/20160605", 
        "libtiff/4.6.0",
        "freeimage/3.18.0")

    scm = {
        "type": "git",
        "subfolder": "hdps/ImageLoaderPlugin",
        "url": "auto",
        "revision": "auto",
    }

    def __get_git_path(self):
        path = load(
            pathlib.Path(pathlib.Path(__file__).parent.resolve(), "__gitpath.txt")
        )
        print(f"git info from {path}")
        return path

    def export(self):
        print("In export")
        # save the original source path to the directory used to build the package
        save(
            pathlib.Path(self.export_folder, "__gitpath.txt"),
            str(pathlib.Path(__file__).parent.resolve()),
        )

    def set_version(self):
        # Assign a version from the branch name
        branch_info = PluginBranchInfo(self.recipe_folder)
        # print(f"Version from branch {branch_info.version}")
        self.version = branch_info.version

    def requirements(self):
        branch_info = PluginBranchInfo(self.__get_git_path())
        print(f"Core requirement {branch_info.core_requirement}")
        self.requires(branch_info.core_requirement)

    # Remove runtime and use always default (MD/MDd)
    def configure(self):
        self.options["libtiff"].shared = True
        self.options["libtiff"].compiler.cppstd = 17
#        self.options["libtiff"].fPIC = True
        self.options["jbig"].shared = True
#        self.options["jbig"].fPIC = True
        self.options["libdeflate"].shared = True
#        self.options["libdeflate"].fPIC = False
        self.options["libjpeg"].shared = True
#        self.options["libjpeg"].fPIC = False
        self.options["libwebp"].shared = True
#        self.options["libwebp"].fPIC = False
        self.options["xz_utils"].shared = True
        self.options["zstd"].shared = True
#        self.options["zstd"].fPIC = False
        self.options["zlib"].shared = True
#        self.options["zlib"].fPIC = True
        self.options["freeimage"].shared = True
#        self.options["freeimage"].fPIC = False
        del self.options.fPIC
        self.settings.compiler.cppstd = "17"

    def system_requirements(self):
        #  May be needed for macOS or Linux
        pass

    def config_options(self):
        pass
        #print(f"self.options.shared {self.options.shared}")
        #print(f"self.options.fPIC {self.options.fPIC}")

    def generate(self):
        generator = None
        if self.settings.os == "Macos":
            generator = "Xcode"
        if self.settings.os == "Linux":
            generator = "Ninja Multi-Config"
        # Use the Qt provided .cmake files
        qtpath = pathlib.Path(self.deps_cpp_info["qt"].rootpath)
        qt_root = str(list(qtpath.glob("**/Qt6Config.cmake"))[0].parents[3].as_posix())

        tc = CMakeToolchain(self, generator=generator)
        if self.settings.os == "Windows" and self.options.shared:
            tc.variables["CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS"] = True
        if self.settings.os == "Linux" or self.settings.os == "Macos":
            tc.variables["CMAKE_CXX_STANDARD_REQUIRED"] = "ON"
        tc.variables["CMAKE_PREFIX_PATH"] = qt_root
        tc.variables["FREEIMAGE_ROOT_DIR"] = pathlib.Path(self.deps_cpp_info["freeimage"].rootpath).as_posix()
        
        # Set the installation directory for ManiVault based on the MV_INSTALL_DIR environment variable
        # or if none is specified, set it to the build/install dir.
        if not os.environ.get("MV_INSTALL_DIR", None):
            os.environ["MV_INSTALL_DIR"] = os.path.join(self.build_folder, "install")
        print("MV_INSTALL_DIR: ", os.environ["MV_INSTALL_DIR"])
        self.install_dir = pathlib.Path(os.environ["MV_INSTALL_DIR"]).as_posix()
        # Give the installation directory to CMake
        tc.variables["MV_INSTALL_DIR"] = self.install_dir
        
        # Use try except to help debugging
        try:
            tc.generate()
        except KeyError as e:
            print("Exception!", sys.exc_info()[0])
            print(e)
            traceback.print_exc()
            raise e

    def _configure_cmake(self):
        cmake = CMake(self)
        cmake.configure(build_script_folder="hdps/ImageLoaderPlugin")
        cmake.verbose = True
        return cmake

    def build(self):
        print(f"Build OS is : {self.settings.os} version: {self.version}")

        # We package FreeImage in separate include, lib and bin directories
        # need to copy dll to allow the cmake copy
        fi_pkg_bin = self.deps_cpp_info["freeimage"].bin_paths[0]
        fi_pkg_inc = self.deps_cpp_info["freeimage"].include_paths[0]
        if self.settings.os == "Windows":
            shutil.copyfile(
                os.path.join(fi_pkg_bin, "FreeImage.dll"),
                os.path.join(fi_pkg_inc, "FreeImage.dll"),
            )
        elif self.settings.os == "Linux":
            shutil.copyfile(
                os.path.join(fi_pkg_bin, "libfreeimage-3.18.0.so"),
                os.path.join(fi_pkg_inc, "libfreeimage-3.18.0.so"),
            )
        else:
            shutil.copyfile(
                os.path.join(fi_pkg_bin, "libfreeimage.dylib"),
                os.path.join(fi_pkg_inc, "libfreeimage.dylib"),
            )

        # The ImageLoaderPlugin build expects the HDPS package to be in this install dir
        hdps_pkg_root = self.deps_cpp_info["hdps-core"].rootpath
        print("Install dir type: ", self.install_dir)
        shutil.copytree(hdps_pkg_root, self.install_dir)

        cmake = self._configure_cmake()
        cmake.build(build_type="Debug")
        cmake.install(build_type="Debug")

        # cmake_release = self._configure_cmake()
        cmake.build(build_type="Release")
        cmake.install(build_type="Release")

    def package(self):
        package_dir = os.path.join(self.build_folder, "package")
        print("Packaging install dir: ", package_dir)
        subprocess.run(
            [
                "cmake",
                "--install",
                self.build_folder,
                "--config",
                "Debug",
                "--prefix",
                os.path.join(package_dir, "Debug"),
            ]
        )
        subprocess.run(
            [
                "cmake",
                "--install",
                self.build_folder,
                "--config",
                "Release",
                "--prefix",
                os.path.join(package_dir, "Release"),
            ]
        )
        self.copy(pattern="*", src=package_dir)

    def package_info(self):
        self.cpp_info.debug.libdirs = ["Debug/lib"]
        self.cpp_info.debug.bindirs = ["Debug/Plugins", "Debug"]
        self.cpp_info.debug.includedirs = ["Debug/include", "Debug"]
        self.cpp_info.release.libdirs = ["Release/lib"]
        self.cpp_info.release.bindirs = ["Release/Plugins", "Release"]
        self.cpp_info.release.includedirs = ["Release/include", "Release"]
