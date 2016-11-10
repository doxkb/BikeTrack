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

#define TFT_WIDTH 320
#define TFT_HEIGHT 240

#define BUTTONS_IN_FRAME 5
#define FRAME_H 25
#define FRAME_W (50 * BUTTONS_IN_FRAME)
#define FRAME_X (TFT_WIDTH - FRAME_W)
#define FRAME_Y (TFT_HEIGHT - FRAME_H)

#define BUTTON1_X FRAME_X
#define BUTTON1_Y FRAME_Y
#define BUTTON1_W (FRAME_W/BUTTONS_IN_FRAME)
#define BUTTON1_H FRAME_H
#define BUTTON1_VALUE 100

#define BUTTON2_X (BUTTON1_X + BUTTON1_W)
#define BUTTON2_Y FRAME_Y
#define BUTTON2_W (FRAME_W/BUTTONS_IN_FRAME)
#define BUTTON2_H FRAME_H
#define BUTTON2_VALUE 200

#define BUTTON3_X (BUTTON2_X + BUTTON2_W)
#define BUTTON3_Y FRAME_Y
#define BUTTON3_W (FRAME_W/BUTTONS_IN_FRAME)
#define BUTTON3_H FRAME_H
#define BUTTON3_VALUE 500

#define BUTTON4_X (BUTTON3_X + BUTTON3_W)
#define BUTTON4_Y FRAME_Y
#define BUTTON4_W (FRAME_W/BUTTONS_IN_FRAME)
#define BUTTON4_H FRAME_H
#define BUTTON4_VALUE 1000

#define BUTTON5_X (BUTTON4_X + BUTTON4_W)
#define BUTTON5_Y FRAME_Y
#define BUTTON5_W (FRAME_W/BUTTONS_IN_FRAME)
#define BUTTON5_H FRAME_H
#define BUTTON5_VALUE 2000

#define BUTTON_RST_X 0
#define BUTTON_RST_Y FRAME_Y
#define BUTTON_RST_W (FRAME_W / BUTTONS_IN_FRAME)
#define BUTTON_RST_H FRAME_H

#define BACKGROUND_COLOR ILI9341_BLACK
#define TEXT_COLOR ILI9341_WHITE
#define TEXT_BACKGROUND_COLOR ILI9341_BLACK
#define BUTTON_BORDER_COLOR ILI9341_WHITE
#define BUTTON_INACTIVE_COLOR ILI9341_BLACK
#define BUTTON_ON_COLOR ILI9341_GREEN
#define BUTTON_OFF_COLOR ILI9341_RED

void initTft()
{
  tft.begin();

  if (!ts.begin())
    Serial.println("Unable to start touchscreen.");

  tft.fillScreen(BACKGROUND_COLOR);
  tft.setRotation(1);  // origin = left,top landscape (USB left upper)

  buttons();
}

void checkTouch()
{
  if (!ts.bufferEmpty())
  {
    TS_Point p = ts.getPoint();

    int x = map(p.x, TS_MINY, TS_MAXY, 0, tft.width());
    int y = map(p.y, TS_MINX, TS_MAXX, 0, tft.height());

    if ((x > BUTTON1_X) && (x < (BUTTON1_X + BUTTON1_W))) {
      if ((y > BUTTON1_Y) && (y <= (BUTTON1_Y + BUTTON1_H))) {
        updateGpsRefreshRate(BUTTON1_VALUE);
        buttons();
      }
    }
    else if ((x > BUTTON2_X) && (x < (BUTTON2_X + BUTTON2_W))) {
      if ((y > BUTTON2_Y) && (y <= (BUTTON2_Y + BUTTON2_H))) {
        updateGpsRefreshRate(BUTTON2_VALUE);
        buttons();
      }
    }
    else if ((x > BUTTON3_X) && (x < (BUTTON3_X + BUTTON3_W))) {
      if ((y > BUTTON3_Y) && (y <= (BUTTON3_Y + BUTTON3_H))) {
        updateGpsRefreshRate(BUTTON3_VALUE);
        buttons();
      }
    }
    else if ((x > BUTTON4_X) && (x < (BUTTON4_X + BUTTON4_W))) {
      if ((y > BUTTON4_Y) && (y <= (BUTTON4_Y + BUTTON4_H))) {
        updateGpsRefreshRate(BUTTON4_VALUE);
        buttons();
      }
    }
    else if ((x > BUTTON5_X) && (x < (BUTTON5_X + BUTTON5_W))) {
      if ((y > BUTTON5_Y) && (y <= (BUTTON5_Y + BUTTON5_H))) {
        updateGpsRefreshRate(BUTTON5_VALUE);
        buttons();
      }
    }
    else if ((x > BUTTON_RST_X) && (x < (BUTTON_RST_X + BUTTON_RST_W))) {
      if ((y > BUTTON_RST_Y) && (y <= (BUTTON_RST_Y + BUTTON_RST_H))) {
        resetDisplay();
      }
    }
  }
}

