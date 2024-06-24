/**
  ******************************************************************************
  * @file    Readme_STM32MP2.txt
  * @author  MCD Application Team
  * @brief   Description of ImageHeader
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2022 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */

**** Introduction

ImageHeader allows to add a stm32 header needed by bootrom but also EL3_launcher in case of A35 bootable image
You find the stm32 image header description in wiki `STM32_header_for_binary_files <https://wiki.st.com/stm32mpu/wiki/STM32_header_for_binary_files>`

Generated Image description
 +----------------------------+
 |   STM32 Header             | 512Bytes in case of v2.0 header
 +----------------------------+
 |   EL3_launcher (optional)  | Needed in case of aarch32 Firmware application with a35 mode=0 (OTP16/bit4) - Default setting - See Linker script for reserved size (around 14KB)
 +----------------------------+
 |   Firmware application     | Max Size depending on MP2 PartNumber and type of image (A35 or M33) - See linker script for further details
 +----------------------------+

**** ImageHeader content :

exe.win-amd64-2.7
-- bz2.pyd
-- _hashlib.pyd
-- library.zip
-- python27.dll
-- Stm32ImageAddHeader.exe  -> Windows script to generate STM32 Image Header on Windows OS
-- unicodedata.pyd
Python27
-- Stm32ImageAddHeader.py   -> Python script to generate STM32 Image Header on Linux OS
Python3
-- Stm32ImageAddHeader.py   -> Python script to generate STM32 Image Header on Linux OS
EL3_Launcher
-- STM32MP2_el3_launcher.bin --> binary added to the application to switch from EL3 64 bits to EL1 32 bits in order to execute A35 applications in aarch32 (default)
-- STM32MP2_el3_launcher.elf -->  associated elf used to find entry point


**** How to use ImageHeader:

In STM32CubeIDE project you need to add the following line in the "Post-build steps" command in project Properties -> C/C++ Build -> Settings -> Build Steps :

  ../[../]ImageHeader/postbuild_STM32MP2.sh  "${gnu_tools_for_stm32_compiler_path}" "${BuildArtifactFileBaseName}"

About arguments:
* "../[../]ImageHeader/"                  -> Relative path from STM32CubeIDE build config build directory to Image generation tools
                                               That's one "../" more than from folder containing .cproject 
* "postbuild_STM32MP2.sh"                 -> bash script file to generate the STM32MP2 Image Header
* "${gnu_tools_for_stm32_compiler_path}"  -> STM32CubeIDE variable containing path to GCC toolchain
                                               used by postbuild_STM32MP2.sh to extract information from application .elf file
* "${BuildArtifactFileBaseName}"          -> STM32CubeIDE variable containing base name of application .elf file
                                               Used by postbuild_STM32MP2.sh to get .elf file and generate Images names

To generate the Image Header, the "postbuild_STM32MP2.sh" script will automatically select either the Python script "Stm32ImageAddHeader.py" on Linux OS or the windows executable "Stm32ImageAddHeader.exe" on Windows OS

**** Generated files
  ${BuildArtifactFileBaseName}.stm32                        -> Default image to by written to SDCard in selected slot
                                                                Generated for A35 and M33 applications
                                                                For A35 this is the image which contains the EL3_Launcher and is suitable for a part with A35 booting in Aarch64 mode
  ${BuildArtifactFileBaseName}_no_el3_launcher_.stm32       -> No EL3 Launcher image to by written to SDCard in selected slot
                                                                Generated only for A35 applications
                                                                This is the image which doesn't contain the EL3_Launcher and is suitable for a part with A35 booting in Aarch32 mode

**** How to regenerate the Windows executable:

If you made some changes in the Python script "Stm32ImageAddHeader.py", you need to update the Windows executable accordingly.
This has to be done on Windows OS only.

- Install Python 2.7 
- Install "cx_Freeze-4.2.3.win-amd64-py2.7.msi" (this version is compatible with Python 2.7)
- in folder Python27 run command: "python setup.py install"
- New files are installed in folder "exe.win-amd64-2.7"
