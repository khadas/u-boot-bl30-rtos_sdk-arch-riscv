How to use the debug methods    {#debug_methods}
==========================

At the FreeRTOS SDK,there are some debug methods supported.You could to use them to debug your program and find out the problem's root cause helpful.

# debug methods list
***
## GCC option
### finstrument-functions option
* function:the linker link a function:it will invoke a hook function when enter the function,then invoke another function when exit the function.
* The hook function has its fixed name _cyg_profile_func_enter and __cyg_profile_func_exit.
* To be avoid invoke itself,the hook function need add no_instrument_function attribute.
* example

```c
  void __attribute__((no_instrument_function))__cyg_profile_func_enter(void *this, void *call)
  {
        printf("enter func:0x%x, caller:0x%x\n", this, call);
  }

  void __attribute__((no_instrument_function))__cyg_profile_func_exit(void *this, void *call)

  {
        printf("exit func:0x%x, caller:0x%x\n", this, call);
  }
```
* cmake option for **module**:set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -finstrument-functions")
* cmake option for **function**:set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -finstrument-functions-exclude-function-list=instrument_function_hide")
* cmake option for **file**:set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -finstrument-functions-exclude-file-list=main.c")

### __builtin_return_address function
* function protype:void * __builtin_return_address (unsigned int level)
* useage:This function returns the return address of the current function, or of one of its callers
* note:It is used to 0 level to get return address of current function,other level is always return 0
***
## CLI command
### task-stats command
* enable the CLI support
* type **task-stats** command:return the content of bellow title:

  **name    number    state    priority    stack_depth    stack_watermark run(ms)     run(percent)**

* name:task of name
* number:task id
* state:task status；X（running），B（blocked），R（ready to schedule），D（deleted）S（suspend）
* priority:priority of task
* stack_depth:stack depth of task
* stack_watermark：the watermark of task stack；
* run(ms)：running time of this task；
* run(percent) ：running percent time of this task；
***
## backtrace
### Step 1: How to enable Backtrace ###

#### Choose your project

If you already know the ***ARCH***, ***SOC***, ***BOARD*** and ***PRODUCT***,

you could specify them by the command line parameters.

	source scripts/env.sh [ARCH] [SOC] [BOARD] [PRODUCT]

For ARCH arm64, SOC a1, BOARD ad401_a113l, PRODUCT speaker, you could type the following command.

	source scripts/env.sh arm64 a1 ad401_a113l speaker

Otherwise, you can choose the project interactively by the following command.

	source scripts/env.sh

#### Config your project

	make menuconfig

steps following:  **Libs Options** -> **Backtrace Support**

### Step 2: How to enable sym table print ###

If you use the following command:

	make

When an exception occurs, the output format is as follows:

	ESR:     96000007
	SPSR:    20000004
	ELR:     13cb4
	LR:      13cb4
	========== Call stack ==========
	[<0000000000013cb4>] N/A
	[<000000000001b094>] N/A
	[<000000000001c194>] N/A
	[<fffffffffffffffc>] N/A
	==========    End    ==========

If you use the following command:

	make backtrace

When an exception occurs, the output format is as follows:

	ESR:     96000007
	SPSR:    20000004
	ELR:     13cb4
	LR:      13cb4
	========== Call stack ==========
	[<0000000000013cb4>] cmd_read32+0x154/0x250
	[<000000000001b094>] FreeRTOS_CLIProcessCommand+0xa4/0x3b0
	[<000000000001c194>] FreeRTOS_CLIEnterConsoleLoop+0x294/0x740
	[<fffffffffffffffc>] N/A
	==========    End    ==========

### Step 3: How to via backtrace help for debug ###

Backtrace default used when program crash (*Asynchronous exception)*) .

Also you can use to other place like this:

print current context stack when **dump_stack()** called by assertion.
***
## version management
  when you compile your project complete,RTOS SDK will collect git information and general a report sheet in a output file.
### source code
The file's path is **output/project_fullname/freertos/sdk_ver.h**.You can use them to trace your source code version.The sheet is follow:

        [cm_hash  remote_name  branch      ]
        [f7e725c  boar riscv   amlogic-dev ]
        [c9d7923  arch riscv   (HEAD       ]
        [db5a75b  soc  riscv   (HEAD       ]
        [cfc1e49  scripts      (HEAD       ]
        [a341cbe  libc         (HEAD       ]
        [e72fb29  freertos     (HEAD       ]
        [601c9d0  prod aocpu   (HEAD       ]
        [c6e9858  build        (HEAD       ]
        [46573f9  drivers_aocp (HEAD       ]
* col_1st:the locate newest commit short hash number
* col_2nd:remote git repository name,first section+last section composite
* col_3rd:locate branch name.HEAD,no means that not to build a branch at local

### binary file
  For binary file,it will enable the switch to output version message
  * support arch:**arm64/riscv**
  * select VERSION_FULL_INFO in the board name's Kconfig
  * make distclean && make
***
## module size statistics
  when you compile your project complete,it will calculate the module size and report them in a output file.
* support arch:**arm64/riscv/xtensa**
* statistics file path:**output/project_fullname/freertos/freertos_module_info.txt**
* report contents:
  + customer section size
  + module size summary
  + binary size summary
* notes：There is few data error cause that xcc map file not to describe align data.It is less than 10 as far as we know
* more details in freertos_module_info.txt
***

