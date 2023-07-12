How to Add a New Driver	{#add_driver}
==========

Let's take ***watchdog*** driver for example.

### Step 1: Prepare your source code. ###
Make directory ***drivers/watchdog***, and put your source code into it.

	mkdir -p drivers/watchdog

### Step 2: Add ***Kconfig***. ###
Write ***Kconfig*** for your driver.

@code
# Copyright (c) 2021-2022 Amlogic, Inc. All rights reserved.

# SPDX-License-Identifier: MIT

config WATCHDOG
	bool "Watchdog driver"
@endcode

Then add the following line to ***drivers/Kconfig***.
@code
rsource "watchdog/Kconfig"
@endcode

### Step 3: Add ***CMakeLists.txt***. ###
Write ***CMakeLists.txt*** for your driver.

@code
# Copyright (c) 2021-2022 Amlogic, Inc. All rights reserved.

# SPDX-License-Identifier: MIT

aml_library_include_directories(
	${CMAKE_CURRENT_LIST_DIR}
)

aml_library_sources_ifdef(
	CONFIG_WATCHDOG
	watchdog.c
)
@endcode

Then add the following line to root ***CMakeLists.txt***.
@code
add_subdirectory(watchdog)
@endcode

### Step 4: Enable the new driver by default. ###
Let's take ***boards/arm64/ad401_a113l*** for example.

Add the following line to ***boards/arm64/ad401_a113l/defconfig***.

@code
CONFIG_WATCHDOG=y
@endcode

### Step 5: Initialize your driver ###
Add driver startup with ***DRIVER_INIT***

```c
#ifdef CONFIG_ARM64
#include "initcall.h"
#endif

void watch_dog_init(void)
{
	vWatchdogInit(10000);
	vWatchdogEnable();
}

#ifdef CONFIG_ARM64
DRIVER_INIT(watch_dog_init);
#endif
```