float maxSpeed = 0;
int maxPitchX = -100;
int maxPitchY = -100;
int minPitchX = -100;
int minPitchY = -100;
int maxRollX = -100;
int maxRollY = -100;
int minRollX = -100;
int minRollY = -100;
float maxFront, maxBack, maxLeft, maxRight;
int forcesSize = 90;
bool bgDrawn = false;
int lastPitchX = -100;
int lastPitchY = -100;
int lastRollX = -100;
int lastRollY = -100;
float minRoll, maxRoll, minPitch, maxPitch;

void resetDisplay(){
  int cSize = forcesSize/2;
  int posX = TFT_WIDTH - forcesSize/2 - 5;
  int posY = TFT_HEIGHT - FRAME_H - 10 - forcesSize;
  int posX2 = TFT_WIDTH - forcesSize - 10;
  int posY2 = TFT_HEIGHT - FRAME_H - 5 - forcesSize/2;
  tft.drawLine(posX, posY, maxPitchX, maxPitchY, BACKGROUND_COLOR);
  tft.drawLine(posX, posY, minPitchX, minPitchY, BACKGROUND_COLOR);
  tft.drawLine(posX2, posY2, maxRollX, maxRollY, BACKGROUND_COLOR);
  tft.drawLine(posX2, posY2, minRollX, minRollY, BACKGROUND_COLOR);
  minRollX = -100;
  minRollY = -100;
  maxRollX = -100;
  maxRollY = -100;
  minPitchX = -100;
  minPitchY = -100;
  maxPitchX = -100;
  maxPitchY = -100;
  maxFront = 0;
  maxBack = 0;
  maxLeft = 0;
  maxRight = 0;
  maxSpeed = 0;
  minRoll = 0;
  maxRoll = 0;
  minPitch = 0;
  maxPitch = 0;
}

time_t prevTimeDisplayUpdate = 0;

void updateDisplay()
{
  if (now() != prevTimeDisplayUpdate) { //update the display only if the time has changed
    prevTimeDisplayUpdate = now();
    drawTime();
    drawSatellites();
  }

  if (currentScreen == home)
  {
      drawSpeed();
      drawLean();
      drawGForces();
  }
}

int activeDrawn = 0;

