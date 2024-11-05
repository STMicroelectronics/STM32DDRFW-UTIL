/**
  ******************************************************************************
  * @file    ddr_tool.c
  * @author  MCD Application Team
  * @brief   Main file for HAL DDR tool.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2023 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "stm32_device_hal.h"

#include "string.h"
#include "stdlib.h"
#include "ddr_tool.h"
#include "stm32mp_util_conf.h"

/* Private typedef -----------------------------------------------------------*/
typedef struct {
  uint32_t (*fct)();
  const char *name;
  const char *usage;
  const char *help;
  uint8_t max_args;
} subcmd_desc;

typedef enum {
  DDR_CMD_HELP = 0,
  DDR_CMD_INFO,
  DDR_CMD_FREQ,
  DDR_CMD_RESET,
  DDR_CMD_PARAM,
  DDR_CMD_PRINT,
  DDR_CMD_EDIT,
  DDR_CMD_SAVE,
  DDR_CMD_STEP,
  DDR_CMD_NEXT,
  DDR_CMD_GO,
  DDR_CMD_TEST,
  DDR_CMD_UNKNOWN,
  DDR_CMD_TEST_HELP,
  DDR_CMD_MAX,
} ddr_cmd_id;

typedef struct {
  char *str;
  uint8_t param_min;
  uint8_t param_max;
} ddr_command;

typedef struct {
  uint8_t off;
  uint8_t len;
} ddr_arg;

/* Private define ------------------------------------------------------------*/
#define CMD_MAX_LEN 1024
#define CMD_MAX_ARG 5
#define DDR_NAME_MAX_LEN 128

static uint32_t DDR_Test_All(uint32_t loop, uint32_t size, uint32_t addr);

const subcmd_desc test[] = {
  {DDR_Test_All, "Test All",
   "[none] | [loop] | [loop] [size] | [loop] [size] [addr]",
   "Execute all tests", 3},
  {DDR_Test_Databus, "Test Simple DataBus", "[addr]",
   "Verifies each data line by walking 1 on fixed address", 1},
  {DDR_Test_DatabusWalk0, "Test DataBusWalking0", "[loop] [addr]",
   "Verifies each data bus signal can be driven low (32 word burst)", 2},
  {DDR_Test_DatabusWalk1, "Test DataBusWalking1", "[loop] [addr]",
   "Verifies each data bus signal can be driven high (32 word burst)", 2},
  {DDR_Test_AddressBus, "Test AddressBus", "[size] [addr]",
   "Verifies each relevant bits of the address and checking for aliasing", 2},
  {DDR_Test_MemDevice, "Test MemDevice", "[size] [addr]",
   "Test the integrity of a physical memory", 2},
  {DDR_Test_SimultaneousSwitchingOutput, "Test SimultaneousSwitchingOutput",
   "[size] [addr] ", "Stress the data bus over an address range", 2},
  {DDR_Test_Noise, "Test Noise", "[pattern] [addr]",
   "Verifies r/w while forcing switching of all data bus lines.", 2},
  {DDR_Test_NoiseBurst, "Test NoiseBurst", "[size] [pattern] [addr]",
   "burst transfers while forcing switching of the data bus lines", 3},
  {DDR_Test_Random, "Test Random", "[size] [loop] [addr]",
   "Verifies r/w and memcopy(burst for pseudo random value", 3},
  {DDR_Test_FrequencySelectivePattern, "Test FrequencySelectivePattern",
   "[size] [addr]", "write & test patterns: Mostly Zero, Mostly One and F/n",
   2},
  {DDR_Test_BlockSequential, "Test BlockSequential", "[size] [loop] [addr]",
   "test incremental pattern", 3},
  {DDR_Test_Checkerboard, "Test Checkerboard", "[size] [loop] [addr]",
   "test checker pattern", 3},
  {DDR_Test_BitSpread, "Test BitSpread", "[size] [loop] [addr]",
   "test Bit Spread pattern", 3},
  {DDR_Test_BitFlip, "Test BitFlip", "[size] [loop] [addr]",
   "test Bit Flip pattern", 3},
  {DDR_Test_WalkingZeroes, "Test WalkingZeroes", "[size] [loop] [addr]",
   "test Walking Ones pattern", 3},
  {DDR_Test_WalkingOnes, "Test WalkingOnes", "[size] [loop] [addr]",
   "test Walking Zeroes pattern", 3},
#ifdef TEST_INFINITE_ENABLE
  {DDR_Test_Infinite_write, "Test infinite write for JEDEC", "[pattern] [addr]",
   "test infinite write pattern", 2},
  {DDR_Test_Infinite_read, "Test infinite read for JEDEC", "[pattern] [addr]",
   "test infinite read pattern", 2},
#endif
};

