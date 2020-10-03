#include <TinyGPS++.h>
#include <SoftwareSerial.h>
#include <LiquidCrystal.h>

double lat1;
double lng1;
int flag=0;
int flag2=0;
int distance;
int totaldis=0;
double x;
double y;
double maxspeedval=0;
int basealtitude;
int maxalt=0;
int flag3=0;
int flag4=1;

int currentalt;

static const int RXPin = 4, TXPin = 3;
static const uint32_t GPSBaud = 9600;
const int rs = 5, en = 6, d4 = 8, d5 = 9, d6 = 10, d7 = 11;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);


// The TinyGPS++ object
TinyGPSPlus gps;

// The serial connection to the GPS device
SoftwareSerial ss(RXPin, TXPin);

void setup(){
  Serial.begin(9600);
  ss.begin(GPSBaud);
  lcd.begin(16, 2);
  lcd.print("System OK");
  
  delay(5000);
 
  
}

void maxspeed()
{ double sped=(gps.speed.kmph());
  if(maxspeedval<sped)
  {
     maxspeedval=sped;
  }
  
}


void totaldistance(double a,double b)
{ 
  if(flag2==0)
  { 
    x=lat1;
    y=lng1;
    flag2++;
  }

  distance=TinyGPSPlus::distanceBetween(x,y,a,b);

  x=a;
  y=b;

 totaldis=(totaldis+distance);
  
}

void maxaltitude()
{ 
 
   currentalt=(gps.altitude.meters());

   if(maxalt<currentalt)
   {
    maxalt=currentalt;
   }
  
}

void loop(){
  // This sketch displays information every time a new sentence is correctly encoded.
  while (ss.available() > 0){
    gps.encode(ss.read());
    if (gps.location.isUpdated()){


            if(flag==0)
            {
             lat1=(gps.location.lat());
            lng1=(gps.location.lng());
            flag++;
            }
     
      maxspeed();

     double lat2=(gps.location.lat());
     double lng2=(gps.location.lng());
     
     
      if(((flag4)%2)==0)
      {
      totaldistance(lat2,lng2);
      }

      if(((flag4)%2)==0)
      {
      maxaltitude();
      }
      flag4++;

      
      

     
      
      Serial.print(totaldis,6);
      Serial.print(" ");
      Serial.print(gps.altitude.meters(),6);
      Serial.print(" ");
      Serial.print(maxalt);
      Serial.print(" ");
      Serial.print(currentalt);
      Serial.print(" ");
      Serial.print(basealtitude);
      Serial.print("\n");

   
    
   
      lcd.clear();
      lcd.print("Alt:");
      lcd.print(maxalt-basealtitude);
      lcd.print(" ");
      lcd.print("Dis:");
      lcd.print(totaldis);
      lcd.setCursor(0, 1);
      lcd.print("speed:");
      lcd.print(maxspeedval);
   
   
      
    }
  }
}
