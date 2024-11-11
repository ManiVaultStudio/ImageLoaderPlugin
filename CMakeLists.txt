cmake_minimum_required(VERSION 3.21)

option(MV_UNITY_BUILD "Combine target source files into batches for faster compilation" OFF)

if(DEFINED CMAKE_TOOLCHAIN_FILE AND CMAKE_TOOLCHAIN_FILE MATCHES "vcpkg")
    set(MV_IMGLOADER_USE_VCPKG ON)
    set(VCPKG_LIBRARY_LINKAGE "dynamic" CACHE STRING "Link vcpkg libraries dynamically")
else()
    set(FREEIMAGE_ROOT_DIR "" CACHE FILEPATH "Path to the directory containing FreeImage .h/.lib/.dll files")
    set(MV_IMGLOADER_USE_VCPKG OFF)
endif()

# -----------------------------------------------------------------------------
# ImageLoader Plugin
# -----------------------------------------------------------------------------
set(PROJECT "ImageLoaderPlugin")
PROJECT(${PROJECT})

if(DEFINED MV_IMGLOADER_USE_VCPKG AND NOT MV_IMGLOADER_USE_VCPKG)
    # vcpkg is used together with conan on ci
    if(DEFINED VCPKG_TARGET_TRIPLET)
        set(MV_IMGLOADER_USE_VCPKG ON)
    else()
        set(MV_IMGLOADER_USE_VCPKG OFF)
    endif()
else()
    set(MV_IMGLOADER_USE_VCPKG OFF)
endif()

# -----------------------------------------------------------------------------
# CMake Options
# -----------------------------------------------------------------------------
set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(MSVC)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /DWIN32 /EHsc /MP /permissive- /Zc:__cplusplus")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /LTCG /NODEFAULTLIB:LIBCMT")
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /MDd")
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /MD")
endif(MSVC)

# -----------------------------------------------------------------------------
# Dependencies
# -----------------------------------------------------------------------------
find_package(Qt6 COMPONENTS Widgets WebEngineWidgets REQUIRED)
find_package(ManiVault COMPONENTS Core PointData ImageData CONFIG)

if(${MV_IMGLOADER_USE_VCPKG})
    find_package(FreeImage CONFIG REQUIRED)
    set(FreeImageTarget freeimage::FreeImage)
else()
    find_package(FreeImage REQUIRED)
    message(STATUS "FreeImage found at ${FREEIMAGE_INCLUDE_PATH}, ${FREEIMAGE_LIBRARY}, ${FREEIMAGE_LIBRARIES} FREEIMAGE_ROOT_DIR: ${FREEIMAGE_ROOT_DIR}")
    set(FreeImageTarget "${FREEIMAGE_LIBRARY}")
endif()



# -----------------------------------------------------------------------------
# Source files
# -----------------------------------------------------------------------------
set(PLUGIN_SOURCES
    src/ImageLoaderPlugin.h
    src/ImageLoaderPlugin.cpp
    src/TreeItem.h
    src/TreeItem.cpp
    src/ImageCollectionScanner.h
    src/ImageCollectionScanner.cpp
    src/ImageCollection.h
    src/ImageCollection.cpp
    src/ImageCollectionsModel.h
    src/ImageCollectionsModel.cpp
    src/Common.h
)

set(PLUGIN_MOC_HEADERS
    src/ImageLoaderPlugin.h
)

set(WIDGET_SOURCES
    src/ImageLoaderDialog.h
    src/ImageLoaderDialog.cpp
    src/SanitizeDataDialog.h
    src/SanitizeDataDialog.cpp
)

set(ACTION_SOURCES
    src/ScanAction.h
    src/ScanAction.cpp
    src/SubsamplingAction.h
    src/SubsamplingAction.cpp
    src/ImageCollectionsAction.h
    src/ImageCollectionsAction.cpp
    src/ImagesAction.h
    src/ImagesAction.cpp
    src/DimensionTagAction.h
    src/DimensionTagAction.cpp
    src/DataLayoutAction.h
    src/DataLayoutAction.cpp
    src/SubsamplingTypeAction.h
    src/SubsamplingTypeAction.cpp
    src/LevelsAction.h
    src/LevelsAction.cpp
    src/SubsamplingRatioAction.h
    src/SubsamplingRatioAction.cpp
    src/SubsamplingFilterTypeAction.h
    src/SubsamplingFilterTypeAction.cpp
    src/DatasetNameAction.h
    src/DatasetNameAction.cpp
    src/ConversionAction.h
    src/ConversionAction.cpp
    src/GroupDataAction.h
    src/GroupDataAction.cpp
    src/AddCoordinatesPointsAction.h
    src/AddCoordinatesPointsAction.cpp
)

