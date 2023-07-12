How to Add a New Product	{#add_product}
==========

Let's take ***speaker*** for example.

### Step 1: Prepare your source code. ###
Make directory ***products/speaker***, and put your **main.c** into it.

	mkdir -p products/speaker

### Step 2: Add ***CMakeLists.txt***. ###
Write ***CMakeLists.txt*** and put it into ***products/speaker***.

@code
# Copyright (c) 2021-2022 Amlogic, Inc. All rights reserved.

# SPDX-License-Identifier: MIT

cmake_minimum_required(VERSION 3.13.1)
project(aml-rtos)

include($ENV{SDK_BASE}/build_system/cmake/root.cmake)

target_include_directories(${TARGET_NAME} PUBLIC include)

aml_sources(main.c)
@endcode

Please change the include directories, source code and dependent libraries accordingly.

### Step 3: Add ***prj.conf***. ###
Write ***prj.conf*** and put it into ***products/speaker***.

@code
#
# Copyright (c) 2021-2022 Amlogic, Inc. All rights reserved.
#
# SPDX-License-Identifier: MIT
#

CONFIG_CLI=y
CONFIG_POSIX=y
...
@endcode

Note that ***prj.conf*** <span style="color:red">defines the default **software** features</span>.\n
Please change the features accordingly.
