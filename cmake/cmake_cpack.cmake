include(InstallRequiredSystemLibraries)

# For help take a look at:
# http://www.cmake.org/Wiki/CMake:CPackConfiguration

### general settings
set(CPACK_PACKAGE_NAME "libvisual")
set(CPACK_PACKAGE_VENDOR "libvisual")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "libvisual, an audio visualisation abstraction library")

# set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_SOURCE_DIR}/COPYING")

### versions
set(CPACK_PACKAGE_VERSION_MAJOR "${GENERIC_MAJOR_VERSION}")
set(CPACK_PACKAGE_VERSION_MINOR "${GENERIC_MINOR_VERSION}")
set(CPACK_PACKAGE_VERSION_PATCH "${GENERIC_MICRO_VERSION}")
set(CPACK_PACKAGE_VERSION "${CPACK_PACKAGE_VERSION_MAJOR}.${CPACK_PACKAGE_VERSION_MINOR}.${CPACK_PACKAGE_VERSION_PATCH}")

set(CPACK_GENERATOR "TGZ")
set(CPACK_PACKAGE_INSTALL_DIRECTORY "${CPACK_PACKAGE_NAME}-${CPACK_PACKAGE_VERSION}")

### source package settings
set(CPACK_SOURCE_GENERATOR "TGZ")
set(CPACK_SOURCE_IGNORE_FILES "~$;[.]swp$;/[.]svn/;/[.]git/;.gitignore;/build/;tags;cscope.*")
set(CPACK_SOURCE_PACKAGE_FILE_NAME "${CPACK_PACKAGE_NAME}-${CPACK_PACKAGE_VERSION}")

include(CPack)
