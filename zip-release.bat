@echo off
"C:\Program Files\7-Zip\7z.exe" a -tzip -mx7 "releases\stm8-utils-%1.zip" "utils.h" "utils.lib" "utils-large.lib"
