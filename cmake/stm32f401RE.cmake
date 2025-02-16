include(FetchContent)
FetchContent_Declare(libopencm3
  GIT_REPOSITORY https://github.com/libopencm3/libopencm3
  GIT_TAG master
)
FetchContent_MakeAvailable(libopencm3)
FetchContent_GetProperties(libopencm3)

set(MCU stm32f401re)
# create a target to build libopencm3 -- only for the target we need
add_custom_target(libopencm3 make TARGETS=stm32/f4 WORKING_DIRECTORY ${libopencm3_SOURCE_DIR})

# Create a specific CPU target with the appropriate options etc
add_library(${MCU} STATIC IMPORTED)
set_property(TARGET ${MCU} PROPERTY INTERFACE_INCLUDE_DIRECTORIES ${libopencm3_SOURCE_DIR}/include)
set_property(TARGET ${MCU} PROPERTY IMPORTED_LOCATION ${libopencm3_SOURCE_DIR}/lib/libopencm3_stm32f4.a)
add_dependencies(${MCU} libopencm3)
target_link_directories(${MCU} INTERFACE ${libopencm3_SOURCE_DIR}/lib)

target_compile_definitions(${MCU} INTERFACE -DSTM32F4 -DSTM32F401RE)

set(COMPILE_OPTIONS
  --static
  -nostartfiles
  -fno-common
  -mcpu=cortex-m4
  -mthumb
  -mfloat-abi=hard
  -mfpu=fpv4-sp-d16
)
target_compile_options(${MCU} INTERFACE ${COMPILE_OPTIONS})
target_link_options(${MCU} INTERFACE ${COMPILE_OPTIONS})

