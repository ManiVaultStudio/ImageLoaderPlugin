cmake_minimum_required(VERSION 3.1.0)

set(PROJECT "ImageLoaderPlugin")

PROJECT(${PROJECT})

set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/Modules/")
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(MSVC)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /DWIN32 /EHsc /MP")
	set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /LTCG /NODEFAULTLIB:LIBCMT")
	set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /MDd")
	set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /MD")
endif(MSVC)

find_package(Qt5 COMPONENTS Widgets WebEngineWidgets REQUIRED)

SET(FREEIMAGE_ROOT_DIR CACHE PATH "Location of FreeImage library")
find_package(FreeImage QUIET)

if (NOT FREEIMAGE_FOUND)
    message(WARNING "Failed to find FreeImage, not building ImageLoaderPlugin")
    return()
endif()

set(PLUGIN_SOURCES
    src/ImageLoaderPlugin.h
    src/ImageLoaderPlugin.cpp
	src/FreeImage.h
    src/FreeImage.cpp
	src/Payload.h 
	src/Payload.cpp
	src/Settings.h
	src/Settings.cpp
	src/Common.h
)

set(LOADER_SOURCES
	src/ImageCollection.h
	src/ImageCollection.cpp
	src/Scanned.h
	src/Scanned.cpp
	src/ImageCollectionLoader.h
	src/ImageCollectionLoader.cpp
)

set(SCANNER_SOURCES
	src/ImageCollectionScanner.h
	src/ImageCollectionScanner.cpp
)

set(WIDGET_SOURCES
	src/CommonSettingsWidget.h
    src/CommonSettingsWidget.cpp
	src/ImageLoaderDialog.h
    src/ImageLoaderDialog.cpp
	src/SubsampleSettingsWidget.h
	src/SubsampleSettingsWidget.cpp
)

file(GLOB MODEL_SOURCES src/*Model.* )

set(AUX src/ImageLoaderPlugin.json)

set(SOURCES 
	${PLUGIN_SOURCES}
	${LOADER_SOURCES}
	${SCANNER_SOURCES}
	${WIDGET_SOURCES}
	${MODEL_SOURCES}
)

file(GLOB UIS src/*.ui)

set(AUX src/ImageLoaderPlugin.json)

source_group(Plugin FILES ${PLUGIN_SOURCES} ${LOADER_SOURCES} ${SCANNER_SOURCES} ${AUX})
source_group(Widgets FILES ${WIDGET_SOURCES})
source_group(UI FILES ${UIS})
source_group(Model FILES ${MODEL_SOURCES})
source_group(Aux FILES ${AUX})

include_directories("$ENV{HDPS_INSTALL_DIR}/$<CONFIGURATION>/include/")
include_directories("${FREEIMAGE_INCLUDE_PATH}")
include_directories(src)

add_library(${PROJECT} SHARED ${SOURCES} ${UIS} ${AUX})

target_compile_features(${PROJECT} PRIVATE cxx_std_17)

add_definitions(-DDISABLE_PERF_MEASUREMENT)
 
target_link_libraries(${PROJECT} Qt5::Widgets)
target_link_libraries(${PROJECT} Qt5::WebEngineWidgets)

if(MSVC)
    set(LIB_SUFFIX "${CMAKE_STATIC_LIBRARY_SUFFIX}")
else()
    set(LIB_SUFFIX "${CMAKE_SHARED_LIBRARY_SUFFIX}")
endif()

target_link_libraries(${PROJECT} "$ENV{HDPS_INSTALL_DIR}/$<CONFIGURATION>/lib/${CMAKE_SHARED_LIBRARY_PREFIX}HDPS_Public${LIB_SUFFIX}")
target_link_libraries(${PROJECT} "$ENV{HDPS_INSTALL_DIR}/$<CONFIGURATION>/lib/${CMAKE_SHARED_LIBRARY_PREFIX}PointData${LIB_SUFFIX}")
target_link_libraries(${PROJECT} "$ENV{HDPS_INSTALL_DIR}/$<CONFIGURATION>/lib/${CMAKE_SHARED_LIBRARY_PREFIX}ImageData${LIB_SUFFIX}")

target_link_libraries(${PROJECT} "${FREEIMAGE_LIBRARY}")

if(MSVC)
    set(FreeImageName "FreeImage")
elseif(APPLE)
    set(FreeImageName "freeimage")
else()
    set(FreeImageName "freeimage-3.18.0")
endif()
    
add_custom_command(TARGET ${PROJECT} POST_BUILD
    COMMAND "${CMAKE_COMMAND}" -E copy_if_different
	"$<TARGET_FILE:${PROJECT}>"
	"$ENV{HDPS_INSTALL_DIR}/$<CONFIGURATION>/Plugins/$<TARGET_FILE_NAME:${PROJECT}>"
	COMMAND "${CMAKE_COMMAND}" -E copy_if_different
	"${FREEIMAGE_INCLUDE_PATH}/${CMAKE_SHARED_LIBRARY_PREFIX}${FreeImageName}${CMAKE_SHARED_LIBRARY_SUFFIX}"
	"$ENV{HDPS_INSTALL_DIR}/$<CONFIGURATION>/${CMAKE_SHARED_LIBRARY_PREFIX}${FreeImageName}${CMAKE_SHARED_LIBRARY_SUFFIX}"
	COMMENT "Copying shared DLLs to HDPS_INSTALL_DIR"
)