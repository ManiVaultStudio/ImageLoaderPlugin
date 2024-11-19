cmake_minimum_required(VERSION 3.21)

option(MV_UNITY_BUILD "Combine target source files into batches for faster compilation" OFF)

# -----------------------------------------------------------------------------
# ImageLoader Plugin
# -----------------------------------------------------------------------------
set(PROJECT "ImageLoaderPlugin")
PROJECT(${PROJECT})

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

include(FetchContent)

# -----------------------------------------------------------------------------
# Dependencies
# -----------------------------------------------------------------------------
find_package(Qt6 COMPONENTS Widgets WebEngineWidgets REQUIRED)
find_package(ManiVault COMPONENTS Core PointData ImageData CONFIG)

FetchContent_Declare(
    FreeImage
    GIT_REPOSITORY https://github.com/biovault/FreeImage
    GIT_TAG 7605c3f71aea57be61be3db73227b5baa445dbb7    # master as of 19-11-2024, version 3.18.0 with cmake files
    GIT_SHALLOW TRUE
)

FetchContent_MakeAvailable(FreeImage)

set_target_properties(FreeImage FreeImageLib LibJPEG LibJXR LibOpenJPEG LibPNG LibRaw LibTIFF4 LibWebP OpenEXR ZLibFreeImage 
    PROPERTIES 
    FOLDER ImageLoaderPluginDependencies
)

# Do not build static library
set_target_properties(FreeImageLib PROPERTIES EXCLUDE_FROM_ALL ON)

# Remove /permissive- for OpenEXR and FreeImage
if(MSVC)
    target_compile_options(OpenEXR PRIVATE /permissive)
    target_compile_options(FreeImage PRIVATE /permissive)
endif(MSVC)

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

target_link_libraries(${PROJECT} PRIVATE FreeImage)

# -----------------------------------------------------------------------------
# Target installation
# -----------------------------------------------------------------------------
install(TARGETS ${PROJECT}
    RUNTIME DESTINATION Plugins COMPONENT PLUGINS # Windows .dll
    LIBRARY DESTINATION Plugins COMPONENT PLUGINS # Linux/Mac .so
)

mv_install_dependencies(${PROJECT} "FreeImage")

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
