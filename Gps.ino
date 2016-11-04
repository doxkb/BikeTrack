TinyGPS gps;
//const int offset = -5;  // Eastern Standard Time (USA)
const int offset = -4;  // Eastern Daylight Time (USA)
time_t prevDisplay = 0; // when the digital clock was displayed

const byte ms100[] = {0xB5 , 0x62 , 0x06 , 0x08 , 0x06 , 0x00 , 0x64 , 0x00 , 0x01 , 0x00 , 0x01 , 0x00 , 0x7A , 0x12};
const byte ms200[] = {0xB5 , 0x62 , 0x06 , 0x08 , 0x06 , 0x00 , 0xC8 , 0x00 , 0x01 , 0x00 , 0x01 , 0x00 , 0xDE , 0x6A};
const byte ms500[] = {0xB5 , 0x62 , 0x06 , 0x08 , 0x06 , 0x00 , 0xF4 , 0x01 , 0x01 , 0x00 , 0x01 , 0x00 , 0x0B , 0x77};
const byte ms1000[] = {0xB5 , 0x62 , 0x06 , 0x08 , 0x06 , 0x00 , 0xE8 , 0x03 , 0x01 , 0x00 , 0x01 , 0x00 , 0x01 , 0x39};
const byte ms2000[] = {0xB5 , 0x62 , 0x06 , 0x08 , 0x06 , 0x00 , 0xD0 , 0x07 , 0x01 , 0x00 , 0x01 , 0x00 , 0xED , 0xBD};
const byte msSet[] = {0xB5 , 0x62 , 0x06 , 0x08 , 0x00 , 0x00 , 0x0E , 0x30};
int theDelay = 500;

void initGps()
{
  Serial.print("Sizeof(gpsobject) = "); Serial.println(sizeof(TinyGPS));
  Serial1.begin(115200);
  Serial1.write(ms500, 14);
  Serial1.write(msSet, 8);
}

void readGps()
{
  checkSerialForRateChange();

  if (timeStatus() != timeNotSet) {
    if (now() != prevDisplay) { //update the display only if the time has changed
      prevDisplay = now();
      getTimeString();
    }
  }

  bool newdata = false;
  if (Serial1.available()) {
    char c = Serial1.read();
    if (gps.encode(c)) {
      newdata = true;
    }
  }

  if (newdata) {
    Serial.println("-------------");
    gpsdump(gps);
    Serial.println("-------------");
    Serial.println();
  }

  if (now() != prevDisplay) { //update the display only if the time has changed
    prevDisplay = now();
    drawTime();
  }
}

void gpsdump(TinyGPS &gps)
{
  float flat, flon;
  unsigned long age;

  gps.f_get_position(&flat, &flon, &age);
  Serial.print("Lat/Long(float): "); Serial.print(getFloatString(flat, 5)); Serial.print(", "); Serial.print(getFloatString(flon, 5));
  Serial.print(" Fix age: "); Serial.print(age); Serial.println("ms.");

  readGpsDateTime(gps);

  Serial.print("Alt(m): "); Serial.print(getFloatString(gps.f_altitude(), 2));  Serial.println();
  Serial.print("Speed(kmh): "); Serial.print(getFloatString(gps.f_speed_kmph(), 2)); Serial.println();
  Serial.print("Satellites: "); Serial.println(gps.satellites());
}

void readGpsDateTime(TinyGPS &gps)
{
  unsigned long age, date, time;
  byte month, day, hour, minute, second, hundredths;
  int year;

  gps.get_datetime(&date, &time, &age);
  Serial.print("Date(ddmmyy): "); Serial.print(date); Serial.print(" Time(hhmmsscc): ");
  Serial.print(time);
  Serial.print(" Fix age: "); Serial.print(age); Serial.println("ms.");

  gps.crack_datetime(&year, &month, &day, &hour, &minute, &second, &hundredths, &age);
  if (age < 1000) {
    setTime(hour, minute, second, day, month, year);
    adjustTime(offset * SECS_PER_HOUR);
  }

  Serial.print("Date: "); Serial.print(static_cast<int>(month)); Serial.print("/");
  Serial.print(static_cast<int>(day)); Serial.print("/"); Serial.print(year);
  Serial.print("  Time: "); Serial.print(static_cast<int>(hour)); Serial.print(":");
  Serial.print(static_cast<int>(minute)); Serial.print(":"); Serial.print(static_cast<int>(second));
  Serial.print("."); Serial.print(static_cast<int>(hundredths));
  Serial.print("  Fix age: ");  Serial.print(age); Serial.println("ms.");
}

void checkSerialForRateChange()
{
  if (Serial.available() > 0) {
    char incomingByte;
    incomingByte = Serial.read();
    if (incomingByte == '1') {
      Serial1.write(ms100, 14);
      Serial1.write(msSet, 8);
      theDelay = 100;
    }
    else if (incomingByte == '2') {
      Serial1.write(ms200, 14);
      Serial1.write(msSet, 8);
      theDelay = 200;
    }
    else if (incomingByte == '3') {
      Serial1.write(ms500, 14);
      Serial1.write(msSet, 8);
      theDelay = 500;
    }
    else if (incomingByte == '4') {
      Serial1.write(ms1000, 14);
      Serial1.write(msSet, 8);
      theDelay = 1000;
    }
    else if (incomingByte == '5') {
      Serial1.write(ms2000, 14);
      Serial1.write(msSet, 8);
      theDelay = 2000;
    }
  }
}

String getFloatString(double number, int digits)
{
  String toReturn = "";
  if (number < 0.0)
  {
    toReturn += "-";
    number = -number;
  }
  double rounding = 0.5;
  for (uint8_t i = 0; i < digits; ++i)
    rounding /= 10.0;

  number += rounding;

  unsigned long int_part = (unsigned long)number;
  double remainder = number - (double)int_part;
  toReturn += String(int_part);

  if (digits > 0)
    toReturn += ".";

  while (digits-- > 0) {
    remainder *= 10.0;
    int toPrint = int(remainder);
    toReturn += String(toPrint);
    remainder -= toPrint;
  }

  return toReturn;
}

String getTimeString()
{
  String toReturn =
    String(hour()) +
    ":" +
    getDigitsString(minute()) +
    ":" +
    getDigitsString(second());
  return toReturn;
}

String getDigitsString(int digits)
{
  String toReturn = "";
  if (digits < 10)
    toReturn += "0";
  toReturn += String(digits);
  return toReturn;
}
