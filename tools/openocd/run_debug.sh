#!/usr/bin/env bash

#tmux new-session -s Debug \; select-layout tiled \; split-window -h  "echo \"Hello\" " \; split-window -h  "echo \"World\" "
#/home/calle/git/PH_WIP/tools/openocd/run_openocd.sh -o /home/calle/src/openocd -i picoprobe -t stm32f1x -e /home/calle/git/PH_WIP/build/src/FSSPH/FSSPH

RUN_EXEC_CMD=$1
TARGET_OPENOCD_PATH=$2
TARGET_OPENOCD_INTERFACE=$3
TARGET_OPENOCD_TARGET=$4
TARGET_BIN=$5
GDB_CFG=$6

echo "$TARGET_BIN" > out
tmux new-session \
"${RUN_EXEC_CMD} -o \
 ${TARGET_OPENOCD_PATH} -i\
 ${TARGET_OPENOCD_INTERFACE} -t \
 ${TARGET_OPENOCD_TARGET}" \; \
 split-window -h -p 80 "arm-none-eabi-gdb ${TARGET_BIN}.elf -x ${GDB_CFG}" \;
#--args ${TARGET_BIN}"
