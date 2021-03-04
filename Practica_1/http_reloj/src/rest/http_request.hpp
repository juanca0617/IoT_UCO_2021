#include <esp8266wifi.h>
#include <esp8266httpclient.h>
#include <ArduinoJson.h>
#include "TimeLib.h"
#include <PubSubClient.h>



//variables y constantes del sistema
extern const char* mqttServer = "driver.cloudmqtt.com";
extern const int   mqttPort = 18821;
extern const char* mqttUser = "dfhsueny";
extern const char* mqttPassword = "fcd2T7Es-Pcs";
const char* INPUT_TOPIC = "input";
const char* ALIVE_TOPIC = "alive";
const char* STATUS_REQUEST = "StatusRequest";
const char* JSON_STATUS = "jsonStatus";
const char* Output = "output"; 
String url = "http://worldtimeapi.org/api/timezone/";  
HTTPClient http;
WiFiClient espClient;
PubSubClient client(espClient);


void enviarMensaje(const char* topic, char* mensaje){
      client.publish(topic, mensaje);
}

// retorna un string con el formato de la fecha
String formarFecha(){ 
  char buf[40];
  sprintf(buf, "%02d/%02d/%4d %02d:%02d:%02d", day(), month(), year(), hour(), minute(), second());
  Serial.println(buf);
  return buf;
}

/***********************************************************************
Toma el string de la peticion get lo convierte en json, luego establece 
la fecha tomando el formato unix y luego envia  mensaje al topic 
JSON_STATUS luego retorna la fecha de la peticion
************************************************************************/
String formatearFecha(String stringJson){
    //variables para y deserialcion del json
    char json[1024];
    stringJson.toCharArray(json, 1024);    
    DynamicJsonDocument doc(1024);    
    deserializeJson(doc, json);
    //toma la fecha del json en formato unix y se corrige la hora
    long fechaUnix = doc["unixtime"];
    fechaUnix -= 18000UL;
    setTime(fechaUnix);
    Serial.println("fecha Json: ");
    Serial.println(now());

    // se da formato a la fecha 
    String fechaEstablecida = formarFecha();

    //se envia mensaje de confirmacion de que todo salio bien con el Json
    enviarMensaje(JSON_STATUS, "Json OK" ); 

    return fechaEstablecida;
  
}
// Busca el indice de un caracter en una cadena
int indice_del_ultimo(char cadena[], char buscar) {
  int i;

  for (i=strlen(cadena); i>=0; i--) {
    if (cadena[i] == buscar)
      return i;
  }
  return -1; // No se ha encontrado
}

String extraerCiudad(String zonaHoraria){
    int len = zonaHoraria.length() + 1;
    char zonaHorariaFormatiada[len];
    zonaHoraria.toCharArray(zonaHorariaFormatiada,len);
    int posIgual;
    posIgual = indice_del_ultimo(zonaHorariaFormatiada, '/');
    if (posIgual == -1) {      
      return "zona horaria incorrecta";
    } else {
        String zonaHoraria = "";
         for (int i = 0; i < (len - posIgual - 1); i++) {
            zonaHoraria += zonaHorariaFormatiada[i+posIgual + 1];
        }  
        Serial.print("Ciudad Zona horaria: ");
        Serial.println(zonaHoraria);
        return zonaHoraria;
    }
}

//Se hace la peticion http a la pagina y se devuelve un string con un mensaje de error o con el mensje que se visualizara en el topic 
String httpRequest(String zonaHoraria)
{

  if (http.begin(url + zonaHoraria))
  {
    int httpCode = http.GET(); // Realizar petición get
    String fecha = "";
    if (httpCode > 0)
    {
      if (httpCode == HTTP_CODE_OK || httpCode == HTTP_CODE_MOVED_PERMANENTLY)
      {
        String mensaje = "";
        fecha = http.getString(); // Obtener respuesta
        fecha = formatearFecha(fecha);
        zonaHoraria = extraerCiudad(zonaHoraria);
        mensaje = "La fecha y hora es: " + fecha + ";  en la ciudad de " + zonaHoraria; //Generar mensaje
        enviarMensaje(STATUS_REQUEST, "zona horaria OK ");
        return mensaje;
      }
      else
      {
        enviarMensaje(STATUS_REQUEST, "Zona horaria especificada no encontrada");
        enviarMensaje(JSON_STATUS, "json fail");
        return "Error";
      }
    }
    else
    {
      enviarMensaje(STATUS_REQUEST, "No se pudo conectar al servidor"); 
      enviarMensaje(JSON_STATUS, "json fail");     
      return "Error";
    }
    http.end();
  }
  else
  {
    enviarMensaje(STATUS_REQUEST, "No se pudo iniciar el ciente http"); 
    enviarMensaje(JSON_STATUS, "json fail");
    return "Error";
  }
}


/******************************************************************************
 * recibe los mensajes de los topic y filtra el topic input para enviar la fecha 
 * al topic output
 *****************************************************************************/
void callback(char* topic, byte* payload, unsigned int length) {
  String zonaHoraria = "" ;
  //Notifica mensaje recibido
  Serial.print("Mensaje recibido el topic: ");
  Serial.println(topic);
  
  //Print mensaje recibido
  Serial.print("Mensaje:");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
    zonaHoraria += (char)payload[i];
  }  
  Serial.println();
  Serial.println("-----------------------");

  //Se filtra el mensaje del topic input y se envia el mensaje por el topic output
  if(strstr(topic,"input")){
    String fechaString = httpRequest(zonaHoraria);
    int len = fechaString.length() + 1;
    char fechaFormatiada[len];
    fechaString.toCharArray(fechaFormatiada,len);
    enviarMensaje(Output, fechaFormatiada);
  } 
 
}
