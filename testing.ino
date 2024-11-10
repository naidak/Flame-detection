#include <ESP8266WiFi.h>
#include <Firebase_ESP_Client.h>
#include "addons/TokenHelper.h"
#include "addons/RTDBHelper.h"
 

#define WIFI_SSID ""
#define WIFI_PASSWORD ""
#define API_KEY "AIzaSyAf3hUN4T0B_CSmimU0yjQ8l0ZoTiAxDvY"
#define DATABASE_URL "https://flame-detection-ba6b7-default-rtdb.europe-west1.firebasedatabase.app/"

int flameSensorPin = 5; 
int buzzerPin = 4;
int ledPin = 16; 
int ledData = 0;

FirebaseData fbdo;
FirebaseAuth auth;
FirebaseConfig config;

unsigned long sendDataPrevMillis = 0;
bool signUpOk = false;

void setup() {

  Serial.begin(115200);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting to Wi-Fi");
  while(WiFi.status()!=WL_CONNECTED){
    Serial.print(".");
    delay(300);
  }
  Serial.println();
  Serial.print("Connected with IP: ");
  Serial.println(WiFi.localIP());
  Serial.println();

  config.api_key = API_KEY;
  config.database_url = DATABASE_URL;
  if(Firebase.signUp(&config, &auth, "", "")){
    Serial.println("signUp OK");
    signUpOk = true;
  }else{
    Serial.printf("%s\n", config.signer.signupError.message.c_str());
  }
  config.token_status_callback = tokenStatusCallback;
  Firebase.begin(&config, &auth);
  Firebase.reconnectWiFi(true);
  pinMode(ledPin, OUTPUT);
  pinMode(flameSensorPin, INPUT); 
  pinMode(buzzerPin, OUTPUT);
}



void loop() {
  
  if(Firebase.ready() && signUpOk && (millis() - sendDataPrevMillis > 5000 || sendDataPrevMillis == 0))
  {
    sendDataPrevMillis = millis();
    ledData = digitalRead(ledPin);
    int flameDetected = digitalRead(flameSensorPin);
    int buzzerData = digitalRead(buzzerPin);
    Skini_iz_base();
    if(Firebase.RTDB.getBool(&fbdo, "flag"))
    {
      bool flag = fbdo.boolData();
      if(flag == false)
      {
        if (flameDetected == 1) {
        digitalWrite(buzzerPin, HIGH); 
        digitalWrite(ledPin, HIGH); 
    } else {
        digitalWrite(buzzerPin, LOW); 
        digitalWrite(ledPin, LOW); 
    }

    if(Firebase.RTDB.setInt(&fbdo, "Sensor/ledData", ledData))
    {
      Serial.println();
      Serial.println(ledData);
      Serial.print(" - successfully saved to: " + fbdo.dataPath());
      Serial.println(" (" + fbdo.dataType() + ")");
    }
    else{
      Serial.println("FAILED: " + fbdo.errorReason());
    }
  
    if(Firebase.RTDB.setInt(&fbdo, "Sensor/buzzerData", buzzerData))
    {
      Serial.println();
      Serial.println(buzzerData);
      Serial.print(" - successfully saved to: " + fbdo.dataPath());
      Serial.println(" (" + fbdo.dataType() + ")");
    }
    else{
      Serial.println("FAILED: " + fbdo.errorReason());
    }

    if(Firebase.RTDB.setInt(&fbdo, "Sensor/flameData", flameDetected))
    {
      Serial.println();
      Serial.println(flameDetected);
      Serial.print(" - successfully saved to: " + fbdo.dataPath());
      Serial.println(" (" + fbdo.dataType() + ")");
    }
    else{
      Serial.println("FAILED: " + fbdo.errorReason());
    }

    }else{
       digitalWrite(buzzerPin, LOW); 
        digitalWrite(ledPin, LOW); 
      flameDetected = 0;
      ledData = 0;
      buzzerData = 0;
          if(Firebase.RTDB.setInt(&fbdo, "Sensor/ledData", ledData))
    {
      Serial.println();
      Serial.println(ledData);
      Serial.print(" - successfully saved to: " + fbdo.dataPath());
      Serial.println(" (" + fbdo.dataType() + ")");
    }
    else{
      Serial.println("FAILED: " + fbdo.errorReason());
    }
  
    if(Firebase.RTDB.setInt(&fbdo, "Sensor/buzzerData", buzzerData))
    {
      Serial.println();
      Serial.println(buzzerData);
      Serial.print(" - successfully saved to: " + fbdo.dataPath());
      Serial.println(" (" + fbdo.dataType() + ")");
    }
    else{
      Serial.println("FAILED: " + fbdo.errorReason());
    }

    if(Firebase.RTDB.setInt(&fbdo, "Sensor/flameData", flameDetected))
    {
      Serial.println();
      Serial.println(flameDetected);
      Serial.print(" - successfully saved to: " + fbdo.dataPath());
      Serial.println(" (" + fbdo.dataType() + ")");
    }
    else{
      Serial.println("FAILED: " + fbdo.errorReason());
    }
    }}
    
  }
}