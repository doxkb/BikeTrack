NXPMotionSense imu;
NXPSensorFusion filter;

void initSensors()
{
  imu.begin();
  filter.begin(100);
}

void readSensors()
{
  float ax, ay, az;
  float gx, gy, gz;
  float mx, my, mz;

  if (imu.available()) {
    imu.readMotionSensor(ax, ay, az, gx, gy, gz, mx, my, mz);
    filter.update(gx, gy, gz, ax, ay, az, mx, my, mz);

    roll = filter.getRoll();
    pitch = filter.getPitch();
    heading = filter.getYaw();
    Serial.print("Orientation: ");
    Serial.print(heading);
    Serial.print(" ");
    Serial.print(pitch);
    Serial.print(" ");
    Serial.println(roll);
  }
}
