# STM32DDRFW-UTIL Firmware

STM32DDRFW-UTIL is the firmware used to initialize DDR and perform DDR tests.

This document describes:
- the Software Architecture and Design of STM32DDRFW-UTIL.
- how to use the STM32DDRFW-UTIL firmware package for DDR configuration and test on STM32MP1 Series MPUs.

## 1 STM32DDRFW-UTIL Architecture and Design

STM32DDRFW-UTIL v1.0.1 applies to STM32MP1 series only (STM32MP15xx and STM32MP13xx).

### 1.1 Package overview

STM32DDRFW-UTIL firmware package has the following structure: 

![](_htmresc/Package_structure.png "Package structure")

STM32DDRFW-UTIL firmware is a software package containing multiple STM32CubeIDE projects applicable for all STM32 products with a DDR which includes:
- BSP, CMSIS and HAL drivers for all applicable STM32MPxxx series.
- DDR_Tool full source code with:
   - Common directory with general purpose content.
   - STM32CubeIDE Projects for all ST supported boards.
- Binaries for all supported ST boards with BootROM image header to be programmed directly into SDCARD or loaded via STM32CubeProgramer without using STM32CubeIDE tool.
- Imageheader package to generate stm32 files with STM32CubeIDE.
- Test Report of all applicable STM32MPxxx series.

### 1.2 Source code organisation

The following file tree shows an example of the source code organization for STM32MP135C-DK project included in the STM32DDRFW-UTIL package:

![](_htmresc/Source_code_organization.png "Project tree")

### 1.3 Design principles

Based on HAL DDR driver, STM32DDRFW-UTIL firmware provides 2 main functionalities:
- Read/Write DDR registers and DDR general information (name, speed, size, …)
- Launch DDR tests

The STM32DDRFW-UTIL firmware also implements a console in DDR Interactive mode allowing to use these functionalities.

**DDR Interactive mode** enables a special way of running the DDR initialization in which we can move from one initialization step to another (forward and backward) and launch DDR tests using command-lines.

#### 1.3.1 HAL DDR driver APIs

The HAL driver APIs (in stm32mpxxxx_hal_ddr.c file) provides the functions allowing to initialize the DDR and to access DDR registers in DDR Interactive mode.

***Note:***
*This HAL driver does not fit the split between DRIVER and BSP as defined in  the STM32Cube specifications. Board (DDR components) and IP (Controller and PHY) are mixed in the same Hal driver.*

|  API name                    |       Description      |
|------------------------------|------------------------|
|**HAL_DDR_Init**   |<ul><li>**brief**<br>DDR init sequence, including <br>- reset/clock/power management (i.e. access to other IPs)<br>- DT information getting<br>- DDRCTRL and DDRPHY configuration and initialization<br>- self-refresh mode setup<br>- data/addr tests execution after training.</li><li>**param**<br>*iddr* structure for DDR initialization settings allowing to define/retrieve some system global features.</li>***Note:*** *In STM32DDRFW-UTIL firmware, the low power mechanism is not used, so all parameters can be set to null or false.*<br><li>**retval** HAL status.</li></ul>|
|**HAL_DDR_MspInit**|<ul><li>**brief**<br>board-specific DDR power initialization if any.<br>***Note:*** *Only used in STM32DDRFW-UTIL firmware if PMIC (power management IC) is implemented on board.*<br></li><li>**param**<br>*type* DDR type.</li><li>**retval** 0 if OK.</li></ul>|

<br>

