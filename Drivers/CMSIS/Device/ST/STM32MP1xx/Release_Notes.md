---
pagetitle: Release Notes for STM32MP15xx CMSIS
lang: en
header-includes: <link rel="icon" type="image/x-icon" href="_htmresc/favicon.png" />
---

::: {.row}
::: {.col-sm-12 .col-lg-4}

<center>
# Release Notes for <mark>\ STM32MP15xx CMSIS </mark>
Copyright &copy; 2021\ STMicroelectronics\
    
[![ST logo](_htmresc/st_logo_2020.png)](https://www.st.com){.logo}
</center>

# Purpose
		
This driver provides the CMSIS device for the STM32MP15xx products. This covers

- STM32MP151Cxx ,STM32MP151Axx, STM32MP151Dxx ,STM32MP151Fxx devices
- STM32MP153Cxx ,STM32MP153Axx, STM32MP153Dxx ,STM32MP153Fxx devices
- STM32MP157Cxx ,STM32MP157Axx, STM32MP157Dxx ,STM32MP157Fxx devices

This driver is composed of the descriptions of the registers under “Include” directory.

Various template file are provided to easily build an application. They can be adapted to fit applications requirements.

- Templates/system_stm32mp1xx.c contains the initialization code referred as SystemInit.
- Startup and linker files are provided as example for STM32CubeIDE©.
  
:::

::: {.col-sm-12 .col-lg-8}

# __Update History__

::: {.collapse}
<input type="checkbox" id="collapse-section0" checked aria-hidden="true">
<label for="collapse-section0" checked aria-hidden="true">__V1.6.0 / 02-June-2022__</label>
<div>

## Main Changes

This is a **Maintenance release** for STM32MP15xx CMSIS

## Contents

- Update bit definition in header files:
  - BSEC : Add missing registers
  - ETH : Update bitfield names
- Update License declaration for startup and linker files
- Change include in system file ( alignment with other  STM32 families)

## Known Limitations

None

## Dependencies

None

</div>
:::

::: {.collapse}
<input type="checkbox" id="collapse-section6" aria-hidden="true">
<label for="collapse-section6" aria-hidden="true">__V1.5.0 / 04-November-2021__</label>
<div>			

## Main Changes

This is a **Maintenance release** for STM32MP15xx CMSIS

## Contents

- Update bit definition in header files:
  - DDR : Update DDR bit registers
  - USBPHYC: Update structure and add bitfields
  - Fix MISRA warnings:
    - Remove duplicate definitions
    - Use 'UL' postfix for _Msk definitions and memory/peripheral base addresses
    - Unexpected space in "startup_stm32mp151axx_cm4 .s"
  - Linker script : 
    - Prevent text section to overide data ( resource table )
	- Suppress the check and add  AT (ADDR (.bss)) to set the LMA to the VMA.
  - Update the licenses declaration

## Known Limitations

None

## Dependencies

None

</div>
:::

::: {.collapse}
<input type="checkbox" id="collapse-section5" aria-hidden="true">
<label for="collapse-section5" aria-hidden="true">__V1.4.0 / 26-February-2021__</label>
<div>			

## Main Changes

This is a **Maintenance release** for STM32MP15xx CMSIS

## Contents

- Header files:
  - Remove C1EMRx registers which does not exist on the silicon
  - Update RNG register structure

## Known Limitations

None

## Dependencies

None

</div>
:::

::: {.collapse}
<input type="checkbox" id="collapse-section4" aria-hidden="true">
<label for="collapse-section4" aria-hidden="true">__V1.3.0 / 20-October-2020__</label>
<div>			

## Main Changes

This is a **Maintenance release** for STM32MP15xx CMSIS

## Contents

- Header files:
  - Rename RCC bit definition to be more compliant with the name from RCC spec
  - Update license with BSD 3-Clause template
  - Fix typo in MDMA register definition

## Known Limitations

None

## Dependencies

None

</div>
:::

::: {.collapse}
<input type="checkbox" id="collapse-section3" aria-hidden="true">
<label for="collapse-section3" aria-hidden="true">__V1.2.0 / 03-February-2020__</label>
<div>			

## Main Changes

This is a **Maintenance release** for STM32MP15xx CMSIS

## Contents

- Header files:
  - Add new Part Number for 800MHz
  - Update license with BSD 3-Clause template
  - Rework CMSIS for RTC/TAMP, GPIO and TIM
  - Rename TIM Break source bit definition
- Update Linker Template file for KEIL and IAR:
  - Add OpenAMP region ( region present by default, to comment if needed )

## Known Limitations

None

## Dependencies

None

</div>
:::

::: {.collapse}
<input type="checkbox" id="collapse-section2" aria-hidden="true">
<label for="collapse-section2" aria-hidden="true">__V1.1.0 / 10-September-2019__</label>
<div>			

## Main Changes

This is the **First Maintenance release** for STM32MP15xx CMSIS

## Contents

- Header files:
  - Update FMC bit definition
  - Update ETH bit definition
  - update EXTI_EXTICR bit definition
  - Update I2C bit definition
  - Update SPI bit definition (SPI_CR1_CRC33_17, SPI_RXCRC, SPI_IER, SPI_I2SCFGR)
  - TMPSENS IP renamed DTS
  - Update FDCAN TXBC bit definition
  - Update DAC_DHR8RD bit definition
  - Add CRYP in STM32MP151Cx and STM32MP153Cx
  - Update TIM / LPTIM bit definition + add missing macros
- Update system_stm32mp1xx.c:
  - Update SystemCoreClock
- Update startup file for KEIL and IAR

## Known Limitations

None

## Dependencies

None

</div>
:::

::: {.collapse}
<input type="checkbox" id="collapse-section1" aria-hidden="true">
<label for="collapse-section1" aria-hidden="true">__V1.0.0 / 22-January-2019__</label>
<div>			

## Main Changes

This is the **First Official release** for STM32MP15xx CMSIS

## Contents

- First official release version of bits and registers definition aligned with STM32MP1 reference manual.

## Known Limitations

None

## Dependencies

None

</div>
:::

:::
:::

<footer class="sticky">
For complete documentation on STM32 Microcontrollers </mark> ,
visit: [[www.st.com/stm32](http://www.st.com/stm32)]{style="font-color: blue;"}
</footer>
