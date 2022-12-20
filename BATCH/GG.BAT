@echo off
resetps 1 >c:\batch\poo
delay 4
call gopatch
delay 4
run /w8 main.cpe >c:\batch\poo
delay 4
@echo on