|  API name in DDR<br>interactive mode only  |       Description      |
|--------------------------------------------|------------------------|
|**HAL_DDR_ASS_Set_Clksrc**|<ul><li>**brief**<br>Set AXI Sub-System Clock Source.<br>***Note:*** *In STM32DDRFW-UTIL firmware, this function is used to switch AXI clock source to a different source, in order to change PLL2 rate when changing DDR frequency.*</li><li>**param**<br>*clksrc* AXI Sub-System clock source.</li><li>**retval** HAL status.</li></ul>|
+|**HAL_DDR_Interactive**   |<ul><li>**brief**<br>Set DDR step and run tool command.<br>User function implemented in ddr_tool.c</li><li>**param**<br>*step* DDR Interactive mode step.</li><li>**retval** boolean.</li></ul>|
+|**HAL_DDR_Dump_Param**    |<ul><li>**brief**<br>Prints input configuration parameters to be set for all DDR settings. This function will print the setting value from the input configuration parameters provided in the source code and used to initialize the DDR at start.</li><li>**param**<br>*config* static DDR configuration used to initialize the DDR setting name (if NULL, all settings are printed out).</li><li>**retval** HAL status.</li></ul>|
+|**HAL_DDR_Dump_Reg**      |<ul><li>**brief**<br>Dump the DDR setting value. This function will print the actual setting value and format the output print if save parameter is true (to be used after DDR initialization in DDR_READY step).</li><li>**param**<br>*name* setting name (if NULL, all settings are printed out) save indicates if the output print has to be formatted with “#define …” (true) or not.</li><li>**retval** HAL status.</li></ul>|
+|**HAL_DDR_Edit_Param**    |<ul><li>**brief**<br>Edit input parameter value. This function allows to change the DDR configuration parameters before initialization in DDR_RESET step.</li><li>**param**<br>*name* setting name.<br>*string* new parameter value.</li><li>**retval** None.</li></ul>|
+|**HAL_DDR_Edit_Reg**      |<ul><li>**brief**<br>Edit DDR setting value. This function allows to change DDR settings after DDR_CTRL_INIT_DONE step for DDRC registers and after DDR_PHY_INIT_DONE step for DDRPHY user input parameters </li><li>**param**<br>*name* setting name<br>*string* new parameter value</li><li>**retval** None.</li></ul>|

#### 1.3.2 DDR Interactive mode

This mode enables a special way of running the DDR initialization in which we can move from one initialization step to another (forward and backward). A set of inline commands are available to set the DDR initialization step and to launch DDR Tool functionalities and tests.

DDR interactive mode uses 5 steps to initialize the DDR controller and the PHY with parameters found in *stm32mp_util_ddr_conf.h*:

![](_htmresc/DDR_interactive_steps.png  "DDR interactive steps")

***Note:***
*stm32mp_util_ddr_conf.h provides each register value for DDR controller and PHY. To set the initial DDR configuration parameters in stm32mp_util_ddr_conf.h, templates are provided for each DDR type and it is necessary to have a good knowledge of the DDR SDRAM datasheet to complete the template and provide the first register values (See §1.3.3.3 Customization).*

#### 1.3.3 DDR configuration in STM32DDRFW-UTIL package

The DDR_Tool project applies to a specific board and contains all related files. All user adaptations should be gathered inside this project directory.
The objective here is not to describe all files, but to focus on major items.

##### 1.3.3.1 .project and .cproject file

These are the two main STM32CubeIDE configuration files:
- ***.project*** lists all the source files that must be built in order to run the project.
- ***.cproject*** contains all the configuration parameters (such as flags, include directories) and can include similar sections in case of multiple build configurations.

##### 1.3.3.2 Flags

  Among the series of flags, some of them are important for the DDR Tool:
- ***DDR_INTERACTIVE*** is mandatory to enable the DDR interactive mode
- ***DDR_TYPE_X*** is also needed in order to determine the DDR type and its density, and to consider the corresponding setting template (for example, *DDR_TYPE_DDR3_4Gb* will apply settings from *stm32mp13xx-ddr3-4Gb-template.h* in STM32MP135C-DK project). See more details in *§1.3.3.3 Customization*.
- ***NO_CACHE_USE**** and ***NO_MMU_USE*** are set by default to ensure real DDR device access for first tests.

##### 1.3.3.3 Customization

The source code of STM32CubeIDE projects for ST boards is provided as example in STM32DDRFW-UTIL firmware package. Each project can be adapted to fit with customized board.
Only two files need to be adapted:
- ***stm32mp_util_conf.h***
This file contains UART, PMIC (if any) and PLL2 related configurations, including instance number, pin control and parameters. A specific definition determines if PMIC is active or not on the board.
- ***stm32mp_util_ddr_conf.h***
This file includes all the DDR settings from template file, depending on flags defined in STM32CubeIDE project (See *§1.3.3.2 Flags*). All known board templates are present in each project, as a starting point.

