#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <Firebase_ESP_Client.h>

// Your network and Firebase project credentials
#define WIFI_SSID "Ahsan"
#define WIFI_PASSWORD "987654321"

#define API_KEY "AIzaSyCvYbezNy07lyVqtZL7GG9G7TuXyyE94DY"
#define DATABASE_URL "https://arduino-project-8c25b-default-rtdb.firebaseio.com/"
#define AUTH_SECRET "yj6EcShErDCpalxoRGCmBlnnG4ms1FYVurBCOZId"

// Define Firebase objects
FirebaseData fbdo;
FirebaseAuth auth;
FirebaseConfig config;

// Define the GPIO pins for the float switches
const int floatSwitch1 = D1;  // First float switch
const int floatSwitch2 = D2;  // Second float switch
const int floatSwitch3 = D3;  // Third float switch

// Variables to hold the last state of the float switches
bool lastStateSwitch1 = HIGH;
bool lastStateSwitch2 = HIGH;
bool lastStateSwitch3 = HIGH;

void setup() {
  Serial.begin(115200);

  // Initialize Wi-Fi
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting to Wi-Fi");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(1000);
  }
  Serial.println("\nConnected to Wi-Fi");

  // Initialize Firebase
  config.api_key = API_KEY;
  config.database_url = DATABASE_URL;
  config.signer.tokens.legacy_token = AUTH_SECRET;
  
  Firebase.begin(&config, &auth);

  // Set the float switch pins as input
  pinMode(floatSwitch1, INPUT_PULLUP);
  pinMode(floatSwitch2, INPUT_PULLUP);
  pinMode(floatSwitch3, INPUT_PULLUP);
}

void loop() {
  // Read the state of the float switches
  bool stateSwitch1 = digitalRead(floatSwitch1) == LOW;
  bool stateSwitch2 = digitalRead(floatSwitch2) == LOW;
  bool stateSwitch3 = digitalRead(floatSwitch3) == LOW;

  // Check if the state of any float switch has changed
  if (stateSwitch1 != lastStateSwitch1 || stateSwitch2 != lastStateSwitch2 || stateSwitch3 != lastStateSwitch3) {
    // Update the last states
    lastStateSwitch1 = stateSwitch1;
    lastStateSwitch2 = stateSwitch2;
    lastStateSwitch3 = stateSwitch3;

    // Create a JSON object
    FirebaseJson json;
    json.set("Switch1", stateSwitch1 ? "UP" : "DOWN");
    json.set("Switch2", stateSwitch2 ? "UP" : "DOWN");
    json.set("Switch3", stateSwitch3 ? "UP" : "DOWN");

    // Send the state to Firebase
    if (Firebase.RTDB.setJSON(&fbdo, "/float_switches_status", &json)) {
      Serial.println("Switch states sent successfully");
    } else {
      Serial.println("Failed to send switch states");
      Serial.println(fbdo.errorReason());
    }
  }

  // Add a delay to prevent too frequent updates
  delay(1000);
}