const int test_nb = sizeof(test) / sizeof(test[0]);

const char *step_str[] = {
    [STEP_DDR_RESET] = "DDR_RESET",
    [STEP_CTL_INIT]  = "DDR_CTRL_INIT_DONE",
    [STEP_PHY_INIT]  = "DDR_PHY_INIT_DONE",
    [STEP_DDR_READY] = "DDR_READY",
    [STEP_RUN]       = "RUN"
};

const ddr_command ddr_cmd[DDR_CMD_MAX] = {
    [DDR_CMD_HELP]         = { "help"       , 0, 0 },
    [DDR_CMD_INFO]         = { "info"       , 0, CMD_MAX_ARG },
    [DDR_CMD_FREQ]         = { "freq"       , 0, 1 },
    [DDR_CMD_RESET]        = { "reset"      , 0, 0 },
    [DDR_CMD_PARAM]        = { "param"      , 0, 2 },
    [DDR_CMD_PRINT]        = { "print"      , 0, 1 },
    [DDR_CMD_EDIT]         = { "edit"       , 2, 2 },
    [DDR_CMD_SAVE]         = { "save"       , 0, 0 },
    [DDR_CMD_STEP]         = { "step"       , 0, 1 },
    [DDR_CMD_NEXT]         = { "next"       , 0, 0 },
    [DDR_CMD_GO]           = { "go"         , 0, 0 },
    [DDR_CMD_TEST]         = { "test"       , 0, CMD_MAX_ARG },
    [DDR_CMD_TEST_HELP]    = { "test help"  , 0, 0 },
};

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
extern HAL_DDR_ConfigTypeDef static_ddr_config;

/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

static uint32_t DDR_Test_All(uint32_t loop, uint32_t size, uint32_t addr)
{
  uint32_t ret = 0;
  int i;

#ifdef TEST_INFINITE_ENABLE
  for (i = 1; i < (int)test_nb - 2; i++)
#else
  for (i = 1; i < (int)test_nb; i++)
#endif
  {
    switch (test[i].max_args)
    {
      case 1:
        ret = test[i].fct(addr);
        break;
      case 2:
        if (   (test[i].fct == DDR_Test_DatabusWalk0)
            || (test[i].fct == DDR_Test_DatabusWalk1))
        {
          ret = test[i].fct(loop, addr);
        }
        else if (test[i].fct == DDR_Test_Noise)
        {
          ret = test[i].fct(0, addr);
        }
        else
        {
          ret = test[i].fct(size, addr);
        }
        break;
      case 3:
        if (test[i].fct == DDR_Test_NoiseBurst)
        {
          ret = test[i].fct(size, 0, addr);
        }
        else
        {
          ret = test[i].fct(size, loop, addr);
        }
        break;
    }

    if (ret != 0)
    {
      printf("%s failed [%d]\n\r", test[i].name, ret);
      return ret;
    }

    printf("result %d:%s = Passed\n\r", i, test[i].name);
  }

  return ret;
}

static void get_entry_string(char *entry)
{
  uint8_t user_entry_valid = 0;
  char user_entry_value = 0;
  int i = 0;

  Serial_Putchar(0xd);
  Serial_Putchar('D');
  Serial_Putchar('D');
  Serial_Putchar('R');
  Serial_Putchar('>');

  while(user_entry_valid == 0)
  {
    /* Scan for user entry */
    user_entry_value = Serial_Scanf(255);
    /* Print the entry character */
    Serial_Putchar(user_entry_value);

    if (user_entry_value == 0xd)
    {
      entry[i] = '\0';
      printf("\r\n");
      user_entry_valid = 1;
    }
    else if (user_entry_value == 0x18)
    {
      i--;
    }
    else
    {
      entry[i++] = user_entry_value;
    }
  }
}

