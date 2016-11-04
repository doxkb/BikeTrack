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

boolean RecordOn = false;

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

void initTft()
{
  tft.begin();
  if (!ts.begin()) {
    if (printTftToSerial)
      Serial.println("Unable to start touchscreen.");
  }
  else {
    if (printTftToSerial)
      Serial.println("Touchscreen started.");
  }

  tft.fillScreen(ILI9341_PURPLE);
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
          if (printTftToSerial)
            Serial.println("Red btn hit");
          updateGpsRefreshRate(1000);
          redBtn();
        }
      }
    }
    else
    {
      if ((x > GREENBUTTON_X) && (x < (GREENBUTTON_X + GREENBUTTON_W))) {
        if ((y > GREENBUTTON_Y) && (y <= (GREENBUTTON_Y + GREENBUTTON_H))) {
          if (printTftToSerial)
            Serial.println("Green btn hit");
          updateGpsRefreshRate(100);
          greenBtn();
        }
      }
    }
  }
}

void drawFrame()
{
  tft.drawRect(FRAME_X, FRAME_Y, FRAME_W, FRAME_H, ILI9341_BLACK);
}

void redBtn()
{
  tft.fillRect(REDBUTTON_X, REDBUTTON_Y, REDBUTTON_W, REDBUTTON_H, ILI9341_RED);
  tft.fillRect(GREENBUTTON_X, GREENBUTTON_Y, GREENBUTTON_W, GREENBUTTON_H, ILI9341_BLUE);
  drawFrame();
  tft.setCursor(GREENBUTTON_X + 6 , GREENBUTTON_Y + (GREENBUTTON_H / 2));
  tft.setTextColor(ILI9341_WHITE);
  tft.setTextSize(2);
  tft.println("ON");
  RecordOn = false;
}

void greenBtn()
{
  tft.fillRect(GREENBUTTON_X, GREENBUTTON_Y, GREENBUTTON_W, GREENBUTTON_H, ILI9341_GREEN);
  tft.fillRect(REDBUTTON_X, REDBUTTON_Y, REDBUTTON_W, REDBUTTON_H, ILI9341_BLUE);
  drawFrame();
  tft.setCursor(REDBUTTON_X + 6 , REDBUTTON_Y + (REDBUTTON_H / 2));
  tft.setTextColor(ILI9341_WHITE);
  tft.setTextSize(2);
  tft.println("OFF");
  RecordOn = true;
}

void drawTime()
{
  //tft.fillRect(3, 3, 100, 21, ILI9341_GREEN);
  tft.setCursor(6, 6);
  tft.setTextColor(ILI9341_WHITE, ILI9341_GREEN);
  tft.setTextSize(2);
  tft.print(getTimeString(false));
}

void drawSensors()
{
  //tft.fillRect(3, 3, 100, 21, ILI9341_GREEN);
  tft.setCursor(6, 50);
  tft.setTextColor(ILI9341_WHITE, ILI9341_GREEN);
  tft.setTextSize(1);
  tft.print("Roll: " + getFloatString(roll, 2) + " Pitch: " + getFloatString(pitch, 2) + " Yaw: " + getFloatString(heading, 2));
}

void drawGps()
{
  //tft.fillRect(3, 3, 100, 21, ILI9341_GREEN);
  tft.setCursor(6, 100);
  tft.setTextColor(ILI9341_WHITE, ILI9341_GREEN);
  tft.setTextSize(1);
  tft.println("Lat: " + getFloatString(latitude, 5));
  tft.println("Lon: " + getFloatString(longitude, 5));
  tft.println("Alt: " + getFloatString(altitude, 2));
  tft.println("Sat: " + String(satellites));
}
