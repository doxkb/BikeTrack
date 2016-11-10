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

String getTimeString(boolean includeMillis)
{
  String toReturn =
    String(hour()) +
    ":" +
    getDigitsString(minute()) +
    ":" +
    getDigitsString(second());

  if (includeMillis)
    toReturn += "." + String(milliseconds());

  return toReturn;
}

String getDateString()
{
  String toReturn =
    String(year()) +
    "/" +
    String(month()) +
    "/" +
    String(day());
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