**e.g.:**
In STM32MP135C-DK project, *stm32mp_util_ddr_conf.h* includes *stm32mp13xx-ddr3-4Gb-template.h* for STM32MP135C DDR3 4Gb configuration. The file contains all the definitions of DDRCTRL and DDRPHY parameters for this specific type of DDR:

```
/**
  * STM32MP135C DDR3 4Gb configuration
  * 1x DDR3L 4Gb, 16-bit, 533MHz.
  * Reference used MT41K256M16TW-107 P from Micron
  *
  * DDR type / Platform DDR3/3L
  * freq        533MHz
  * width      16
  * datasheet   1
  * DDR density 4
  * timing mode optimized
  * Scheduling/QoS options : type = 6
  * address mapping : RBC
  * Tc > + 85C : N
  *****************************************************************************
  * @attention
  *
  * Copyright (c) 2020 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
#ifndef STM32MP13XX_DDR3_4GBIT_H
#define STM32MP13XX_DDR3_4GBIT_H

#define DDR_MEM_NAME "DDR3-1066 bin F 1x4Gb 533MHz v1.53"
#define DDR_MEM_SPEED 533000
#define DDR_MEM_SIZE 0x20000000

#define DDR_MSTR 0x00040401
#define DDR_MRCTRL0 0x00000010
#define DDR_MRCTRL1 0x00000000
#define DDR_DERATEEN 0x00000000
#define DDR_DERATEINT 0x00800000
#define DDR_PWRCTL 0x00000000
#define DDR_PWRTMG 0x00400010
#define DDR_HWLPCTL 0x00000000
#define DDR_RFSHCTL0 0x00210000
#define DDR_RFSHCTL3 0x00000000
#define DDR_RFSHTMG 0x0081008B
#define DDR_CRCPARCTL0 0x00000000
#define DDR_DRAMTMG0 0x121B2414
#define DDR_DRAMTMG1 0x000A041C
#define DDR_DRAMTMG2 0x0608090F
…
#define DDR_PGCR 0x01442E02
#define DDR_PTR0 0x0022AA5B
#define DDR_PTR1 0x04841104
#define DDR_PTR2 0x042DA068
#define DDR_ACIOCR 0x10400812
#define DDR_DXCCR 0x00000C40
#define DDR_DSGCR 0xF200011F
#define DDR_DCR 0x0000000B
#define DDR_DTPR0 0x38D488D0
#define DDR_DTPR1 0x098B00D8
#define DDR_DTPR2 0x10023600
#define DDR_MR0 0x00000840
#define DDR_MR1 0x00000000
#define DDR_MR2 0x00000208
#define DDR_MR3 0x00000000
#define DDR_ODTCR 0x00010000
#define DDR_ZQ0CR1 0x00000038
#define DDR_DX0GCR 0x0000CE81
#define DDR_DX1GCR 0x0000CE81

#endif

```

These templates are available in DDR Tool projects. You can also generate a template file using STM32CubeMX Generate Code functionality (See *§3.2.4 Device Tree generation*) and copy its content in STM32CubeIDE project. The template provides DDR parameters based on DDR types used on ST boards. For customized board, with a different DDR reference, the compliance of each DDR parameter with DDR datasheet must be verified and corrections might be required.

#### 1.3.4 DDR tests

##### 1.3.4.1 Test description

Tests are classified in the three following types:
- **Basic tests:** These simple and running fast tests are intended to capture the major configuration or hardware issues showing off immediately.
- **Intensive tests:** These tests use extensive coverage of data and address patterns for noise and high SSO conditions, high throughput traffic or interleaved read/write. Depending on the parameters, the test run time may be long. An intensive test can be deployed progressively, with test trial before launching long and exhaustive test sequences.
- **Stress tests:** These tests are intensive and executed with stretched conditions (such as a small frequency increase 10-20 MHz), with a skew of parameters (for example a fine step delay increase) or with specific frequency selective patterns.

