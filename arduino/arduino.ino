#include <ThingSpeak.h >
#include <WiFi.h>
#include <DHT.h>  // Including library for dht
#include <OneWire.h>
#include <DallasTemperature.h>

#define  CHANNEL_ID 1950802
#define  CHANNEL_API_KEY "J5FLXJ9BGS2B2YLN"
 
const char *ssid =  "unknown";
const char *pass =  "1111g4444";
const char* server = "api.thingspeak.com";

float tempwater;
float tempair;
float pH = 8.1;
float level = 9.2;

 double sumAirTemp = 0;
 double sumWaterTemp = 0;
 double sumPh = 0;
 double sumLevel = 0;
 double sumAirTempSqr = 0;
 double sumWaterTempSqr = 0;
 double sumPhSqr = 0;
 double sumLevelSqr = 0;
 float co1 = 0;
 double co2 = 0;
 double counter = 0;
 
#define DHTPIN 14
//pin where the dht11 is connected
 
DHT dht(DHTPIN, DHT11);

WiFiClient client;

// GPIO where the DS18B20 is connected to
const int oneWireBus = 4;     

// Setup a oneWire instance to communicate with any OneWire devices
OneWire oneWire(oneWireBus);

// Pass our oneWire reference to Dallas Temperature sensor 
DallasTemperature sensors(&oneWire);
 
void setup() 
{
       Serial.begin(115200);
       //delay(10);
       dht.begin();

       // Start the DS18B20 sensor
       sensors.begin();
 
       Serial.println("Connecting to ");
       Serial.println(ssid);
 
 
       WiFi.begin(ssid, pass);
 
      while (WiFi.status() != WL_CONNECTED) 
     {
            delay(500);
            Serial.print(".");
     }
      Serial.println("");
      Serial.println("WiFi connected to");
      Serial.print(ssid);
      ThingSpeak.begin(client);
}
 
void loop() 
{
          counter++;
          
          tempair = dht.readTemperature();

              Serial.println("ºC");
      sensors.requestTemperatures(); 
          tempwater = sensors.getTempCByIndex(0);
          pH = 8.3 - ((tempwater-20) * (8.1/400));
          level = 8+((tempair - 24)*0.11388893);

          Serial.println(tempair);
          Serial.print(tempwater);
          Serial.println("ºC");

          sumAirTemp += tempair;
          sumWaterTemp += tempwater;
          sumPh += pH;
          sumLevel += level;

          sumAirTempSqr += tempair*tempair;
          sumWaterTempSqr += tempwater*tempwater;
          sumPhSqr += pH*pH;
          sumLevelSqr += level*level;

          co1 = (float)(counter * (sumWaterTemp*sumPh) - sumWaterTemp * sumPh)
                  / sqrt((counter * sumWaterTempSqr- (sumWaterTemp * sumWaterTemp)) * (counter * sumPhSqr - (sumPh * sumPh)));
           
          ThingSpeak.setField(1, tempair);
          ThingSpeak.setField(2, pH);
          ThingSpeak.setField(3, level);
          ThingSpeak.setField(4, tempwater);
          ThingSpeak.setField(5, co1);

          
          ThingSpeak.writeFields(CHANNEL_ID, CHANNEL_API_KEY);
          delay(15000); 
}
