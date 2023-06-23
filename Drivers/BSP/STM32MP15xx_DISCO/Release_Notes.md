---
pagetitle: Release Notes for STM32MP1xx-Disco BSP Drivers
lang: en
header-includes: <link rel="icon" type="image/x-icon" href="_htmresc/favicon.png" />
---
::: {.row}
::: {.col-sm-12 .col-lg-4}

<center>
# Release Notes for <mark>STM32MP1xx-Disco BSP Drivers</mark>
Copyright &copy; 2021 STMicroelectronics\
    
[![ST logo](_htmresc/st_logo_2020.png)](https://www.st.com){.logo}
</center>

# Purpose

The BSP (Board Specific Package) drivers are parts of the STM32Cube package based on the HAL drivers and provide a set of high level APIs relative to the hardware components and features in the disco boards coming with the STM32Cube package for a given STM32 serie.

The BSP drivers allow a quick access to the boards’ services using high level APIs and without any specific configuration as the link with the HAL and the external components is done in intrinsic within the drivers.

From project settings points of view, user has only to add the necessary driver’s files in the workspace and call the needed functions from examples. However some low level configuration functions are weak and can be overridden by the applications if user wants to change some BSP drivers default behavior.
:::

::: {.col-sm-12 .col-lg-8}
# Update History

::: {.collapse}
<input type="checkbox" id="collapse-section0" checked aria-hidden="true">
<label for="collapse-section0" aria-hidden="true">__V1.6.0 / 02-June-2022__</label>
<div>			

## Main Changes

**Maintenance Release of STMP32MP1xx_DISCO BSP**

## Contents

- Add BSP_Error_Handler() to manage BSP Errors

## Known Limitations

- None

## Dependencies

- None

## Notes

- stm32mp15xx_disco_conf_template.h file must be copied in user application as >stm32mp15xx_disco_conf.h with optional configuration update

</div>
:::

::: {.collapse}
<input type="checkbox" id="collapse-section6" aria-hidden="true">
<label for="collapse-section6" aria-hidden="true">__V1.5.0 / 04-November-2021__</label>
<div>			

## Main Changes

**Maintenance Release of STMP32MP1xx_DISCO BSP**

## Contents

- Update the licenses declaration

## Known Limitations

- None

## Dependencies

- None

## Notes

stm32mp15xx_disco_conf_template.h file must be copied in user application as >stm32mp15xx_disco_conf.h with optional configuration update

</div>
:::

::: {.collapse}
<input type="checkbox" id="collapse-section5" aria-hidden="true">
<label for="collapse-section5" aria-hidden="true">__V1.4.0 / 27-January-2021__</label>
<div>			

## Main Changes

**Maintenance Release** of STMP32MP15xx_DISCO BSP

## Contents

- No Update

## Known Limitations

- None

## Dependencies

- None

## Notes

stm32mp15xx_disco_conf_template.h file must be copied in user application as >stm32mp15xx_disco_conf.h with optional configuration update

</div>
:::

::: {.collapse}
<input type="checkbox" id="collapse-section4" aria-hidden="true">
<label for="collapse-section4" aria-hidden="true">__V1.3.0 / 20-October-2020__</label>
<div>			

## Main Changes

**Maintenance Release** of STMP32MP15xx_DISCO BSP

## Contents

- Fix unterminated extern "C" definitions:
  - impacted files : stm32mp15xx_disco_conf_template.h and stm32mp15xx_disco_errno.h

## Known Limitations

- None

## Dependencies

- None

## Notes

stm32mp15xx_disco_conf_template.h file must be copied in user application as >stm32mp15xx_disco_conf.h with optional configuration update

</div>
:::

::: {.collapse}
<input type="checkbox" id="collapse-section3" aria-hidden="true">
<label for="collapse-section3" aria-hidden="true">__V1.2.0 / 03-February-2020__</label>
<div>			

## Main Changes

**Maintenance Release** of STMP32MP15xx_DISCO BSP

## Contents

- No update

## Known Limitations

- None

## Dependencies

- None

## Notes

stm32mp15xx_disco_conf_template.h file must be copied in user application as >stm32mp15xx_disco_conf.h with optional configuration update

</div>
:::

::: {.collapse}
<input type="checkbox" id="collapse-section2" aria-hidden="true">
<label for="collapse-section2" aria-hidden="true">__V1.1.0 / 10-September-2019__</label>
<div>			

## Main Changes

**First Maintenance Release** of STMP32MP15xx_DISCO BSP

## Contents

- Add support of console input from COM

- Rename stm32mp15xx_eval_stpmu1 file to stm32mp15xx_eval_stpmic1

## Known Limitations

- None

## Dependencies

- None

## Notes

stm32mp15xx_disco_conf_template.h file must be copied in user application as >stm32mp15xx_disco_conf.h with optional configuration update

</div>
:::

::: {.collapse}
<input type="checkbox" id="collapse-section1" aria-hidden="true">
<label for="collapse-section1" aria-hidden="true">__V1.0.0 / 22-January-2019__</label>
<div>			

## Main Changes

**First official release STM32MP15xx-DISCO** board drivers for STM32CubeMP1 FW package

## Known Limitations

- None

## Dependencies

- None

## Notes

- stm32mp15xx_disco_conf_template.h file must be copied in user application as >stm32mp15xx_disco_conf.h with optional configuration update

</div>
:::

:::
:::

<footer class="sticky">
For complete documentation on <mark>STM32 Microcontrollers</mark> ,
visit: [[www.st.com](http://www.st.com/STM32)]{style="font-color: blue;"}
</footer>
