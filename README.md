# MOMO:

*A Simple Brake / Throttle controller w/ Arduino Leonardo- D&M WIP by Jess Sullivan*
    
***extra bits regarding cmake & compilation:***     
    
*USB port must be accessible to $USER:*
```shell script

# to check permissions on port:
ls -l /dev/ttyACM*

# add $USER to rw group of port, usually group 'dialout':
sudo adduser $USER dialout

# to change permissions at port:
sudo chmod a+rw /dev/ttyACM0
```
    
*Regarding cmake, intellij, etc-
the standard arduino executable + gui is still preferred for actually uploading to arduino hardware*
    
```shell script
# get ide here, at least for uploading arduino .elf:
# https://www.arduino.cc/en/Main/Software

# something along the lines of:
tar xvf arduino-1.8.10/arduino-1.8.10-linux64.tar.xz 
chmod u+x arduino-1.8.10/install.sh
sudo ./arduino-1.8.10/install.sh

# using avrdude --> atmega: (unlikely to work well)
avrdude -C/../avrdude.conf -v -patmega328p -carduino -P/dev/my_com_port -b57600 -D -Uflash:w:/...my_project.hex:i
``` 
    
*retrieve HID depends:*
```shell script
# std arduino usb library:
wget https://raw.githubusercontent.com/arduino/ArduinoCore-samd/master/cores/arduino/USB/PluggableUSB.h
# pluggable depend:
wget https://raw.githubusercontent.com/omnidan/ArduinoPure/master/arduino/USBAPI.h
```
    
*.elf --> hex:*
``` shell script
avr-objcopy -j .text -j .data -O ihex my_project.elf my_project.hex
```     
