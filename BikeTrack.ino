#include <SPI.h>
#include <ILI9341_t3.h>
#include <XPT2046_Touchscreen.h>
#include <TinyGPS.h>
#include <TimeLib.h>
#include <EEPROM.h>
#include <i2c_t3.h>
#include <BaroSensor.h>

//Enums
enum SpeedType{
  kmph, //kilometers per hour
  mph, //miles per hour
  mps //meter per second
};

enum Screens{
  home,
  settings
};

//Globals
#define printSensorsToSerial true
#define printGpsToSerial true
SpeedType speedType = kmph;
Screens currentScreen = home;
float speed;
short satellites;
String speedUnits = "km/h";
float longitude, latitude, altitude, gpsAltitude;
float temperature, atmosphericPressure;
float roll, pitch, yaw;
int sensorUpdateRate = 100;
int gpsUpdateRate = 1000;

//Main functions
void setup(void);
void loop();

//Tft functions
void initTft();
void checkTouch();
void redBtn();
void greenBtn();
void drawTime();
void drawSensors();
void drawGps();
void updateDisplay();

//Gps functions
void initGps();
void readGps();
void gpsdump(TinyGPS &gps);
void gpsdump(TinyGPS &gps);
void checkSerialForRateChange();
int milliseconds();
void updateGpsRefreshRate(int rate);

//String functions
String getFloatString(double number, int digits);
String getTimeString(boolean includeMillis);
String getDateString();
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
  readGps();
  checkTouch();
  updateDisplay();
}
