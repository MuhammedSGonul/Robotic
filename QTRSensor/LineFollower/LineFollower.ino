//////////////////////////////////////////////////////////////////////////
/*                       Muhammed Said Gönül          

  This algorithm was created by team "ADA" to compete in the "Design-Run" 
            category in the 2019 MEB - TEKNOFEST competitions.
                    It was renewed on 21.05.2022.

                         #MilliTeknolojiHamlesi                         */
//////////////////////////////////////////////////////////////////////////


/* LIBRARIES */
#include <QTRSensors.h>
/* LIBRARIES */


/* PIN CONFIGURATIONS */
#define leftEN 6    //  Left motor controlling PWM signal on pin 6 (ENA)
#define rightEN 5   //  Right motor controlling PWM signal on pin 5 (ENB)
#define leftF 13    //  Left motor forward pin (IN1)
#define leftB 4    //  Left motor backward pin (IN2)
#define rightF 3    //  Right motor forward pin (IN3)
#define rightB 2    //  Right motor backward pin (IN4)
/* PIN CONFIGURATIONS */


/* PID OPTIONS */
#define kp .1
#define kd 4

int error = 0, lastError = 0, rateError = 0, out = 0, position;
int setPoint = 2500;    //  The center of the sensor corresponds to the value of 2500. The distance between the two sensors is equal to 500. 
/* PID OPTIONS */


/* MOTOR OPTIONS */
int maxSpeed = 200, normalSpeed = 140, rightMotorSpeed, leftMotorSpeed;
/* MOTOR OPTIONS */


/* QTR SENSOR OPTIONS */
QTRSensors qtrrc;
#define NUM_SENSORS 6
unsigned int sensorValues[NUM_SENSORS];
/* QTR SENSOR OPTIONS */


void setup(){

  qtrrc.setTypeRC();
  qtrrc.setSensorPins((const uint8_t[]) {7, 8, 9, 10, 11, 12}, NUM_SENSORS);

  
  pinMode(leftEN,OUTPUT);
  pinMode(rightEN,OUTPUT);
  pinMode(rightF,OUTPUT);
  pinMode(rightB,OUTPUT);  
  pinMode(leftF,OUTPUT);
  pinMode(leftB,OUTPUT);


  digitalWrite(leftEN, LOW);
  digitalWrite(rightEN, LOW);

  
  for (int i = 0; i < 70; i++){       // 40 ms * 70 = 2800ms delay for calibrating sensor
    qtrrc.calibrate();
    digitalWrite(13,HIGH);
    delay(20);
    digitalWrite(13,LOW);
    delay(20);       
  }


}


void loop(){
  
  position = qtrrc.readLineBlack(sensorValues);   //  Position varies between 0-5000. For a white line, use readLineWhite()
  error = position - setPoint;  
  rateError = error - lastError;
  out = kp * error + kd * rateError;
  lastError = error;

  rightMotorSpeed = normalSpeed + out;
  leftMotorSpeed = normalSpeed - out;

  
  if (rightMotorSpeed > maxSpeed){
  rightMotorSpeed = maxSpeed; 
  }
  if (leftMotorSpeed > maxSpeed ){
    leftMotorSpeed = maxSpeed; 
  }
  if(leftMotorSpeed < -maxSpeed){
    leftMotorSpeed = -maxSpeed;
  }
  if(rightMotorSpeed < -maxSpeed){
    rightMotorSpeed = -maxSpeed;
  }

  if (rightMotorSpeed > 0 && leftMotorSpeed < 0){   //  turning left
    controlMotor(-1, -leftMotorSpeed, rightMotorSpeed);
  }
  if(rightMotorSpeed > 0 && leftMotorSpeed > 0){    //  go straight
    controlMotor(0, leftMotorSpeed, rightMotorSpeed);
  }
  if (rightMotorSpeed < 0 && leftMotorSpeed > 0){   //  turning right
    controlMotor(1, leftMotorSpeed, -rightMotorSpeed);
  } 

 
}


void controlMotor(int direction, int leftMotorPow, int rightMotorPow){
  
  if(direction == -1){ 
      digitalWrite(rightF, HIGH);
      digitalWrite(rightB, LOW);
      digitalWrite(leftF, LOW);
      digitalWrite(leftB, HIGH);  
  }
  if(direction == 0){
      digitalWrite(rightB, LOW);
      digitalWrite(rightF, HIGH);
      digitalWrite(leftB, LOW);
      digitalWrite(leftF, HIGH);
  }
  if(direction == 1){
      digitalWrite(rightF, LOW);
      digitalWrite(rightB, HIGH);
      digitalWrite(leftF, HIGH);
      digitalWrite(leftB, LOW);     
  }

    analogWrite(rightEN, rightMotorPow);
    analogWrite(leftEN, leftMotorPow);
}
