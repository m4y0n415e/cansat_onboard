# Generated Cmake Pico project file

cmake_minimum_required(VERSION 3.13)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Initialise pico_sdk from installed location
# (note this can come from environment, CMake cache etc)

# == DO NOT EDIT THE FOLLOWING LINES for the Raspberry Pi Pico VS Code Extension to work ==
if(WIN32)
    set(USERHOME $ENV{USERPROFILE})
else()
    set(USERHOME $ENV{HOME})
endif()
set(sdkVersion 2.2.0)
set(toolchainVersion 14_2_Rel1)
set(picotoolVersion 2.2.0-a4)
set(picoVscode ${USERHOME}/.pico-sdk/cmake/pico-vscode.cmake)
if (EXISTS ${picoVscode})
    include(${picoVscode})
endif()
# ====================================================================================
set(PICO_BOARD pico2 CACHE STRING "Board type")

# Pull in Raspberry Pi Pico SDK (must be before project)
include(pico_sdk_import.cmake)

project(CS_Soft C CXX ASM)

# Initialise the Raspberry Pi Pico SDK
pico_sdk_init()

add_subdirectory(lib/no-OS-FatFS-SD-SPI-RPi-Pico/FatFs_SPI)

# Add executable. Default name is the project name, version 0.1

add_executable(CS_Soft 
    src/main.c
    src/atm_sen_module.c
    lib/bmp280/bmp280_i2c.c
    lib/bmp280/bmp280_i2c_hal.c
    lib/dfrobot_oxygen_sensor/dfrobot_oxygen.c
    src/time_manager.c
    src/gps_module.c
    lib/minmea/minmea.c
    src/microsd_module.c
    src/hw_config.c
    lib/nRF905/nRF905.c
    src/radio_module.c
    )

pico_set_program_name(CS_Soft "CS_Soft")
pico_set_program_version(CS_Soft "0.1")

# Modify the below lines to enable/disable output over UART/USB
pico_enable_stdio_uart(CS_Soft 0)
pico_enable_stdio_usb(CS_Soft 1)

# Add the standard library to the build
target_link_libraries(CS_Soft
        pico_stdlib
        pico_time
        hardware_i2c
        hardware_spi
        hardware_adc
        hardware_uart
        hardware_dma
        pico_util
#        hardware_rtc
        FatFs_SPI
    )

# Add the standard include files to the build
target_include_directories(CS_Soft PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}
        ${CMAKE_CURRENT_LIST_DIR}/src
        lib/bmp280
        lib/dfrobot_oxygen_sensor
        lib/minmea
        ${PICO_SDK_PATH}/src/common/pico_base_headers/include
        ${PICO_SDK_PATH}/src/common/pico_util/include
        ${PICO_SDK_PATH}/src/common/pico_time/include
        ${PICO_SDK_PATH}/src/rp2_common/hardware_rtc/include
        ${CMAKE_CURRENT_LIST_DIR}/lib/no-OS-FatFS-SD-SPI-RPi-Pico/FatFs_SPI/sd_driver
        ${CMAKE_CURRENT_LIST_DIR}/lib/no-OS-FatFS-SD-SPI-RPi-Pico/FatFs_SPI/ff15/source
        ${CMAKE_CURRENT_LIST_DIR}/lib/no-OS-FatFS-SD-SPI-RPi-Pico/FatFs_SPI/include
    )

pico_add_extra_outputs(CS_Soft)

add_compile_options(-Dtimegm=mktime)