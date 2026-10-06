# ports/template/portfile.cmake
# Automatically generated/copied overlay portfile

# CMAKE_CURRENT_LIST_DIR is ports/<package_name>
# Resolves to external/<package_name>
get_filename_component(PORT_NAME "${CMAKE_CURRENT_LIST_DIR}" NAME)
set(SOURCE_PATH "${CMAKE_CURRENT_LIST_DIR}/../../external/${PORT_NAME}")

vcpkg_cmake_configure(
    SOURCE_PATH "${SOURCE_PATH}"
)

vcpkg_cmake_install()
vcpkg_cmake_config_fixup(PACKAGE_NAME "${PORT_NAME}")

if(EXISTS "${SOURCE_PATH}/LICENSE")
    vcpkg_install_copyright(FILE_LIST "${SOURCE_PATH}/LICENSE")
endif()