set(AUX src/ImageLoaderPlugin.json)

file(GLOB UIS src/*.ui)

set(AUX src/ImageLoaderPlugin.json)

set(SOURCES 
    ${PLUGIN_SOURCES}
    ${LOADER_SOURCES}
    ${SCANNER_SOURCES}
    ${WIDGET_SOURCES}
    ${MODEL_SOURCES}
    ${ACTION_SOURCES}
)

source_group(Plugin FILES ${PLUGIN_SOURCES} ${AUX})
source_group(Widgets FILES ${WIDGET_SOURCES})
source_group(Actions FILES ${ACTION_SOURCES})
source_group(UI FILES ${UIS})
source_group(Aux FILES ${AUX})

QT6_WRAP_UI(UI_HEADERS ${UIS})

# -----------------------------------------------------------------------------
# CMake Target
# -----------------------------------------------------------------------------
add_library(${PROJECT} SHARED ${SOURCES} ${UIS} ${AUX})

# -----------------------------------------------------------------------------
# Target include directories
# -----------------------------------------------------------------------------
target_include_directories(${PROJECT} PRIVATE "${ManiVault_INCLUDE_DIR}")
target_include_directories(${PROJECT} PRIVATE src)

if(NOT ${MV_IMGLOADER_USE_VCPKG})
    target_include_directories(${PROJECT} PRIVATE "${FREEIMAGE_INCLUDE_PATH}")
endif()

# -----------------------------------------------------------------------------
# Target properties
# -----------------------------------------------------------------------------
target_compile_features(${PROJECT} PRIVATE cxx_std_17)

target_compile_definitions(${PROJECT} PRIVATE DISABLE_PERF_MEASUREMENT)
 
if(MV_UNITY_BUILD)
    set_target_properties(${PROJECT} PROPERTIES UNITY_BUILD ON)
endif()

# -----------------------------------------------------------------------------
# Target library linking
# -----------------------------------------------------------------------------
target_link_libraries(${PROJECT} PRIVATE Qt6::Widgets)
target_link_libraries(${PROJECT} PRIVATE Qt6::WebEngineWidgets)

target_link_libraries(${PROJECT} PRIVATE ManiVault::Core)
target_link_libraries(${PROJECT} PRIVATE ManiVault::PointData)
target_link_libraries(${PROJECT} PRIVATE ManiVault::ImageData)

target_link_libraries(${PROJECT} PRIVATE ${FreeImageTarget})

# -----------------------------------------------------------------------------
# Target installation
# -----------------------------------------------------------------------------
install(TARGETS ${PROJECT}
    RUNTIME DESTINATION Plugins COMPONENT PLUGINS # Windows .dll
    LIBRARY DESTINATION Plugins COMPONENT PLUGINS # Linux/Mac .so
)

if(FREEIMAGE_ROOT_DIR)
    set(DEPENDENCIES_FOLDERS "${FREEIMAGE_ROOT_DIR}/bin")
endif()
mv_install_dependencies(${PROJECT})

add_custom_command(TARGET ${PROJECT} POST_BUILD
    COMMAND "${CMAKE_COMMAND}"
    --install ${CMAKE_CURRENT_BINARY_DIR}
    --config $<CONFIGURATION>
    --prefix ${ManiVault_INSTALL_DIR}/$<CONFIGURATION>
)

# -----------------------------------------------------------------------------
# Miscellaneous
# -----------------------------------------------------------------------------
# Automatically set the debug environment (command + working directory) for MSVC
if(MSVC)
    set_property(TARGET ${PROJECT} PROPERTY VS_DEBUGGER_WORKING_DIRECTORY $<IF:$<CONFIG:DEBUG>,${ManiVault_INSTALL_DIR}/debug,${ManiVault_INSTALL_DIR}/release>)
    set_property(TARGET ${PROJECT} PROPERTY VS_DEBUGGER_COMMAND $<IF:$<CONFIG:DEBUG>,"${ManiVault_INSTALL_DIR}/debug/ManiVault Studio.exe","${ManiVault_INSTALL_DIR}/release/ManiVault Studio.exe">)
endif()
