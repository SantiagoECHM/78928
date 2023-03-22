#include <Arduino.h>
#ifdef ESP32
#include <WiFi.h>
#include <AsyncTCP.h>
#elif defined(ESP8266)
#include <ESP8266WiFi.h>
#include <ESPAsyncTCP.h>
#endif
#include <ESPAsyncWebSrv.h>

#include <WiFi.h>

const char *ssid="Alpha";
const char *password="caracoles123";
AsyncWebServer server(80);

void setup(){
  Serial.begin(115200);
  conectarse();
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    int numParametros = request->params();
    Serial.print(numParametros);
    if(numParametros==0){
      request->send(200,"text/html", "<H1>Hola mundo</H1>");
    } else{
      AsyncWebParameter *p = request->getParam(0);
      String html="<H1>hola" + p->value()+" desde ESP32</H1>";
      request->send(200, "text/html", html);            
    }
  });
    
  server.on("/adios", HTTP_GET,[](AsyncWebServerRequest *r){
    r->send(200, "text/html", "<h1>adios</h1>");        
 });
 server.begin();
}   

void loop(){}
  void conectarse(){
      Serial.print("conectando "); Serial.println(ssid);
      WiFi.begin(ssid, password);
      while (WiFi.status()!=WL_CONNECTED)  {
        delay(500); 
        Serial.print(".");                
      } 
      Serial.println(WiFi.localIP());       
}