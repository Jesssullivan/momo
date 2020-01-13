
#include <Joystick.h> 



int logs = 1;  // use Serial monitor?

// pinout:
int dual = 0;  // potentiometer pin
int resetLed = 13;
int startButton = 2;
int resetSwitch = 1;


// generic:
int rmin = 0;
int rmax = 0;


// initialize a new instance of Joystick_:
Joystick_ Joystick;


void lprint(String text) {
  if (logs) {
    Serial.print(String(text));
  }
};

void logger(String axis, int value) {
  lprint(axis + " value = " + String(value));
  lprint("\n");
};


int globalRead(int pin) {

  int val = 0;

  // show value for throttle is starting with 20 blinks
  for (int x = 0; x < 20; ++x) {
    delay(200);
    digitalWrite(resetLed, LOW);
    delay(200);
    digitalWrite(resetLed, HIGH);
  };

  val = analogRead(pin);
  return int(val);
};


void setPin(int pin, int rmin, int rmax) {

  int val = 0;
  int valRead = analogRead(pin);

  if (valRead > 0) {  // no action if not true

    // translate valRead to limits
    val = (valRead - rmin) * (1024 / rmax);

    if (val / 4 >= 127) {
      val = ((val / 4) - 127) * 2;
      val = (val > 254) ? 254 : val; 
      Joystick.setThrottle(val);
      logger("Throttle", val);
    } else {
      val = ((val / 4) - 127) * 2;
      val = (val > 254) ? 254 : val; 
      Joystick.setYAxis(-(val));
      logger("Brake", -(val));
    }
  }
}

// RUN:

void setup() {
  Serial.begin(9600);
  Joystick.begin();
  pinMode(resetLed, OUTPUT);

  // setup: 

  int valA = 0;
  int valB = 0;

  delay(1000);
  lprint("Setting up limits ... \n");
  lprint("\n");
  delay(200);
  
  lprint("getting first value ... \n");
  lprint("\n");
  delay(500);
  
  valA = globalRead(dual);
  lprint("received value  " + String(valA));
  lprint("\n\n");
  
  delay(500);
  lprint("getting second value ... \n");
  lprint("\n");
  delay(500);

  valB = globalRead(dual);
  lprint("received value  " + String(valB));
  lprint("\n\n");
  
  rmin = (valA > valB) ? valB : valA;
  rmax = (valA < valB) ? valB : valA;

  lprint("Finished setup: maxValue = " + String(rmax) + " \n");
  lprint("\n");
  lprint("Finished setup: minValue = " + String(rmin) + " \n");
  lprint("\n");
}


void loop() {
  delay(10);  // delay value in ms, for read stability
  setPin(dual, rmin, rmax);
}