These tests are intended to catch low-margin timings of a configuration that may cause elusive errors and eventual crashes later during run time.
A stress test campaign must always be done during the system bring-up. Stress tests may also be run in case of suspicious failure. Any test and its skewed parameter must be directed to pinpoint the observed failure (for example, when errors are related to specific bit or byte).

All the available tests are detailed in the table below:

![](_htmresc/Test_description.png "Test description")

In case of test failure, the DDR settings must be adjusted in the initial configuration parameters.

##### 1.3.4.2 Test infinite read/write access to ddr

In order to test infinite write/read access to DDR, you have to define flag *"TEST_INFINITE_ENABLE"* in your project.

These tests can only be stopped in Engineering Boot mode execution by:
- breaking the debugger in STM32CubeIDE
- changing the value of the variable "go_loop" in "DDR_Test_Infinite_write/read" function (in *ddr_tests.c* file)

## 2 How to use STM32DDRFW-UTIL firmware

### 2.1 Hardware connections

- Your PC is connected to the board through ST-LINK.
- UART communication is handled either by the ST-LINK connection, or by a dedicated port (if available on the HW). By default, the STMicroelectronics boards use UART4 with Bps=115200 Bit=8 Parity=None Stop=1.
- Set the correct boot pins on your board (in either Engineering mode or Boot from SD).

![](_htmresc/Hardware_connections.png "Hardware connections")

***Note:***

*- Refer to the board manual to set the boot pins in the right position*

**e.g. on STM32MP13xx :**

![](_htmresc/Boot_pins.png "Boot pins")

