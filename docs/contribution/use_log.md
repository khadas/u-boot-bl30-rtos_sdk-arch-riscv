How to Use Ulog {#use_ulog}
===========================================

### Overview
The log is to output the status, process and other information of the software to different media (for example: file, console, display, etc.), display and save. Provide reference for software traceability, performance analysis, system monitoring, fault warning and other functions during software debugging and maintenance. It can be said that the use of logs consumes at least 80% of the software life cycle.

Ulog is a very simple and easy to use C/C++ log component. The first letter u stands for μ, which means micro. It can achieve the lowest **ROM<1K, RAM<0.2K** resource usage. Ulog is not only small in size, but also has very comprehensive functions. Its design concept refers to another C/C++ open source log library: EasyLogger (referred to as elog), and has made many improvements in terms of functions and performance. The main features are as follows:

* The backend of the log output is diversified and can support, for example, serial port, network, file, flash memory and other backend forms.

* The log output is designed to be thread-safe and supports asynchronous output mode.

* The logging system is highly reliable and is still available in complex environments such as interrupted ISRs and Hardfault.

* The log supports runtime/compilation time to set the output level.

* The log content supports global filtering by keyword and label.

* The APIs and log formats are compatible with linux syslog.

* Support for dumping debug data to the log in hex format.

* Compatible with `rtdbg` (RTT's early log header file) and EasyLogger's log output API.

### Configuration Options ###

The path to configure ulog using menuconfig in the ENV tool is as follows:

```c
 Libs Options → Ulog
```

 The ulog configuration options are described below. In general, the default configuration is used:

```c
[ ] using syslog       /* Enable syslog api*/
[ ] output level       /* Enable level of log with configurable*/
[*] using filter with output /* Enable shell cmd to setting filter log string*/
[*] using async output /* Enable log output by asynchronous*/
[*] using isr log      /* Enable interrupted ISR log, ie log output API can also be used in ISR */
[*] using tag          /* Enable TAG with each log output*/
(5120) async output buffer size /* set the cache size, default 5120Bytes*/
[*] using color        /* Enable colored log*/
[ ] output time        /* Enable time for log output*/
[ ] output time with timestamp /* Enable time with timestamp*/
```

### Example
Due to the increasing log output, in order to avoid the log being outputted indiscriminately, it is necessary to use a tag to classify each log. The definition of the label is in the form of **modular**. The following code is a description of the ulog routine. Users can add the code to top of source code.

See the `examples/btapp/inc/bt_app_log.h` ulog routine file with the `LOG_TAG` macro defined at the top of the file:

```c
#ifdef CONFIG_ULOG
#define LOG_TAG     "BTAPP"     /* The label corresponding to this module. When not defined, default: NO_TAG*/
#define LOG_LVL     LOG_LVL_DBG   /* The log output level corresponding to this module. When not defined, default: debug level*/
#include "ulog.h"                 /* this header file Must be under LOG_TAG and LOG_LVL*/
#else
#define LOG_E(...)
#define LOG_W(...)
#define LOG_I(...)
#define LOG_D(...)
#define LOG_RAW(...)
#define LOG_HEX(name, width, buf, size)
#endif
```

**NOTE:**
- The definition log tag must be above `#include "ulog.h"`, otherwise the default NO_TAG will be used (not recommended to define these macros in the header file).

- It need define the LOG_X with not existe Ulog component to avoid compile error.

- The file of bt_app_log.h should be include by bt_app module only, if this file include by other module, it maybe build with redifined LOG_TAG.

- The *LOG_LVL* contain *LOG_LVL_DBG/LOG_LVL_INFO/LOG_LVL_WARNING/LOG_LVL_ERROR/LOG_LVL_ASSERT*

The following *LOG_X* APIs are used to output each level log string.

```c
/* output different level log by LOG_X API */
LOG_D("Example for debug log");
LOG_I("Example for information log");
LOG_W("Example for warning log");
LOG_E("Example for error log");
```

These log output APIs support the printf format and will automatically wrap lines at the end of the log.
