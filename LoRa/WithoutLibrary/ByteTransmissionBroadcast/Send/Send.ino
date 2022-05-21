//////////////////////////////////////////////////////////////////////////
/*          Muhammed Said Gönül
   
    Tested on EBYTE - E32 868T20D SX1276 module.
   
    LoRa Pinouts    /   Arduino Pinouts
 *      GND     ------->      GND      
 *      VCC     ------->      5V
 *      AUX     ------->      
 *      TXD     ------->      D2
 *      RXD     ------->      D3
 *      M1      ------->      D4
 *      M0      ------->      D5
  Notice: If you are testing in Arduino MEGA and not working, 
  try this pin configuration : RXD ----> D10, TXD ----> D11.
 
 */
//////////////////////////////////////////////////////////////////////////

#include <SoftwareSerial.h>

#define PIN_RX 2
#define PIN_TX 3
#define PIN_M0 4
#define PIN_M1 5


uint8_t Data;
SoftwareSerial ESerial(PIN_RX, PIN_TX);



void setup() { 
  pinMode(PIN_AX, INPUT);
  pinMode(PIN_M0, OUTPUT);
  pinMode(PIN_M1, OUTPUT);


  Serial.begin(9600);
  ESerial.begin(9600);
  Serial.println("Starting sender...");
  delay(1500);

}

void loop() {
  Data++;
  ESerial.write((uint8_t*) &Data, sizeof(Data));
  Serial.print("Sending: "); Serial.println(Data);

  delay(160);   //  It transmits at a frequency of 6.25 Hz.

}