void buttons()
{
  if (activeDrawn == gpsUpdateRate)
    return;

  int rectRadius = 5;

  tft.fillRoundRect(BUTTON1_X, BUTTON1_Y, BUTTON1_W, BUTTON1_H, rectRadius, BUTTON_INACTIVE_COLOR);
  tft.fillRoundRect(BUTTON2_X, BUTTON2_Y, BUTTON2_W, BUTTON2_H, rectRadius, BUTTON_INACTIVE_COLOR);
  tft.fillRoundRect(BUTTON3_X, BUTTON3_Y, BUTTON3_W, BUTTON3_H, rectRadius, BUTTON_INACTIVE_COLOR);
  tft.fillRoundRect(BUTTON4_X, BUTTON4_Y, BUTTON4_W, BUTTON4_H, rectRadius, BUTTON_INACTIVE_COLOR);
  tft.fillRoundRect(BUTTON5_X, BUTTON5_Y, BUTTON5_W, BUTTON5_H, rectRadius, BUTTON_INACTIVE_COLOR);
  tft.fillRoundRect(BUTTON_RST_X, BUTTON_RST_Y, BUTTON_RST_W, BUTTON_RST_H, rectRadius, ILI9341_RED);

  if (gpsUpdateRate == BUTTON1_VALUE)
    tft.fillRoundRect(BUTTON1_X, BUTTON1_Y, BUTTON1_W, BUTTON1_H, rectRadius, BUTTON_ON_COLOR);
  else if (gpsUpdateRate == BUTTON2_VALUE)
    tft.fillRoundRect(BUTTON2_X, BUTTON2_Y, BUTTON2_W, BUTTON2_H, rectRadius, BUTTON_ON_COLOR);
  else if (gpsUpdateRate == BUTTON3_VALUE)
    tft.fillRoundRect(BUTTON3_X, BUTTON3_Y, BUTTON3_W, BUTTON3_H, rectRadius, BUTTON_ON_COLOR);
  else if (gpsUpdateRate == BUTTON4_VALUE)
    tft.fillRoundRect(BUTTON4_X, BUTTON4_Y, BUTTON4_W, BUTTON4_H, rectRadius, BUTTON_ON_COLOR);
  else if (gpsUpdateRate == BUTTON5_VALUE)
    tft.fillRoundRect(BUTTON5_X, BUTTON5_Y, BUTTON5_W, BUTTON5_H, rectRadius, BUTTON_ON_COLOR);

  tft.drawRoundRect(BUTTON1_X, BUTTON1_Y, BUTTON1_W, BUTTON1_H, rectRadius, BUTTON_BORDER_COLOR);
  tft.drawRoundRect(BUTTON2_X, BUTTON2_Y, BUTTON2_W, BUTTON2_H, rectRadius, BUTTON_BORDER_COLOR);
  tft.drawRoundRect(BUTTON3_X, BUTTON3_Y, BUTTON3_W, BUTTON3_H, rectRadius, BUTTON_BORDER_COLOR);
  tft.drawRoundRect(BUTTON4_X, BUTTON4_Y, BUTTON4_W, BUTTON4_H, rectRadius, BUTTON_BORDER_COLOR);
  tft.drawRoundRect(BUTTON5_X, BUTTON5_Y, BUTTON5_W, BUTTON5_H, rectRadius, BUTTON_BORDER_COLOR);
  tft.drawRoundRect(BUTTON_RST_X, BUTTON_RST_Y, BUTTON_RST_W, BUTTON_RST_H, rectRadius, BUTTON_BORDER_COLOR);

  int yPos = BUTTON1_Y + (BUTTON1_H/2 - 3);
  tft.setTextColor(TEXT_COLOR);
  tft.setFont(LiberationSans_8_Bold_Italic);
  tft.setCursor(BUTTON1_X + 6, yPos);
  tft.println(String(BUTTON1_VALUE));
  tft.setCursor(BUTTON2_X + 6, yPos);
  tft.println(String(BUTTON2_VALUE));
  tft.setCursor(BUTTON3_X + 6, yPos);
  tft.println(String(BUTTON3_VALUE));
  tft.setCursor(BUTTON4_X + 6, yPos);
  tft.println(String(BUTTON4_VALUE));
  tft.setCursor(BUTTON5_X + 6, yPos);
  tft.println(String(BUTTON5_VALUE));
  tft.setCursor(BUTTON_RST_X + 6, yPos);
  tft.println("RESET");

  activeDrawn = gpsUpdateRate;
}

uint16_t rgb565_from_triplet(uint8_t red, uint8_t green, uint8_t blue)
{
  red   >>= 3;
  green >>= 2;
  blue  >>= 3;
  return (red << 11) | (green << 5) | blue;
}

void drawTime()
{
  tft.fillRect(0,0, TFT_WIDTH, 25, ILI9341_DARKGREEN);
  tft.setCursor(6, 6);
  tft.setTextColor(ILI9341_BLACK);
  tft.setFont(LiberationSans_14_Bold_Italic);
  tft.setTextSize(2);
  tft.print(getTimeString(false));
}

void drawSatellites()
{
  tft.setCursor(TFT_WIDTH - 43, 10);
  tft.setTextColor(ILI9341_BLACK);
  tft.setFont(LiberationSans_10_Bold_Italic);
  if (satellites >= 100)
    tft.println("Sat: 0");
  else
    tft.println("Sat: " + String(satellites));
}


