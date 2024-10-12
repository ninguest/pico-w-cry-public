# This file will be configured to contain variables for CPack. These variables
# should be set in the CMake list file of the project before CPack module is
# included. The list of available CPACK_xxx variables and their associated
# documentation may be obtained using
#  cpack --help-variable-list
#
# Some variables are common to all generators (e.g. CPACK_PACKAGE_NAME)
# and some are specific to a generator
# (e.g. CPACK_NSIS_EXTRA_INSTALL_COMMANDS). The generator specific variables
# usually begin with CPACK_<GENNAME>_xxxx.


set(CPACK_BUILD_SOURCE_DIRS "C:/Users/n/Desktop/Robo-Pico-Kit-CircuitPython-main/test/RPIPicoBasics/2-PicoWFlash;C:/Users/n/Desktop/Robo-Pico-Kit-CircuitPython-main/test/RPIPicoBasics/2-PicoWFlash/build")
set(CPACK_CMAKE_GENERATOR "Ninja")
set(CPACK_COMPONENT_UNSPECIFIED_HIDDEN "TRUE")
set(CPACK_COMPONENT_UNSPECIFIED_REQUIRED "TRUE")
set(CPACK_DEFAULT_PACKAGE_DESCRIPTION_FILE "C:/Program Files/Raspberry Pi/Pico SDK v1.5.1/cmake/share/cmake-3.25/Templates/CPack.GenericDescription.txt")
set(CPACK_DEFAULT_PACKAGE_DESCRIPTION_SUMMARY "JDBlink1w built using CMake")
set(CPACK_GENERATOR "ZIP;TGZ")
set(CPACK_INCLUDE_TOPLEVEL_DIRECTORY "OFF")
set(CPACK_INSTALL_CMAKE_PROJECTS "C:/Users/n/Desktop/Robo-Pico-Kit-CircuitPython-main/test/RPIPicoBasics/2-PicoWFlash/build;JDBlink1w;ALL;/")
set(CPACK_INSTALL_PREFIX "C:/Program Files (x86)/JDBlink1w")
set(CPACK_MODULE_PATH "C:/Program Files/Raspberry Pi/Pico SDK v1.5.1/pico-sdk/cmake")
set(CPACK_NSIS_DISPLAY_NAME "JDBlink1w 0.1.1")
set(CPACK_NSIS_INSTALLER_ICON_CODE "")
set(CPACK_NSIS_INSTALLER_MUI_ICON_CODE "")
set(CPACK_NSIS_INSTALL_ROOT "$PROGRAMFILES")
set(CPACK_NSIS_PACKAGE_NAME "JDBlink1w 0.1.1")
set(CPACK_NSIS_UNINSTALL_NAME "Uninstall")
set(CPACK_OBJCOPY_EXECUTABLE "C:/Program Files/Raspberry Pi/Pico SDK v1.5.1/gcc-arm-none-eabi/bin/arm-none-eabi-objcopy.exe")
set(CPACK_OBJDUMP_EXECUTABLE "C:/Program Files/Raspberry Pi/Pico SDK v1.5.1/gcc-arm-none-eabi/bin/arm-none-eabi-objdump.exe")
set(CPACK_OUTPUT_CONFIG_FILE "C:/Users/n/Desktop/Robo-Pico-Kit-CircuitPython-main/test/RPIPicoBasics/2-PicoWFlash/build/CPackConfig.cmake")
set(CPACK_PACKAGE_DEFAULT_LOCATION "/")
set(CPACK_PACKAGE_DESCRIPTION_FILE "C:/Program Files/Raspberry Pi/Pico SDK v1.5.1/cmake/share/cmake-3.25/Templates/CPack.GenericDescription.txt")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "JDBlink1w built using CMake")
set(CPACK_PACKAGE_FILE_NAME "JDBlink1w-0.1.1-PICO")
set(CPACK_PACKAGE_INSTALL_DIRECTORY "JDBlink1w 0.1.1")
set(CPACK_PACKAGE_INSTALL_REGISTRY_KEY "JDBlink1w 0.1.1")
set(CPACK_PACKAGE_NAME "JDBlink1w")
set(CPACK_PACKAGE_RELOCATABLE "true")
set(CPACK_PACKAGE_VENDOR "Humanity")
set(CPACK_PACKAGE_VERSION "0.1.1")
set(CPACK_PACKAGE_VERSION_MAJOR "0")
set(CPACK_PACKAGE_VERSION_MINOR "1")
set(CPACK_PACKAGE_VERSION_PATCH "1")
set(CPACK_READELF_EXECUTABLE "C:/Program Files/Raspberry Pi/Pico SDK v1.5.1/gcc-arm-none-eabi/bin/arm-none-eabi-readelf.exe")
set(CPACK_RESOURCE_FILE_LICENSE "C:/Program Files/Raspberry Pi/Pico SDK v1.5.1/cmake/share/cmake-3.25/Templates/CPack.GenericLicense.txt")
set(CPACK_RESOURCE_FILE_README "C:/Program Files/Raspberry Pi/Pico SDK v1.5.1/cmake/share/cmake-3.25/Templates/CPack.GenericDescription.txt")
set(CPACK_RESOURCE_FILE_WELCOME "C:/Program Files/Raspberry Pi/Pico SDK v1.5.1/cmake/share/cmake-3.25/Templates/CPack.GenericWelcome.txt")
set(CPACK_SET_DESTDIR "OFF")
set(CPACK_SOURCE_7Z "ON")
set(CPACK_SOURCE_GENERATOR "7Z;ZIP")
set(CPACK_SOURCE_OUTPUT_CONFIG_FILE "C:/Users/n/Desktop/Robo-Pico-Kit-CircuitPython-main/test/RPIPicoBasics/2-PicoWFlash/build/CPackSourceConfig.cmake")
set(CPACK_SOURCE_ZIP "ON")
set(CPACK_SYSTEM_NAME "PICO")
set(CPACK_THREADS "1")
set(CPACK_TOPLEVEL_TAG "PICO")
set(CPACK_WIX_SIZEOF_VOID_P "4")

if(NOT CPACK_PROPERTIES_FILE)
  set(CPACK_PROPERTIES_FILE "C:/Users/n/Desktop/Robo-Pico-Kit-CircuitPython-main/test/RPIPicoBasics/2-PicoWFlash/build/CPackProperties.cmake")
endif()

if(EXISTS ${CPACK_PROPERTIES_FILE})
  include(${CPACK_PROPERTIES_FILE})
endif()
