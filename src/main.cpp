#include <WiFi.h>
#include <WiFiAP.h>
#include <WiFiMulti.h>
#include <WiFiUdp.h>
#include <WiFiScan.h>
#include <ETH.h>
#include <WiFiClient.h>
#include <WiFiSTA.h>
#include <WiFiServer.h>
#include <WiFiType.h>
#include <WiFiGeneric.h>
#include <stdint.h>
#include <Arduino.h>
#include <HTTPClient.h>
#include <AutoConnect.h>
#include <WebServer.h>
#include <Wire.h>
#include "rgb_lcd.h"
#include <ArduinoJson.h>

AutoConnect Portal;
AutoConnectConfig config;

rgb_lcd lcd;

const int colorR = 255;
const int colorG = 255;
const int colorB = 255;

String inputString = "";
HTTPClient http, http2;
boolean wifiConnectionStatus = false;

boolean connectToWIFI(char* ssid, char* password);
String createUserMove(String fromMove, String toMove);
String removeLastChar(String input);

void setup() {
  Serial.begin(9600);
  delay(1000);

  config.apid = "RollingPawns"; 
  config.psk = "rollingpawnadmin";  

  Portal.config(config);  

  if (Portal.begin()) { 
    Serial.println("HTTP server:" + WiFi.localIP().toString()); 
  }
  config.apip = WiFi.localIP();
  Portal.config(config); 
  if (Portal.begin()) { 
    Serial.println("Setting current IP"); 
  }  

    lcd.begin(16, 2);
    lcd.setRGB(colorR, colorG, colorB);
    lcd.print("Let's Play Chess");

    delay(1000);

}

void loop() {
  Portal.handleClient();
  if ((WiFi.status() == WL_CONNECTED)) {
      
      Serial.println("I got Connected");
      String moveCombination[2];
      String userMove = "";
      int count = 0;
      String chessEngineMoves = "";
      
      do{
        if (Serial.available() > 0) {
          inputString = removeLastChar(Serial.readStringUntil('\n'));

          if(moveCombination[count - 1] != inputString){
            moveCombination[count]=inputString;
            count++;
          }
        } 

      } while(count < 2);
      userMove = createUserMove(moveCombination[0], moveCombination[1]);

      Serial.println("Your move is:" + userMove);

      Serial.println("********************************************");
      Serial.println("Sending move to UI");
      Serial.println("********************************************");

      http.begin("http://192.168.1.8:3000/move"); // This was JD's local machine. We need to host it on heroku
      http.addHeader("Content-Type", "application/x-www-form-urlencoded");

      String payload_ui = "from=" + (moveCombination[0]) + "&to=" + (moveCombination[1]);
      int httpResponseCode_ui = http.POST(payload_ui);
      String response_ui = http.getString();
      if(httpResponseCode_ui>0){

        Serial.println("*******************   SENT MOVE TO UI     **********************");
        Serial.println(response_ui); 
        Serial.println("******************************************************************");
    
      }else{
    
        Serial.println("ERROR SENDING REQUEST TO UI");
        Serial.println(response_ui); 
      }

      http.end();
    

      Serial.println("********************************************");
      Serial.println("Asking chess engine for it's move . . . .");
      Serial.println("********************************************");

      http.begin("http://square-off.herokuapp.com/makeMove");
      http.addHeader("Content-Type", "application/x-www-form-urlencoded");

      String payload = "from=" + (moveCombination[0]) + "&to=" + (moveCombination[1]) + "&gameId=5e6e92132c353f001465fe834";
      Serial.println(payload);
      int httpResponseCode = http.POST(payload);
      DynamicJsonDocument doc(2048);
      deserializeJson(doc, http.getStream());
      if(httpResponseCode>0){

        Serial.println("*******************   CHESS ENGINE MOVE     **********************");
        // Serial.println(response); 
        String ai_response = "FROM=" + doc["aiMove"]["from"].as<String>() + " TO=" + doc["aiMove"]["to"].as<String>();
        Serial.println(ai_response);
        lcd.clear();
        lcd.print("Engine Move:");
        lcd.setCursor(0, 1);
        lcd.print(ai_response);
        delay(100);
        Serial.println("******************************************************************");
    
      }else{
        Serial.println("ERROR SENDING REQUEST");
      }
      
      http.end();    
      Serial.flush();
      Serial.println("READY FOR YOUR NEXT MOVE . . .");
      }

}

//Create user move
String createUserMove(String fromMove, String toMove){
  String userMove = "";
  userMove = fromMove + toMove;
  return userMove;
}

//Remove new line from a String
String removeLastChar(String input){
      unsigned int stringLength = input.length();
      input.remove( stringLength - 1 );
      return input;
}