*- Tera Term software (https://ttssh2.osdn.jp/index.html.en) has been used to test this release, but other terminal emulators could also be used.*

### 2.2 How to compile and launch DDR Tool project

STM32DDRFW-UTIL package contains binaries that can be used directly on STM32 boards and STM32CubeIDE projects that allow to use the DDR Tool in Engineering mode and to modify the source code.

This section describes how to compile and launch these projects in STM32CubeIDE.

#### 2.2.1 Import the project in STM32CubeIDE

The projects contained in STM32DDRFW-UTIL package have been tested on STM32CubeIDE v1.10.0.
To import a DDR Tool project in STM32CubeIDE, follow these steps:
- Go to File > Import
- Select "Existing Projects into Workspace" > Next
- Browse to your project location (In the STM32DDRFW-UTIL package, the projects are stored in DDR_Tool directory)
- Select one of the listed project > Finish

#### 2.2.2 Build project

The project can contain several build configurations. In that case, you should firstly select the configuration corresponding to your hardware version:
- Right click on the project in the Project Explorer
- Go to Build configurations > Set Active
- Select your configuration

To build the project in STM32CubeIDE:
- Right click on the project in the Project Explorer
- And click on "Build Project"

#### 2.2.3 Launch project

To launch the project:
- Right click on the project in the Project Explorer
- Select "Debug As / Debug Configurations”
- In STM32 Cortex-M C/C++ Application / Select your debug configuration (if not present, double-click on "STM32 Cortex-M C/C++ Application")
- In Main tab:
   - Make sure binary file .elf is correctly set
- In Debugger tab:
   - Make sure JTAG/SWD link (Engineering mode) is selected for "Load Mode"
   - Debug probe uses ST-LINK
- On your board, make sure the boot pins are set in Engineering mode (See *§2.1 Hardware connections*)
- Then launch the Debug session

### 2.3 How to use STM32DDRFW-UTIL functionalities

#### 2.3.1 Using command lines

1. Before running the project, open a terminal emulator application on your PC and configure with the serial setup menu the right port to be used and the baud rate of the serial link (in general 115200 baud)
2. Run the project in the debug session of STM32CubeIDE (See *§2.2.3 Launch project*)
3. You will see the project banner displayed in your terminal, followed by the DDR prompt.

##### 2.3.1.1 Command description

The following table describes all the available commands with
- \<type\> = ctl, phy (for all DDR controller or PHY registers) or one category: static, timing, map, perf, dyn

- \<reg\> = name of the register (mstr for example)

**Command description:**

![](_htmresc/Command_description.png "Command description")

***Note:***

- *The "param" command is a simple way to test the modified settings, as it modifies the input parameters ('param' read from stm32mp_util_ddr_conf.h). It is recommended to execute this command at step 0. The modified values are applied at the correct DDR steps.*
- *The "print" and "edit" commands directly access the CTRL and PHY registers, so the values can be overridden by the input parameters when the driver executes the initialization steps. These commands are used for detailed debug of the DDR initialization.*

##### 2.3.1.2 Command examples

Here is an example of commands launched on STM32MP13XX_DK board:

```
----------------------------TERMINAL----------------------------
=============== UTILITIES-DDR Tool ===============
Model: STM32MP13XX_DK
RAM: DDR3-1066 bin F 1x4Gb 533MHz v1.53
0:DDR_RESET
DDR>
----------------------------------------------------------------
```

Print help command:

```
----------------------------TERMINAL----------------------------
DDR>help
commands:

help                       displays help
info                       displays DDR information
info  <param> <val>        changes DDR information
      with <param> = step, name, size or speed
freq                       displays the DDR PHY frequency in kHz
freq  <freq>               changes the DDR PHY frequency
param [type|reg]           prints input parameters
param <reg> <val>          edits parameters in step 0
print [type|reg]           dumps registers
edit <reg> <val>           modifies one register
save                       output formatted DDR regs to be saved
step                       lists the available step
step <n>                   go to the step <n>
next                       goes to the next step
go                         continues the DDR TOOL execution
reset                      reboots machine
test [help] | <n> [...]    lists (with help) or executes test <n>

with for [type|reg]:
  all registers if absent
  <type> = ctl, phy
           or one category (static, timing, map, perf, dyn)
  <reg> = name of the register
----------------------------------------------------------------
```

Before running tests, enter DDR_READY step:
```
----------------------------TERMINAL----------------------------
DDR>step 3
step to 3:DDR_READY
1:DDR_CTRL_INIT_DONE
2:DDR PHY_INIT_DONE
3:DDR_READY
----------------------------------------------------------------
```

Then you can execute tests:
**e.g.** Execute Simple Databus test 1 and then execute all tests:

```
----------------------------TERMINAL----------------------------
DDR> 
DDR>test 1 0xc0000000
Result: Pass [Test Simple DataBus]
DDR>
DDR>test 0
result 1:Test Simple DataBus = Passed
result 2:Test DataBusWalking0 = Passed
result 3:Test DataBusWalking1 = Passed
result 4:Test AddressBus = Passed
result 5:Test MemDevice = Passed
result 6:Test SimultaneousSwitchingOutput = Passed
result 7:Test Noise = Passed
result 8:Test NoiseBurst = Passed
result 9:Test Random = Passed
result 10:Test FrequencySelectivePattern = Passed
result 11:Test BlockSequential = Passed
result 12:Test Checkerboard = Passed
result 13:Test BitSpread = Passed
result 14:Test BitFlip = Passed
result 15:Test WalkingZeroes = Passed
result 16:Test WalkingOnes = Passed
Result: Pass [Test All]
----------------------------------------------------------------
```

You can also use print or save commands to get all registers. The command save will output formatted DDR register values to be copied directly in the DDR configuration file:
```
----------------------------TERMINAL----------------------------
DDR>save

/* DDR REG VALUES TO BE SAVED */
#define DDR_MEM_NAME  "DDR3-1066 bin F 1x4Gb 533MHz v1.53"
#define DDR_MEM_SPEED 533000
#define DDR_MEM_SIZE  0x20000000

/* ctl.static */
#define DDR_MSTR 0x00040401
#define DDR_MRCTRL0 0x00000010
...
----------------------------------------------------------------
```

#### 2.3.2 Using STM32CubeMX
The exact same operations can be executed using the graphical user interface provided by DDR Tool in STM32CubeMX (See *§3.2.5 DDR Test Suite* for more details)
- Within STM32CubeIDE:
   - Launch the debug session as indicated in *§2.2.3 Launch project*
- Within STM32CubeMX DDR Test Suite:
   - uncheck “SYSRAM Loading” box
   - DDR interactive connection Select & Connect on the UART of utilities-DDR (by default ST-Link UART on ST boards)
   - run DDR Tests

You can watch DDR video tutorial in STM32CubeMX for more information (Help/Tutorial Videos/DDR Tuning Tests).

### 2.4 How to use binaries from STM32DDRFW-UTIL package
Signed binaries (containing STM32 header) are provided in STM32DDRFW-UTIL for each supported ST boards.  This will allow the user to launch DDR Tool directly from SD Card.
- Copy the signed firmware binary you want to launch at offset 0x10000 (Section 128) on your SD card (using *HxD* tool for example).
- Take the SD Card and insert it in SDMMC1 instance of the platform.
- Set the boot pins of your board in « Boot from SD » mode (See *§2.1 Hardware connections*).
- Open a terminal. Reset the board and check DDR prompt on UART.
- Then you will be able to use the DDR Tool as described in *§2.3 How to use STM32DDRFW-UTIL functionalities*.

You can also flash these binaries into SYSRAM using STM32CubeProgrammer in command line or through STM32CubeMX (See *§3.1 STM32CubeProgrammer* for more details).

***Note:***

*If you need to modify the source code, you can use the ‘signed’ project to generate a new binary containing STM32 header or you can use the default project and sign the output binary using Signing Tool in STM32CubeProgrammer (See §3.1.2 Signing tool).*

## 3 Interaction with STM32CubeProgrammer and STM32CubeMX

### 3.1 STM32CubeProgrammer

STM32CubeProgrammer is the official STMicroelectronics tool for creating partitions into any Flash device available on STM32 platforms.

Once installed, STM32CubeProgrammer allows populating and updating the partitions with the prebuilt binaries.

The connection between the host PC and the board can be done through UART or USB serial links.

For installation, see the STM32CubeProgrammer [wiki](https://wiki.st.com/stm32mpu/wiki/STM32CubeProgrammer).

In the scope of STM32DDRFW-UTIL, STM352CubeProgrammer can be used to sign a binary (see *§3.1.2 Signing tool*) and to load a signed binary in SYSRAM to execute the DDR Tool project (see *§3.1.3 SYSRAM loading*).

#### 3.1.1 Installing the STM32CubeProgrammer tool

STM32CubePropgrammer package can be downloaded with a myST account [here](https://www.st.com/en/development-tools/stm32cubeprog.html).

Download the package then execute the installer and follow the instructions. You will find the tool binaries in install path:

*C:\Program Files\STMicroelectronics\STM32Cube\STM32CubeProgrammer\bin*

#### 3.1.2 Signing tool

STM32MP Signing Tool is a key tool that guarantees a secure platform, it ensures the signing of binary images using ECC keys generated by KeyGen tool.

The STM32MP Signing Tool software generates a signed binary file from:
- ***Binary image file***: contains the binary data to be programmed for the STM32MP1 Series device.
- ***Public key file***: contains the generated ECC public key in PEM format (generated with the [KeyGen tool](https://wiki.st.com/stm32mpu/wiki/KeyGen_tool)).
- ***Private key file***: contains the encrypted ECC private key in PEM format (generated with the [KeyGen tool](https://wiki.st.com/stm32mpu/wiki/KeyGen_tool)).

Signing tool “STM32MP_SigningTool_CLI.exe” is located in the install path.

The key generator STM32MP_KeyGen_CLI.exe is in the same directory.

For signing a DDR Tool binary built in STM32CubeIDE, you have to launch the two executable programs from the install path
*C:\Program Files\STMicroelectronics\STM32Cube\STM32CubeProgrammer\bin*:
- ***STM32MP_KeyGen_CLI.exe*** for generating the keys
- ***STM32MP_SigningTool_CLI.exe*** for signing the binary

For more information, read Signing tool [wiki](https://wiki.st.com/stm32mpu/wiki/Signing_tool). 

**e.g. on STM32MP135C-DK:**
1. Generate the 8 public/private keys with Keygen. This step is mandatory to insert STM32 header version 2. Launch *STM32MP_KeyGen_CLI.exe* in a command prompt:

<span style="color:blue;"> 

```
STM32MP_KeyGen_CLI -abs KeyFolder/ -pwd abc1 abc2 abc3 abc4 abc5 abc6 abc7 abc8 -n 8
```

</span>

![](_htmresc/STM32MP_KeyGen.png "STM32MP KeyGen")

2. Then launch Signing tool using the generated keys. Replace the name and location of the input and output binaries in the following command line:

<span style="color:blue;"> 

```
STM32MP_SigningTool_CLI -bin input.bin -la 0x2ffe0000 -o output_signed.stm32 -ep 0x2ffe0000 -t fsbl -iv 2 stm32 -pubk KeyFolder/publicKey00.pem KeyFolder/publicKey01.pem KeyFolder/publicKey02.pem KeyFolder/publicKey03.pem KeyFolder/publicKey04.pem KeyFolder/publicKey05.pem KeyFolder/publicKey06.pem KeyFolder/publicKey07.pem -prvk KeyFolder/privateKey00.pem KeyFolder/privateKey01.pem KeyFolder/privateKey02.pem KeyFolder/privateKey03.pem KeyFolder/privateKey04.pem KeyFolder/privateKey05.pem KeyFolder/privateKey06.pem KeyFolder/privateKey07.pem -pwd abc1 abc2 abc3 abc4 abc5 abc6 abc7 abc8
```

</span>

Expected result on the screenshot:

![](_htmresc/STM32MP_SigningTool.png "STM32MP SigningTool")

**e.g. on STM32MP157C-EVAL:**

STM32MP15 does not require to generate additional key. Use the following command line to sign a binary (replace *input* and *output* file names):

<span style="color:blue;"> 

```
STM32MP_SigningTool_CLI -bin input.bin -o output_signed.stm32 -la 0x2ffc2500 -of 1 -ep 0x2ffc2500 --public-key key\publicKey.pem --private-key key\privateKey.pem -iv 0x0 -pwd "bootrom-dev"
```

</span>


Expected result on the screenshot:

![](_htmresc/STM32MP_SigningTool2.png "STM32MP SigningTool 2")

You can then use the output binary by copying it on SDCARD or by loading the image in SYSRAM using STM32CubeProgrammer or STM32CubeMX.

#### 3.1.3 SYSRAM loading

STM352CubeProgrammer can be used to load a signed binary (from the binaries directory of the STM32DDRFW-UTIL firmware package or generated from a STM32CubeIDE signed project) into the SYSRAM of the board.

To do so connect the host PC and the board using USB or UART serial links. Make sure the boot pin configuration is properly set for serial boot on your board. Then, in a command prompt, launch the proper command line.

**e.g. for STM32MP135C-DK:**

1. Boot pin is configured in “Forced USB boot for flashing”

![](_htmresc/Forced_USB_boot_for_flashing.jpg "Forced USB boot for flashing")

2. Launch the programmer to load DDR Tool (set the proper port configuration and input file)

<span style="color:blue;"> 

```
STM32_Programmer_CLI.exe -c port=COM4 -w STM32MP135C-DK_DDR_UTILITIES_A7_signed.stm32 0x01 –-start 0x01
```

</span>

Expected result on the screenshot:

![](_htmresc/STM32Programmer.png "STM32Programmer")

Once loaded, you can open a terminal and check that the DDR prompt is responding. Then you will be able to use the DDR Tool.

### 3.2 STM32CubeMX

STM32CubeMX is an official STMicroelectronics graphical software configuration tool that helps developers to use the STM32 by means of a user interface and guides the user through to the initial configuration of a firmware project.

#### 3.2.1 Installing STM32CubeMX

STM32CubeMX package can be downloaded with a myST account [here](https://www.st.com/en/development-tools/stm32cubemx.html).

Download the package then execute the installer and follow the instructions. You will find the tool binaries in install path:

*C:\Program Files\STMicroelectronics\STM32Cube\STM32CubeMX*

#### 3.2.2 Start new project
The following paragraphs describe how to start and launch a new project based on STM32MP135C-DK for the example.

- Open STM32CubeMX then “Start My project from ST Board”

![](_htmresc/Start_My_project_from_ST_Board.png "Start My project from ST Board")

- Select your ST board model

![](_htmresc/ST_board_model.png "ST board model")

#### 3.2.3 DDR configuration

- Once the project is opened, configure the DDR settings in “Pinout and configuration” tab

![](_htmresc/DDR_configuration.png "DDR_configuration")

   - Select DDR in System Core
   - Then select the proper DDR type
   - And set the correct parameter settings

- DDR frequency is set in “Clock and Configuration” tab

![](_htmresc/Clock_and_Configuration.png "Clock and Configuration")

STM32CubeMX will resolve the clock issues and set the correct PLL configuration that can also be found in the generated device tree.

- Once the DDR configuration is set, you can then run DDR Test Suite in “Tools” tab (See *§3.2.5  DDR Test Suite*).

In case, you want to start a project with a new DDR model on a customized board, you can also generate directly the device tree containing all the DDR register definitions (See *§3.2.4 Device Tree generation*).

#### 3.2.4 Device Tree generation
-	In “Project Manager” tab, set project name and select the Toolchain/IDE STM32CubeIDE

![](_htmresc/Project_manager.png "Project Manager")

- Then click on “Generate code”
- You will find all the definitions of DDR parameters in stm32mp13-mx.dtsi at the location defined in “Project Manager”
- Copy these definitions in the corresponding STM32CubeIDE project given as example to start your customized STM32CubeIDE project.

#### 3.2.5 DDR Test Suite

##### 3.2.5.1 DDR Tool execution

If you want to start the DDR Test Suite on your board with an available STM32DDRFW-UTIL firmware available, you will need to run the DDR Tool project first.

###### 3.2.5.1.1-Engineering mode or Boot from SD card

- Run the project in Engineering mode or Boot from SD card (See *§2 How to use STM32DDRFW-UTIL firmware*).
- In DDR Test Suite, make sur SYSRAM loading is unchecked.

![](_htmresc/SYSRAM_loading_unchecked.png "SYSRAM_loading_unchecked")

- Select the correct port com then connect the board. Target information is displayed when the tool is running properly.

![](_htmresc/Target_information.png "Target information")

###### 3.2.5.1.2 Serial boot

SYSRAM loading option in STM32CubeMX uses STM32CubeProgrammer to flash the board.
- Connect your board using UART or USB link then set the right boot pin configuration (See *§3.1.3 SYSRAM loading*).
- In DDR Test Suite, check SYSRAM loading box. Select the port used to load the binary file.
- Select the DDR utilities binary file to be loaded (make sure you are using a signed binary with .stm32 extension)

![](_htmresc/DDR_utilities_binary.png "DDR utilities binary")

- Then click on “Connect” button to launch SYSRAM loading.

![](_htmresc/Connect.png "Connect")

- Target information is displayed when the tool is running properly.

##### 3.2.5.2 DDR tests
Once the firmware is loaded and running using any of the methods mentioned above, you will then be able to load the DDR configuration defined in your project or to run the DDR Tests.

- “Load Registers” operation allows to load the static DDR configuration define in the STM32CubeIDE project.

In the DDR interactive logs, the static parameters are displayed and “step 3” indicates that the DDR initialization is done.

“Save Config File” button output formatted DDR registers in DDR_Config_file.txt (check logs for the file location if no project has been created). It calls the “save” command described in Command description table.

![](_htmresc/Load_registers.png "Load_registers")

- The tests described in *§1.3.4.1 Test description* can be executed in the DDR Test Suite of STM32CubeMX.

Select a test and click on “Run test” button.

Test result is displayed in DDR interactive logs:

![](_htmresc/Test_result.png "Test result")

***Note:***

- *As mentioned in §1.3.4.2 Test infinite read/write access to ddr, “Infinite read” and “Infinite write” tests are only available in Engineering mode and require breaking in STM3CubeIDE debugger. That is why these tests are not available in STM32CubeMX DDR Test Suite.*
- *In STM32CubeMX, test 17 of the DDR Test Suite is implemented in STM32CubeMX:*

***Test 17 “Overclocking (5%) test”*** *: Run Level1 intensive tests with DDR clock increase by ~5% (up to 30MHz)*



