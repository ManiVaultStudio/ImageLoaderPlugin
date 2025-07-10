set(FREEIMAGE_INCLUDE_DIR_HINTS "" CACHE PATH "FreeImage include directory")
set(FREEIMAGE_LIBRARY_DIR_HINTS "" CACHE PATH "FreeImage library directory")

unset(FREEIMAGE_FOUND)

if(TARGET FreeImage)
    set(FREEIMAGE_FOUND TRUE)
    message(STATUS "Found FreeImage")
    message(STATUS "  Target : FreeImage")
else()
    list(APPEND FREEIMAGE_CHECK_INCLUDE_DIRS
        ${FREEIMAGE_INCLUDE_DIR_HINTS}
        /usr/include 
        /usr/local/include 
        /opt/homebrew/include
    )

    list(APPEND FREEIMAGE_CHECK_LIBRARY_DIRS
        ${FREEIMAGE_LIBRARY_DIR_HINTS}
        /usr/lib 
        /usr/local/lib 
        /opt/homebrew/lib
    )

    find_path(FreeImage_INCLUDE_DIRS
        NAMES 
        FreeImage.h
        PATHS 
        ${FREEIMAGE_CHECK_INCLUDE_DIRS}
    )

    find_library(FreeImage_LIBRARIES
        NAMES 
        freeimage
        PATHS 
        ${FREEIMAGE_CHECK_LIBRARY_DIRS}
    )

    if(FREEIMAGE_INCLUDE_DIRS AND FREEIMAGE_LIBRARIES)
        set(FREEIMAGE_FOUND TRUE)

        add_library(FreeImage INTERFACE IMPORTED)
        target_include_directories(
            FreeImage INTERFACE ${FREEIMAGE_INCLUDE_DIRS})
        target_link_libraries(
            FreeImage INTERFACE ${FREEIMAGE_LIBRARIES})

        message(STATUS "Found FreeImage")
        message(STATUS "  Includes : ${FREEIMAGE_INCLUDE_DIRS}")
        message(STATUS "  Libraries : ${FREEIMAGE_LIBRARIES}")
    else()
         message(FATAL_ERROR "Could not find FreeImage")
    endif()
endif()
