cmake_minimum_required(VERSION 3.3)

set(libraries common)
set(executables receiver transmitter)

#include_directories(common)
#include_directories(receiver)
#include_directories(transmitter)


#include (CMakeForceCompiler)
set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_PROCESSOR cortex-m0plus)
set(CMAKE_INCLUDE_PATH "")
enable_language(ASM)

project(nrf51822_xxac)

#CMAKE_FORCE_C_COMPILER(/usr/local/gcc-arm-none-eabi-4_9-2015q3/bin/arm-none-eabi-gcc GNU)

set(tools /usr/local/gcc-arm-none-eabi-4_9-2015q3)
set(CMAKE_CXX_COMPILER  ${tools}/bin/arm-none-eabi-g++)
set(CMAKE_C_COMPILER    ${tools}/bin/arm-none-eabi-gcc)
set(CMAKE_ASM_COMPILER  ${tools}/bin/arm-none-eabi-gcc)
set(SIZE_COMMAND        ${tools}/bin/arm-none-eabi-size)
set(OBJCOPY_COMMAND     ${tools}/bin/arm-none-eabi-objcopy)

set(CHIP NRF51)


set(BUILD_SHARED_LIBS OFF)

# NRF related
set(NRF_SDK $ENV{HOME}/nrf/sdk/nrf51_sdk_latest)
if (defined $ENV{NRF_SDK})
    set(NRF_SDK $ENV{NRF_SDK})
endif()

set(SDK_COMPONENTS          ${NRF_SDK}/components)
set(NRF_TOOLCHAIN           ${SDK_COMPONENTS}/toolchain)
set(DEVICE_INCLUDES         ${SDK_COMPONENTS}/device)
set(NRF_GCC                 ${NRF_TOOLCHAIN}/gcc)

set(MAIN_TARGET ${PROJECT_NAME}.out)

set(LINKER_SCRIPT ${PROJECT_SOURCE_DIR}/linker.ld)

set(PREPROCESSOR_FLAGS "-D${CHIP} -DNRF51422 -DSWI_DISABLE0 -DBOARD_PCA10028 -DBSP_DEFINES_ONLY")
set(CMAKE_C_FLAGS "-T${LINKER_SCRIPT} -std=c99 ${PREPROCESSOR_FLAGS} -mcpu=cortex-m0 -mthumb -mabi=aapcs -Wall -O3 -g3 -mfloat-abi=soft -ffunction-sections -fdata-sections -fno-strict-aliasing -fno-builtin --short-enums  -MP -MD -Wl,--gc-sections --specs=nano.specs -L ${SDK_COMPONENTS}/toolchain/gcc ")
set(CMAKE_ASM_FLAGS "${PREPROCESSOR_FLAGS}")

include_directories(
#    ${PROJECT_SOURCE_DIR}
    ${NRF_TOOLCHAIN}
#    ${NRF_SDK_ROOT}/examples/bsp
    ${NRF_TOOLCHAIN}/cmsis/include
    ${SDK_COMPONENTS}/device
#    ${SDK_COMPONENTS}/drivers_nrf/hal
#    ${SDK_COMPONENTS}/drivers_nrf/delay
#    ${SDK_COMPONENTS}/drivers_nrf/radio_config
#    ${SDK_COMPONENTS}/drivers_nrf/nrf_soc_nosd
#    ${SDK_COMPONENTS}/libraries/timer
#    ${SDK_COMPONENTS}/libraries/util
#    ${SDK_COMPONENTS}/libraries/log
#    ${SDK_COMPONENTS}/libraries/log/src
)

set(BOOT_SOURCE_FILES
    ${NRF_GCC}/gcc_startup_nrf51.S
    ${NRF_TOOLCHAIN}/system_nrf51.c
)

foreach(library ${libraries})
    message(${library})
    include(${library}/build.cmake)

    file(GLOB_RECURSE ${library}_src ${library}/src/c/*.c)
    add_library(${library} ${${library}_src})
    set(${library}_public_include_directories ${public_include_directories} ${PROJECT_SOURCE_DIR}/${library}/src/include)
    set_property(TARGET ${library} APPEND PROPERTY INCLUDE_DIRECTORIES ${include_directories})
    set_property(TARGET ${library} APPEND PROPERTY INCLUDE_DIRECTORIES ${PROJECT_SOURCE_DIR}/${library}/src/include)
endforeach()

foreach(executable ${executables})
    message(${executable})
    include(${executable}/build.cmake)

    file(GLOB_RECURSE ${executable}_src ${executable}/src/c/*.c)
    add_executable(${executable} ${${executable}_src} ${BOOT_SOURCE_FILES})
    set_property(TARGET ${executable} APPEND PROPERTY INCLUDE_DIRECTORIES ${include_directories})
    foreach(dependency ${dependencies})
        target_link_libraries(${executable} ${dependency})
        set_property(TARGET ${executable} APPEND PROPERTY INCLUDE_DIRECTORIES ${include_directories})
        set_property(TARGET ${executable} APPEND PROPERTY INCLUDE_DIRECTORIES ${${dependency}_public_include_directories})
        set_property(TARGET ${executable} APPEND PROPERTY LINK_DIRECTORIES ${PROJECT_BINARY_DIR})

        add_custom_command(
                TARGET ${executable}
                COMMAND ${SIZE_COMMAND} ${executable}
                COMMAND ${OBJCOPY_COMMAND} -O ihex ${executable} ${executable}.hex
                COMMAND ${OBJCOPY_COMMAND} -O binary ${executable} ${executable}.bin
        )
    endforeach()
endforeach()

#add_executable(${MAIN_TARGET} ${SOURCE_FILES})

#add_custom_command(
#        TARGET ${MAIN_TARGET}
#        COMMAND ${SIZE_COMMAND} ${MAIN_TARGET}
#        COMMAND ${OBJCOPY_COMMAND} -O ihex ${MAIN_TARGET} ${PROJECT_NAME}.hex
#        COMMAND ${OBJCOPY_COMMAND} -O binary ${MAIN_TARGET} ${PROJECT_NAME}.bin
#)
