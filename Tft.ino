// This is calibration data for the raw touch data to the screen coordinates
#define TS_MINX 300
#define TS_MINY 430
#define TS_MAXX 3680
#define TS_MAXY 3690

#define STMPE_CS 8
XPT2046_Touchscreen ts(STMPE_CS);
#define TFT_CS 10
#define TFT_DC  9
ILI9341_t3 tft = ILI9341_t3(TFT_CS, TFT_DC);

boolean RecordOn = true;

#define FRAME_X 150
#define FRAME_Y 130
#define FRAME_W 150
#define FRAME_H 75

#define REDBUTTON_X FRAME_X
#define REDBUTTON_Y FRAME_Y
#define REDBUTTON_W (FRAME_W/2)
#define REDBUTTON_H FRAME_H

#define GREENBUTTON_X (REDBUTTON_X + REDBUTTON_W)
#define GREENBUTTON_Y FRAME_Y
#define GREENBUTTON_W (FRAME_W/2)
#define GREENBUTTON_H FRAME_H

#define BACKGROUND_COLOR ILI9341_PURPLE
#define TEXT_COLOR ILI9341_WHITE
#define TEXT_BACKGROUND_COLOR ILI9341_BLACK
#define BUTTON_BORDER_COLOR ILI9341_BLACK
#define BUTTON_INACTIVE_COLOR ILI9341_BLUE
#define BUTTON_ON_COLOR ILI9341_GREEN
#define BUTTON_OFF_COLOR ILI9341_RED

void initTft()
{
  tft.begin();

  if (!ts.begin())
    Serial.println("Unable to start touchscreen.");

  tft.fillScreen(BACKGROUND_COLOR);
  // origin = left,top landscape (USB left upper)
  tft.setRotation(1);

  redBtn();
}

void checkTouch()
{
  if (!ts.bufferEmpty())
  {
    TS_Point p = ts.getPoint();

    int x = map(p.x, TS_MINY, TS_MAXY, 0, tft.width());
    int y = map(p.y, TS_MINX, TS_MAXX, 0, tft.height());

    if (RecordOn)
    {
      if ((x > REDBUTTON_X) && (x < (REDBUTTON_X + REDBUTTON_W))) {
        if ((y > REDBUTTON_Y) && (y <= (REDBUTTON_Y + REDBUTTON_H))) {
          updateGpsRefreshRate(2000);
          redBtn();
        }
      }
    }
    else
    {
      if ((x > GREENBUTTON_X) && (x < (GREENBUTTON_X + GREENBUTTON_W))) {
        if ((y > GREENBUTTON_Y) && (y <= (GREENBUTTON_Y + GREENBUTTON_H))) {
          updateGpsRefreshRate(100);
          greenBtn();
        }
      }
    }
  }
}

int lastSensorDisplayUpdate = -5000;
int lastGpsDisplayUpdate = -5000;
time_t prevTimeDisplayUpdate = 0;

void updateDisplay()
{
  if (now() != prevTimeDisplayUpdate) { //update the display only if the time has changed
    prevTimeDisplayUpdate = now();
    drawTime();
  }

  if (currentScreen == home)
  {
    int nowMills = millis();
    if (nowMills - lastSensorDisplayUpdate > sensorUpdateRate)
    {
      lastSensorDisplayUpdate = nowMills;
      drawSensors();
    }

    if (nowMills - lastGpsDisplayUpdate > gpsUpdateRate)
    {
      lastGpsDisplayUpdate = nowMills;
      drawGps();
    }
  }
}

void redBtn()
{
  if (RecordOn){
    tft.fillRect(REDBUTTON_X, REDBUTTON_Y, REDBUTTON_W, REDBUTTON_H, BUTTON_OFF_COLOR);
    tft.fillRect(GREENBUTTON_X, GREENBUTTON_Y, GREENBUTTON_W, GREENBUTTON_H, BUTTON_INACTIVE_COLOR);
    tft.drawRect(FRAME_X, FRAME_Y, FRAME_W, FRAME_H, BUTTON_BORDER_COLOR);
    tft.setCursor(GREENBUTTON_X + 6 , GREENBUTTON_Y + (GREENBUTTON_H / 2));
    tft.setTextColor(TEXT_COLOR);
    tft.setTextSize(2);
    tft.println(String(gpsUpdateRate));
    RecordOn = false;
  }
}

void greenBtn()
{
  if (!RecordOn){
    tft.fillRect(GREENBUTTON_X, GREENBUTTON_Y, GREENBUTTON_W, GREENBUTTON_H, BUTTON_ON_COLOR);
    tft.fillRect(REDBUTTON_X, REDBUTTON_Y, REDBUTTON_W, REDBUTTON_H, BUTTON_INACTIVE_COLOR);
    tft.drawRect(FRAME_X, FRAME_Y, FRAME_W, FRAME_H, BUTTON_BORDER_COLOR);
    tft.setCursor(REDBUTTON_X + 6 , REDBUTTON_Y + (REDBUTTON_H / 2));
    tft.setTextColor(TEXT_COLOR);
    tft.setTextSize(2);
    tft.println(String(gpsUpdateRate));
    RecordOn = true;
  }
}

void drawTime()
{
  //tft.fillRect(3, 3, 100, 21, ILI9341_GREEN);
  tft.setCursor(6, 6);
  tft.setTextColor(TEXT_COLOR, TEXT_BACKGROUND_COLOR);
  tft.setTextSize(2);
  tft.print(getTimeString(false));
}

void drawSensors()
{
  //tft.fillRect(3, 3, 100, 21, ILI9341_GREEN);
  tft.setCursor(0, 50);
  tft.setTextColor(TEXT_COLOR, TEXT_BACKGROUND_COLOR);
  tft.setTextSize(1);
  tft.println("ax: " + getFloatString(ax, 2) + " ay: " + getFloatString(ay, 2) + " az: " + getFloatString(az, 2) + " g");
  tft.println("Roll: " + getFloatString(roll, 2) + " Pitch: " + getFloatString(pitch, 2) + " Yaw: " + getFloatString(yaw, 2));
  tft.println("Altitude: " + getFloatString(altitude, 2) + " m");
  tft.println("Temperature: " + getFloatString(temperature, 2) + " C");
  tft.println("Atm pressure: " + getFloatString(atmosphericPressure, 2) + " hPa");
}

void drawGps()
{
  //tft.fillRect(3, 3, 100, 21, ILI9341_GREEN);
  tft.setCursor(0, 100);
  tft.setTextColor(TEXT_COLOR, TEXT_BACKGROUND_COLOR);
  tft.setTextSize(1);
  tft.println("Lat: " + getFloatString(latitude, 5));
  tft.println("Lon: " + getFloatString(longitude, 5));

  if (satellites >= 100)
    tft.println("Sat: 00");
  else if (satellites >= 10)
    tft.println("Sat: " + String(satellites));
  else
    tft.println("Sat: 0" + String(satellites));

  tft.setCursor(6, 150);
  tft.setTextColor(TEXT_COLOR, TEXT_BACKGROUND_COLOR);
  tft.setTextSize(3);
  tft.println(String(speed) + speedUnits);
}
