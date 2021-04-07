 
#include <Servo.h>
Servo myServoA; // create instance of Servo named myServoA
Servo myServoB; // create instance of Servo named myServoB
Servo myServoC; // create instance of Servo named myServoC

int servoSpeed = 120; // 0 = CCW full speed | 90 = zero speed | 180 = CW full speed
unsigned long rotationTime = 3.3 * 1000; // how long servos run per cycle

const byte servoPinA = 11;
const byte servoPinB = 10;
const byte servoPinC = 9;


void setup() {

  myServoA.attach(servoPinA); // correlate instance of Servo with pin
  myServoB.attach(servoPinB); // correlate instance of Servo with pin
  myServoC.attach(servoPinC); // correlate instance of Servo with pin

}

void loop() {

  myServoA.write(servoSpeed); // set servo to servoSpeed
  myServoB.write(servoSpeed); // set servo to servoSpeed
  myServoC.write(servoSpeed); // set servo to servoSpeed

  delay(rotationTime);

  myServoA.write(90); // set servo to "off"
  myServoB.write(90); // set servo to "off"
  myServoC.write(90); // set servo to "off"

  delay(5000); //delay for 5 sec while servos are "off"

}
