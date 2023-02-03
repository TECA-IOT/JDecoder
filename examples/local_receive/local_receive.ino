#include <Arduino.h>
#include <string.h>
#include <Ufox.h>
#include "decode_hex.h"

char txt[20];
#define btn   13
#define RXLED  17

Ufox wisol;
char bufferRx[24];        //almacena los datos recibidos en cadena de caracteres
uint8_t hexBuffer[12];    //contiene los hex ya convertidos en bytes numerico  
uint8_t id;               //id del dispositivo
void setup() {
  
  pinMode(RXLED,OUTPUT);
  
  Serial.begin(9600);
  wisol.begin(9600);
  
  while(!Serial);  //comentar si usará una fuente de energía externa

  Serial.println("Leyendo datos...");
  
}

void loop() {
  
   String rec = wisol.command("AT$RL");   
   if(rec.c_str() != ""){
    Serial.print("recibido: ");
    Serial.println(rec);
    //limpiar 
    clean_str_ufox(bufferRx, rec.c_str());                    //se limpia la cadena
    
    convertStringToHEX(hexBuffer, 12, (char*)bufferRx);
    id = GET_UINT8(hexBuffer, BYTE9);
    if(id == 0x1){
      /*****************************************************/
      //ID
      Serial.print("ID-------------->0x");
      Serial.println(GET_UINT8(hexBuffer, BYTE9),HEX);
      //TEMPERATURA
      Serial.print("TEMPERATURA----->");
      Serial.print(GET_FLOAT(hexBuffer, BYTE0));
      Serial.println(" °C");
      //HUMEDAD
      Serial.print("HUMEDAD--------->");
      Serial.print(GET_FLOAT(hexBuffer, BYTE4));
      Serial.println(" %");
      //BATERIA
      Serial.print("BATERIA--------->");
      Serial.print(GET_UINT8(hexBuffer, BYTE8),DEC);
      Serial.println(" %");
    }
   }   
}
