# MOMO
  

[***Jump to Quickstart Instructions***](#foobar)  
          
For more introductions and notes on Open Source collaboration with the D&M Makerspace, visit:       
- [Some {development} software notes here](https://github.com/PSU-HC/home-wiki/wiki/Developer-Student-Software)
- [Some {rapid fabrication} software notes here](https://github.com/PSU-HC/home-wiki/wiki/CAD-CAM-Student-Software)
- [...Or the D&M Makerspace Home Page over here](https://makerspace.plymouthcreate.net/)
    
***Current features:***         
        
- single, split lever brake / throttle        
- configurable for either digital (encoder) or analog (potentiometer) angle sensors
- USB "HID compliant" serial communication 
- 8 bit brake + throttle output regardless of motion constraints or hardware type   

*In progress:*
  
- Rebound support for throttle / brake
- Updates in the works for SimuCUBE FFB w/ STM32F4 microcontroller
        
          
***Hardware Source Links (Fusion 360):***
     
- [Digital + powered feedback - WIP](https://myhub.autodesk360.com/ue2819a04/shares/public/SH56a43QTfd62c1cd9680486105b49c7f3c5)
- [Digital, single limit](https://myhub.autodesk360.com/ue2819a04/shares/public/SH56a43QTfd62c1cd968133e296ee005ddd1)
- [Digital, bolt limited](https://myhub.autodesk360.com/ue2819a04/shares/public/SH56a43QTfd62c1cd9682a1ff9148d0f03e5)
- [Analog, bolt limited](https://a360.co/2Rb2a1w)
- [Analog, variable limits](https://a360.co/30D7Ft9)
    
    
<h4 id="foobar"> </h4>     

```shell script
# clone this repo:
git clone https://github.com/jesssullivan/momo 

# get updated joystick library:
git clone https://github.com/MHeironimus/ArduinoJoystickLibrary 

# encoder support uses pjrc's library (standard `Encoder` lib available from arduino IDE)
#  pjrc: http://www.pjrc.com/teensy/td_libs_Encoder.html

# move / make sure directory "Joystick" is in the ``` Arduino/libraries ``` folder.  
# if you are using Arduino IDE, you could do something like:
zip ~/Downloads/ArduinoJoystickLibrary/Joystick .
# ....then add the archive as a .zip  in the Libraries menu.  
# Or if Arduino is installed at $HOME, something along the lines of:
cp -rf Joystick ~/Arduino/libraries 

# edit the config.h file for pinout, hardware selection, etc
gedit src/UnifiedThrottleBrake/config.h

cd momo/src

# compile and upload from your shell
# (alternatively your can use the Arduino IDE uploader)
arduino --upload UnifiedThrottleBrake/UnifiedThrottleBrake.ino

# you can change branches with:
git checkout master
# or:
git checkout testing
# :)
```
    
  
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
avr-objcopy -j .text -j .data -O ihex main.elf main.hex
```     
