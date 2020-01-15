# MOMO:

*A Simple Brake / Throttle controller w/ Arduino Leonardo- D&M WIP by Jess Sullivan*
   
# Quickstart:       
    
For more introductions and notes on Open Source collaboration with the D&M Makerspace, visit:       
- [Some {development} software notes here](https://github.com/PSU-HC/home-wiki/wiki/Developer-Student-Software)
- [Some {rapid fabrication} software notes here](https://github.com/PSU-HC/home-wiki/wiki/CAD-CAM-Student-Software)
- [...Or the D&M Makerspace Home Page over here](https://makerspace.plymouthcreate.net/)

    
       
```shell script
# clone this repo:
git clone https://github.com/jesssullivan/momo 

# get updated joystick library:
git clone https://github.com/MHeironimus/ArduinoJoystickLibrary 

# move / make sure directory "Joystick" is in the ``` Arduino/libraries ``` folder.  
# if you are using Arduino IDE, you could ``` zip ArduinoJoystickLibrary/Joystick ```
# then add the archive as a .zip  in the Libraries menu.  
# Or, if Arduino is installed at $HOME, ``` cp -rf Joystick ~/Arduino/libraries ```

cd momo

# you can change branches with 
# ``` git checkout master ```
# or ```git checkout testing ```

# compile and upload:
arduino --upload arduino/momo/momo.ino
# :)
```
   

***Current features:***         
    
- USB "HID compliant" serial communication 
- variable brake / throttle limits (-100 <--> 0 <--> 100) are saved to EEPROM 
- limits can be reset / calibrated / updated on the fly 
- 8 bit brake + throttle output regardless of V++ limits / motion constraints 

     
***extra notes regarding cmake & compilation, YMMV:***     
    
*USB port must be accessible to $USER:*

```
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
