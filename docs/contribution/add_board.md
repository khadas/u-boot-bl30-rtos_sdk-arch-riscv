How to Add a New Board	{#add_board}
==========

Let's take ***ad401_a113l*** for example.

### Step 1: Prepare your source code. ###
Make directory ***boards/arm64/ad401_a113l***, and put your source code into it.

	mkdir -p boards/arm64/ad401_a113l

### Step 2: Add ***Kconfig***. ###
Write ***Kconfig*** and put it into ***boards/arm64/ad401_a113l***.

@code
# Copyright (c) 2021-2022 Amlogic, Inc. All rights reserved.

# SPDX-License-Identifier: MIT

config BOARD_AD401_A113L
	bool "Amlogic AD401 Board"
	select SOC_A1
	help
	  Enable Amlogic AD401 Board.
...
@endcode

Please change the content accordingly.

### Step 3: Add ***CMakeLists.txt***. ###
Write ***CMakeLists.txt*** and put it into ***boards/arm64/ad401_a113l***.

@code
# Copyright (c) 2021-2022 Amlogic, Inc. All rights reserved.

# SPDX-License-Identifier: MIT

aml_library_include_directories(
	${CMAKE_CURRENT_SOURCE_DIR}
)

aml_library_sources(
	...
)
@endcode

Please change the include directories, source code and dependent libraries accordingly.

### Step 4: Add ***defconfig***. ###
Write ***defconfig*** and put it into ***boards/arm64/ad401_a113l***.

@code
#
# Copyright (c) 2021-2022 Amlogic, Inc. All rights reserved.
#
# SPDX-License-Identifier: MIT
#

CONFIG_BOARD_AD401_A113L=y
...
CONFIG_SERIAL=y
...
@endcode

Note that ***defconfig*** <span style="color:red">defines the default **hardware** features</span>.\n
Please change the features accordingly.
