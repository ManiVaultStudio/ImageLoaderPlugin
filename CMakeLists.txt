cmake_minimum_required(VERSION 3.1.0)

set(PROJECT "ImageLoaderPlugin")

PROJECT(${PROJECT})

set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_CURRENT_SOURCE_DIR}/cmake/Modules/")
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(FREEIMAGE_ROOT_DIR ${CMAKE_SOURCE_DIR}/FreeImage CACHE FILEPATH "Path to the directory containing FreeImage .h/.lib/.dll files")

if(MSVC)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /DWIN32 /EHsc /MP")
	set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /LTCG /NODEFAULTLIB:LIBCMT")
	set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /MDd")
	set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /MD")
endif(MSVC)

find_package(Qt5 COMPONENTS Widgets WebEngineWidgets REQUIRED)
# Normalize the incoming install path
file(TO_CMAKE_PATH $ENV{HDPS_INSTALL_DIR} INSTALL_DIR)

SET(FREEIMAGE_ROOT_DIR CACHE PATH "Location of FreeImage library")
find_package(FreeImage QUIET)

if (NOT FREEIMAGE_FOUND)
    message(FATAL_ERROR "Failed to find FreeImage, not building ImageLoaderPlugin")
    return()
endif()

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

set(WIDGET_SOURCES
	src/CommonSettingsWidget.h
    src/CommonSettingsWidget.cpp
	src/ImageLoaderDialog.h
    src/ImageLoaderDialog.cpp
	src/SubsampleSettingsWidget.h
	src/SubsampleSettingsWidget.cpp
    src/SanitizeDataDialog.h
	src/SanitizeDataDialog.cpp
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
)

source_group(Plugin FILES ${PLUGIN_SOURCES} ${AUX})
source_group(Widgets FILES ${WIDGET_SOURCES})
source_group(UI FILES ${UIS})
source_group(Aux FILES ${AUX})

message(STATUS "HDPS CORE in ${INSTALL_DIR}")
include_directories("${INSTALL_DIR}/$<CONFIGURATION>/include/")
include_directories("${FREEIMAGE_INCLUDE_PATH}")
include_directories(src)

QT5_WRAP_UI(UI_HEADERS ${UIS})

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

target_link_libraries(${PROJECT} "${INSTALL_DIR}/$<CONFIGURATION>/lib/${CMAKE_SHARED_LIBRARY_PREFIX}HDPS_Public${LIB_SUFFIX}")
target_link_libraries(${PROJECT} "${INSTALL_DIR}/$<CONFIGURATION>/lib/${CMAKE_SHARED_LIBRARY_PREFIX}PointData${LIB_SUFFIX}")
target_link_libraries(${PROJECT} "${INSTALL_DIR}/$<CONFIGURATION>/lib/${CMAKE_SHARED_LIBRARY_PREFIX}ImageData${LIB_SUFFIX}")

target_link_libraries(${PROJECT} "${FREEIMAGE_LIBRARY}")

if(MSVC)
    set(FreeImageName "FreeImage")
elseif(APPLE)
    set(FreeImageName "freeimage")
else()
    set(FreeImageName "freeimage-3.18.0")
endif()

file(GET_RUNTIME_DEPENDENCIES
	LIBRARIES "${FREEIMAGE_INCLUDE_PATH}/${CMAKE_SHARED_LIBRARY_PREFIX}${FreeImageName}${CMAKE_SHARED_LIBRARY_SUFFIX}"
	RESOLVED_DEPENDENCIES_VAR r_deps
	UNRESOLVED_DEPENDENCIES_VAR u_deps) 

message(STATUS, "Dependencies ${r_deps}")

install(TARGETS ${PROJECT}
   RUNTIME DESTINATION Plugins COMPONENT SHAREDLIB
   LIBRARY DESTINATION Plugins COMPONENT LINKLIB
   ARCHIVE DESTINATION lib COMPONENT LINKLIB
)

# copy the FreeImage library to the root install directory (the same level as the hdps executable)
install(FILES
	"${FREEIMAGE_INCLUDE_PATH}/${CMAKE_SHARED_LIBRARY_PREFIX}${FreeImageName}${CMAKE_SHARED_LIBRARY_SUFFIX}"
	DESTINATION "${INSTALL_DIR}/$<CONFIGURATION>" COMPONENT IMPORTLIBS
)

add_custom_command(TARGET ${PROJECT} POST_BUILD
	COMMAND "${CMAKE_COMMAND}"
	--install ${CMAKE_BINARY_DIR}
	--config $<CONFIGURATION>
	--prefix ${INSTALL_DIR}/$<CONFIGURATION>
)
