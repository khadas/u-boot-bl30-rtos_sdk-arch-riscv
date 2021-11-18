set(common_flags "")

set(c_flags "")

set(linker_flags "-Wl,--print-memory-usage,-Map=${TARGET_NAME}.map,--gc-sections")

if(CONFIG_LIBC_STD)
set(linker_flags "${linker_flags},--wrap=_malloc_r,--wrap=_free_r,--wrap=_realloc_r")
endif()

execute_process(COMMAND ${SDK_BASE}/scripts/version_generator.sh ${SDK_BASE}/products/${PRODUCT} OUTPUT_VARIABLE PRODUCT_VERSION_MSG OUTPUT_STRIP_TRAILING_WHITESPACE)                                                                                                                
add_definitions(-DVERSION=\"${PRODUCT_VERSION_MSG}\")
message(STATUS "product version:${PRODUCT_VERSION_MSG}")

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -march=rv32imc -mabi=ilp32 -DN200_REVA=1 -D__ASM -DSOC_${CONFIG_SOC_NAME} -D${COMPILER} -imacros${AUTOCONF_H}")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O2 -g -ffunction-sections -fdata-sections -fno-common -fgnu89-inline")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} --specs=nano.specs --specs=nosys.specs")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -march=rv32imc -mabi=ilp32 -nostdlib -DN200_REVA=1 -DSOC_${CONFIG_SOC_NAME} -D${COMPILER} -imacros${AUTOCONF_H}")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O2 -g -ffunction-sections -fdata-sections -fno-common -fno-builtin -fgnu89-inline")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} --specs=nano.specs --specs=nosys.specs")
set(CMAKE_ASM_FLAGS "${CMAKE_ASM_FLAGS} -march=rv32imc -mabi=ilp32 -DN200_REVA=1 -DSOC_${CONFIG_SOC_NAME} -D${COMPILER} -imacros${AUTOCONF_H}")
set(CMAKE_ASM_FLAGS "${CMAKE_ASM_FLAGS} -O2 -g -ffunction-sections -fdata-sections -fno-common -fgnu89-inline")
set(CMAKE_ASM_FLAGS "${CMAKE_ASM_FLAGS} --specs=nano.specs --specs=nosys.specs")

