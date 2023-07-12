How to Add a New SoC	{#add_soc}
==========

Let's take ***a1*** for example.

### Step 1: Prepare your source code. ###
Make directory ***soc/arm64/a1***, and put your source code into it.

	mkdir -p soc/arm64/a1

### Step 2: Add ***Kconfig***. ###
Write ***Kconfig*** and put it into ***soc/arm64/a1***.

@code
# Copyright (c) 2021-2022 Amlogic, Inc. All rights reserved.

# SPDX-License-Identifier: MIT

config SOC_A1
	bool "A1 SoC"
	select ARM64
	select ARMV8
	select ARM_CA35_64_BIT
	help
	  Enable A1 SOC of ARM64.
...
if SOC_A1
...
endif # SOC_A1
@endcode

Please change the content accordingly.

### Step 3: Add ***CMakeLists.txt***. ###
Write ***CMakeLists.txt*** and put it into ***soc/arm64/a1***.

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
