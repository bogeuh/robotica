#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#include <WiFiUdp.h>
#include <ArduinoOTA.h>
#include <PubSubClient.h>
#include <Wire.h>
#include <Adafruit_MPL115A2.h>
//set name to connect to mqtt
WiFiClient Kuifelmans;
//set name for temp sensor
Adafruit_MPL115A2 mpl115a2;

PubSubClient client(Kuifelmans);

//local wifi account
const char* ssid = "telenet-E436E8A";
const char* password = "be8pYrfEvjzm";

//mqtt
const char* mqtt_server = "192.168.0.155";

//built-in blinky  
const int redled = 0;
const int blueled = 2;

// set relay trigger pins
//power with 5V from usb pin, trigger with 3v pins
const int Relay1Pin = 13;
const int Relay2Pin = 14;

long now = millis();
long lastMeasure = 0;
float pressureKPA = 0, temperatureC = 0;


void setup() {
  Serial.begin(115200);
  Serial.print("initializing...");
  
  setup_wifi();
  setup_ota();
  //setup interaction 
  client.setServer(mqtt_server, 1883);
  //check mqtt server topics with callback
  client.setCallback(callback);
  //start the temp sensor
  mpl115a2.begin(); 
  //setup blinker
  pinMode(redled, OUTPUT);
  pinMode(blueled, OUTPUT);
  pinMode(Relay1Pin, OUTPUT);
  pinMode(Relay2Pin, OUTPUT);
}

void setup_wifi() {
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  //will reboot untill wifi is connected
  while (WiFi.waitForConnectResult() != WL_CONNECTED) {
    Serial.println("WIFI Connection Failed! Rebooting...");
    ESP.restart();
  }
  Serial.print("Wifi Connected");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
}

void setup_ota() {
  // Port defaults to 8266
  // ArduinoOTA.setPort(8266);

  // Hostname defaults to esp8266-[ChipID]

  ArduinoOTA.setHostname("Kuifel");

  // No authentication by default
  // ArduinoOTA.setPassword("admin");

  // Password can be set with it's md5 value as well
  // MD5(admin) = 21232f297a57a5a743894a0e4a801fc3
  // ArduinoOTA.setPasswordHash("21232f297a57a5a743894a0e4a801fc3");

  ArduinoOTA.onStart([]() {
    String type;
    if (ArduinoOTA.getCommand() == U_FLASH)
      type = "sketch";
    else // U_SPIFFS
      type = "filesystem";

    // NOTE: if updating SPIFFS this would be the place to unmount SPIFFS using SPIFFS.end()
    Serial.println("Start updating " + type);
  });
  ArduinoOTA.onEnd([]() {
    Serial.println("\nEnd");
  });
  ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
    Serial.printf("Progress: %u%%\r", (progress / (total / 100)));
  });
  ArduinoOTA.onError([](ota_error_t error) {
    Serial.printf("Error[%u]: ", error);
    if (error == OTA_AUTH_ERROR) Serial.println("Auth Failed");
    else if (error == OTA_BEGIN_ERROR) Serial.println("Begin Failed");
    else if (error == OTA_CONNECT_ERROR) Serial.println("Connect Failed");
    else if (error == OTA_RECEIVE_ERROR) Serial.println("Receive Failed");
    else if (error == OTA_END_ERROR) Serial.println("End Failed");
  });
  ArduinoOTA.begin();
}

void callback(String topic, byte* message, unsigned int length) {
  Serial.print("Time is now: ");
  Serial.println(now);

  Serial.print("Message arrived on topic: ");
  Serial.print(topic);
  Serial.print(". Message: ");
  String messageTemp;
  
  for (int i = 0; i < length; i++) {
    Serial.print((char)message[i]);
    messageTemp += (char)message[i];
  }
  Serial.println();
  now = millis();
  // Publishes new temperature and humidity every 30 seconds*/  
  if (now - lastMeasure > 30000) {
    lastMeasure = now;
        
    
    //pin 14 is connected to shutdown on sensor
    //digitalWrite(14, LOW);
    //Serial.print("Setting low");
    //delay(1000);
    //pressureKPA = mpl115a2.getPressure();  
    //Serial.print("Pressure (kPa): "); Serial.print(pressureKPA, 4); Serial.println(" kPa");
  
    temperatureC = mpl115a2.getTemperature();
    //the mqtt time_out is 15 seconds
    //after 15 seconds the published msg is reset, but new temp isnt read and published
    //till after 30 seconds
    //so either keep alive longer or retain the pubished msg till overwritten , true added
    
    //Serial.print("Temp (*C): "); Serial.print(temperatureC, 1); Serial.println(" *C");
    delay(500);
    client.publish("read_temp", String(temperatureC).c_str(), true);
    // Publishes Temperature and Humidity values
    
  }
  //relay 1 = motor on/off
  if(topic=="kuifel/relay1"){
      Serial.print("Changing Room lamp to ");
      if(messageTemp == "on"){
        digitalWrite(redled, HIGH);
        digitalWrite(Relay1Pin, HIGH);
        Serial.print("kuifel On");
        Serial.println();
      }
      else if(messageTemp == "off"){
        digitalWrite(redled, LOW);
        digitalWrite(Relay1Pin, LOW);
        Serial.print("kuifel Off");
        Serial.println();
      }
  }
  //relay 2 is forward , backward?
  if(topic=="kuifel/relay2"){
      Serial.print("setting relay to: ");
      if(messageTemp == "on"){
        digitalWrite(blueled, HIGH);
        digitalWrite(Relay2Pin, HIGH);
        Serial.print("out");
        Serial.println();
      }
      else if(messageTemp == "off"){
        digitalWrite(blueled, LOW);
        digitalWrite(Relay2Pin, LOW);
        Serial.print("in");
        Serial.println();
      }
  }
}
// This functions reconnects your ESP8266 to your MQTT broker
// Change the function below if you want to subscribe to more topics with your ESP8266 

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    /*
     YOU MIGHT NEED TO CHANGE THIS LINE, IF YOU'RE HAVING PROBLEMS WITH MQTT MULTIPLE CONNECTIONS
     To change the ESP device ID, you will have to give a new name to the ESP8266.
     Here's how it looks:
       if (client.connect("ESP8266Client")) {
     You can do it like this:
       if (client.connect("ESP1_Office")) {
     Then, for the other ESP:
       if (client.connect("ESP2_Garage")) {
      That should solve your MQTT multiple connections problem
    */

    if (client.connect("Kuifelmans", "bogeuh", "wigginx")) {
      Serial.println("connected");  
      // Subscribe or resubscribe to a topic
      // You can subscribe to more topics (to control more LEDs in this example)
      client.subscribe("kuifel/+");
      client.publish("kuifel/test", "kuifel is online");
      client.subscribe("read_temp");
      //client.subscribe("somfy/+");
            
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();
  //if(!client.loop())
  //  client.connect("ESP8266Client", "bogeuh", "wigginx");

  ArduinoOTA.handle();
  /*blinktest
  digitalWrite(blueled, LOW);
  digitalWrite(redled, HIGH);
  delay(500); // wait for 1/10 second
  digitalWrite(blueled, HIGH);
  digitalWrite(redled, LOW);
  delay(500);*/
  
}