void drawLean(){
  int cSize = forcesSize/2;
  int posX = TFT_WIDTH - forcesSize/2 - 5;
  int posY = TFT_HEIGHT - FRAME_H - 10 - forcesSize;
  int posX2 = TFT_WIDTH - forcesSize - 10;
  int posY2 = TFT_HEIGHT - FRAME_H - 5 - forcesSize/2;

  if (!bgDrawn){
    tft.drawCircle(posX, posY, cSize, ILI9341_WHITE);
    tft.fillRect(posX-cSize, posY, cSize*2+2, cSize, BACKGROUND_COLOR);
    tft.drawLine(posX - cSize, posY, posX + cSize, posY, ILI9341_WHITE);

    tft.drawCircle(posX2, posY2, cSize, ILI9341_WHITE);
    tft.fillRect(posX2, posY2 - cSize, cSize, cSize*2 + 2, BACKGROUND_COLOR);
    tft.drawLine(posX2, posY2 - cSize, posX2, posY2 + cSize, ILI9341_WHITE);
    bgDrawn = true;
  }

  bool maxChanges = false;

  if (roll < 0 && minRoll > roll)
  {
    minRoll = roll;
    maxChanges = true;
  }
  if (roll > 0 && maxRoll < roll)
  {
    maxRoll = roll;
    maxChanges = true;
  }
  if (pitch < 0 && minPitch > pitch)
  {
    minPitch = pitch;
    maxChanges = true;
  }
  if (pitch > 0 && maxPitch < pitch)
  {
    maxPitch = pitch;
    maxChanges = true;
  }

  if (maxChanges){
    tft.setFont(LiberationSans_8);
    tft.setCursor(posX - 40, posY-10);
    tft.fillRect(posX-40, posY -10, 80, 9, BACKGROUND_COLOR);
    tft.println("("+getFloatString(-minPitch, 1)+")");
    tft.setCursor(posX + 13, posY-10);
    tft.println("("+getFloatString(maxPitch, 1)+")");

    tft.fillRect(posX2-29, posY2 -32, 29, 9, BACKGROUND_COLOR);
    tft.setCursor(posX2-29, posY2 -32);
    tft.println("("+getFloatString(maxRoll, 1)+")");
    tft.fillRect(posX2-29, posY2 +24, 29, 9, BACKGROUND_COLOR);
    tft.setCursor(posX2-29, posY2 +24);
    tft.println("("+getFloatString(-minRoll, 1)+")");
  }

  float s = sin(pitch * 0.0174533);
  float c = cos(pitch * 0.0174533);
  int tx = 0;
  int ty = -forcesSize/2 +2 ;
  int xnew = (int)((tx * c - ty * s) + posX);
  int ynew = (int)((tx * s + ty * c) + posY);
  if (lastPitchX > 0 && (xnew != lastPitchX || ynew != lastPitchY))
  {
    tft.drawLine(posX, posY, lastPitchX, lastPitchY, BACKGROUND_COLOR);
  }

  if (minPitchX == -100)
  {
    minPitchX = xnew;
    minPitchY = ynew;
    maxPitchX = xnew;
    maxPitchY = ynew;
  }
  else{
    if (xnew < minPitchX)
    {
      tft.drawLine(posX, posY, minPitchX, minPitchY, BACKGROUND_COLOR);
      minPitchX = xnew;
      minPitchY = ynew;
    }
    if (xnew > maxPitchX)
    {
      tft.drawLine(posX, posY, maxPitchX, maxPitchY, BACKGROUND_COLOR);
      maxPitchX = xnew;
      maxPitchY = ynew;
    }
    tft.drawLine(posX, posY, maxPitchX, maxPitchY, ILI9341_GREEN);
    tft.drawLine(posX, posY, minPitchX, minPitchY, ILI9341_GREEN);
  }
  lastPitchX = xnew;
  lastPitchY = ynew;
  tft.drawLine(posX, posY, xnew, ynew, ILI9341_RED);


  s = sin(roll * 0.0174533);
  c = cos(roll * 0.0174533);
  tx = -forcesSize/2 + 2;
  ty = 0;
  xnew = (int)((tx * c - ty * s) + posX2);
  ynew = (int)((tx * s + ty * c) + posY2);
  if (lastRollX > 0 && (xnew != lastRollX || ynew != lastRollY))
  {
    tft.drawLine(posX2, posY2, lastRollX, lastRollY, BACKGROUND_COLOR);
  }

  if (minRollX == -100)
  {
    minRollX = xnew;
    minRollY = ynew;
    maxRollX = xnew;
    maxRollY = ynew;
  }
  else{
    if (ynew < minRollY)
    {
      tft.drawLine(posX2, posY2, minRollX, minRollY, BACKGROUND_COLOR);
      minRollX = xnew;
      minRollY = ynew;
    }
    if (ynew > maxRollY)
    {
      tft.drawLine(posX2, posY2, maxRollX, maxRollY, BACKGROUND_COLOR);
      maxRollX = xnew;
      maxRollY = ynew;
    }
    tft.drawLine(posX2, posY2, maxRollX, maxRollY, ILI9341_GREEN);
    tft.drawLine(posX2, posY2, minRollX, minRollY, ILI9341_GREEN);
  }

  lastRollX = xnew;
  lastRollY = ynew;
  tft.drawLine(posX2, posY2, xnew, ynew, ILI9341_RED);

  tft.setCursor(posX - forcesSize/2-15, posY - forcesSize/2);
  tft.setFont(LiberationSans_10_Bold_Italic);
  tft.setTextColor(TEXT_COLOR);
  tft.fillRect(posX - forcesSize/2-15, posY - forcesSize/2, 35, 10, BACKGROUND_COLOR);
  tft.println(getFloatString(pitch, 1));

  tft.setCursor(posX2 - forcesSize/2-15, posY2 - forcesSize/2);
  tft.setFont(LiberationSans_10_Bold_Italic);
  tft.setTextColor(TEXT_COLOR);
  tft.fillRect(posX2 - forcesSize/2-15, posY2 - forcesSize/2, 35, 10, BACKGROUND_COLOR);
  tft.println(getFloatString(roll, 1));
}