static int parse_entry_string(char *entry, size_t size, int *command,
                              char *argv[])
{
  int i = 0;
  int n = 0;
  ddr_arg cmd = {0, 0};
  ddr_arg arg = {0, 0};

  /* Identify command and parameters */
  for (i = 0; i < (int)size; i++)
  {
    if (entry[i] == '\0')
    {
      break;
    }
    if (entry[i] == ' ')
    {
      continue;
    }

    if (n == 0)
    {
      cmd.off = i;
    }
    else
    {
      arg.off = i;
    }

    while ((entry[i] != ' ') && (entry[i] != '\0'))
    {
      i++;
    }

    if (n == 0)
    {
      cmd.len = i - cmd.off;
    }
    else
    {
      arg.len = i - arg.off;
      strncpy(argv[n - 1], &entry[arg.off], arg.len);
      strncpy(argv[n - 1] + arg.len, "\0", 1);
    }

    n++;

    if (n > CMD_MAX_ARG + 1)
    {
      printf("Too many arguments (max=%d)\n\r", CMD_MAX_ARG);
      return -1;
    }

    i--;
  }

  /* Case of empty string, simple ignore, no error */
  if ((n == 0) && (cmd.len == 0))
  {
    return 0;
  }

  /* Get command */
  for (i = 0; i < DDR_CMD_UNKNOWN; i++)
  {
    if (   !strncmp(&entry[cmd.off], ddr_cmd[i].str, cmd.len)
        && (cmd.len == strlen(ddr_cmd[i].str)))
    {
      *command = i;
      break;
    }
  }

  /* Case of unknown command */
  if (i == DDR_CMD_UNKNOWN)
  {
    printf("Unknown command\n\r");
    return -2;
  }

  /* Check number of parameters */
  if ((n - 1) < ddr_cmd[*command].param_min)
  {
    printf("%s: argument missing (min=%d)\n\r", ddr_cmd[*command].str,
                                                ddr_cmd[*command].param_min);
    return -3;
  } else if ((n - 1) > ddr_cmd[*command].param_max)
  {
    printf("%s: too many arguments (max=%d)\n\r", ddr_cmd[*command].str,
                                                  ddr_cmd[*command].param_max);
    return -4;
  }

  /* Case of commands with "help" first parameter, ignore other ones */
  if ((n >= 2) && (argv[0] != NULL))
  {
    if (!strcmp(argv[0], "help"))
    {
      switch (*command)
      {
      case DDR_CMD_TEST:
            *command = DDR_CMD_TEST_HELP;
            break;
      }
      return 1;
    }
  }

  /* return number of arguments, including command and parameters */
  return n;
}

static int64_t string_to_num(char *string)
{
  char *end_ptr;
  uint64_t val = strtoul(string, &end_ptr, 0);

  if (end_ptr == string)
  {
    return -1;
  }

  return val;
}

static void print_usage(void)
{
  const char *usage = {
    "commands:\n\r\n\r"
    "help                       displays help\n\r"
    "info                       displays DDR information\n\r"
    "info  <param> <val>        changes DDR information\n\r"
    "      with <param> = step, name, size or speed\n\r"
    "freq                       displays the DDR PHY frequency in kHz\n\r"
    "freq  <freq>               changes the DDR PHY frequency\n\r"
    "param [type|reg]           prints input parameters\n\r"
    "param <reg> <val>          edits parameters in step 0\n\r"
    "print [type|reg]           dumps registers\n\r"
    "edit <reg> <val>           modifies one register\n\r"
    "save                       output formated DDR regs to be saved\n\r"
    "step                       lists the available step\n\r"
    "step <n>                   go to the step <n>\n\r"
    "next                       goes to the next step\n\r"
    "go                         continues the DDR TOOL execution\n\r"
    "reset                      reboots machine\n\r"
    "test [help] | <n> [...]    lists (with help) or executes test <n>\n\r"
    "\n\rwith for [type|reg]:\n\r"
    "  all registers if absent\n\r"
    "  <type> = ctl, uib, uia, uim, uis\n\r"
    "  <reg> = name of the register\n\r"
  };

  printf(usage);
}

static void print_single_subcmd_usage(const subcmd_desc *array, int i)
{

      printf("  %2d: %s %s\n\r", i, array[i].name, array[i].usage);
      printf("          %s\n\r", array[i].help);
}

static void print_subcmd_usage(const subcmd_desc *array, const int size)
{
  int i;

  printf("commands:\n\r\n\r");

  for (i = 0; i < size; i++)
  {
      print_single_subcmd_usage(array, i);
  }
}

static bool check_step(HAL_DDR_InteractStepTypeDef step,
                       HAL_DDR_InteractStepTypeDef expected)
{
  if (step != expected)
  {
    printf("invalid step %d:%s expecting %d:%s\n\r",
           step, step_str[step], expected, step_str[expected]);
    return false;
  }

  return true;
}

