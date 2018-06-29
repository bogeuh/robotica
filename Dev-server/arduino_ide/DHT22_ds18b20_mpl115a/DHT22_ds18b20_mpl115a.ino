// Example testing sketch for various DHT humidity/temperature sensors
// Written by ladyada, public domain

#include "DHT.h"

#define DHTPIN 14     // what digital pin we're connected to

// Uncomment whatever type you're using!
//#define DHTTYPE DHT11   // DHT 11
#define DHTTYPE DHT22   // DHT 22  (AM2302), AM2321
//#define DHTTYPE DHT21   // DHT 21 (AM2301)

// Connect pin 1 (on the left) of the sensor to +5V
// NOTE: If using a board with 3.3V logic like an Arduino Due connect pin 1
// to 3.3V instead of 5V!
// Connect pin 2 of the sensor to whatever your DHTPIN is
// Connect pin 4 (on the right) of the sensor to GROUND
// Connect a 10K resistor from pin 2 (data) to pin 1 (power) of the sensor

// Initialize DHT sensor.
// Note that older versions of this library took an optional third parameter to
// tweak the timings for faster processors.  This parameter is no longer needed
// as the current DHT reading algorithm adjusts itself to work on faster procs.
DHT dht(DHTPIN, DHTTYPE);

//
//ds18b20 sensor setup
//
#include <OneWire.h>
#include <DallasTemperature.h>

#define ONE_WIRE_BUS 13

OneWire oneWire(ONE_WIRE_BUS);

DallasTemperature sensors(&oneWire);

 float Celcius=0;
 float Fahrenheit=0;

//MPL115A config
#include <Adafruit_MPL115A2.h>
#include <Wire.h>
//set name for temp sensor
Adafruit_MPL115A2 mpl115a2;

long now = millis();
long lastMeasure = 0;
float pressureKPA = 0, temperatureC = 0;
 
void setup() {
  Serial.begin(9600);
  Serial.println("DHTxx test!");

  dht.begin();
  sensors.begin();
  mpl115a2.begin(); 
}

void loop() {
  // Wait a few seconds between measurements.
  delay(2000);

  // Reading temperature or humidity takes about 250 milliseconds!
  // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
  float h = dht.readHumidity();
  // Read temperature as Celsius (the default)
  float t = dht.readTemperature();
  // Read temperature as Fahrenheit (isFahrenheit = true)
  float f = dht.readTemperature(true);

  // Check if any reads failed and exit early (to try again).
  if (isnan(h) || isnan(t) || isnan(f)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }

  // Compute heat index in Fahrenheit (the default)
  float hif = dht.computeHeatIndex(f, h);
  // Compute heat index in Celsius (isFahreheit = false)
  float hic = dht.computeHeatIndex(t, h, false);

  Serial.print("Humidity: ");
  Serial.print(h);
  Serial.print(" %\t");
  Serial.print("Temperature: ");
  Serial.print(t);
  Serial.print(" *C ");
  Serial.print(f);
  Serial.print(" *F\t");
  Serial.print("Heat index: ");
  Serial.print(hic);
  Serial.print(" *C ");
  Serial.print(hif);
  Serial.println(" *F");
  sensors.requestTemperatures(); 
  Celcius=sensors.getTempCByIndex(0);
  Fahrenheit=sensors.toFahrenheit(Celcius);
  Serial.print(" C  ");
  Serial.print(Celcius);
  Serial.print(" F  ");
  Serial.println(Fahrenheit);
  delay(1000);
  now = millis();
  // Publishes new temperature and humidity every 30 seconds*/  
  if (now - lastMeasure > 30000) {
    lastMeasure = now;
        
    
    //pin 14 is connected to shutdown on sensor
    //digitalWrite(14, LOW);
    //Serial.print("Setting low");
    //delay(1000);
    pressureKPA = mpl115a2.getPressure();  
    Serial.print("Pressure (kPa): "); Serial.print(pressureKPA, 4); Serial.println(" kPa");
  
    temperatureC = mpl115a2.getTemperature();
    //the mqtt time_out is 15 seconds
    //after 15 seconds the published msg is reset, but new temp isnt read and published
    //till after 30 seconds
    //so either keep alive longer or retain the pubished msg till overwritten , true added
    
    Serial.print("Temp (*C): "); Serial.print(temperatureC, 1); Serial.println(" *C");

    //only serial output for testing
    //client.publish("read_temp", String(temperatureC).c_str(), true);
    // Publishes Temperature and Humidity values
  } 
}
