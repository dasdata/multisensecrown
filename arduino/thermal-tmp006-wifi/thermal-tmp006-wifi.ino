#include <SPI.h>
#include <Wire.h>
#include <WiFiNINA.h>
#include <Adafruit_Sensor.h>
#include "Adafruit_TMP006.h"
#include "MAX30105.h"
#include "heartRate.h"
#include "Zanshin_BME680.h" // Include the BME680 Sensor library

#include "arduino_setup.h" 

#define REPORTING_PERIOD_MS     1000 
uint32_t tsLastReport = 0;

String heatData;
Adafruit_TMP006 TempSensorData;

MAX30105 particleSensor;
long samplesTaken = 0; //Counter for calculating the Hz or read rate
long unblockedValue; //Average IR at power up
long startTime; //Used to calculate measurement rate
const byte RATE_SIZE = 4; //Increase this for more averaging. 4 is good.
byte rates[RATE_SIZE]; //Array of heart rates
byte rateSpot = 0;
long lastBeat = 0; //Time at which the last beat occurred

float beatsPerMinute;
int beatAvg;

int isHeadOn = 0; 

int bluePin = 3;
int greenPin = 4;
int redPin = 5;
int req = 0;

///////please enter your sensitive data in the Secret tab/arduino_secrets.h
char ssid[] = SECRET_SSID;        // your network SSID (name)
char pass[] = SECRET_PASS;    // your network password (use for WPA, or use as key for WEP)
int keyIndex = 0;            // your network key Index number (needed only for WEP)

int status = WL_IDLE_STATUS;
char server[] = HOST;    // name address for DasData (using DNS)

// Initialize the Ethernet client library
// with the IP address and port of the server
// that you want to connect to (port 80 is default for HTTP):
WiFiSSLClient client;

BME680_Class BME680; ///< Create an instance of the BME680 class
float altitude(const int32_t press, const float seaLevel = 1013.25); ///< Forward function declaration with default value for sea level
float altitude(const int32_t press, const float seaLevel) 
{
  static float Altitude;
  Altitude = 44330.0*(1.0-pow(((float)press/100.0)/seaLevel,0.1903)); // Convert into altitude in meters
  return(Altitude);
} // of method altitude()

void setup() {
  while (!Serial) delay(10);
  Serial.begin(115200);
  delay(100);

  pinMode(redPin, OUTPUT);
  pinMode(greenPin, OUTPUT);
  pinMode(bluePin, OUTPUT);  

  if (! TempSensorData.begin()) {
    Serial.println("No TempSensorData sensor found");
    while (1);
  } else {
     Serial.println("TMP006 fine"); 
    }

  while (!BME680.begin(I2C_STANDARD_MODE)) // Start BME680 using I2C, use first device found
  {
    Serial.print(F("-  Unable to find BME680. Trying again in 5 seconds.\n"));
    delay(5000);
  } // of loop until device is located
  Serial.print(F("- Setting 16x oversampling for all sensors\n"));
  BME680.setOversampling(TemperatureSensor,Oversample16); // Use enumerated type values
  BME680.setOversampling(HumiditySensor,   Oversample16); // Use enumerated type values
  BME680.setOversampling(PressureSensor,   Oversample16); // Use enumerated type values
  Serial.print(F("- Setting IIR filter to a value of 4 samples\n"));
  BME680.setIIRFilter(IIR4); // Use enumerated type values
  Serial.print(F("- Setting gas measurement to 320\xC2\xB0\x43 for 150ms\n")); // "�C" symbols
  BME680.setGas(320,150); // 320�c for 150 milliseconds
  
  // Initialize sensor
  if (particleSensor.begin(Wire, I2C_SPEED_FAST) == false) //Use default I2C port, 400kHz speed
  {
    Serial.println("MAX30105 was not found. Please check wiring/power. ");
    while (1);
  }
  else {
     Serial.println("MAX30105 fine"); 
    }

   //Setup to sense up to 18 inches, max LED brightness
  byte ledBrightness = 0xFF; //Options: 0=Off to 255=50mA
  byte sampleAverage = 4; //Options: 1, 2, 4, 8, 16, 32
  byte ledMode = 2; //Options: 1 = Red only, 2 = Red + IR, 3 = Red + IR + Green
  int sampleRate = 400; //Options: 50, 100, 200, 400, 800, 1000, 1600, 3200
  int pulseWidth = 411; //Options: 69, 118, 215, 411
  int adcRange = 2048; //Options: 2048, 4096, 8192, 16384

  particleSensor.setup(ledBrightness, sampleAverage, ledMode, sampleRate, pulseWidth, adcRange); //Configure sensor with these settings

  particleSensor.setPulseAmplitudeRed(0); //Turn off Red LED
  particleSensor.setPulseAmplitudeGreen(0); //Turn off Green LED

  //Take an average of IR readings at power up
  unblockedValue = 0;
  for (byte x = 0 ; x < 32 ; x++)
  {
    unblockedValue += particleSensor.getIR(); //Read the IR value
  }
  unblockedValue /= 32;
  startTime = millis();

    // attempt to connect to WiFi network:
  while (status != WL_CONNECTED) {
    Serial.print("Attempting to connect to SSID: ");
    Serial.println(ssid);
    // Connect to WPA/WPA2 network. Change this line if using open or WEP network:
    status = WiFi.begin(ssid, pass); 
    // wait 2 seconds for connection:
    delay(2000);
    Serial.println("We're online!");
  }
 }
 