static bool pll2_set_rate(uint64_t pll2_clk_rate)
{
  RCC_PLLInitTypeDef PLL2;
  uint64_t fck_ref;
  uint64_t fbdiv;
  uint64_t value;

  HAL_RCCEx_GetPLL2Config(&PLL2);

  switch (PLL2.PLLSource)
  {
    case RCC_PLLSOURCE_HSI:
      fck_ref = HSI_VALUE;
      break;
    case RCC_PLLSOURCE_HSE:
      fck_ref = HSE_VALUE;
      break;
    default:
      printf("PLL2 OSC clock source is not set properly\n\r");
      return false;
  }

  /* Disable the PLL2 */
  PLL2.PLLState = RCC_PLL_OFF;
  if (HAL_RCCEx_PLL2Config(&PLL2) != HAL_OK)
  {
    return false;
  }

  /* Calculate new settings */
  fbdiv = pll2_clk_rate * PLL2.POSTDIV1 * PLL2.POSTDIV2 * PLL2.FREFDIV;
  PLL2.FBDIV = fbdiv / fck_ref;
  value = fbdiv % fck_ref;
  PLL2.FRACIN = (value * (1 << 24)) / fck_ref;

  /* Configure and enable the PLL2 */
  PLL2.PLLState = RCC_PLL_ON;
  if (HAL_RCCEx_PLL2Config(&PLL2) != HAL_OK)
  {
    return false;
  }

  return true;
}

static void do_info(HAL_DDR_InteractStepTypeDef step, int argc, char *argv[])
{
  unsigned long value;
  static char ddr_name[DDR_NAME_MAX_LEN];
  char *end_ptr;

  if (argc == 1)
  {
    printf("step = %d : %s\n\r", step, step_str[step]);
    printf("name = %s\n\r", static_ddr_config.info.name);
    printf("size = 0x%lx\n\r", static_ddr_config.info.size);
    printf("speed = %d kHz\n\r", static_ddr_config.info.speed);
    return;
  }

  if (argc < 3)
  {
    printf("not enough parameters\n\r");
    return;
  }

  if (!strcmp(argv[0], "name"))
  {
    int i;
    uint32_t name_len = argc;

    for (i = 1; i < (argc - 1); i++)
    {
      name_len += strlen(argv[i]) + 1;
    }

    static_ddr_config.info.name = ddr_name;

    strcpy(ddr_name, argv[1]);
    for (i = 2; i < (argc - 1); i++)
    {
      strcat(ddr_name, " ");
      strcat(ddr_name, argv[i]);
    }
    printf("name = %s\n\r", ddr_name);
    return;
  }

  if (!strcmp(argv[0], "size"))
  {
    value = strtoll(argv[1], &end_ptr, 16);
    if (end_ptr == argv[1])
    {
      printf("invalid value %s\n\r", argv[1]);
    }
    else
    {
      static_ddr_config.info.size = value;
      printf("size = 0x%lx\n\r", static_ddr_config.info.size);
    }
    return;
  }

  if (!strcmp(argv[0], "speed"))
  {

    value = strtoll(argv[1], &end_ptr, 10);
    if (end_ptr == argv[1])
    {
      printf("invalid value %s\n\r", argv[1]);
    }
    else
    {
      static_ddr_config.info.speed = value;
      printf("speed = %d kHz\n\r", static_ddr_config.info.speed);
      value = (unsigned long)HAL_RCCEx_GetPLL2ClockFreq() * 2;
      printf("DDRPHY = %ld kHz\n\r", value / 1000);
    }
    return;
  }

  printf("argument %s invalid\n\r", argv[0]);
}

static bool do_freq(int argc, char *argv[])
{
  unsigned long frequency;
  unsigned long value;
  char *end_ptr;
  bool ret_code = true;

  if (argc == 2)
  {
    /* DDR freq = PLL2 freq * 2 */
    value = (strtoll(argv[0], &end_ptr, 0) * 1000) / 2;
    if (end_ptr == argv[0])
    {
      printf("invalid argument %s\n\r", argv[0]);
      return false;
    }

    frequency = (unsigned long)HAL_RCCEx_GetPLL2ClockFreq();
    if (value == frequency)
    {
      printf("Same frequency, no change.\n\r");
      return true;
    }
    else if (frequency == 0)
    {
      printf("Frequency has never been set before, no change.\n\r");
      return true;
    }
    else
    {
      ret_code = pll2_set_rate(value);
    }
  }

  frequency = (unsigned long)HAL_RCCEx_GetPLL2ClockFreq();
  printf("DDRPHY = %ld kHz\n\r", (2 * frequency) / 1000);

  return ret_code;
}

