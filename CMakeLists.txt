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

if (DEFINED ENV{CI})
  set(temp_prefix_path ${CMAKE_PREFIX_PATH})
  message(STATUS "vcpkg search paths: ${CMAKE_PREFIX_PATH}")
  foreach(pkgpath ${temp_prefix_path})
    file(GLOB_RECURSE ALL_FILES CONFIGURE_DEPENDS "${pkgpath}/*")
    list(APPEND CMAKE_PREFIX_PATH "${pkgpath}/cmake/mv")
        message(STATUS "CMAKE_PREFIX_PATH ${CMAKE_PREFIX_PATH}")
  endforeach()
  set(VCPKG_TRACE_FIND_PACKAGE ON)
endif()
# -----------------------------------------------------------------------------
# Dependencies
# -----------------------------------------------------------------------------
find_package(Qt6 COMPONENTS Widgets WebEngineWidgets Concurrent REQUIRED)
find_package(ManiVault COMPONENTS Core PointData ImageData CONFIG QUIET REQUIRED)

# Add the core cmake scripts - need this on the CI with vcpkg
# Note the core is called mv-core (due tocore being a vcpkg reserved name)
# TODO find a better mechanism
if (DEFINED ENV{CI})
  message(STATUS "ManiVault installed ${ManiVault_INSTALL_DIR}") 
  set(ManiVault_DIR "${ManiVault_INSTALL_DIR}/cmake/mv")
  message(STATUS "ManiVault core at ${ManiVault_DIR}")
  include("${ManiVault_DIR}/mv_install_dependencies_utils.cmake")
  include("${ManiVault_DIR}/mv_plugin_meta_data.cmake")
  #include("${ManiVault_DIR}/Utils.cmake") # For mv_set_version
  #set(ManiVault_CMAKE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/external/mv-core/ManiVault/cmake" CACHE INTERNAL "")
  #mv_set_version() # For MV_VERSION
endif()

if(MV_IML_USE_SYSTEM_FREEIMAGE)
    set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)
    find_package(FreeImage REQUIRED)
else()

    find_package(freeimage CONFIG REQUIRED)
    message(STATUS "FreeImage Version: ${FreeImage_VERSION}")
    #set_target_properties(freeimage::freeimage LibJPEG LibJXR LibOpenJPEG LibPNG LibRaw LibTIFF4 LibWebP OpenEXR ZLibFreeImage 
    #    PROPERTIES 
    #    FOLDER LoaderPlugins/Dependencies/ImageLoaderPlugin
    #)

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

if(APPLE)
    set_target_properties(${PROJECT} PROPERTIES
        INSTALL_RPATH "@executable_path/../Frameworks"
        INSTALL_RPATH_USE_LINK_PATH TRUE
    )
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

target_link_libraries(${PROJECT} PRIVATE freeimage::freeimage-static)

# -----------------------------------------------------------------------------
# Target installation
# -----------------------------------------------------------------------------
install(TARGETS ${PROJECT}
    RUNTIME DESTINATION Plugins COMPONENT PLUGINS # Windows .dll
    LIBRARY DESTINATION Plugins COMPONENT PLUGINS # Linux/Mac .so
)



# TODO fix the install - for some reason the 
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
