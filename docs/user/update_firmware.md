How to Update Firmware {#update_firmware}
===============

To update firmware, please follow the procedure below:

### Step 1: Choose your project ###

Select the package target by the following command.

	source scripts/pkg_env.sh

### Step 2: Generate package file ###

Please use the following command in project root directory.

	make package

Let's take ***ad401_a113l*** board for example.
Once the compilation is completed, the images can be found in the directory ***output/package/arm64-xtensa-a1-ad401_a113l/aml_upgrade_package.img***

### Step 3: Prepare the burning environment ###

Connect your PC with board via serial tools(usb-ttl)

Keep pressing *Enter* key till you see the command prompt when rebooting the board.

Now type *adnl* to enter flashing mode.

	Hit any key to stop autoboot:  0
	ad401# 
	ad401# 
	ad401# adnl
	GPIOH_6: not found
	PHY2=0xfe004420

	USB RESET
	SPEED ENUM
	sof

### Step 4: Burn package ###

#### For Windows, use *Aml_Burn_Tool* ####
Open *Aml_Burn_Tool* to identify board (make sure the USB connected)

Click **Setting** -> **Load Img** and select **Reset After Burn** checkbox and finally click **Start**

#### For Ubuntu, use *adnl_burn_pkg* ####
Please use the following command in project root directory.

	adnl_burn_pkg -p output/package/arm64-xtensa-a1-ad401_a113l/aml_upgrade_package.img -r 1

Once the process is completed, you should see similar output as follows.

	UART initialized

	Amlogic FreeRTOS - projects/amlogic-dev (2022-01-14 15:44:11)
	GIC initialized
	[ARM] Mailbox initialized
	Clock initialized
	GPIO IRQ initialized
	WCN initialized
	Clock source initialized

	Welcome to speaker!
