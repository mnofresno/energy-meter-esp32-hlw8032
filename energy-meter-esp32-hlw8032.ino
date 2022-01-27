//#include "HLW8032.h"
#define RXD2 16
#define TXD2 17


//HLW8032 HL;

void setup() {
   Serial.begin(115200);
   Serial.println("Goodnight moon!");

   delay(1000);
   Serial.println("Starting HLW8032");
   delay(1000);
  Serial2.begin(4800, SERIAL_8N1, RXD2, TXD2);
  //  HL.begin(Serial,4);   
}

unsigned int V;

void loop() {

    
  while (Serial2.available()) {
    Serial.print(char(Serial2.read()));
  }
}
