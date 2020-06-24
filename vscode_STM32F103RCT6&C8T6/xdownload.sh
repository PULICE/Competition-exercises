#!/bin/sh
#-c "interface jlink" 这里-c是命令控制参数interface jlink接口关键字后跟目标调试器“transport select swd"选择
#调试方式为SWD，"adapter_khz 1000"调试速度1000kHz, "target/stm32f1x.cfg" 指定烧录目标板配置文件"program ./build/vscode_STM32F103RCT6.hex" 
#指定要烧录的可执行文件"reset" 对目标板进行软复位"shutdown"关闭openocd      临时待下载可执行文件路径./CT117E-LCD.hex  -c "adapter_khz 1000" 
make -j8
openocd -c "interface jlink" -c "transport select swd" -f "target/stm32f1x.cfg" \
-c "program ./build/vscode_STM32F103RCT6.hex" -c "reset" -c "shutdown"
