## *a WIP by Jess @ the D&M Makerspace.*
         
     

[***Jump to Quickstart Instructions***](#quickstart)  
[***Jump to all designs***](#drawings)   
[***Jump to additional notes***](#notes)  


  
<h4 id="drawings"> </h4>     

***Iterations of Hardware- Source Links (Fusion 360):***

<br>
 
- [22.2 Reversed Mount Digital](https://a360.co/32xEzMR) 
- [(archived 22.2 Enclosed Digital)](https://a360.co/2PidrMl)
- [alt. Shelled 22.2 Reversed Mount Digital](https://a360.co/2VeH4Sv)
- [22.2mm Shelled Digital Limited](https://a360.co/39VZawA)
- [22.2 Digital + powered feedback (WIP)](https://a360.co/2P3snh5)
- [alt, previous 22.2mm Digital Limited](https://a360.co/2vDDMxk)
- [Digital, single limit](https://myhub.autodesk360.com/ue2819a04/shares/public/SH56a43QTfd62c1cd968133e296ee005ddd1)
- [Digital, bolt limited](https://myhub.autodesk360.com/ue2819a04/shares/public/SH56a43QTfd62c1cd9682a1ff9148d0f03e5)
- [Analog, bolt limited](https://a360.co/2Rb2a1w)
- [Analog, variable limits](https://a360.co/30D7Ft9)

<br>
<br>

[![Hmmm](http://img.youtube.com/vi/Bt2yWvXx7MU/0.jpg)](https://www.youtube.com/watch?v=Bt2yWvXx7MU "Um...")

<br>

For more introductions to Open Source collaboration with the D&M Makerspace, visit:       
- [Some {development} software notes here](https://github.com/PSU-HC/home-wiki/wiki/Developer-Student-Software)
- [Some {rapid fabrication} software notes here](https://github.com/PSU-HC/home-wiki/wiki/CAD-CAM-Student-Software)
- [...Or the D&M Makerspace Home Page over here](https://makerspace.plymouthcreate.net/)
 
 
***Current features:***           
        
- robust, single lever for brake & throttle     
- hardware uses standard, 22.2mm diameter handlebar tubing  
- configurable on the fly for either left (CW = throttle) or right (CCW = throttle) lever setup    
- USB "HID compliant" serial communication  
- 8 bit brake + throttle output regardless of motion constraints or hardware type       

<br>
    
*Previously:*
- configurable for either digital (encoder) or analog (potentiometer) angle sensors

<br>     

***Quickstart:***   <br>

<h4 id="quickstart"> </h4>     

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

cd momo/utb

# edit the config.h (pinout, logging, etc):
nano config.h

# compile and upload from your shell
# (alternatively your can use the Arduino IDE uploader)
arduino --upload utb.ino

# you could work on another branch with:
# git checkout testing
# :)
```
        
        
<h4 id="notes"> </h4>     

***extra notes regarding cmake & compilation, YMMV:***     
    
*USB port must be accessible to $USER:*
```
# to checks  permissions on port:
ls -l /dev/ttyACM*

# add $USER to rw group of port, usually group 'dialout':
sudo adduser $USER dialout

# to change permissions at port:
sudo chmod a+wrw /dev/ttyACM0
```
    
*Regarding cmake, intellij, etc-
    
```shell script
# get ide here, at least for uploading arduino .elf:
# https://www.arduino.cc/en/Main/Software

# something along the lines of:~~~~
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