static void do_param(HAL_DDR_InteractStepTypeDef step, int argc, char *argv[])
{
  char reg_name[(argc == 1)? 0 : strlen(argv[0])];

  switch (argc)
  {
  case 1:
    HAL_DDR_Dump_Param(&static_ddr_config, NULL);
    break;
  case 2:
    HAL_DDR_Convert_Case(argv[0], reg_name, 1); /* convert to upper case */

    if (HAL_DDR_Dump_Param(&static_ddr_config, reg_name) != HAL_OK)
    {
      printf("invalid argument %s\n\r", argv[0]);
    }
    break;
  case 3:
    if (!check_step(step, STEP_DDR_RESET))
    {
      return;
    }
    HAL_DDR_Convert_Case(argv[0], reg_name, 1); /* convert to upper case */
    HAL_DDR_Edit_Param(&static_ddr_config, reg_name, argv[1]);
    break;
  }
}

static void do_print(int argc, char * const argv[])
{
  char reg_name[(argc == 1)? 0 : strlen(argv[0])];

  switch (argc)
  {
    case 1:
      HAL_DDR_Dump_Reg(NULL, false);
      break;
    case 2:
      HAL_DDR_Convert_Case(argv[0], reg_name, 1); /* convert to upper case */

      if (HAL_DDR_Dump_Reg(reg_name, false) != HAL_OK)
      {
        printf("invalid argument %s\n\r", reg_name);
      }
      break;
  }
}

static HAL_DDR_InteractStepTypeDef do_step(HAL_DDR_InteractStepTypeDef step,
                                           int argc, char *argv[])
{
  int i;
  unsigned long value;
  char *end_ptr;

  switch (argc)
  {
    case 1:
      for (i = 0; i <= STEP_RUN; i++)
      {
        printf("%d:%s\n\r", i, step_str[i]);
      }
      break;

    case 2:
      value = strtoll(argv[0], &end_ptr, 0);
      if ((end_ptr == argv[0]) || (value > STEP_RUN))
      {
        printf("invalid argument %s\n\r", argv[1]);
        goto end;
      }

      if ((value != STEP_DDR_RESET) && (value <= step))
      {
        printf("invalid target %d:%s, current step is %d:%s\n\r",
               (int)value, step_str[value], step, step_str[step]);
        goto end;
      }
      printf("step to %d:%s\n\r", (int)value, step_str[value]);
      return (HAL_DDR_InteractStepTypeDef)value;
  };

end:
  return step;
}

static char argv0[CMD_MAX_LEN / 4] = "\0";
static char argv1[CMD_MAX_LEN / 4] = "\0";
static char argv2[CMD_MAX_LEN / 4] = "\0";
static char argv3[CMD_MAX_LEN / 4] = "\0";

static void do_subcmd(int argc, char *argv[], const subcmd_desc *array,
                      const int size)
{
  int i;
  int64_t value;
  int local_argc = argc;
  uint32_t retcode;

  if (local_argc == 1)
  {
    printf("Please enter a sub command number\n\r\n\r");
    print_subcmd_usage(array, size);
    return;
  }

  /* Check sub command ID */
  value = string_to_num(argv[0]);
  if ((value < 0) || (value >= size))
  {
    printf("Unknown sub command [%s]\n\r\n\r", argv[0]);
    print_subcmd_usage(array, size);
    return;
  }

  /*
   * Test All command is also differently treated, regrding number of
   * arguments (0, 1, 2). So complete arguments to have a generic API including
   * number of valid arguments in third position.
   */
  if ((array == test) && (array[value].fct == DDR_Test_All))
  {
    switch (local_argc - 2)
    {
    case 0:
      argv1[0] = argv2[0] = argv3[0] = '0';
      argv1[1] = argv2[1] = argv3[1] = '\0';
      local_argc = array[value].max_args + 2;
      break;
    case 1:
      argv2[0] = argv3[0] = '0';
      argv2[1] = argv3[1] = '\0';
      local_argc = array[value].max_args + 2;
      break;
    case 2:
      argv3[0] = '0';
      argv3[1] = '\0';
      local_argc = array[value].max_args + 2;
      break;
    case 3:
      local_argc = array[value].max_args + 2;
      break;
    default:
      local_argc = -1;
      break;
    }
  }

  /* Check number of sub command arguments */
  if ((uint8_t)(local_argc - 2) != array[value].max_args)
  {
    /* any other cases */
    printf("Incorrect number of arguments\n\r\n\r");
    print_single_subcmd_usage(array, value);
    return;
  }

  /* Check argument validity */
  for (i = 1; i <= (local_argc - 2); i++)
  {
    if (string_to_num(argv[i]) < 0)
    {
      printf("Invalid argument #%d %s\n\r\n\r", i, argv[i]);
      print_single_subcmd_usage(array, value);
      return;
    }
  }

  switch (array[value].max_args)
  {
    case 0:
      retcode = array[value].fct();
      break;
    case 1:
      retcode = array[value].fct((uint32_t)string_to_num(argv[1]));
      break;
    case 2:
      retcode = array[value].fct((uint32_t)string_to_num(argv[1]),
                                (uint32_t)string_to_num(argv[2]));
      break;
    case 3:
      retcode = array[value].fct((uint32_t)string_to_num(argv[1]),
                                (uint32_t)string_to_num(argv[2]),
                                (uint32_t)string_to_num(argv[3]));
      break;
    default:
      printf("Number of arguments not supported\n\r");
      retcode = 0XFFFFFFFF;
      break;
  }

  if (retcode != 0)
  {
    printf("%s failed [%d]\n\r", array[value].name, retcode);
    return;
  }

    printf("Result: Pass [%s]\n\r", array[value].name);
}

