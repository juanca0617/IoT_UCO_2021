/**************************************************************************************
 * Este programa funciona pormedio de un broker mqtt el cual recibe una zona horaria en 
 * el topic de input y devuelve un mensaje con la fecha y hora de la zona horaria de la 
 * entrada.
 * by: Juan Camilo Hincapie Muñoz 
 * Materia: Iot
***************************************************************************************/
//inclision de librerias
#include <Arduino.h>
#include "rest/http_request.hpp"


//constantes wifi
const char *ssid = "UNE_HFC_3D70";
const char *password = "ADB7CDFE";

void setup_wifi()
{
  delay(10);
  //se conecta WIFI a la red
  Serial.println();
  Serial.print(F("Connecting to "));
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(F("."));
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}



void setup()
{
  // se inicia conexión WIFI
  setup_wifi();
  Serial.begin(9600); 

  //Se inicia conexión mqtt 
  client.setServer(mqttServer, mqttPort);
  client.setCallback(callback);

  //validación de conexión mqtt
  while (!client.connected()) {
    Serial.println("Connecting to MQTT...");
 
    if (client.connect("ESP8266Client", mqttUser, mqttPassword )) {
 
      Serial.println("Conectado al Broker mqtt ");  
 
    } else {
 
      Serial.print("Fallo al conectarse al broker mqtt, error: ");
      Serial.print(client.state());
      delay(2000);
 
    }
  }

  //se publica la conexión exitosa en el topic alive
  client.publish(ALIVE_TOPIC, "conexion exitosa esp");
  client.subscribe(INPUT_TOPIC);
}

void loop()
{ 
  client.loop();
}