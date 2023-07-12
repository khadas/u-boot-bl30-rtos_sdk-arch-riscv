Getting Started	{#getting_started}
===============

This section illustrates how to run a shell sample on the  board.

### Step 1: Connect the board to your PC through UART, and check the name of the serial port. ###

	$ sudo dmesg
	...
	[ 3446.774090] usb 1-5.5: new full-speed USB device number 12 using xhci_hcd
	[ 3446.877423] usb 1-5.5: New USB device found, idVendor=10c4, idProduct=ea60, bcdDevice= 1.00
	[ 3446.877444] usb 1-5.5: New USB device strings: Mfr=1, Product=2, SerialNumber=3
	[ 3446.877454] usb 1-5.5: Product: CP2102 USB to UART Bridge Controller
	[ 3446.877461] usb 1-5.5: Manufacturer: Silicon Labs
	[ 3446.877467] usb 1-5.5: SerialNumber: 0001
	[ 3446.885160] cp210x 1-5.5:1.0: cp210x converter detected
	[ 3446.888694] usb 1-5.5: cp210x converter now attached to ttyUSB0

You will see similar log messages as mentioned above.

### Step 2: Open the first serial port (ttyUSB0) on your PC. ###

### Step 3: POWER on the board. ###

You should see the following output:

## Booting kernel from Legacy Image at 00001000 ...
	Image Name:   rtos
	Image Type:   AArch64 U-Boot Standalone Program (uncompressed)
	Data Size:    1903394 Bytes = 1.8 MiB
	Load Address: 00001000
	Entry Point:  00001000
	Verifying Checksum ... OK
	Loading Standalone Program ... OK
	UART initialized

	Amlogic FreeRTOS - projects/amlogic-dev (2022-01-14 13:44:08)
	GIC initialized
	[ARM] Mailbox initialized
	Clock initialized
	GPIO IRQ initialized
	WCN initialized
	Wi-Fi initialized
	Clock source initialized

	Welcome to speaker!

Now the board is ready to use.
