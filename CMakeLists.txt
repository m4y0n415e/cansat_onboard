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

# add_subdirectory(libs/no-OS-FatFS-SD-SPI-RPi-Pico/FatFs_SPI)

# Add executable. Default name is the project name, version 0.1

add_executable(CS_Soft 
    code/main.c
    code/atm_sen_module.c
    libs/bmp280/bmp280_i2c.c
    libs/bmp280/bmp280_i2c_hal.c
    code/time_manager.c
    libs/dfrobot_oxygen_sensor/dfrobot_oxygen.c
    libs/minmea/minmea.c
    code/gps_module.c
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
        #FatFs_SPI
        hardware_adc
        hardware_uart
        pico_util
        hardware_dma
        #hardware_rtc
    )

# Add the standard include files to the build
target_include_directories(CS_Soft PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}
        ${CMAKE_CURRENT_LIST_DIR}/code
        libs/bmp280
        libs/dfrobot_oxygen_sensor
        libs/minmea
        ${PICO_SDK_PATH}/src/common/pico_base_headers/include
        ${PICO_SDK_PATH}/src/common/pico_util/include
        ${PICO_SDK_PATH}/src/common/pico_time/include
        ${PICO_SDK_PATH}/src/rp2_common/hardware_rtc/include
    )

pico_add_extra_outputs(CS_Soft)

add_compile_options(-Dtimegm=mktime)