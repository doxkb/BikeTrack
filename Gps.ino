TinyGPS gps;
//const int offset = -5;  // Eastern Standard Time (USA)
const int offset = -4;  // Eastern Daylight Time (USA)
time_t prevDisplay = 0; // when the digital clock was displayed
int timeChangeMills = 0;

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
  timeChangeMills = millis();
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
    timeChangeMills = millis();
    drawTime();
  }
}

int milliseconds()
{
  return millis() - timeChangeMills;
}

void gpsdump(TinyGPS &gps)
{
  unsigned long age;
  gps.f_get_position(&latitude, &longitude, &age);
  altitude = gps.f_altitude();
  readGpsDateTime(gps);

  switch (speedType){
    case mph:
      speed = gps.f_speed_mph();
      speedUnits = "mph";
      break;
    case kmph:
      speed = gps.f_speed_kmph();
      speedUnits = "km/h";
      break;
    case mps:
      speed = gps.f_speed_mps();
      speedUnits = "mps";
      break;
  }

  satellites = gps.satellites();

  Serial.print("Lat/Long(float): "); Serial.print(getFloatString(latitude, 5)); Serial.print(", "); Serial.println(getFloatString(longitude, 5));
  Serial.print("Alt(m): "); Serial.print(getFloatString(altitude, 2));  Serial.println();
  Serial.print("Speed(" + speedUnits + "): "); Serial.print(getFloatString(speed, 2)); Serial.println();
  Serial.print("Satellites: "); Serial.println(satellites);
}

void readGpsDateTime(TinyGPS &gps)
{
  unsigned long age, date, time;
  byte month, day, hour, minute, second, hundredths;
  int year;

  gps.get_datetime(&date, &time, &age);
  Serial.print("GPS Date(ddmmyy): "); Serial.print(date); Serial.print(" Time(hhmmsscc): ");
  Serial.println(time);

  gps.crack_datetime(&year, &month, &day, &hour, &minute, &second, &hundredths, &age);
  if (age < 1000) {
    setTime(hour, minute, second, day, month, year);
    adjustTime(offset * SECS_PER_HOUR);
  }

  Serial.print("TEENSY Date: "); Serial.println(getDateString());
  Serial.print("  Time: "); Serial.print(getTimeString());Serial.print("."); Serial.println(milliseconds());
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
    else if (incomingByte == 'm')
      speedType = mph;
    else if (incomingByte == 'k')
      speedType = kmph;
    else if (incomingByte == 's')
      speedType = mps;
  }
}
