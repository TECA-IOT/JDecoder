#include <Arduino.h>
#include <string.h>
#include <Ufox.h>
#include "decode_hex.h"


#define btn   13
#define RXLED  17

Ufox wisol;
char bufferRx[24];
uint8_t hexBuffer[12];
Bits_t data;


void setup() {
  
  pinMode(RXLED,OUTPUT);
  
  Serial.begin(115200);
  wisol.begin(9600);
  
  while(!Serial);  //comentar si usará una fuente de energía externa

  Serial.println("CONFIGURACION TERMINADA");
  Serial.println("LEENDO ID PAC");
  for(int i = 0; i<2; i++){
    String rec = wisol.command("AT$RL");    
    Serial.print("recibido: ");
    Serial.println(rec);
  }
  
}

void loop() {
  
   String rec = wisol.command("AT$RL");   
   if(rec.c_str() != ""){
    Serial.print("recibido: ");
    Serial.println(rec);
    //limpiar 
    clean_str_ufox(bufferRx, rec.c_str());                    //se limpia la cadena
    Serial.print("RX:");
    Serial.println(bufferRx);
    convertStringToHEX(hexBuffer, 12, (char*)bufferRx);
    /*****************************************************/
    Serial.print("ID---------->0x");
    Serial.println(GET_UINT32(hexBuffer, BYTE8),HEX);
    Serial.print("PULSOSA----->");
    Serial.println(GET_UINT16(hexBuffer, BYTE0));

    Serial.print("PULSOS------>");
    Serial.println(GET_UINT32(hexBuffer, BYTE2));


    Serial.print("BYTE-------->");
    Serial.println(GET_UINT8(hexBuffer, BYTE6),HEX);
    bits_value(&data, GET_UINT8(hexBuffer, BYTE6));
    Serial.print("REBOT------->");
   
    Serial.println(data.b7);
    Serial.print("REFLASH----->");
    Serial.println(data.b6);
    Serial.print("FRAUDE------>");
    Serial.println(data.b5);
    Serial.print("STATUS_CTRL->");
    Serial.println(data.b4);
    Serial.print("S_ENERGIA--->");
    Serial.println(data.b3);
    Serial.print("DNLK-------->");
    Serial.println(data.b2);
    Serial.print("UPDATE------>");
    Serial.println(data.b1);
    Serial.print("CONFIG------>");
    Serial.println(GET_UINT8(hexBuffer, BYTE7),HEX);
    
   }   
}
