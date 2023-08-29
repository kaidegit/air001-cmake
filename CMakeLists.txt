set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_VERSION 1)
cmake_minimum_required(VERSION 3.22)

# specify cross-compilers and tools

# set the gcc path if you want use the specific version
 set(GCC_PATH "C:/Softwares/xpack-arm-none-eabi-gcc-12.2.1-1.2/bin")
# otherwise the cmake will find the gcc in the system path

if (GCC_PATH)
    if (WIN32)  # add .exe for windows
        set(CMAKE_C_COMPILER ${GCC_PATH}/arm-none-eabi-gcc.exe)
        set(CMAKE_CXX_COMPILER ${GCC_PATH}/arm-none-eabi-g++.exe)
        set(CMAKE_ASM_COMPILER ${GCC_PATH}/arm-none-eabi-gcc.exe)
        set(CMAKE_AR ${GCC_PATH}/arm-none-eabi-ar.exe)
        set(CMAKE_OBJCOPY ${GCC_PATH}/arm-none-eabi-objcopy.exe)
        set(CMAKE_OBJDUMP ${GCC_PATH}/arm-none-eabi-objdump.exe)
        set(SIZE ${GCC_PATH}/arm-none-eabi-size.exe)
    else ()
        set(CMAKE_C_COMPILER ${GCC_PATH}/arm-none-eabi-gcc)
        set(CMAKE_CXX_COMPILER ${GCC_PATH}/arm-none-eabi-g++)
        set(CMAKE_ASM_COMPILER ${GCC_PATH}/arm-none-eabi-gcc)
        set(CMAKE_AR ${GCC_PATH}/arm-none-eabi-ar)
        set(CMAKE_OBJCOPY ${GCC_PATH}/arm-none-eabi-objcopy)
        set(CMAKE_OBJDUMP ${GCC_PATH}/arm-none-eabi-objdump)
        set(SIZE ${GCC_PATH}/arm-none-eabi-size)
    endif ()
else ()
    set(CMAKE_C_COMPILER arm-none-eabi-gcc)
    set(CMAKE_CXX_COMPILER arm-none-eabi-g++)
    set(CMAKE_ASM_COMPILER arm-none-eabi-gcc)
    set(CMAKE_AR arm-none-eabi-ar)
    set(CMAKE_OBJCOPY arm-none-eabi-objcopy)
    set(CMAKE_OBJDUMP arm-none-eabi-objdump)
    set(SIZE arm-none-eabi-size)
endif ()
set(CMAKE_TRY_COMPILE_TARGET_TYPE STATIC_LIBRARY)

# project settings
project(Air001_CMake C CXX ASM)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_C_STANDARD 11)

#Uncomment for software floating point
#add_compile_options(-mfloat-abi=soft)

add_compile_options(-mcpu=cortex-m0 -mthumb -mthumb-interwork)
add_compile_options(-ffunction-sections -fdata-sections -fno-common -fmessage-length=0)

# uncomment to mitigate c++17 absolute addresses warnings
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-register")

# Enable assembler files preprocessing
add_compile_options($<$<COMPILE_LANGUAGE:ASM>:-x$<SEMICOLON>assembler-with-cpp>)

if ("${CMAKE_BUILD_TYPE}" STREQUAL "Release")
    message(STATUS "Maximum optimization for speed")
    add_compile_options(-Ofast)
elseif ("${CMAKE_BUILD_TYPE}" STREQUAL "RelWithDebInfo")
    message(STATUS "Maximum optimization for speed, debug info included")
    add_compile_options(-Ofast -g)
elseif ("${CMAKE_BUILD_TYPE}" STREQUAL "MinSizeRel")
    message(STATUS "Maximum optimization for size")
    add_compile_options(-Os)
else ()
    message(STATUS "Minimal optimization, debug info included")
    add_compile_options(-Og -g)
endif ()

include_directories(
        main
        config
        Arduino-Air001-Drivers/CMSIS/Device/AIR001xx/Include
        Arduino-Air001-Drivers/CMSIS/Include
        Arduino-Air001-Drivers/AIR001xx_HAL_Driver/Inc
        Arduino-Air001-Drivers/AIR001xx_HAL_Driver/Inc/Legacy
)

add_definitions(
        -DAIR001_DEV
        -DUSE_HAL_DRIVER
)
#查找HAL库中非template结尾的文件
file(GLOB HAL_Lib
        "Arduino-Air001-Drivers/AIR001xx_HAL_Driver/Src/*.c"
        Arduino-Air001-Drivers/AIR001xx_HAL_Driver/Src/Legacy/*.c
)
list(FILTER HAL_Lib EXCLUDE REGEX ".*template\\.c$")

file(GLOB_RECURSE SOURCES
        main/*.c
        Arduino-Air001-Drivers/CMSIS/Device/AIR001xx/Source/system_air001xx.c
        Arduino-Air001-Drivers/CMSIS/Device/AIR001xx/Source/gcc/startup_air001xx.s
        ${HAL_Lib}
)

set(LINKER_SCRIPT ${CMAKE_SOURCE_DIR}/ldscript.ld)

add_link_options(-Wl,-gc-sections,--print-memory-usage,-Map=${PROJECT_BINARY_DIR}/${PROJECT_NAME}.map)
add_link_options(-specs=nano.specs -specs=nosys.specs -static -lc -lm)
add_link_options(-Wl,--no-warn-rwx-segments)
add_link_options(-mcpu=cortex-m0 -mthumb -mthumb-interwork)
add_link_options(-T ${LINKER_SCRIPT})

add_executable(${PROJECT_NAME}.elf ${SOURCES} ${LINKER_SCRIPT})

set(HEX_FILE ${PROJECT_BINARY_DIR}/${PROJECT_NAME}.hex)
set(BIN_FILE ${PROJECT_BINARY_DIR}/${PROJECT_NAME}.bin)

add_custom_command(TARGET ${PROJECT_NAME}.elf POST_BUILD
        COMMAND ${CMAKE_OBJCOPY} -Oihex $<TARGET_FILE:${PROJECT_NAME}.elf> ${HEX_FILE}
        COMMAND ${CMAKE_OBJCOPY} -Obinary $<TARGET_FILE:${PROJECT_NAME}.elf> ${BIN_FILE}
        COMMENT "Building ${HEX_FILE}
Building ${BIN_FILE}")

# Air001目前应该是进入了pyocd的develop分支了，以后可能不需要指定包了
add_custom_target(flash ALL
        COMMAND pyocd flash --pack ${CMAKE_SOURCE_DIR}/Keil.AIR001_DFP.1.0.1.pack --target air001dev ${HEX_FILE}
        DEPENDS ${PROJECT_NAME}.elf
        COMMENT "Flashing ${HEX_FILE}"
)