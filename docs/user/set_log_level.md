How to Dynamically Tweak Log	{#set_log_level}
================================

### Overview
The Amlogic RTOS SDK uses *ulog* to trace the log. Users can tweak log config dynamically with console command.

To get the usage, please use following command.

	ad409_a113l> ulog
	shell cmds of ulog
	  tool            [1]: enable tool, [0]: disable tool.
	  filter          show ulog filter settings.
	  setkw           [keyword] set ulog global filter keyword.
	  settag          [tag] set ulog global filter tag.
	  setlvl          [level] set ulog global filter level(1~7).
	  settaglvl       [tag][level] set ulog filter level(1~7) with [tag].

### Note
The sub command of *tool* is used to trace BT HCI log. It would add HEX header with each log data for HCI parsing log.

Please ignore this section if you don't have BT module.
### Show Filter
Type *ulog filter* to show current filter setting. The default value are as follows.

	ad409_a113l> ulog filter
	--------------------------------------
	ulog global filter:
	level   : Debug
	tag     : NULL
	keyword : NULL
	--------------------------------------
	ulog tag's level filter:
	settings not found

### Tweak
Type *setkw settag setlvl settaglvl* to tweak the tag and level of log string. The *level* value is listed below.
| Level   | Name | Value|    Description |
| :----- | :--: | :--: |:-------- |
| LOG_LVL_ASSERT |  assertion | 0 | Unhandled and fatal errors occurred, so that the system could not continue to run. These are assertion logs. |
| LOG_LVL_ERROR |  error | 3| The log that is output when a serious, unrepairable error occurs is an error level log. |
| LOG_LVL_WARNING |  warning | 4 | These warning logs are output when there are some less important errors with repairability. |
| LOG_LVL_INFO | information| 6 | A log of important prompt information that is viewed by the upper-level user of the module, for example, initialization success, current working status, and so on. This level of log is generally retained during mass production. |
| LOG_LVL_DBG | debug | 7 | The debug log that is viewed by the developer of this module. This level of log is generally closed during mass production. |

As can be seen from the above classification, the output level of the log can be set in the following four aspects of ulog:

* **Global static** log level：Defined to the `ULOG_OUTPUT_LVL` macro, reference the ulog_def.h.
* **Global Dynamics** log level：Use the *ulog setlvl* command to set it.
* **Module static** log level：The `LOG_LVL` macro is defined in the module (file), similar to the way the log tag macro `LOG_TAG` is defined.
* **Module dynamics** log level：Use the *ulog settaglvl* command to set it.

Their scope of action is **Global Static**>**Global Dynamics**>**Module Static**>**Module Dynamic**.

If you want to change the LOG_LVL.  Let's take `LOG_LVL_INFO` for Example.
Please type the following command.

	ad409_a113l> ulog settaglvl Example 6
	ad409_a113l> ulog filter
	--------------------------------------
	ulog global filter:
	level   : Debug
	tag     : NULL
	keyword : NULL
	--------------------------------------
	ulog tag's level filter:
			Example: Info


### Restore
If you want to restore the default setting, please use the commands below.
### Clear the config without parameters

	ulog settag

### Clear the keyword without parameters

	ulog setkw

### Set the global dynamics level to LOG_LVL_DBG

	ulog setlvl 7

Then use *ulog filter* to check whether the value was restored.
