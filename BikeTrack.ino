#include <SPI.h>
#include <ILI9341_t3.h>
#include <XPT2046_Touchscreen.h>
#include <TinyGPS.h>
#include <TimeLib.h>
#include <NXPMotionSense.h>
#include <Wire.h>
#include <EEPROM.h>

//Main functions
void setup(void);
void loop();

//Tft functions
void initTft();
void checkTouch();
void redBtn();
void greenBtn();
void drawTime();

//Gps functions
void initGps();
void readGps();
void gpsdump(TinyGPS &gps);
void gpsdump(TinyGPS &gps);
void checkSerialForRateChange();
String getFloatString(double number, int digits);
String getTimeString();
String getDigitsString(int digits);

//Sensor functions
void initSensors();
void readSensors();

void setup(void)
{
  Serial.begin(115200);
  delay(1000);

  initSensors();
  initGps();
  initTft();
}

void loop()
{
  readSensors();
  checkTouch();
  readGps();
}
