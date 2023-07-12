How to Add a initcall	{#add_initcall}
==========

Call the model init functions to initialize their subsystems.
We provide driver(DRIVER_INIT) init and servers init(SERVICE_INIT).
 We need to call InitcallInitialize to initialize the module

Let's take ***netd_init*** for example.

### Step 1: Include. ###
To use initcall, you need to include initcall.h first

@code
#include "initcall.h"
@endcode

### Step 2: call Initcall.. ###
Call the initialization function in the mian function to initialize the existing functions in the service section

@code
InitcallInitialize(SECTION_SERVICE);
@endcode

Please change the include directories, source code and dependent libraries accordingly.

### Step 3: Add netd_init. ###
add ietd_init to init section(The init section is a separate area in the ld script)

@code
SERVICE_INIT(netd_init);
@endcode

