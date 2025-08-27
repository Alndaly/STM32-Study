cmake_minimum_required(VERSION 3.27)

# Roots
include("../roots.cmake")

set(CONTEXT Project.Debug+STM32F103C8)
set(TARGET ${CONTEXT})
set(OUT_DIR "${SOLUTION_ROOT}/out/Project/STM32F103C8/Debug")
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(HEX_FILE "Project.hex")
set(LD_MAP_FILE "Project.axf.map")
set(LD_SCRIPT "${SOLUTION_ROOT}/Project/RTE/Device/STM32F103C8/ac6_linker_script.sct.src")
set(LD_REGIONS "${SOLUTION_ROOT}/Project/RTE/Device/STM32F103C8/regions_STM32F103C8.h")
set(LD_SCRIPT_PP "${CMAKE_CURRENT_BINARY_DIR}/ac6_linker_script.sct")

# Processor Options
set(CPU Cortex-M3)
set(FPU NO_FPU)
set(BYTE_ORDER Little-endian)

# Toolchain config map
include("toolchain.cmake")

# Setup project
project(${CONTEXT} LANGUAGES C ASM)

# Compilation database
add_custom_target(database
  COMMAND ${CMAKE_COMMAND} -E make_directory "${OUT_DIR}"
  COMMAND ${CMAKE_COMMAND} -E copy_if_different "${CMAKE_CURRENT_BINARY_DIR}/compile_commands.json" "${OUT_DIR}"
)
set(CMAKE_C_STANDARD_INCLUDE_DIRECTORIES ${CMAKE_C_IMPLICIT_INCLUDE_DIRECTORIES})

# Setup context
add_executable(${CONTEXT})
set_target_properties(${CONTEXT} PROPERTIES PREFIX "" SUFFIX ".axf" OUTPUT_NAME "Project")
set_target_properties(${CONTEXT} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${OUT_DIR})
add_library(${CONTEXT}_GLOBAL INTERFACE)

# Includes
target_include_directories(${CONTEXT} PUBLIC
  "${SOLUTION_ROOT}/Project/RTE/_Debug_STM32F103C8"
  "${SOLUTION_ROOT}/Project/RTE/Device/STM32F103C8"
  "${CMSIS_PACK_ROOT}/ARM/CMSIS/6.2.0/CMSIS/Core/Include"
  "${CMSIS_PACK_ROOT}/Keil/STM32F1xx_DFP/2.4.1/Device/Include"
  "${CMSIS_PACK_ROOT}/Keil/STM32F1xx_DFP/2.4.1/Device/StdPeriph_Driver/inc"
)

# Defines
target_compile_definitions(${CONTEXT} PUBLIC
  $<$<COMPILE_LANGUAGE:C,CXX>:
    STM32F10X_MD
    _RTE_
  >
)

# Compile options
target_compile_options(${CONTEXT} PUBLIC
  $<$<COMPILE_LANGUAGE:ASM>:
    "SHELL:${ASM_CPU}"
    "SHELL:${ASM_FLAGS}"
    "SHELL:${ASM_BYTE_ORDER}"
    "SHELL:-gdwarf-5"
    "SHELL:-masm=auto"
  >
  $<$<COMPILE_LANGUAGE:C>:
    "SHELL:${CC_CPU}"
    "SHELL:${CC_FLAGS}"
    "SHELL:${CC_BYTE_ORDER}"
    "SHELL:-gdwarf-5"
    "SHELL:-std=gnu11"
    "SHELL:-Wno-macro-redefined"
    "SHELL:-Wno-pragma-pack"
    "SHELL:-Wno-parentheses-equality"
    "SHELL:-Wno-license-management"
  >
)

# Compile Options Abstractions
add_library(${CONTEXT}_ABSTRACTIONS INTERFACE)
cbuild_set_options_flags(CC "none" "on" "" "" CC_OPTIONS_FLAGS_${CONTEXT})
cbuild_set_options_flags(ASM "none" "on" "" "" ASM_OPTIONS_FLAGS_${CONTEXT})
target_compile_options(${CONTEXT}_ABSTRACTIONS INTERFACE
  $<$<COMPILE_LANGUAGE:C>:
    "SHELL:${CC_OPTIONS_FLAGS_${CONTEXT}}"
  >
  $<$<COMPILE_LANGUAGE:ASM>:
    "SHELL:${ASM_OPTIONS_FLAGS_${CONTEXT}}"
  >
)

# Add groups and components
include("groups.cmake")
include("components.cmake")

target_link_libraries(${CONTEXT} PUBLIC
  Group_Source_Files
  ARM_CMSIS_CORE_6_1_1
  Keil_Device_Startup_1_0_0
  Keil_Device_StdPeriph_Drivers_ADC_3_6_0
  Keil_Device_StdPeriph_Drivers_BKP_3_6_0
  Keil_Device_StdPeriph_Drivers_CAN_3_6_0
  Keil_Device_StdPeriph_Drivers_CEC_3_6_0
  Keil_Device_StdPeriph_Drivers_CRC_3_6_0
  Keil_Device_StdPeriph_Drivers_DAC_3_6_0
  Keil_Device_StdPeriph_Drivers_DBGMCU_3_6_0
  Keil_Device_StdPeriph_Drivers_DMA_3_6_0
  Keil_Device_StdPeriph_Drivers_EXTI_3_6_0
  Keil_Device_StdPeriph_Drivers_FSMC_3_6_0
  Keil_Device_StdPeriph_Drivers_Flash_3_6_0
  Keil_Device_StdPeriph_Drivers_Framework_3_6_0
  Keil_Device_StdPeriph_Drivers_GPIO_3_6_0
  Keil_Device_StdPeriph_Drivers_I2C_3_6_0
  Keil_Device_StdPeriph_Drivers_IWDG_3_6_0
  Keil_Device_StdPeriph_Drivers_PWR_3_6_0
  Keil_Device_StdPeriph_Drivers_RCC_3_6_0
  Keil_Device_StdPeriph_Drivers_RTC_3_6_0
  Keil_Device_StdPeriph_Drivers_SDIO_3_6_0
  Keil_Device_StdPeriph_Drivers_SPI_3_6_0
  Keil_Device_StdPeriph_Drivers_TIM_3_6_0
  Keil_Device_StdPeriph_Drivers_USART_3_6_0
  Keil_Device_StdPeriph_Drivers_WWDG_3_6_0
)

# Linker options
target_link_options(${CONTEXT} PUBLIC
  "SHELL:${LD_CPU}"
  "SHELL:${_LS}\"${LD_SCRIPT_PP}\""
  "SHELL:${LD_MAP}"
  "SHELL:--entry=Reset_Handler"
  "SHELL:--info summarysizes"
  "SHELL:--summary_stderr"
  "SHELL:--diag_suppress=L6314W"
)
set_target_properties(${CONTEXT} PROPERTIES LINK_DEPENDS ${LD_SCRIPT})

# Linker script pre-processing
add_custom_command(TARGET ${CONTEXT} PRE_LINK COMMAND ${CPP} ARGS ${CPP_ARGS_LD_SCRIPT} BYPRODUCTS ${LD_SCRIPT_PP})

# Hex Conversion
 add_custom_command(TARGET ${CONTEXT} POST_BUILD COMMAND ${CMAKE_OBJCOPY} ${ELF2HEX})
