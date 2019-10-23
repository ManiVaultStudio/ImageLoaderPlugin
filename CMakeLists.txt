cmake_minimum_required(VERSION 3.1.0)

set(PROJECT "ImageLoaderPlugin")

PROJECT(${PROJECT})

set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/Modules/")

if(MSVC)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /DWIN32 /EHsc /MP")
	set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /LTCG /NODEFAULTLIB:LIBCMT")
	set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /MDd")
	set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /MD")
endif(MSVC)

find_package(Qt5 COMPONENTS Widgets WebEngineWidgets REQUIRED)

find_package(FreeImage REQUIRED)

set(PLUGIN_SOURCES
    src/ImageLoaderPlugin.h
    src/ImageLoaderPlugin.cpp
	src/ImageLoaderDialog.h
    src/ImageLoaderDialog.cpp
	src/FreeImage.h
    src/FreeImage.cpp
	src/Images.h
	src/Images.cpp
    src/Common.h
)

set(LOADER_SOURCES
	src/ImageCollection.h
	src/ImageCollection.cpp
	src/ImageCollections.h
	src/ImageCollections.cpp
	src/ImageCollectionsLoader.h
	src/ImageCollectionsLoader.cpp
)

set(SCANNER_SOURCES
	src/ImageScanner.h
	src/ImageScanner.cpp
)

set(IMAGE_SEQUENCE_SOURCES
	src/ImageSequenceScanner.h
	src/ImageSequenceScanner.cpp
	src/ImageSequenceWidget.h
    src/ImageSequenceWidget.cpp
)

set(IMAGE_STACK_SOURCES
	src/ImageStackScanner.h
    src/ImageStackScanner.cpp
	src/ImageStackWidget.h
    src/ImageStackWidget.cpp
)

set(MULTI_PART_IMAGE_SEQUENCE_SOURCES
	src/MultiPartImageSequenceScanner.h
	src/MultiPartImageSequenceScanner.cpp
	src/MultiPartImageSequenceWidget.h
	src/MultiPartImageSequenceWidget.cpp
)

set(SUBSAMPLE_SETTINGS_SOURCES
	src/SubsampleSettings.h
	src/SubsampleSettings.cpp
	src/SubsampleSettingsWidget.h
	src/SubsampleSettingsWidget.cpp
)

set(COLOR_SETTINGS_SOURCES
	src/ColorSettings.h
	src/ColorSettings.cpp
	src/ColorSettingsWidget.h
	src/ColorSettingsWidget.cpp
)

set(AUX src/ImageLoaderPlugin.json)

set(SOURCES 
	${PLUGIN_SOURCES}
	${LOADER_SOURCES}
	${SCANNER_SOURCES}
	${IMAGE_SEQUENCE_SOURCES}
	${IMAGE_STACK_SOURCES}
	${MULTI_PART_IMAGE_SEQUENCE_SOURCES}
	${SUBSAMPLE_SETTINGS_SOURCES}
	${COLOR_SETTINGS_SOURCES}
)

file(GLOB UIS src/*.ui)

set(AUX src/ImageLoaderPlugin.json)

source_group(Plugin FILES ${PLUGIN_SOURCES} ${LOADER_SOURCES} ${SCANNER_SOURCES} ${AUX})
source_group(ImageSequence FILES ${IMAGE_SEQUENCE_SOURCES})
source_group(ImageStack FILES ${IMAGE_STACK_SOURCES})
source_group(MultiPartImageSequence FILES ${MULTI_PART_IMAGE_SEQUENCE_SOURCES})
source_group(UI FILES ${UIS})
source_group(SubsampleSettings FILES ${SUBSAMPLE_SETTINGS_SOURCES})
source_group(ColorSettings FILES ${COLOR_SETTINGS_SOURCES})
source_group(Aux FILES ${AUX})

include_directories("$ENV{HDPS_INSTALL_DIR}/$<CONFIGURATION>/include/")
include_directories("${FREEIMAGE_INCLUDE_PATH}")

add_library(${PROJECT} SHARED ${SOURCES} ${UIS} ${AUX})

target_compile_features(${PROJECT} PRIVATE cxx_std_17)

target_link_libraries(${PROJECT} Qt5::Widgets)
target_link_libraries(${PROJECT} Qt5::WebEngineWidgets)
target_link_libraries(${PROJECT} "$ENV{HDPS_INSTALL_DIR}/$<CONFIGURATION>/lib/HDPS_Public.lib")
target_link_libraries(${PROJECT} "$ENV{HDPS_INSTALL_DIR}/$<CONFIGURATION>/lib/ImageData.lib")
target_link_libraries(${PROJECT} "${FREEIMAGE_LIBRARY}")

add_custom_command(TARGET ${PROJECT} POST_BUILD
    COMMAND "${CMAKE_COMMAND}" -E copy_if_different
	"$<TARGET_FILE:${PROJECT}>"
	"$ENV{HDPS_INSTALL_DIR}/$<CONFIGURATION>/Plugins/$<TARGET_FILE_NAME:${PROJECT}>"
	COMMAND "${CMAKE_COMMAND}" -E copy_if_different
	"${FREEIMAGE_INCLUDE_PATH}/FreeImage.dll"
	"$ENV{HDPS_INSTALL_DIR}/$<CONFIGURATION>/FreeImage.dll"
	COMMENT "Copying shared DLLs to HDPS_INSTALL_DIR"
)