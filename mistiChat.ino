//A small chat server for the esp8266!
//For PlatformIO
//#include <Arduino.h>
#include <ESP8266WiFi.h>
//How many clients should I handle?
#define Client_Limit 10
#define ServerPort 23
//Wifi network configuration
#define SSID "*************"
#define PASS "*************"
//So the esp wont be waiting for an eternity if your wifi shuts down
#define timeOut 13
#define bufferSize 40
//The server initialization
WiFiServer server(ServerPort);
WiFiClient chatUsers[Client_Limit];
//This just stores the last msg sended
String usr[Client_Limit];
void ClientMsg(int data);
void setup() {
    Serial.begin(115200);
    Serial.print("\nConnecting to Wifi network: "); Serial.print(SSID); //Show me the WiFi network you're connecting to
    Serial.print("\n[");//Begin of the progress bar
    //Connect to WiFi
    WiFi.mode(WIFI_STA);
    WiFi.begin(SSID, PASS);
    short i = 0;
    //Progress bar
    while(WiFi.status() != WL_CONNECTED && i <= timeOut * 10){
      delay(100);
      i++;
      Serial.print("=");
    }
    Serial.print("]\nConnected!\nIP: ");//Progress bar end and show the WiFi's IP
    Serial.print(WiFi.localIP());//______/
    Serial.println("\nWaiting for connection: ");
}

void loop(){
    uint8_t i;
    Serial.print("."); //Just to show nobody is around
    if(server.hasClient()){
      for(i =0; i < Client_Limit; i++){
        if(!chatUsers[i] || !chatUsers[i].connected()){
          if(chatUsers[i]) chatUsers[i].stop();
          chatUsers[i] = server.available();
          continue;
        }
      }
      WiFiClient chatUser = server.available();
      chatUser.stop();
    }
    for(uint8_t i; i < Client_Limit; i++){
      if(chatUsers[i] && chatUsers[i].connected()){
        if(chatUsers[i].available()){
          while(chatUsers[i].available()){
            ClientMsg(chatUsers[i].read());
          }
        }
      }
    }
    delay(50);
}
void ClientMsg(int data){
  for(short i = 0; i < Client_Limit; i++){
      if (chatUsers[i] && chatUsers[i].connected()){
        chatUsers[i].write(data);
        chatUsers[i].write("\n");
        delay(1);
      }
    }
}
