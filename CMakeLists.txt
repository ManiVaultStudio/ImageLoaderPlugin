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

set(SOURCES
    src/ImageLoader.h
    src/ImageLoader.cpp
	src/ImageSequence.h
    src/ImageSequence.cpp
	src/ImageStack.h
    src/ImageStack.cpp
	src/ImageLoaderDialog.h
    src/ImageLoaderDialog.cpp
	src/ImageSequenceWidget.h
    src/ImageSequenceWidget.cpp
	src/ImageStackWidget.h
    src/ImageStackWidget.cpp
)

set(UIS 
	src/ImageLoaderDialog.ui
	src/ImageSequenceWidget.ui
	src/ImageStackWidget.ui
)

set(AUX src/ImageLoader.json)

source_group(Plugin FILES ${SOURCES} ${AUX})

include_directories("$ENV{HDPS_INSTALL_DIR}/$<CONFIGURATION>/include/")
include_directories("${FREEIMAGE_INCLUDE_PATH}")

add_library(${PROJECT} SHARED ${SOURCES} ${UIS})

target_link_libraries(${PROJECT} Qt5::Widgets)
target_link_libraries(${PROJECT} Qt5::WebEngineWidgets)
target_link_libraries(${PROJECT} "$ENV{HDPS_INSTALL_DIR}/$<CONFIGURATION>/lib/HDPS_Public.lib")
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