How to Build SDK {#build}
===============

## [Install Dependencies](sys_requirements.html) ##

## Choose your project ##

The project is a combination of<span style="color:red"> **ARCH**, **SOC**, **BOARD**,</span> and<span style="color:red"> **PRODUCT**.</span>\n
If you already know the these parameters, you could specify them by the command line parameters.

	source scripts/env.sh [ARCH] [SOC] [BOARD] [PRODUCT]

For ARCH *arm64*, SOC *a1*, BOARD *ad401_a113l*, PRODUCT *speaker*, you could type the following command.\n

	source scripts/env.sh arm64 a1 ad401_a113l speaker

Otherwise, you could choose one project from the list.\n

	source scripts/env.sh

## Build ##

To build a specific product for a specific board, use the following command:

	make

Note that the building environment will be extracted automatically for the first time running.\n
After the compilation, the images can be found in the directory ***output/${ARCH}-${BOARD}-${PRODUCT}/images***.

For ARCH *arm64*, SOC *a1*, BOARD *ad401_a113l*, PRODUCT *speaker*,
the images can be found in the directory ***output/arm64-ad401_a113l-speaker/images***.

---
### Menuconfig ###
To adjust the default configuration, please use menuconfig.

	make menuconfig

### Build Document ###

	make docs

To view the local document, please open ***output/docs/html/index.html*** with any web browser.
