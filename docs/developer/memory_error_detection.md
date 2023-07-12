How to Use Memory Error Detection {#memory_error_detection}
===============

To dynamic memory debug, please follow the procedure below:

### Step 1: Memory out-of-bounds example ###

Create a memory out-of-bounds case:

```c
void memory_test(void)
{
    static uint8_t *pBuf = NULL;
    pBuf = malloc(5);
    memset(pBuf, 0, 10);
}
```

Execute the out-of-bounds scan command on the serial console:

	ad409_a113l> heap overflow

The scan results are as follows(compile by *make backtrace*):

	ERROR!!! detected buffer overflow(TAIL)
		Task owner:(console) buffer address:(2c3350) request size:(5) block size:(48)
		CallTrace:
		[<00000000000144ac>] total_heap_info+0x6c/0x90
		[<000000000001b0a4>] FreeRTOS_CLIProcessCommand+0xa4/0x3b0
		[<000000000001c1a4>] FreeRTOS_CLIEnterConsoleLoop+0x294/0x740
		[<fffffffffffffffc>] N/A
		[<00000000002d2bc0>] N/A
	Memory out of bounds occurs (1) times

### Step 2: Memory leak example ###

Create a memory leak case:

```c
void memory_test(void)
{
    static uint8_t *pBuf = NULL;
    pBuf = pvPortMalloc(5);
    pBuf = NULL;
}
```

Execute the memory leak scan command on the serial console:

	ad409_a113l> heap leak

The scan results are as follows(compile by *make backtrace*):

	WARNING!!! detected buffer leak
		Task owner:(console) buffer address:(2c3350) request size:(5) block size:(48)
		CallTrace:
		[<00000000000142cc>] total_heap_info+0x6c/0x80
		[<000000000001b094>] FreeRTOS_CLIProcessCommand+0xa4/0x3b0
		[<000000000001c194>] FreeRTOS_CLIEnterConsoleLoop+0x294/0x740
		[<fffffffffffffffc>] N/A
		[<00000000002d2bc0>] N/A
