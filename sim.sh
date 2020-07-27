#!/bin/sh
~/sdcc/sdcc/sim/ucsim/stm8.src/sstm8 -t STM8S208 -X 16M -I if=rom[0x5800] -C 'sim_cmds.txt' 'bin/Test/test'
