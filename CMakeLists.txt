cmake_minimum_required(VERSION 3.22)

option(MV_UNITY_BUILD "Combine target source files into batches for faster compilation" OFF)
option(MV_IML_USE_SYSTEM_FREEIMAGE "Use local freeimage installation instead of building it" OFF)

# -----------------------------------------------------------------------------
# ImageLoader Plugin
# -----------------------------------------------------------------------------
set(PROJECT "ImageLoaderPlugin")
PROJECT(${PROJECT} C CXX)

# -----------------------------------------------------------------------------
# CMake Options
# -----------------------------------------------------------------------------
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /DWIN32 /EHsc /MP /permissive- /Zc:__cplusplus")
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /MDd")
    set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "${CMAKE_CXX_FLAGS_RELWITHDEBINFO} /MD")
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /MD")
endif()

# -----------------------------------------------------------------------------
# Dependencies
# -----------------------------------------------------------------------------
find_package(Qt6 COMPONENTS Widgets WebEngineWidgets Concurrent REQUIRED)
find_package(ManiVault COMPONENTS Core PointData ImageData CONFIG QUIET)

if(MV_IML_USE_SYSTEM_FREEIMAGE)
    set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)
    find_package(FreeImage REQUIRED)
else()
    include(FetchContent)
    
    FetchContent_Declare(
        FreeImage
        GIT_REPOSITORY https://github.com/biovault/FreeImage
        GIT_TAG v3.19.0 # version 3.19.0.1, freeimage + cmake files + some updated modules
    )

    FetchContent_MakeAvailable(FreeImage)

    get_target_property(FreeImageVersion FreeImageLib VERSION)
    message(STATUS "FreeImage Version: ${FreeImageVersion}")

    set_target_properties(FreeImage FreeImageLib LibJPEG LibJXR LibOpenJPEG LibPNG LibRaw LibTIFF4 LibWebP OpenEXR ZLibFreeImage 
        PROPERTIES 
        FOLDER LoaderPlugins/Dependencies/ImageLoaderPlugin
    )

    # Do not build static library
    set_target_properties(FreeImageLib PROPERTIES EXCLUDE_FROM_ALL ON)

    # Remove /permissive- for OpenEXR and FreeImage
    if(CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
        target_compile_options(OpenEXR PRIVATE /permissive)
        target_compile_options(FreeImage PRIVATE /permissive)
        target_compile_options(FreeImageLib PRIVATE /permissive) # we don't need the static library, but just in case someone does build it
    endif()
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
    src/OmeHelper.h
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
    src/MirrorAction.h
    src/MirrorAction.cpp
)

set(AUX 
    PluginInfo.json
)

set(SOURCES 
    ${PLUGIN_SOURCES}
    ${WIDGET_SOURCES}
    ${ACTION_SOURCES}
)

source_group(Plugin FILES ${PLUGIN_SOURCES})
source_group(Widgets FILES ${WIDGET_SOURCES})
source_group(Actions FILES ${ACTION_SOURCES})
source_group(Aux FILES ${AUX})

# -----------------------------------------------------------------------------
# CMake Target
# -----------------------------------------------------------------------------
add_library(${PROJECT} SHARED ${SOURCES} ${AUX})

# -----------------------------------------------------------------------------
# Target include directories
# -----------------------------------------------------------------------------
target_include_directories(${PROJECT} PRIVATE "${ManiVault_INCLUDE_DIR}")

# -----------------------------------------------------------------------------
# Target properties
# -----------------------------------------------------------------------------
target_compile_features(${PROJECT} PRIVATE cxx_std_20)

target_compile_definitions(${PROJECT} PRIVATE DISABLE_PERF_MEASUREMENT)
 
if(MV_UNITY_BUILD)
    set_target_properties(${PROJECT} PROPERTIES UNITY_BUILD ON)
endif()

# -----------------------------------------------------------------------------
# Target library linking
# -----------------------------------------------------------------------------
target_link_libraries(${PROJECT} PRIVATE Qt6::Widgets)
target_link_libraries(${PROJECT} PRIVATE Qt6::WebEngineWidgets)
target_link_libraries(${PROJECT} PRIVATE Qt6::Concurrent)

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

mv_handle_plugin_config(${PROJECT})

# -----------------------------------------------------------------------------
# Miscellaneous
# -----------------------------------------------------------------------------
# Automatically set the debug environment (command + working directory) for MSVC
if(CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    set_property(TARGET ${PROJECT} PROPERTY VS_DEBUGGER_WORKING_DIRECTORY $<IF:$<CONFIG:DEBUG>,${ManiVault_INSTALL_DIR}/Debug,$<IF:$<CONFIG:RELWITHDEBINFO>,${ManiVault_INSTALL_DIR}/RelWithDebInfo,${ManiVault_INSTALL_DIR}/Release>>)
    set_property(TARGET ${PROJECT} PROPERTY VS_DEBUGGER_COMMAND $<IF:$<CONFIG:DEBUG>,"${ManiVault_INSTALL_DIR}/Debug/ManiVault Studio.exe",$<IF:$<CONFIG:RELWITHDEBINFO>,"${ManiVault_INSTALL_DIR}/RelWithDebInfo/ManiVault Studio.exe","${ManiVault_INSTALL_DIR}/Release/ManiVault Studio.exe">>)
endif()
