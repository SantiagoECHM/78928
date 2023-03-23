#include <WiFi.h>
#include <WebServer.h>
#include <Arduino.h>
#include <ESPAsyncWebSrv.h>
#include <Adafruit_Sensor.h>
#include <DHT.h>
#include "DHTesp.h"
DHT dht(15, DHT11);

const char* ssid = "Alpha";
const char* password = "caracoles123";

AsyncWebServer server(80);

String webpage = "<!DOCTYPE html><html><head><title>Page Title</title></head><body style='background-color: #EEEEEE;'><span style='color: #003366;'><h1>This is a Heading</h1><p>This is a paragraph.</p></span></body></html>";

void setup(){
  Serial.begin(115200);
  conectarse();

  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    int numParametros = request->params();
    Serial.print(numParametros);
    Serial.println("Estado\tHumedad (%)\tTemperatura (C)\t(F)\tIndiceCalor (C)\t(F)");
    if(numParametros==0){
      request->send(200,"text/html", webpage);
    } else{
      AsyncWebParameter *p = request->getParam(0);
      String html="<H1>hola" + p->value()+" desde ESP32</H1>";
      request->send(200, "text/html", html);            
    }
  });
  server.on("/temperatura", HTTP_GET, [](AsyncWebServerRequest *request){
    String html1 = "<h1>La temperatura actual es: </h1>"+getTemperatura();
    request->send(200,"text/html", html1);    
  });

  server.on("/humedad", HTTP_GET, [](AsyncWebServerRequest *request){
    String html2 = "<h1>La húmedad actual es: </h1>"+getHumedad();
    request->send(200,"text/html", html2);    
  });

  server.on("/fharenheit", HTTP_GET, [](AsyncWebServerRequest *request){
    String html3 = "<h1>La temperatura en grados fharenhit actual es: </h1>"+getFharenheit();
    request->send(200,"text/html", html3);    
  });

  server.on("/calorc", HTTP_GET, [](AsyncWebServerRequest *request){
    String html4 = "<h1>Calor en grados celcius actual es: </h1>"+getCalorc();
    request->send(200,"text/html", html4);    
  });

  server.on("/calorf", HTTP_GET, [](AsyncWebServerRequest *request){
    String html5 = "<h1>Calor en grados farenhit actual es: </h1>"+getCalorf();
    request->send(200,"text/html", html5);    
  });

  server.begin();
}

void loop(){
  float humedad =  dht.readHumidity();
  float temp = dht.readTemperature();    
  float f = dht.readTemperature(true);   

  if (isnan(humedad) || isnan(temp) || isnan(f)) {
    Serial.println(F("Failed to read from DHT sensor!"));
    return;
  }

  float hif = dht.computeHeatIndex(f, humedad);     
  float hic = dht.computeHeatIndex(temp, humedad, false);                      
  Serial.print("\t");
  Serial.print(humedad);
  Serial.print("\t\t");
  Serial.print(temp);
  Serial.print("\t\t");
  Serial.print(f);
  Serial.print("\t\t");
  Serial.print(hic);
  Serial.print("\t\t");
  Serial.println(hif);
  delay(2000);
}

void conectarse(){
  WiFi.begin(ssid, password);
  Serial.println("Estableciendo conexión con SSID "+String(ssid));  

  while(WiFi.status() != WL_CONNECTED){
    delay(1000);
    Serial.println(".");
  }  
  Serial.print("Conectado a la red con dirección ip: ");
  Serial.print(WiFi.localIP());
}

String getHumedad(){
  float humedad = dht.readHumidity();
  String hum ="";
  hum.concat(humedad);
  return hum;
}

String getTemperatura(){
  float temp = dht.readTemperature();
  String tem = "";
  tem.concat(temp);
  return tem;  
}

String getFharenheit(){
  float f = dht.readTemperature(true);
  String f1 = "";
  f1.concat(f);
  return f1;
}

String getCalorc(){
  float temp = dht.readTemperature();
  float humedad = dht.readHumidity();
  float hic = dht.computeHeatIndex(temp, humedad, false);
  String calorc="";
  calorc.concat(hic);
  return calorc;
}

String getCalorf(){
  float humedad = dht.readHumidity();
  float f = dht.readTemperature(true);
  float hif = dht.computeHeatIndex(f, humedad);
  String calorf = "";
  calorf.concat(calorf);  
  return calorf;  
}

