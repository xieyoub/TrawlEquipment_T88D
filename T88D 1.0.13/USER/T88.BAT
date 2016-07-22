SET PATH=M:\keil\ARM\ARMCC\bin\;G:\Windows\system32;G:\Windows;G:\Windows\System32\Wbem;G:\Windows\System32\WindowsPowerShell\v1.0\;G:\Program Files (x86)\ATI Technologies\ATI.ACE\Core-Static;G:\Program Files (x86)\Microsoft SQL Server\100\Tools\Binn\;G:\Program Files\Microsoft SQL Server\100\Tools\Binn\;G:\Program Files\Microsoft SQL Server\100\DTS\Binn\;G:\Program Files\TortoiseGit\bin;G:\Windows\system32\config\systemprofile\.dnx\bin;G:\Program Files\Microsoft DNX\Dnvm\;G:\Program Files (x86)\Windows Kits\8.1\Windows Performance Toolkit\;G:\Program Files (x86)\Autodesk\Backburner\;G:\Program Files (x86)\Brackets\command;G:\Program Files (x86)\IDM Computer Solutions\UltraEdit;G:\Program Files (x86)\Altium Designer Summer 09\SystemG:\Qt\Qt5.5.1\5.5\msvc2013_64;G:\Program Files (x86)\IDM Computer Solutions\UltraCompare
SET CPU_TYPE=STM32F103RC
SET CPU_VENDOR=STMicroelectronics
SET UV2_TARGET=T88
SET CPU_CLOCK=0x044AA200
"M:\keil\ARM\ARMCC\bin\ArmCC" --Via "..\obj\main.__i"
"M:\keil\ARM\ARMCC\bin\ArmCC" --Via "..\obj\stm32f10x_it.__i"
"M:\keil\ARM\ARMCC\bin\ArmCC" --Via "..\obj\system_stm32f10x.__i"
"M:\keil\ARM\ARMCC\bin\ArmCC" --Via "..\obj\led.__i"
"M:\keil\ARM\ARMCC\bin\ArmCC" --Via "..\obj\encode.__i"
"M:\keil\ARM\ARMCC\bin\ArmCC" --Via "..\obj\key.__i"
"M:\keil\ARM\ARMCC\bin\ArmCC" --Via "..\obj\config.__i"
"M:\keil\ARM\ARMCC\bin\ArmCC" --Via "..\obj\usart.__i"
"M:\keil\ARM\ARMCC\bin\ArmCC" --Via "..\obj\core_cm3.__i"
"M:\keil\ARM\ARMCC\bin\ArmAsm" --Via "..\obj\startup_stm32f10x_hd._ia"
"M:\keil\ARM\ARMCC\bin\ArmCC" --Via "..\obj\misc.__i"
"M:\keil\ARM\ARMCC\bin\ArmCC" --Via "..\obj\stm32f10x_gpio.__i"
"M:\keil\ARM\ARMCC\bin\ArmCC" --Via "..\obj\stm32f10x_rcc.__i"
"M:\keil\ARM\ARMCC\bin\ArmCC" --Via "..\obj\stm32f10x_tim.__i"
"M:\keil\ARM\ARMCC\bin\ArmCC" --Via "..\obj\stm32f10x_usart.__i"
"M:\keil\ARM\ARMCC\bin\ArmCC" --Via "..\obj\stm32f10x_dma.__i"
"M:\keil\ARM\ARMCC\bin\ArmCC" --Via "..\obj\stm32f10x_exti.__i"
"M:\keil\ARM\ARMCC\bin\ArmCC" --Via "..\obj\delay.__i"
"M:\keil\ARM\ARMCC\bin\ArmCC" --Via "..\obj\sys.__i"
"M:\keil\ARM\ARMCC\bin\ArmLink" --Via "..\OBJ\T88.lnp"
M:\keil\ARM\ARMCC\bin\fromelf.exe "..\OBJ\T88.axf" --i32combined --output "..\OBJ\T88.hex"
