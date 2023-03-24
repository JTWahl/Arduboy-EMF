//TODO: do hardware mod to actually test pin A5 for emf
//TODO: add audio and toggle?

#include <Arduboy2.h>
#include "sprites.h"

Arduboy2 arduboy;
BeepPin1 beep;

//C:\Users\t420\AppData\Local\Temp\arduino_build_323027

constexpr uint8_t frameRate = 40; // Frame rate in frames per second

int sensorPin = A5;
int calibration = 0;
int baseline = 0;
bool ledState = true;
float sensorInterval = 42.5;
int minCal = sensorInterval * -6;
int maxCal = sensorInterval * 6;
  
void setup() {
  arduboy.begin();
  beep.begin();
  arduboy.setFrameRate(frameRate);
  pinMode(sensorPin, INPUT);
}

void loop() {
   // pause render until it's time for the next frame
  if (!(arduboy.nextFrame())) return;

  //not sure what this does!
  beep.timer();

  //clear the screen
  arduboy.clear();

  //write app name text
  arduboy.setCursor(45, 0);
  arduboy.print("-EMF SENSOR-");
 
  //write led toggle text
  arduboy.setCursor(30, 20);
  arduboy.print("A - Toggle LED");

  //write reset app text
  arduboy.setCursor(30, 30);
  arduboy.print("UP/DOWN - Reset");

  //write audio toggle
  arduboy.setCursor(30, 40);
  arduboy.print("B - Tare");

  //write calibration text
  arduboy.setCursor(30, 50);
  arduboy.print("<-- Tune -->");

  arduboy.pollButtons();
 
  if (arduboy.justPressed(LEFT_BUTTON)) calibrate(0);
  if (arduboy.justPressed(RIGHT_BUTTON)) calibrate(1);
  if (arduboy.justReleased(A_BUTTON)) ledToggle();
  if (arduboy.justReleased(B_BUTTON)) tare();
  if (arduboy.justReleased(UP_BUTTON) || arduboy.justReleased(DOWN_BUTTON)) resetApp();

  led();

  if (getSensor() < sensorInterval) {
    drawMeter(0);
  } else if (getSensor() < sensorInterval * 2) {
    drawMeter(1);
  } else if (getSensor() < sensorInterval * 3) {
    drawMeter(2);
  } else if (getSensor() < sensorInterval * 4) {
    drawMeter(3);
  } else if (getSensor() < sensorInterval * 5) {
    drawMeter(4);
  } else if (getSensor() < sensorInterval * 6) {
    drawMeter(5);
  } else {
    drawMeter(6);
  }
 
  arduboy.display();
}

void resetApp() {
  calibration = 0;
  baseline = 0;
  ledState = true;
}

//toggles the led on or off
void ledToggle() {
  ledState = !ledState;  
}

//draws an led while the sensor is greater than the minimum interval
void led() {
//TODO: draw LED rgb depending on getSensor()'s value not just boolean
  if (ledState && getSensor() > sensorInterval){
    arduboy.setRGBled(map(calibration, 0, maxCal + sensorInterval, 0, 255), 0, analogRead(sensorPin));
  } else {
    arduboy.setRGBled(0, 0, 0);
  }
}

//sets the baseline value as the current value of the sensor
void tare() {
  baseline = getSensor();
}

//plays a corresponding frequency for each sensor input, from low to high
void playBeep(int frequency) {
  beep.tone((beep.freq(256 + frequency)));
  arduboy.delayShort(30); // Delay for 30ms
}

//calibrates the sensor by adding or subtracting ten to the sensor value
void calibrate(bool state) {  
  if (!state) { //calibrate down
    if (calibration > minCal) calibration -= sensorInterval;
  } else { //calibrate up
    if (calibration < maxCal) calibration += sensorInterval;
  }
}

//calculates sensor value
int getSensor() {
  return analogRead(sensorPin) + calibration - baseline;
}

//draws the meter sprite on screen
void drawMeter(int meterLevel) {
  if (meterLevel == 0) {
    //draw meter 0
    Sprites::drawOverwrite(0,10, meter0, 0);
    arduboy.setCursor(2, 2);
    arduboy.print("0");
  }

  if (meterLevel == 1) {
    //draw meter 1
    Sprites::drawOverwrite(0,10, meter1, 0);
    arduboy.setCursor(2, 2);
    arduboy.print("1");

  }

  if (meterLevel == 2) {
    //draw meter 2
    Sprites::drawOverwrite(0,10, meter2, 0);
    arduboy.setCursor(2, 2);
    arduboy.print("2");
  }

  if (meterLevel == 3) {
    //draw meter 3
    Sprites::drawOverwrite(0,10, meter3, 0);
    arduboy.setCursor(2, 2);
    arduboy.print("3");
  }

  if (meterLevel == 4) {
    //draw meter 4
    Sprites::drawOverwrite(0,10, meter4, 0);
    arduboy.setCursor(2, 2);
    arduboy.print("4");
  }

  if (meterLevel == 5) {
    //draw meter 5
    Sprites::drawOverwrite(0,10, meter5, 0);
    arduboy.setCursor(2, 2);
    arduboy.print("5");
  }

  if (meterLevel == 6) {
    //draw meter 6
    Sprites::drawOverwrite(0,10, meter6, 0);
    arduboy.setCursor(2, 2);
    arduboy.print("6");
  }
}
