# Copyright (c) 2021-2022 Amlogic, Inc. All rights reserved.

# SPDX-License-Identifier: MIT

set(common_flags "")
set(c_flags "")

set(linker_flags "-Wl,--print-memory-usage,-Map=${TARGET_NAME}.map,--gc-sections")

if(CONFIG_LIBC_STD)
set(linker_flags "${linker_flags},--wrap=_malloc_r,--wrap=_free_r,--wrap=_realloc_r,--wrap=_calloc_r")
endif()

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -march=rv32imc -mabi=ilp32 -imacros${AUTOCONF_H}")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} --specs=nano.specs --specs=nosys.specs")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O2 -g -ffunction-sections -fdata-sections -fno-common -fgnu89-inline")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -march=rv32imc -mabi=ilp32 -nostdlib -imacros${AUTOCONF_H}")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} --specs=nano.specs --specs=nosys.specs")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O2 -g -ffunction-sections -fdata-sections -fno-common -fno-builtin -fgnu89-inline")
set(CMAKE_ASM_FLAGS "${CMAKE_ASM_FLAGS} -march=rv32imc -mabi=ilp32 -D__ASM -imacros${AUTOCONF_H}")
set(CMAKE_ASM_FLAGS "${CMAKE_ASM_FLAGS} --specs=nano.specs --specs=nosys.specs")
set(CMAKE_ASM_FLAGS "${CMAKE_ASM_FLAGS} -O2 -g -ffunction-sections -fdata-sections -fno-common -fgnu89-inline")

