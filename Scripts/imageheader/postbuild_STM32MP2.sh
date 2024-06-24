#!/bin/bash -
# ******************************************************************************
# @file      postbuild_STM32MP2.sh
# @author    MCD Application Team
# @brief     postbuild command script
#*****************************************************************************
# @attention
#
# Copyright (c) 2022 STMicroelectronics.
# All rights reserved.
#
# This software is licensed under terms that can be found in the LICENSE file
# in the root directory of this software component.
# If no LICENSE file comes with this software, it is provided AS-IS.
#
#*****************************************************************************

# arg1 : toolchain bin directory path
# arg2 : elf file base name (without .elf extension)
CompilerPath="${1}"
elf_file_basename="${2}"

# Default parameters
header_version="2.0"
cpu_name="8-M.MAIN"
debug=0

# Default memory mapping
sysram_start_lowbits=0x00000000
sysram_end_lowbits=0x0003FFFF
retram_start_lowbits=0x00080000
retram_end_lowbits=0x0009FFFF

local_script_path=$(dirname $0)
local_script_path=$(readlink -f ${local_script_path})

while [[ $# -gt 2 ]]; do
    key="$3"

    case $key in
        -v|--version)
        version="$4"
        shift
        shift
        ;;
        -c|--cpu)
        cpu="$4"
        shift
        shift
        ;;
        *) echo "Wrong parameter : $3"
        exit 1
        ;;
  esac
done

if [ -n "$version" ] ; then
  case "$version" in
    2.0)
      header_version="$version"
      ;;
    2.1)
      header_version="$version"
      ;;
    2.2)
      header_version="$version"
      ;;
    *)
      echo "ERROR header version not supported : $version"
      exit 1
	  ;;
  esac
fi
echo "header_version = $header_version"

if [ -n "$cpu" ] ; then
  case "$cpu" in
    8-A)
      cpu_name="$cpu"
      ;;
    8-M.MAIN)
      cpu_name="$cpu"
      ;;
    6S-M)
      cpu_name="$cpu"
      ;;
    *)
      echo "ERROR cpu name not supported : $cpu"
      exit 1
	  ;;
  esac
fi
echo "cpu_name = $cpu_name"

case "$(uname -s)" in
  Linux)
    #line for python
    echo Postbuild with python script
    imgtool="${local_script_path}/Python3/Stm32ImageAddHeader.py"
    cmd="python"
    ;;
  *)
    #line for window executable
    echo Postbuild with windows executable
    imgtool="${local_script_path}/exe.win-amd64-2.7/Stm32ImageAddHeader.exe"
    cmd=""
    ;;
esac

readelf_path=$(find ${CompilerPath} -name "*readelf*" -print)
objcopy_path=$(find ${CompilerPath} -name "*objcopy*" -print)

if [ ${debug} -ne 0 ] ; then
  echo "<D> readelf_path                =<${readelf_path}>"
  echo "<D> objcopy_path                =<${objcopy_path}>"
fi

elf_entry_point=$(${readelf_path} -h ${elf_file_basename}.elf | grep "Entry point address" | sed 's/.*0x\([^ ]*\)/0x\1/')
if [ ${debug} -ne 0 ] ; then
  echo "<D> elf_entry_point             =<$elf_entry_point>"
fi

# elf_entry_point is in format 0x%x so Reformat to 0x%08X and process entry-point address
formatted_ep_addr=$(printf "0x%08x" $((elf_entry_point)))
if [ ${debug} -ne 0 ] ; then
  echo "<D> formatted_ep_addr           =<$formatted_ep_addr>"
fi

if [ "${cpu_name}" == "" ] ; then
cpu_name=$(${readelf_path} -A ${elf_file_basename}.elf | grep "Tag_CPU_name" | sed -e 's/.*Tag_CPU_name: *\"\([^\"]*\)\"/\1/')
fi

if [ ${debug} -ne 0 ] ; then
  echo "<D> cpu_name                    =<$cpu_name>"
