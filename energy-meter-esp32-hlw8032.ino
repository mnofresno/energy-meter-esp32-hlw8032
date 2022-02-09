#include <WiFiManager.h>
#include <PubSubClient.h>
#include <PZEM004Tv30.h>

const char* topic_to_transmit= "energy-meter-data";
const char* mqtt_server = "192.168.1.1";
String logBuffer = "";

int reporting_period = 2000;
unsigned long startMillis;
unsigned long currentMillis;

WiFiClient espClient;
WiFiManager wm;
PubSubClient client(espClient);

#define CURRENT_FACTOR 2.103
#define MAX_LOG_BUFFER 5000

#if !defined(PZEM_RX_PIN) && !defined(PZEM_TX_PIN)
#define PZEM_RX_PIN 16
#define PZEM_TX_PIN 17
#endif

#if !defined(PZEM_SERIAL)
#define PZEM_SERIAL Serial2
#endif


#if defined(ESP32)
/*************************
 *  ESP32 initialization
 * ---------------------
 * 
 * The ESP32 HW Serial interface can be routed to any GPIO pin 
 * Here we initialize the PZEM on Serial2 with RX/TX pins 16 and 17
 */
PZEM004Tv30 pzem(PZEM_SERIAL, PZEM_RX_PIN, PZEM_TX_PIN);
#elif defined(ESP8266)
/*************************
 *  ESP8266 initialization
 * ---------------------
 * 
 * Not all Arduino boards come with multiple HW Serial ports.
 * Serial2 is for example available on the Arduino MEGA 2560 but not Arduino Uno!
 * The ESP32 HW Serial interface can be routed to any GPIO pin 
 * Here we initialize the PZEM on Serial2 with default pins
 */
//PZEM004Tv30 pzem(Serial1);
#else
/*************************
 *  Arduino initialization
 * ---------------------
 * 
 * Not all Arduino boards come with multiple HW Serial ports.
 * Serial2 is for example available on the Arduino MEGA 2560 but not Arduino Uno!
 * The ESP32 HW Serial interface can be routed to any GPIO pin 
 * Here we initialize the PZEM on Serial2 with default pins
 */
PZEM004Tv30 pzem(PZEM_SERIAL);
#endif

void log(const char* data = "") {
  if (logBuffer.length() > MAX_LOG_BUFFER) {
    logBuffer = "";
  }
  logBuffer += String(millis()) + "> " + String(data) + "\n" + logBuffer;
}

void setup() {
  // Debugging Serial port
  Serial.begin(115200);
  startMillis = millis();  //initial start time

  // Uncomment in order to reset the internal energy counter
  // pzem.resetEnergy()
  initializeWifiAndMqtt();
}

void initializeWifiAndMqtt() {
  WiFi.mode(WIFI_STA);
  bool res;
  res = wm.autoConnect("EnergyMeterIoT","password"); // password protected ap
    
  if(!res) {
    log("Failed to connect");
    // ESP.restart();
  } 
  else {
    //if you get here you have connected to the WiFi    
    log("connected...yeey :)");
    
    wm.setConfigPortalBlocking(false);
    wm.startConfigPortal();
    wm.server->on("/log", [&]() {
      wm.server->send(200, "text/plain charset=utf-8", logBuffer);
    });
    client.setServer(mqtt_server, 1883);
    client.setCallback(mqtt_callback);
  }
}

void loop() {
  wm.process();
  if (!client.connected()) {
    reconnect();
  }
  client.loop();
  
  measurementLoop();
}

void measurementLoop() {
  currentMillis = millis();  //get the current "time" (actually the number of milliseconds since the program started)
  if (currentMillis - startMillis >= reporting_period){
    printSerialDebug();
    startMillis = currentMillis;  //IMPORTANT to save the start time of the current LED state.
  }  
}

void mqtt_callback(char* topic, byte* message, unsigned int length) {
  log("Message arrived on topic: ");
  log(topic);
  log(". Message: ");
  String messageTemp;
  
  for (int i = 0; i < length; i++) {
    log((char)message[i]);
    messageTemp += (char)message[i];
  }
  log();
}

void sendStatus(float voltage, float current, float power, float energy, float frequency, float pf) {
  String output = "";
  output.concat(voltage);
  output.concat("|");
  output.concat(current);
  output.concat("|");
  output.concat(power);
  output.concat("|");
  output.concat(energy);
  output.concat("|");
  output.concat(frequency);
  output.concat("|");
  output.concat(pf);
  client.publish(
    topic_to_transmit,
    output.c_str()
  );
}

void printSerialDebug() {
  // Print the custom address of the PZEM
  log("Custom Address:");
  //log(pzem.readAddress(), HEX);

  // Read the data from the sensor
  float voltage = pzem.voltage();
  float current = pzem.current() * CURRENT_FACTOR;
  float power = pzem.power() * CURRENT_FACTOR;
  float energy = pzem.energy() * CURRENT_FACTOR;
  float frequency = pzem.frequency();
  float pf = pzem.pf();
  sendStatus(voltage, current, power, energy, frequency, pf);
  // Check if the data is valid
  if(isnan(voltage)){
      log("Error reading voltage");
  } else if (isnan(current)) {
      log("Error reading current");
  } else if (isnan(power)) {
      log("Error reading power");
  } else if (isnan(energy)) {
      log("Error reading energy");
  } else if (isnan(frequency)) {
      log("Error reading frequency");
  } else if (isnan(pf)) {
      log("Error reading power factor");
  } else {      
    log(String("Voltage: " + String(voltage)).c_str());
    log(String("Current: " + String(current)).c_str());
    log(String("Power: " + String(power)).c_str());
    log(String("Energy: " + String(energy)).c_str());
    log(String("Frequency: " + String(frequency)).c_str());
    log(String("PF: " + String(frequency)).c_str());

  }
  //delay(2000);
}

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    log("Attempting MQTT connection...");
    // Attempt to connect
    if (client.connect("ESP8266Client")) {
      log("connected");
      // Subscribe
      
      //log("Subscribing topic:");
      //log(topic_to_subscribe);
      //client.subscribe(topic_to_subscribe);
    } else {
      log("failed, rc=");
      log(client.state());
      log(" try again in 1 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}