void drawGForces(){
  int rSize = forcesSize;
  int posX = TFT_WIDTH - rSize - 5;
  int posY = TFT_HEIGHT - FRAME_H - rSize - 5;
  float left, right, front, back;

  if (ax < 0)
  {
    left = 0;
    right = -ax;
  }
  else if (ax > 0)
  {
    left = ax;
    right = 0;
  }

  if (ay < 0)
  {
    front = 0;
    back = -ay;
  }
  else if (ay > 0)
  {
    front = ay;
    back = 0;
  }

  if (front > maxFront)
    maxFront = front;
  if (back > maxBack)
    maxBack = back;
  if (left > maxLeft)
    maxLeft = left;
  if (right > maxRight)
    maxRight = right;

  tft.fillRoundRect(posX,posY, rSize, rSize, 20, BACKGROUND_COLOR);
  tft.drawRoundRect(posX,posY, rSize, rSize, 20, ILI9341_WHITE);
  tft.setTextColor(TEXT_COLOR);
  tft.setFont(LiberationSans_14_Bold_Italic);
  int fontSize = 14;
  tft.setCursor(posX + rSize/2 - fontSize - 1, posY+2);
  tft.println(getFloatString(front, 1));
  tft.setCursor(posX + rSize/2 - fontSize - 1, posY + rSize - fontSize - 2);
  tft.println(getFloatString(back, 1));

  tft.setCursor(posX +2, posY + rSize/2 -8);
  tft.println(getFloatString(left, 1));
  tft.setCursor(posX + rSize - 31, posY + rSize/2 - 8);
  tft.println(getFloatString(right, 1));

  tft.setFont(LiberationSans_8);
  tft.setCursor(posX +2, posY + rSize/2 -8+16);
  tft.println("("+getFloatString(maxLeft, 1)+")");
  tft.setCursor(posX + rSize - 31, posY + rSize/2 - 8+16);
  tft.println("("+getFloatString(maxRight, 1)+")");
  tft.setCursor(posX + rSize/2 - fontSize - 1, posY+2 + 16);
  tft.println("("+getFloatString(maxFront, 1)+")");
  tft.setCursor(posX + rSize/2 - fontSize - 1, posY + rSize - fontSize - 2-10);
  tft.println("("+getFloatString(maxBack, 1)+")");
}

float lastSpeed = -1000;
void drawSpeed(){
  if (abs(speed - lastSpeed) < 0.5)
    return;
  int posY = 40;
  tft.fillRect(0,posY, 200, 45, ILI9341_BLACK);
  tft.setTextColor(TEXT_COLOR);
  tft.setCursor(6, posY);
  tft.setFont(LiberationSans_32_Bold_Italic);
  tft.println(String(speed));

  if (speed < 10)
    tft.setCursor(96, posY+16);
  else if (speed < 100)
    tft.setCursor(118, posY+16);
  else
    tft.setCursor(140, posY+16);

  tft.setFont(LiberationSans_16_Italic);
  tft.println(speedUnits);
  lastSpeed = speed;

  if (speed > maxSpeed)
    maxSpeed = speed;

  tft.setCursor(6, posY + 34);
  tft.setFont(LiberationSans_10_Italic);
  tft.println("max: " + getFloatString(maxSpeed, 2) + speedUnits);
}
