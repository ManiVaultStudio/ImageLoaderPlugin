set(FREEIMAGE_INCLUDE_DIR_HINTS "" CACHE PATH "FreeImage include directory")
set(FREEIMAGE_LIBRARY_DIR_HINTS "" CACHE PATH "FreeImage library directory")

unset(FREEIMAGE_FOUND)

if(TARGET FreeImage)
    set(FREEIMAGE_FOUND TRUE)
    message(STATUS "Found FreeImage")
    message(STATUS "  Target : FreeImage")
else()
    set(FREEIMAGE_CHECK_INCLUDE_DIRS)
    set(FREEIMAGE_CHECK_LIBRARY_DIRS)
    if(FREEIMAGE_INCLUDE_DIR_HINTS)
        list(APPEND FREEIMAGE_CHECK_INCLUDE_DIRS ${FREEIMAGE_INCLUDE_DIR_HINTS})
    endif()
    list(APPEND FREEIMAGE_CHECK_INCLUDE_DIRS
        /usr/include 
        /usr/local/include 
        /opt/homebrew/include
    )

    if(FREEIMAGE_LIBRARY_DIR_HINTS)
        list(APPEND FREEIMAGE_CHECK_LIBRARY_DIRS ${FREEIMAGE_LIBRARY_DIR_HINTS})
    endif()
    list(APPEND FREEIMAGE_CHECK_LIBRARY_DIRS
        /usr/lib 
        /usr/local/lib 
        /opt/homebrew/lib
    )

    find_path(FreeImage_INCLUDE_DIRS
        NAMES 
        FreeImage.h
        PATHS 
        ${FREEIMAGE_CHECK_INCLUDE_DIRS})

    find_library(FreeImage_LIBRARIES
        NAMES 
        freeimage
        PATHS 
        ${FREEIMAGE_CHECK_LIBRARY_DIRS})

    if(FreeImage_INCLUDE_DIRS AND FreeImage_LIBRARIES)
        set(FREEIMAGE_FOUND TRUE)

        add_library(FreeImage SHARED IMPORTED)
        set_target_properties(FreeImage PROPERTIES
        IMPORTED_LOCATION "${FreeImage_LIBRARIES}"
        INTERFACE_INCLUDE_DIRECTORIES "${FreeImage_INCLUDE_DIRS}"
        )

        message(STATUS "Found FreeImage")
        message(STATUS "  Includes : ${FREEIMAGE_INCLUDE_DIRS}")
        message(STATUS "  Libraries : ${FREEIMAGE_LIBRARIES}")
    else()
        message(WARNING "Searched include dirs: ${FREEIMAGE_CHECK_INCLUDE_DIRS}")
        message(WARNING "Searched lib dirs: ${FREEIMAGE_CHECK_LIBRARY_DIRS}")
        message(FATAL_ERROR "Could not find FreeImage")
    endif()
endif()
