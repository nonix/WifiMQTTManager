#include "secrets.h"
#include <WiFiMQTTManager.h>

// Button that will put device into Access Point mode to allow for re-entering WiFi and MQTT settings
#define RESET_BUTTON 12

WiFiMQTTManager wmm(RESET_BUTTON, AP_PASSWORD);  // AP_PASSWORD is defined in the secrets.h file

// just a header declaration
//void subscriptionCallback(char* topic, byte* message, unsigned int length);

void setup() {
  Serial.begin(115200);
  Serial.println(F("WiFiMQTTManager Basic Example"));
  // set debug to true to get verbose logging
  // wm.wm.setDebugOutput(true);
  // most likely need to format FS but only on first use
  // wmm.formatFS = true;
  // optional - define the function that will subscribe to topics if needed
  wmm.subscribeTo = subscribeTo;
  // required - allow WiFiMQTTManager to do it's setup
  wmm.setup(__SKETCH_NAME__);
  // optional - define a callback to handle incoming messages from MQTT
//  wmm.subscriptionCallback = subscriptionCallback;

}

void loop() {
  // required - allow WiFiMQTTManager to check for new MQTT messages,
  // check for reset button push, and reconnect to MQTT if necessary
  wmm.loop();

  // optional - example of publishing to MQTT a sensor reading once a 1 minute
  long now = millis();
  if (now - wmm.lastMsg > 60000) {
    wmm.lastMsg = now;
    float temperature = 70; // read sensor here
    Serial.print("Temperature: ");
    Serial.println(temperature);
    char topic[100];
    snprintf(topic, sizeof(topic), "%s%s%s", "sensor/", wmm.friendlyName, "/temperature");
    wmm.client->publish(topic, String(temperature).c_str(), true);
  }


}

// optional function to subscribe to MQTT topics
void subscribeTo() {
  Serial.println("subscribing to some topics...");
  // subscribe to some topic(s)
  char topic[100];
  snprintf(topic, sizeof(topic), "%s%s%s", "switch/", wmm.friendlyName, "/led1/output");
  wmm.client->subscribe(topic);
}

// optional function to process MQTT subscribed to topics coming in
void subscriptionCallback(char* topic, byte* message, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("]: ");
  String messageTemp;

  for (int i = 0; i < length; i++) {
    Serial.print((char)message[i]);
    messageTemp += (char)message[i];
  }
  Serial.println();

  //if (String(topic) == "switch/esp1234/led1/output") {
  //  Serial.print("Changing led1 output to ");
  //}
}
