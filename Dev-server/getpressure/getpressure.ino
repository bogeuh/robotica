
#include <ESP8266WiFi.h>
#include <PubSubClient.h>

#include <Wire.h>
#include <Adafruit_MPL115A2.h>

Adafruit_MPL115A2 mpl115a2;
const char* ssid = "loveshack";
const char* password = "cathyenlieven";
const char* mqtt_server = "192.168.0.155";

WiFiClient feather_huzzah_1;
PubSubClient client(feather_huzzah_1);


//built-in blinky is pin0
const int lamp = 0;

// Timers auxiliar variables
long now = millis();
long lastMeasure = 0;

// Don't change the function below. This functions connects your ESP8266 to your router
void setup_wifi() {
  delay(10);
  // We start by connecting to a WiFi network
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("WiFi connected - ESP IP address: ");
  Serial.println(WiFi.localIP());
}

// This functions is executed when some device publishes a message to a topic that your ESP8266 is subscribed to
// Change the function below to add logic to your program, so when a device publishes a message to a topic that 
// your ESP8266 is subscribed you can actually do something
void callback(String topic, byte* message, unsigned int length) {
  Serial.print("Message arrived on topic: ");
  Serial.print(topic);
  Serial.print(". Message: ");
  String messageTemp;
  
  for (int i = 0; i < length; i++) {
    Serial.print((char)message[i]);
    messageTemp += (char)message[i];
  }
  Serial.println();
  if(topic=="get_temp"){
      Serial.print("Changing Room lamp to ");
      if(messageTemp == "on"){
        digitalWrite(lamp, HIGH);
        Serial.print("blinky On");
      }
      else if(messageTemp == "off"){
        digitalWrite(lamp, LOW);
        Serial.print("blinky Off");
      }
  }
  Serial.println();
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
    if (client.connect("ESP8266Client", "bogeuh", "wigginx")) {
      Serial.println("connected");  
      // Subscribe or resubscribe to a topic
      // You can subscribe to more topics (to control more LEDs in this example)
      client.subscribe("read_temp");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

// The setup function sets your ESP GPIOs to Outputs, starts the serial communication at a baud rate of 115200
// Sets your mqtt broker and sets the callback function
// The callback function is what receives messages and actually controls the LEDs
void setup() {
  pinMode(lamp, OUTPUT);
  
  //not using dht sensor
  //dht.begin();
  mpl115a2.begin();
  Serial.begin(115200);
  setup_wifi();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);

}


//void setup(void) 
//{
//  pinMode(0, OUTPUT);
//  pinMode(14, OUTPUT);
//  Serial.begin(9600);
//  Serial.println("Hello!");
//  
//  Serial.println("Getting barometric pressure ...");
//  mpl115a2.begin();
//}

void loop() {

  if (!client.connected()) {
    reconnect();
  }
  if(!client.loop())
    client.connect("ESP8266Client", "bogeuh", "wigginx");

  now = millis();
  // Publishes new temperature and humidity every 30 seconds
  if (now - lastMeasure > 30000) {
    lastMeasure = now;
    float pressureKPA = 0, temperatureC = 0;    
    
    //pin 14 is connected to shutdown on sensor
    //digitalWrite(14, LOW);
    //Serial.print("Setting low");
    //delay(1000);
    pressureKPA = mpl115a2.getPressure();  
    Serial.print("Pressure (kPa): "); Serial.print(pressureKPA, 4); Serial.println(" kPa");
  
    temperatureC = mpl115a2.getTemperature();  
    Serial.print("Temp (*C): "); Serial.print(temperatureC, 1); Serial.println(" *C");

    // Publishes Temperature and Humidity values
    client.publish("read_temp", String(temperatureC).c_str());

  }
} 
////old function
//void loop(void) 
//{
//  float pressureKPA = 0, temperatureC = 0;    
//  digitalWrite(14, LOW);
//  Serial.print("Setting low");
//  delay(1000);
//  pressureKPA = mpl115a2.getPressure();  
//  Serial.print("Pressure (kPa): "); Serial.print(pressureKPA, 4); Serial.println(" kPa");
//
//  temperatureC = mpl115a2.getTemperature();  
//  Serial.print("Temp (*C): "); Serial.print(temperatureC, 1); Serial.println(" *C");
//  digitalWrite(0, HIGH);
//  delay(500);
//  digitalWrite(0, LOW);
//  delay(500);  
//
//}
