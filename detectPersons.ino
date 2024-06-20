#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include <Seeed_Arduino_SSCMA.h>
#include <arduino_secrets.h>

//change to your ssid and password
const char* ssid = SECRET_SSID;
const char* password = SECRET_PASS;

//change to your token and chatid
const String botToken = SECRET_BOTT;
const String chatId = SECRET_CHNL;

SSCMA AI;

void setup()
{
    AI.begin();
    Serial.begin(9600);
    WiFi.begin(ssid, password); 
    while (WiFi.status() != WL_CONNECTED) {
      delay(1000);
      Serial.println("Connecting to WiFi...");
    }
}

void loop()
{
    if (!AI.invoke()){
      for (int i = 0; i < AI.boxes().size(); i++){
          //Get the model inference
          if(AI.boxes()[i].score>70){
            Serial.println("People detected");
            //Call the function to send the monitering result to Telegram
            if(WiFi.status() == WL_CONNECTED) {
              sendMessage("People Detected");
              delay(1000);
            }
          }
      }
    }
}

//The function that access Telegram and send some message
void sendMessage(String text) {
  String url = "https://api.telegram.org/bot" + botToken + "/sendMessage";
  String payload = "{\"chat_id\":\"" + chatId + "\",\"text\":\"" + text + "\"}";
  
  HTTPClient http;
  http.begin(url);
  http.addHeader("Content-Type", "application/json");
  
  int statusCode = http.POST(payload);
  if (statusCode == 200) {
    Serial.println("Message sent successfully!");
  } else {
    Serial.println("Failed to send message.");
  }
  http.end();
}