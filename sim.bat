@echo off
sstm8.exe -t STM8S208 -X 16M -I if=rom[0x5800] -C "sim_cmds.txt" "bin\Test\test"
rem sstm8.exe -t STM8S208 -X 16M -S uart=1,port=10000 -I if=rom[0x5800] "bin\Test\test"
rem start "PuTTY" putty.exe -telnet -P 10000 localhost