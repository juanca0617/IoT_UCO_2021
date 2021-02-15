/* 
Codigo para encender 3 leds por un segundo
---------------------------------------------------------------------------------
By: Juan Camilo Hincapie Muñoz
Materia: IOT
*/
#include <Arduino.h>
#define LED_1 5
#define LED_2 4
#define LED_3 0

void setup() {
  //se inician los puertos GPIO como salidas.
  pinMode(LED_1, OUTPUT);
  pinMode(LED_2, OUTPUT);
  pinMode(LED_3, OUTPUT);
}


void loop() {

  // se enciende cada led por un segundo y luego se apaga
  digitalWrite(LED_1, HIGH);   
  delay(1000);                       
  digitalWrite(LED_1, LOW);    
  delay(1000);                 

  digitalWrite(LED_2, HIGH);   
  delay(1000);                  
  digitalWrite(LED_2, LOW);    
  delay(1000);  


  digitalWrite(LED_3, HIGH);   
  delay(1000);                       
  digitalWrite(LED_3, LOW);    
  delay(1000);  
}