fi

if [ "${cpu_name}" == "8-A" ] ; then
  core="CA35"
elif [ "${cpu_name}" == "8-M.MAIN" ] ; then
  core="CM33"
elif [ "${cpu_name}" == "6S-M" ] ; then
  core="CM0PLUS"
  echo "ERROR ${core} not managed"
  exit 1
else
  core="Unknown"
  echo "ERROR core is unknown"
  exit 1
fi

${objcopy_path} -O binary ${elf_file_basename}.elf ${elf_file_basename}_pb.bin

if [ "${core}" == "CA35" ] ; then
  binary_type="0x00"
  allowedmem_start_lowbits="${sysram_start_lowbits}"
  allowedmem_end_lowbits="${sysram_end_lowbits}"
  ep_addr_for_image="0e${formatted_ep_addr:4:6}"
elif [ "${core}" == "CM33" ] ; then
  binary_type="0x30"
  allowedmem_start_lowbits="${retram_start_lowbits}"
  allowedmem_end_lowbits="${retram_end_lowbits}"
  ep_addr_for_image="0e${formatted_ep_addr:4:6}"
fi

if [ ${debug} -ne 0 ] ; then
  echo "<D> binary_type                 =<$binary_type>"
  echo "<D> allowedmem_start_lowbits    =<$allowedmem_start_lowbits>"
  echo "<D> allowedmem_end_lowbits      =<$allowedmem_end_lowbits>"
  echo "<D> ep_addr_for_image           =<$ep_addr_for_image>"
  echo "<D> 0x\${formatted_ep_addr:4:6}=<$((0x${formatted_ep_addr:4:6}))>"
  echo "<D> 0x\${allowedmem_start_lowbits:4:6}  =<$((0x${allowedmem_start_lowbits:4:6}))>"
  echo "<D> 0x\${allowedmem_end_lowbits:4:6}    =<$((0x${allowedmem_end_lowbits:4:6}))>"
fi

if [ \( $((0x${formatted_ep_addr:4:6})) -lt $((0x${allowedmem_start_lowbits:4:6})) \) -o \
     \( $((0x${formatted_ep_addr:4:6})) -gt $((0x${allowedmem_end_lowbits:4:6})) \) ] ; then
  echo "ERROR !! entry point address not in allowed memory (SYSRAM for A35, RETRAM for M33)"
  exit 1
fi

if [ "${core}" == "CA35" ] ; then
  # Add EL3_Launcher for A35 application
  el3_launcher_entry_point=$(${readelf_path} -h ${local_script_path}/EL3_Launcher/STM32MP2_el3_launcher.elf | grep "Entry point address" | sed 's/.*0x\([^ ]*\)/0x\1/')
  # elf_entry_point is in format 0x%x so Reformat to 0x%08X and process entry-point address
  formatted_el3_ep_addr=$(printf "0x%08x" $((el3_launcher_entry_point)))
  el3_launcher_ep_addr_for_image="0e${formatted_el3_ep_addr:4:6}"

  el3_launcher_end_addr=$(${readelf_path} -s ${local_script_path}/EL3_Launcher/STM32MP2_el3_launcher.elf | grep "EL3_STACK_BASE" | sed 's/.*: \([^ ]*\) .*/0x\1/')
  formatted_el3_launcher_end_addr=$(printf "0x%08x" $((el3_launcher_end_addr)))
  formatted_el3_launcher_end_addr="0x0e${formatted_el3_launcher_end_addr:4:6}"
  el3_launcher_end_offset=$((formatted_el3_launcher_end_addr-0x0E000000))

  cp ${local_script_path}/EL3_Launcher/STM32MP2_el3_launcher.bin ${elf_file_basename}_pad_el3lnch.bin
  el3_launcher_size=$(stat -c%s ${local_script_path}/EL3_Launcher/STM32MP2_el3_launcher.bin)
  el3_launcher_padding_size=$((el3_launcher_end_offset-el3_launcher_size-0x2600))

  no_el3_launcher_padding_size=$((el3_launcher_end_offset-0x2600))

  if [ ${debug} -ne 0 ] ; then
    echo "<D> el3_launcher_entry_point        =<$el3_launcher_entry_point>"
    echo "<D> formatted_el3_ep_addr           =<$formatted_el3_ep_addr>"
    echo "<D> el3_launcher_ep_addr_for_image  =<$el3_launcher_ep_addr_for_image>"
    echo "<D> el3_launcher_end_addr           =<$el3_launcher_end_addr>"
    echo "<D> formatted_el3_launcher_end_addr =<$formatted_el3_launcher_end_addr>"
    echo "<D> el3_launcher_size               =<$el3_launcher_size>"
    echo "<D> el3_launcher_padding_size       =<$el3_launcher_padding_size>"
  fi
  dd if=/dev/zero bs=1 count=${el3_launcher_padding_size} >>${elf_file_basename}_pad_el3lnch.bin 2>/dev/null
  cat ${elf_file_basename}_pad_el3lnch.bin ${elf_file_basename}_pb.bin >${elf_file_basename}_pb_el3lnch.bin
  # Add padding in place of el3 launcher for no_el3lnch image
  dd if=/dev/zero bs=1 count=${no_el3_launcher_padding_size} >${elf_file_basename}_no_el3lnch_pad.bin 2>/dev/null
  cat ${elf_file_basename}_no_el3lnch_pad.bin ${elf_file_basename}_pb.bin >${elf_file_basename}_pb_no_el3lnch.bin
