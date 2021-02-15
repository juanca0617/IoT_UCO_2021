/* 
Codigo para el control de tres leds de manera local por medio de una pagina web
---------------------------------------------------------------------------------
By: Juan Camilo Hincapie Muñoz
Materia: IOT
*/


#include <Arduino.h>
#include <ESP8266WiFi.h>
//Reemplazar con la conexion wifi
const char* ssid     = "*********";
const char* password = "*********";

//pines GPIO a los que estan conectados los leds
int ledPin1 = 5; 
int ledPin2 = 4;
int ledPin3 = 0;

// variables para saber el estado actual de los leds
int value1 = LOW;
int value2 = LOW;
int value3 = LOW;
//se declara servidor por el puerto 80
WiFiServer server(80);
 
void setup() {
  // se inicia puerto serial
  Serial.begin(115200);
  delay(10);

 // Se declaran los puertos GPIO como salidas y se inician apagadas
  pinMode(ledPin1, OUTPUT);
  digitalWrite(ledPin1, LOW);
  pinMode(ledPin2, OUTPUT);
  digitalWrite(ledPin2, LOW);
  pinMode(ledPin3, OUTPUT);
  digitalWrite(ledPin3, LOW);
 
  // conexion a wifi
  Serial.println();
  Serial.println();
  Serial.print("Conectando a red wifi:");
  Serial.println(ssid);
 
  WiFi.begin(ssid, password);
 
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi conectado");
 
  // inicio del servidor
  server.begin();
  Serial.println("Servidor iniciado");
 
 // muestra direccion ip para conexion por web local
  Serial.print("usar esta URL para conectarse: ");
  Serial.print("http://");
  Serial.print(WiFi.localIP());
  Serial.println("/");
 
}
 
void loop() {
  // verifica si se ha conectado un cliente
  WiFiClient client = server.available();
  if (!client) {
    return;
  }

  // espera si el cliente envia datos
  Serial.println("Nuevo cliente");
  while(!client.available()){
    delay(1);
  }
 
  // se lee la peticion
  String request = client.readStringUntil('\r');
  Serial.println(request);
  client.flush();
 
  // lectura de peticion y logica de encendido o apagado de leds
 //pin1
  if (request.indexOf("/LED1=ON") != -1)  {
    digitalWrite(ledPin1, HIGH);
    value1 = HIGH;
  }
  if (request.indexOf("/LED1=OFF") != -1)  {
    digitalWrite(ledPin1, LOW);
    value1 = LOW;
  }
  
  //pin 2  
  if (request.indexOf("/LED2=ON") != -1)  {
    digitalWrite(ledPin2, HIGH);
    value2 = HIGH;
  }
  if (request.indexOf("/LED2=OFF") != -1)  {
    digitalWrite(ledPin2, LOW);
    value2 = LOW;
  }

  //pin3  
  if (request.indexOf("/LED3=ON") != -1)  {
    digitalWrite(ledPin3, HIGH);
    value3 = HIGH;
  }
  if (request.indexOf("/LED3=OFF") != -1)  {
    digitalWrite(ledPin3, LOW);
    value3 = LOW;
  }
 
 
  // Retorno del response en html
  client.println("HTTP/1.1 200 OK");
  client.println("Content-Type: text/html");
  client.println(""); 
  client.println("<!DOCTYPE HTML>");
  client.println("<html>");
  client.println("<body  background=\" https://www.akamai.com/es/es/multimedia/images/custom/2019/iot/edge-cloud-solutions-hero.jpg?imwidth=1366\" \">");
  client.print("<center><h1 style=\"color:white\">Leds IOT:</h1>");
   

  client.println("<br><br>");
  //Botones de encendido y apagado
  client.println("<style>.btnRed{ padding:6px;border-radius:10px; background-color:#D53D2D; color:white; height:60px; width:300px; font-size:16px}</style>");
  client.println("<style>.btnYellow{ padding:6px;border-radius:10px; background-color:#CEDF1A; color:white; height:60px; width:300px; font-size:16px}</style>");
  client.println("<style>.btnGreen{ padding:6px;border-radius:10px; background-color:#3CD620; color:white; height:60px; width:300px; font-size:16px}</style>");
  client.println("<style>.box{background-color:#949A93s} </style>");
    
  client.println("<a href=\"/LED1=ON\"\"><button class=\"btnRed\">Encender</button></a> <a href=\"/LED2=ON\"\"><button class=\"btnYellow\">Encender</button></a> <a href=\"/LED3=ON\"\"><button class=\"btnGreen\">Encender</button></a><br>");
  client.println("<a href=\"/LED1=OFF\"\"><button class=\"btnRed\" >Apagar</button></a> <a href=\"/LED2=OFF\"\"><button class=\"btnYellow\" >Apagar</button></a> <a href=\"/LED3=OFF\"\"><button class=\"btnGreen\" >Apagar</button></a> <br>"); 
  client.println("<br><br>");

  // Imagenes de los Leds cambian segun su estado
  
  if(value1 == HIGH) {
    client.println("<img  width=\"100\" height=\"100\" src=\"http://www.clker.com/cliparts/M/h/R/9/8/H/red-led-on-md.png\">");
  } else {
    client.println("<img width=\"100\" height=\"100\"  src=\"https://www.clker.com/cliparts/X/A/P/f/5/n/red-led-off-md.png\">");
  }

  if(value2 == HIGH) {
     client.println("<img width=\"100\" height=\"100\"  src=\"http://www.clker.com/cliparts/d/K/2/s/U/e/led-yellow-circle-md.png\">");
  } else {
      client.println("<img width=\"100\" height=\"100\"  src=\"https://www.vippng.com/png/full/450-4501261_small-yellow-led-on-off.png\">");
  }

  if(value3 == HIGH) {
    client.println("<img width=\"100\" height=\"100\"  src=\"http://www.clker.com/cliparts/z/r/p/I/x/a/green-led-on-md.png\">");
  } else {
    client.println("<img width=\"100\" height=\"100\"  src=\"http://www.clker.com/cliparts/6/l/f/V/f/H/green-led-off-md.png\">");
  }
  

  client.println("</center>");
  client.println("</body>");
  client.println("</html>");
  
 
  delay(1);
  Serial.println("Cliente desconectado");
  Serial.println("");
 
}