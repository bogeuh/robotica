#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#include <WiFiUdp.h>
#include <ArduinoOTA.h>
#include <PubSubClient.h>
#include <Wire.h>
#include <Adafruit_MPL115A2.h>

Adafruit_MPL115A2 mpl115a2;
WiFiClient feather_huzzah_1;
PubSubClient client(feather_huzzah_1);
//setup wifi and mqtt server add mqtt_user pasw
const char* ssid = "telenet-E436E8A";
const char* password = "be8pYrfEvjzm";
const char* mqtt_server = "192.168.0.155";
//read somfy remote state via input on pin 13
//connect to somfy IC pin for specific led : led1 leftmost
//count from this on up to move through individual or all motors
const byte interruptPin = 13;
//not used
//volatile byte interruptCounter = 0;
int state = 0;
const byte motor_select_Pin = 14;
const byte motor_up_Pin = 2;
const byte motor_stop_Pin = 0;
const byte motor_down_Pin = 15;
int motor = 0;
int motor_selected = 0;


//built-in blinky is pin0 relay switcher uses 12
const int lamp = 0;
const int switcher = 12;
// Timers auxiliar variables
long now = millis();
long lastMeasure = 0;
// The setup function sets your ESP GPIOs to Outputs, starts the serial communication at a baud rate of 115200
// Sets your mqtt broker and sets the callback function
// The callback function is what receives messages and actually controls the LEDs
void setup() {
  pinMode(lamp, OUTPUT);
  pinMode(switcher, OUTPUT);
  //somfy outputpins
  pinMode(motor_select_Pin, OUTPUT);
  //the digitalWrites here will fuck up things at boot
  //i think they do, test this plz
  digitalWrite(motor_select_Pin, HIGH);
  pinMode(motor_up_Pin, OUTPUT);
  digitalWrite(motor_up_Pin, HIGH);
  pinMode(motor_stop_Pin, OUTPUT);
  digitalWrite(motor_stop_Pin, HIGH);
  pinMode(motor_down_Pin, OUTPUT);
  digitalWrite(motor_down_Pin, HIGH);
  
  //somfy remote input
  pinMode(interruptPin, INPUT_PULLUP);
  //attachInterrupt(digitalPinToInterrupt(interruptPin), handleInterrupt, FALLING);
  
  //not using dht sensor
  //dht.begin();
  mpl115a2.begin();
  Serial.begin(115200);
  setup_wifi();
  //mqtt connection setup
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
  setup_ota();



}
// Don't change the function below. This functions connects your ESP8266 to your router
void setup_wifi() {
  delay(10);
  // We start by connecting to a WiFi network
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(200);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("WiFi connected - ESP IP address: ");
  Serial.println(WiFi.localIP());
} 
//SOMFY TELIS 4 remote 
//simulate buttonpresses, use input on led 1 to detect status
//the order in which the led is on or off determines the current sequence
//and allows motor select identification
//the input is LOW when led 1 is on
//led is on with motor 1 and all motors
void setup_ota() {
  // Port defaults to 8266
  // ArduinoOTA.setPort(8266);

  // Hostname defaults to esp8266-[ChipID]

  ArduinoOTA.setHostname("blind_one");

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

void step_motor(){
    digitalWrite(motor_select_Pin, LOW);
    delay(100); // wait for 1/10 second
    digitalWrite(motor_select_Pin, HIGH);
    delay(100);//longer delay needed so inputpin can get high between stepping motor
}
void detect_motor() {
    state = digitalRead(interruptPin);
    Serial.print("state: ");
    Serial.print(state);
    Serial.println();
    step_motor();
    state = digitalRead(interruptPin);
    Serial.print("state: ");
    Serial.print(state);
    Serial.println();
    if (state == 0) {
      //either all or first
      step_motor();
      state = digitalRead(interruptPin);
      Serial.print("state: ");
      Serial.print(state);
      Serial.println();
      if (state == 0){
        //two times low is motor1
        motor = 1;
        Serial.println("motor is 1");
      }
      else if (state == 1){
        //low followed by high is motor 2
        motor = 2;
        Serial.println("motor is 2");
      }
    }
    else if (state == 1) {
      // 2,3,or 4
      step_motor();
      state = digitalRead(interruptPin);
      Serial.print("state: ");
      Serial.print(state);
      Serial.println();
      if (state == 0){
        //high followed by low is all motors
        motor = 5;
        Serial.println("motor is 5");
      }
      else if (state == 1){
        //high followed by high is 3 or 4
        step_motor();
        state = digitalRead(interruptPin);
        Serial.print("state: ");
        Serial.print(state);
        Serial.println();
        if (state == 0){
          //high-high-low all motors
          motor = 5;
          Serial.println("motor is 5");        
        }
        else if (state == 1){
          //low -low -low is motor 4
          motor = 4;
          Serial.println("motor is 4");        
        }
        
      }
  
      
      
    }

  
}
//once we know which motor we have selected, move to the desired motor
//two options, desired motor has a higher number than detected or lower
//move forward until both numbers are the same
void step_detected_motor(){
    if (motor < motor_selected){
      while (motor != motor_selected){
        step_motor();
        motor++;
      }
    }
    else if (motor > motor_selected){
      //+5 since we can't step back so increase
      while (motor != (motor_selected+5)){
        step_motor();
        motor++;
      }      
    }
}
//select specific motor
void set_motor1() {
  motor_selected = 1;
  if (motor != motor_selected){
    detect_motor();
    step_detected_motor();
  }
  
}
void set_motor2() {
  motor_selected = 2;
  if (motor != motor_selected){
    detect_motor();
    step_detected_motor();
  }
}
void set_motor3() {
  motor_selected = 3;
  if (motor != motor_selected){
    detect_motor();
    step_detected_motor();
  }
}
void set_motor4() {
  motor_selected = 4;
  if (motor != motor_selected){  
    detect_motor();
    step_detected_motor();
  }
}
void set_motor5() {
  motor_selected = 5;
  if (motor != motor_selected){
    detect_motor();
    step_detected_motor();
  }
}
//what to do with the selection, up, down or stop
void motor_up(){
  digitalWrite(motor_up_Pin, LOW);
  delay(100); // wait for 1/10 second
  digitalWrite(motor_up_Pin, HIGH);
}
void motor_down(){
  digitalWrite(motor_down_Pin, LOW);
  delay(100); // wait for 1/10 second
  digitalWrite(motor_down_Pin, HIGH);
}
void motor_stop(){
  digitalWrite(motor_stop_Pin, LOW);
  delay(1000); // wait for 1/10 second
  digitalWrite(motor_stop_Pin, HIGH);
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

  //led light on of seems swapped
  if(topic=="blink_led"){
      Serial.print("Changing Room lamp to ");
      if(messageTemp == "on"){
        digitalWrite(lamp, LOW);
        Serial.print("blinky On");
        Serial.println();
      }
      else if(messageTemp == "off"){
        digitalWrite(lamp, HIGH);
        Serial.print("blinky Off");
        Serial.println();
      }
  }
  //is relay switcher swapped aswell?
  if(topic=="relay"){
      Serial.print("setting relay to: ");
      if(messageTemp == "on"){
        digitalWrite(switcher, LOW);
        Serial.print("relay On");
        Serial.println();
      }
      else if(messageTemp == "off"){
        digitalWrite(switcher, HIGH);
        Serial.print("relay Off");
        Serial.println();
      }
  }
//bit long winded repeat
//need to parse the messages?
  if(topic=="somfy/motor"){
    if(messageTemp == "one_up"){
      set_motor1();
      motor_up();
    }
    else if(messageTemp == "two_up"){
      set_motor2();
      motor_up();
    }
    else if(messageTemp == "three_up"){
      set_motor3();
      motor_up();
    }
    else if(messageTemp == "four_up"){
      set_motor4();
      motor_up();
    }
    else if(messageTemp == "all_up"){
      set_motor5();
      motor_up();
    }
    else if(messageTemp == "one_down"){
      set_motor1();
      motor_down();
    }
    else if(messageTemp == "two_down"){
      set_motor2();
      motor_down();
    }
    else if(messageTemp == "three_down"){
      set_motor3();
      motor_down();
    }
    else if(messageTemp == "four_down"){
      set_motor4();
      motor_down();
    }                    
    else if(messageTemp == "all_down"){
      set_motor5();
      motor_down();
    }
    else if(messageTemp == "one_stop"){
      set_motor1();
      motor_stop();
    }
    else if(messageTemp == "two_stop"){
      set_motor2();
      motor_stop();
    }
    else if(messageTemp == "three_stop"){
      set_motor3();
      motor_stop();
    }
    else if(messageTemp == "four_stop"){
      set_motor4();
      motor_stop();
    }
    else if(messageTemp == "all_stop"){
      set_motor5();
      motor_stop();
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
    if (client.connect("ESP8266Client", "bogeuh", "wigginx")) {
      Serial.println("connected");  
      // Subscribe or resubscribe to a topic
      // You can subscribe to more topics (to control more LEDs in this example)
      client.subscribe("blink_led");
      client.subscribe("relay");
      client.subscribe("somfy/+");      
      
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
  ArduinoOTA.handle();
/*  if(!client.loop())
    client.connect("ESP8266Client", "bogeuh", "wigginx");
*/  // Publishes new temperature and humidity every 30 seconds

  // move this to second remote esp for indoor temps
  now = millis();
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

