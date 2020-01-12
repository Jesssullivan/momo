
*retrieve HID depends:*
```shell script
# std arduino usb library:
wget https://raw.githubusercontent.com/arduino/ArduinoCore-samd/master/cores/arduino/USB/PluggableUSB.h
# pluggable depend:
wget https://raw.githubusercontent.com/omnidan/ArduinoPure/master/arduino/USBAPI.h
```

*extra cmake bits:*
``` shell script
# .elf --> hex:
avr-objcopy -j .text -j .data -O ihex my_project.elf my_project.hex
```     

*note, the standard arduino executable is vastly preferred for arduino hardware!  see below for to add .bashrc entry*
```shell script
# avrdude --> atmega:
avrdude -C/../avrdude.conf -v -patmega328p -carduino -P/dev/my_com_port -b57600 -D -Uflash:w:/...my_project.hex:i
``` 
    
*append arduino executable to path:*
```shell script
# get ide here:
# https://www.arduino.cc/en/Main/Software
# 
# after unpacking / installing @ ~/
sudo echo 'alias duino=~/arduino-1.8.10/arduino' >> ~/.bashrc
source ~/.bashrc  # refresh shell
```