bool HAL_DDR_Interactive(HAL_DDR_InteractStepTypeDef step)
{
  char buffer[CMD_MAX_LEN];
  char *argv[CMD_MAX_ARG + 1] = {argv0, argv1, argv2, argv3}; /* NULL terminated */
  int argc;
  int cmd;
  static int next_step = -1;

  if ((next_step < 0) && (step == STEP_DDR_RESET))
  {
    next_step = STEP_DDR_RESET;
  }

//  printf("** step %d ** %s / %d\n\r", step, step_str[step], next_step);

  if (next_step < 0)
  {
    return false;
  }

  if (step > STEP_RUN)
  {
    printf("** step %d ** INVALID\n\r", step);
    return false;
  }

  printf("%d:%s\n\r", step, step_str[step]);

  if ((HAL_DDR_InteractStepTypeDef)next_step > step)
  {
    return false;
  }

  while ((HAL_DDR_InteractStepTypeDef)next_step == step)
  {
    get_entry_string(buffer);

    argc = parse_entry_string(buffer, strlen(buffer), &cmd, argv);
    if (argc < 0)
    {
      printf("Error [%d]\n\r", argc);
      continue;
    }
    else if (argc == 0)
    {
      continue;
    }

    switch (cmd)
    {
    case DDR_CMD_HELP:
      print_usage();
      break;

    case DDR_CMD_TEST_HELP:
      print_subcmd_usage(test, test_nb);
      break;

    case DDR_CMD_INFO:
      do_info(step, argc, argv);
      break;

    case DDR_CMD_FREQ:
      do_freq(argc, argv);
      if ((argc == 2) && (step > STEP_CTL_INIT))
      {
        printf("### Please update PLL settings and DDR timings ###\n\r");
        printf("### in your project. Then rebuild and restart. ###\n\r");
      }
      break;

    case DDR_CMD_RESET:
      WRITE_REG(RCC->GRSTCSETR, 0x1U);
      break;

    case DDR_CMD_PARAM:
      do_param(step, argc, argv);
      break;

    case DDR_CMD_PRINT:
      do_print(argc, argv);
      break;

    case DDR_CMD_EDIT:
      HAL_DDR_Edit_Reg(argv0, argv1);
      break;

    case DDR_CMD_SAVE:
      HAL_DDR_Dump_Reg(NULL, true);
      break;

    case DDR_CMD_GO:
      next_step = STEP_RUN;
      printf("### Exit DDR INTERACTIVE mode. Please RESET the BOARD ###\n\r");
      break;

    case DDR_CMD_NEXT:
      next_step = step + 1;
      if (next_step == STEP_RUN)
      {
        printf("### Exit DDR INTERACTIVE mode. Please RESET the BOARD ###\n\r");
      }
      break;

    case DDR_CMD_STEP:
      next_step = do_step(step, argc, argv);
      break;

    case DDR_CMD_TEST:
      if (!check_step(step, STEP_DDR_READY))
      {
        continue;
      }
      do_subcmd(argc, argv, test, test_nb);
      break;

    default:
      break;
    }
  }

  return next_step == STEP_DDR_RESET;
}
