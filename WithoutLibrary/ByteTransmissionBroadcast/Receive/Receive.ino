//////////////////////////////////////////////////////////////////////////
/*          Muhammed Said Gönül
   
    Tested on EBYTE - E32 868T20D SX1276 module.
   
    LoRa Pinouts    /   Arduino Pinouts
 *      GND     ------->      GND      
 *      VCC     ------->      5V
 *      AUX     ------->      
 *      TXD     ------->      D3
 *      RXD     ------->      D2
 *      M1      ------->      D4
 *      M0      ------->      D5

  Notice: If you are testing in Arduino MEGA and not working, 
  try this pin configuration : RXD ----> D10, TXD ----> D11.
 
 */
//////////////////////////////////////////////////////////////////////////


#include <SoftwareSerial.h>


#define PIN_RX 3    
#define PIN_TX 2
#define PIN_M0 4
#define PIN_M1 5




uint8_t Data;
uint16_t timeoutPeriod = 2200;
SoftwareSerial ESerial(PIN_RX, PIN_TX);

unsigned long arrTime = 0, lagTime = 0;


void setup() {
  pinMode(PIN_M0, OUTPUT);
  pinMode(PIN_M1, OUTPUT);

  Serial.begin(9600);
  ESerial.begin(9600);

  
  Serial.println("Starting reader...");
  delay(1500);
}

void loop() {


  if (ESerial.available()) {
    ESerial.readBytes((uint8_t*)&Data, (uint8_t) sizeof(Data));
    lagTime = millis() - arrTime;
    arrTime = millis();

    Serial.print("Arriving data : ");
    Serial.print(Data);
    Serial.print("  Lag Time : ");
    Serial.println(lagTime);

  }
  else {
    if (millis() - arrTime >= timeoutPeriod) {
      Serial.print("Timeout -- Values not arrive.  Timeout : ");
      Serial.print(timeoutPeriod);
      Serial.println("  miliseconds");
      delay(5000);
    }

  }


}
