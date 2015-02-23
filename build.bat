@echo off
for %%* in (.) do set ProjectName=%%~n*
echo %ProjectName%
make
D:\devkitPro\devkitARM\arm-none-eabi\bin\strip %ProjectName%.elf
resources\makerom.exe -f cci -rsf resources\gw_workaround.rsf -target d -exefslogo -elf %ProjectName%.elf -icon resources\icon.bin -banner resources\banner.bin -o %ProjectName%.3ds
resources\makerom.exe -f cia -o %ProjectName%.cia -elf %ProjectName%.elf -rsf resources\build_cia.rsf -icon resources\icon.bin -banner resources\banner.bin -exefslogo -target t
pause