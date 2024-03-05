cmake_minimum_required(VERSION 3.12.0)

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
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_CURRENT_SOURCE_DIR}/cmake/Modules/")
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(FREEIMAGE_ROOT_DIR "" CACHE FILEPATH "Path to the directory containing FreeImage .h/.lib/.dll files")

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

if(FREEIMAGE_ROOT_DIR STREQUAL "") 
    find_package(FreeImage CONFIG REQUIRED)
    set(FREEIMAGELINKLIB freeimage::FreeImage)
else()
    # when downloading the pre-compiled package manually
    find_package(FreeImage REQUIRED)
    set(FREEIMAGELINKLIB ${FREEIMAGE_LIBRARY})
endif()

message(STATUS "FreeImage found at ${FREEIMAGE_INCLUDE_PATH}, ${FREEIMAGE_LIBRARY}, FREEIMAGE_ROOT_DIR: ${FREEIMAGE_ROOT_DIR}, FREEIMAGELINKLIB: ${FREEIMAGELINKLIB}")

# -----------------------------------------------------------------------------
# Set install directory
# -----------------------------------------------------------------------------
# Check if the directory to the ManiVault installation has been provided
if(NOT DEFINED MV_INSTALL_DIR)
    set(MV_INSTALL_DIR "" CACHE PATH "Directory where ManiVault is installed")
    message(FATAL_ERROR "Please set MV_INSTALL_DIR to the directory where ManiVault is installed")
endif()
file(TO_CMAKE_PATH ${MV_INSTALL_DIR} MV_INSTALL_DIR)

message(STATUS "HDPS CORE in ${MV_INSTALL_DIR}")

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

qt_wrap_cpp(IMAGELOADER_MOC ${PLUGIN_MOC_HEADERS} TARGET ${PROJECT})
target_sources(${PROJECT} PRIVATE ${IMAGELOADER_MOC})

# -----------------------------------------------------------------------------
# Target include directories
# -----------------------------------------------------------------------------
target_include_directories(${PROJECT} PRIVATE "${MV_INSTALL_DIR}/$<CONFIGURATION>/include/")
target_include_directories(${PROJECT} PRIVATE "${FREEIMAGE_INCLUDE_PATH}")
target_include_directories(${PROJECT} PRIVATE src)

# -----------------------------------------------------------------------------
# Target properties
# -----------------------------------------------------------------------------
target_compile_features(${PROJECT} PRIVATE cxx_std_17)

add_definitions(-DDISABLE_PERF_MEASUREMENT)
 
# -----------------------------------------------------------------------------
# Target library linking
# -----------------------------------------------------------------------------
target_link_libraries(${PROJECT} PRIVATE Qt6::Widgets)
target_link_libraries(${PROJECT} PRIVATE Qt6::WebEngineWidgets)

set(MV_LINK_PATH "${MV_INSTALL_DIR}/$<CONFIGURATION>/lib")
set(PLUGIN_LINK_PATH "${MV_INSTALL_DIR}/$<CONFIGURATION>/$<IF:$<CXX_COMPILER_ID:MSVC>,lib,Plugins>")
set(MV_LINK_SUFFIX $<IF:$<CXX_COMPILER_ID:MSVC>,${CMAKE_LINK_LIBRARY_SUFFIX},${CMAKE_SHARED_LIBRARY_SUFFIX}>)

set(MV_LINK_LIBRARY "${MV_LINK_PATH}/${CMAKE_SHARED_LIBRARY_PREFIX}MV_Public${MV_LINK_SUFFIX}")
set(POINTDATA_LINK_LIBRARY "${PLUGIN_LINK_PATH}/${CMAKE_SHARED_LIBRARY_PREFIX}PointData${MV_LINK_SUFFIX}") 
set(IMAGEDATA_LINK_LIBRARY "${PLUGIN_LINK_PATH}/${CMAKE_SHARED_LIBRARY_PREFIX}ImageData${MV_LINK_SUFFIX}") 

target_link_libraries(${PROJECT} PRIVATE "${MV_LINK_LIBRARY}")
target_link_libraries(${PROJECT} PRIVATE "${POINTDATA_LINK_LIBRARY}")
target_link_libraries(${PROJECT} PRIVATE "${IMAGEDATA_LINK_LIBRARY}")

target_link_libraries(${PROJECT} PRIVATE "${FREEIMAGELINKLIB}")

# -----------------------------------------------------------------------------
# Target installation
# -----------------------------------------------------------------------------
install(TARGETS ${PROJECT}
    RUNTIME DESTINATION Plugins COMPONENT PLUGINS # Windows .dll
    LIBRARY DESTINATION Plugins COMPONENT PLUGINS # Linux/Mac .so
)

if(MSVC)
    # copy the FreeImage library to the root install directory (the same level as the ManiVault executable)
    set(FreeImageName "FreeImage")
    install(FILES
        "${FREEIMAGE_ROOT_DIR}/bin/${CMAKE_SHARED_LIBRARY_PREFIX}${FreeImageName}${CMAKE_SHARED_LIBRARY_SUFFIX}"
        DESTINATION "${MV_INSTALL_DIR}/$<CONFIGURATION>" COMPONENT IMPORTLIBS
    )
endif()

add_custom_command(TARGET ${PROJECT} POST_BUILD
    COMMAND "${CMAKE_COMMAND}"
    --install ${CMAKE_CURRENT_BINARY_DIR}
    --config $<CONFIGURATION>
    --prefix ${MV_INSTALL_DIR}/$<CONFIGURATION>
)

# -----------------------------------------------------------------------------
# Miscellaneous
# -----------------------------------------------------------------------------
# Automatically set the debug environment (command + working directory) for MSVC
if(MSVC)
    set_property(TARGET ${PROJECT} PROPERTY VS_DEBUGGER_WORKING_DIRECTORY $<IF:$<CONFIG:DEBUG>,${MV_INSTALL_DIR}/debug,${MV_INSTALL_DIR}/release>)
    set_property(TARGET ${PROJECT} PROPERTY VS_DEBUGGER_COMMAND $<IF:$<CONFIG:DEBUG>,"${MV_INSTALL_DIR}/debug/ManiVault Studio.exe","${MV_INSTALL_DIR}/release/ManiVault Studio.exe">)
endif()
