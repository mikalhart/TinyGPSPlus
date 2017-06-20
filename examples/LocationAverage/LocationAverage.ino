#include <TinyGPS++.h>
/* 
   This sample sketch should be the second you try out when you are testing a TinyGPS++
   (TinyGPSPlus) installation.  In normal use, you feed TinyGPS++ objects characters from
   a serial NMEA GPS device, but this example uses static strings for simplicity.

   It also demonstrates the average location ability.
*/

// A sample NMEA stream.
const char *gpsStream =
  "$GPRMC,045103.000,A,3014.1984,N,09749.2872,W,0.67,161.46,030913,,,A*7C\r\n"
  "$GPGGA,045104.000,3014.1985,N,09749.2873,W,1,09,1.2,211.6,M,-22.5,M,,0000*62\r\n"
  "$GPRMC,045200.000,A,3014.3820,N,09748.9514,W,36.88,65.02,030913,,,A*77\r\n"
  "$GPGGA,045201.000,3014.3864,N,09748.9411,W,1,10,1.2,200.8,M,-22.5,M,,0000*6C\r\n"
  "$GPRMC,045251.000,A,3014.4275,N,09749.0626,W,0.51,217.94,030913,,,A*7D\r\n"
  "$GPGGA,045252.000,3014.4273,N,09749.0628,W,1,09,1.3,206.9,M,-22.5,M,,0000*6F\r\n";

// The TinyGPS++ object
TinyGPSPlus gps;

void setup()
{
  SerialUSB.begin(9600);

  while(!SerialUSB);

  // default 10. This function is dynamic, which means it can be called during runtime. 
  // Thus it is resized later in `void displayInfo()`
  gps.location.average.resize(2);

  SerialUSB.println(F("LocationAverage.ino"));
  SerialUSB.println(F("Basic demonstration of TinyGPS++ and average location (no device needed)"));
  SerialUSB.print(F("Testing TinyGPS++ library v. ")); SerialUSB.println(TinyGPSPlus::libraryVersion());
  SerialUSB.println(F("by Mikal Hart, Average Location added by Daniel Robinson"));
  SerialUSB.println();

  while (*gpsStream)
    if (gps.encode(*gpsStream++))
      displayInfo();

  SerialUSB.println();
  SerialUSB.println(F("Done."));
  
  pinMode(LED_BUILTIN, OUTPUT);
}

void loop()
{
  // blink LED
  static int time_now = 0, led_enabled = 0;
  if (millis() - time_now > 1000) {
    led_enabled ^= 1;
    digitalWrite(LED_BUILTIN, led_enabled);
    time_now = millis();
    SerialUSB.println(time_now);
  }
}

void displayInfo()
{
  SerialUSB.print(F("Location: ")); 
  if (gps.location.isValid())
  {
    SerialUSB.print(gps.location.lat(), 9);
    SerialUSB.print(F(","));
    SerialUSB.print(gps.location.lng(), 9);
  }
  else
  {
    SerialUSB.print(F("INVALID"));
  }

  SerialUSB.print(F("  Date/Time: "));
  if (gps.date.isValid())
  {
    SerialUSB.print(gps.date.month());
    SerialUSB.print(F("/"));
    SerialUSB.print(gps.date.day());
    SerialUSB.print(F("/"));
    SerialUSB.print(gps.date.year());
  }
  else
  {
    SerialUSB.print(F("INVALID"));
  }

  SerialUSB.print(F(" "));
  if (gps.time.isValid())
  {
    if (gps.time.hour() < 10) SerialUSB.print(F("0"));
    SerialUSB.print(gps.time.hour());
    SerialUSB.print(F(":"));
    if (gps.time.minute() < 10) SerialUSB.print(F("0"));
    SerialUSB.print(gps.time.minute());
    SerialUSB.print(F(":"));
    if (gps.time.second() < 10) SerialUSB.print(F("0"));
    SerialUSB.print(gps.time.second());
    SerialUSB.print(F("."));
    if (gps.time.centisecond() < 10) SerialUSB.print(F("0"));
    SerialUSB.print(gps.time.centisecond());
  }
  else
  {
    SerialUSB.print(F("INVALID"));
  }

  SerialUSB.println();

  if (gps.location.average.isUpdated())
  {
    gps.location.average.resize(4); // take note of this
    SerialUSB.print(F("Average Location: ")); 
    SerialUSB.print(gps.location.average.lat(), 9);
    SerialUSB.print(F(","));
    SerialUSB.println(gps.location.average.lng(), 9);
    SerialUSB.println();
  }
}