fi

if [ "${core}" == "CA35" ] ; then
  command="${cmd} ${imgtool} ${elf_file_basename}_pb_el3lnch.bin ${elf_file_basename}.stm32 -bt ${binary_type} -ep ${el3_launcher_ep_addr_for_image} -hv ${header_version}"
  ${command}
  ret=$?
  if [ ${debug} -ne 0 ] ; then
    echo "<D> ret                         =<$ret>"
  fi
  command="${cmd} ${imgtool} ${elf_file_basename}.bin ${elf_file_basename}_nopad_no_el3lnch.stm32 -bt ${binary_type} -ep ${ep_addr_for_image} -hv ${header_version} -la 0x0E002600"
  ${command}
  command="${cmd} ${imgtool} ${elf_file_basename}_pb_no_el3lnch.bin ${elf_file_basename}_no_el3lnch.stm32 -bt ${binary_type} -ep ${ep_addr_for_image} -hv ${header_version}"
  ${command}
  ret2=$?
  ret=$((ret+ret2))
  if [ ${debug} -ne 0 ] ; then
    echo "<D> ret                         =<$ret>"
    echo "<D> ret2                        =<$ret2>"
  fi
else
  command="${cmd} ${imgtool} ${elf_file_basename}_pb.bin ${elf_file_basename}.stm32 -bt ${binary_type} -ep ${ep_addr_for_image} -hv ${header_version}"
  ${command}
  ret=$?
fi

if [ ${debug} -eq 0 ] ; then
  rm -f ${elf_file_basename}_pb.bin
  rm -f ${elf_file_basename}_pb_el3lnch.bin
  rm -f ${elf_file_basename}_pad_el3lnch.bin
  rm -f ${elf_file_basename}_no_el3lnch_pad.bin
  rm -f ${elf_file_basename}_pb_no_el3lnch.bin
fi

if [ ${ret} -eq 0 ] ; then
  if [ "${core}" == "CA35" ] ; then
    echo "${elf_file_basename}.elf stm32 image header and EL3 launcher added. Output file: ${elf_file_basename}.stm32"
    echo "${elf_file_basename}.elf stm32 image header added. Output file: ${elf_file_basename}_no_el3lnch.stm32"
  else
    echo "${elf_file_basename}.elf stm32 image header added. Output file: ${elf_file_basename}.stm32"
  fi
else
  echo "postbuild_STM32MP2.sh failed"
fi
