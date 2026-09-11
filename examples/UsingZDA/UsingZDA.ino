#include <TinyGPSPlus.h>

/*
   Extract UTC time and a four-digit year from ZDA using TinyGPSCustom.
   Like BasicExample, this sketch uses sample strings; no GPS device is needed.

   ZDA fields are: time (hhmmss.ss), day, month, year (yyyy), local zone hours,
   and local zone minutes. The first four fields describe UTC; this example
   does not apply the local zone fields or convert the date to an epoch value.

   Custom sentence names must match exactly. GPZDA and GNZDA therefore have
   separate listeners here. For a real receiver, enable ZDA output if necessary
   and feed gps.encode() characters from its serial port instead of gpsStream.

   These are field strings from checksum-passing sentences, not a guarantee of
   a valid fix or trustworthy time. Applications decide how to handle missing,
   invalid, or stale data. ZDA does not update gps.date or gps.time; its fields
   are read through the custom listeners below.
*/

const char *gpsStream =
  "$GPZDA,235959.00,31,12,2099,00,00*64\r\n"
  "$GNZDA,000000.00,01,01,2100,00,00*7B\r\n"
  "$GPZDA,120000.00,11,09,2226,00,00*68\r\n";

TinyGPSPlus gps;

// Field numbering starts at 1, immediately after the sentence name.
TinyGPSCustom gpUtc(gps, "GPZDA", 1);
TinyGPSCustom gpDay(gps, "GPZDA", 2);
TinyGPSCustom gpMonth(gps, "GPZDA", 3);
TinyGPSCustom gpYear(gps, "GPZDA", 4);

TinyGPSCustom gnUtc(gps, "GNZDA", 1);
TinyGPSCustom gnDay(gps, "GNZDA", 2);
TinyGPSCustom gnMonth(gps, "GNZDA", 3);
TinyGPSCustom gnYear(gps, "GNZDA", 4);

void displayZda(const char *sentence, TinyGPSCustom &utc, TinyGPSCustom &day,
                TinyGPSCustom &month, TinyGPSCustom &year)
{
  if (!year.isUpdated())
    return;

  Serial.print(sentence);
  Serial.print(F(" UTC date: "));
  Serial.print(year.value());
  Serial.print(F("-"));
  Serial.print(month.value());
  Serial.print(F("-"));
  Serial.print(day.value());
  Serial.print(F(" time: "));
  Serial.println(utc.value());
  // value() clears each listener's isUpdated() flag.
}

void setup()
{
  Serial.begin(115200);
  Serial.println(F("UsingZDA.ino (no device needed)"));

  while (*gpsStream)
  {
    if (gps.encode(*gpsStream++))
    {
      // Read updates after each completed sentence, before feeding the next.
      displayZda("GPZDA", gpUtc, gpDay, gpMonth, gpYear);
      displayZda("GNZDA", gnUtc, gnDay, gnMonth, gnYear);
    }
  }
}

void loop()
{
}