void loop() { 
  getPresence(); // activate wearable  

  float bodytemp = TempSensorData.readObjTempC()+3.9;   
  static int32_t  temp, humidity, pressure, gas;                                    // Variable to store readings
  static char     buf[16];                                                          // Text buffer for sprintf
  static float    alt;                                                              // temp variable for altitude
  static uint16_t loopCounter = 0;                                                  // Display iterations
  if (loopCounter % 25 == 0)                                                        // Display header every 25 loops
  {                                                                                 //
  //  Serial.print(F("\nLoop Temp\xC2\xB0\x43 Humid% Press hPa   Alt m Air m"));      // Show header plus unicode "�C"
  //  Serial.print(F("\xE2\x84\xA6\n==== ====== ====== ========= ======= ======\n")); // and "?" symbols
  } // if-then time to show headers                                                 //
   BME680.getSensorData(temp,humidity,pressure,gas);                                 // Get the most recent readings
   sprintf(buf, "%4d %3d.%02d", ++loopCounter%9999,                                  // Clamp iterations to 9999,
          (int8_t)(temp/100),(uint8_t)(temp%100));                                  // Temperature in decidegrees
   Serial.print(buf);                                                                //
   sprintf(buf, "%3d.%03d", (int8_t)(humidity/1000),(uint16_t)(humidity%1000));      // Humidity in milli-percent
   Serial.print(buf);                                                                //
   sprintf(buf, "%7d.%02d", (int16_t)(pressure/100),(uint8_t)(pressure%100));        // Pressure in Pascals
   Serial.print(buf);                                                                //
   alt = altitude(pressure);                                                         // temp variable for altitude
   sprintf(buf, "%5d.%02d", (int16_t)(alt),((uint8_t)(alt*100)%100));                // Altitude in meters
   Serial.print(buf);                                                                //
   sprintf(buf, "%4d.%02d\n", (int16_t)(gas/100),(uint8_t)(gas%100));                // Resistance in milliohms
   Serial.print(buf);     
   String sensorData = ""; 
  if (isHeadOn == 1 && bodytemp > 35.5) {   
      sensorData += bodytemp + String("|"); // detected temperature
      Serial.print(" Detected Temperature: ");   Serial.print(bodytemp);   Serial.println("*C");
      sensorData += random(94,97) + String("|"); // OxygenSaturation
      sensorData += random(65, 133) + String("|"); // HeartRate
      sensorData += "" + String("|"); // MedicationName
      sensorData += "" + String("|"); // MedicationWhen
      sensorData += "" + String("|"); // MedicationDosage
      sensorData += "nano33" + String("|"); // OtherNotes
      sensorData += temp + String("|"); // Indoor_Temperature
      sensorData += humidity + String("|"); // Indoor_Humid
      sensorData += pressure + String("|"); // Indoor_AirQuality 
      sensorData += gas + String("|"); // Indoor gas
      sensorData += "" + String("|");
      sensorData += "" + String("|");
      sensorData += "" + String("|");
      sensorData += "" + String("|");

      Serial.println("\nStarting connection to server...");
      // if you get a connection, report back via serial:
      if (client.connect(server, PORT)) { 
      Serial.println("connected to server");
      // Make a HTTP request:  
         sendData(String(sensorData));   
        }
    
      setColor(0, 0, 0); // null 
      delay(1000);
    }

    else { Serial.println("Stand by mode");  delay(5000);
      }
    
   
}

void getPresence() {
  samplesTaken++; 
   /* 
  Serial.print("IR[");
  Serial.print(particleSensor.getIR());
  Serial.print("] Hz[");
  Serial.print((float)samplesTaken / ((millis() - startTime) / 1000.0), 2);
  Serial.print("]"); 
  Serial.print(" delta[");
  Serial.print(currentDelta);
  Serial.print("]");
 */
  long currentDelta = particleSensor.getIR() - unblockedValue;
  if (currentDelta > (long)100) { isHeadOn = 1;   Serial.print(" Something is there!"); }
   else {isHeadOn = 0;}
  
}
 

void setColor(int red, int green, int blue)
{ 
  analogWrite(redPin, red);
  analogWrite(greenPin, green);
  analogWrite(bluePin, blue);  
}

 
void sendData(String sensorData) { 
    setColor(0, 255, 0); // green     
    String data = ""; 
    data += "/i.aspx?d=";
    data += sensorData;
    data += "&s=";
    data += DSTOKEN;
    data += "&a=";
    data += ATOKEN;
  
    String request = "GET ";
    request += data;
    request += " HTTP/1.1\r\nHost: ";
    request += HOST;
    request += "\r\n\r\n";

    Serial.print (req);
    Serial.println(request );
    client.println(request);
    delay(300);
  //  client.println("GET /?q=sample HTTP/1.1");
  //  client.println("Host: www.dasdata.co"); 
  
  }
