#include "DHTesp.h"

 

#define DHTpin 15

DHTesp dht;

 
#include <Arduino.h>
#ifdef ESP32
#include <WiFi.h>
#include <AsyncTCP.h>
#elif defined(ESP8266)
#include <ESP8266WiFi.h>
#include <ESPAsyncTCP.h>
#endif
#include <ESPAsyncWebSrv.h>

const char* ssid = "Alpha";
const char* password = "caracoles123";
AsyncWebServer server (80);

  float humedad =  dht.getHumidity();

  float temperatura = dht.getTemperature();                            


void setup(){

  Serial.begin(115200);

  conectarse();

  Serial.println();

  Serial.println("Estado\tHumedad (%)\tTemperatura (C)\t(F)\tIndiceCalor (C)\t(F)");

  dht.setup(DHTpin, DHTesp::DHT11);
  server.on("/temperatura", HTTP_GET, [](AsyncWebServerRequest *request){
     Serial.print("\t\t");
      Serial.print(temperatura, 1);
    request -> send (200, "text/html", "<H1> adios </H1>");
  });
  
  server.begin();

}

void loop()
{

  delay(dht.getMinimumSamplingPeriod());
 

  if (isnan(humedad) || isnan(temperatura)) {

    Serial.println("No se pudo leer sensor DHT!");

    return;

  }

  Serial.print( dht.getStatusString() );

  Serial.print("\t");

  Serial.print(humedad, 1);

  Serial.print("\t\t");


  Serial.print( dht.toFahrenheit(temperatura), 1);

  Serial.print("\t\t");

  Serial.print( dht.computeHeatIndex(temperatura, humedad, false), 1);

  Serial.print("\t\t");

  Serial.println(dht.computeHeatIndex(dht.toFahrenheit(temperatura), humedad, true), 1);

}

void conectarse(){
  Serial.print("conectando");
    Serial.println(ssid);
    WiFi.begin(ssid,password);

    while(WiFi.status() != WL_CONNECTED){
      delay(500);
      Serial.print(".");
    }

    Serial.print(WiFi.localIP());
}