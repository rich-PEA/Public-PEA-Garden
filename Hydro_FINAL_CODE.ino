#define BLYNK_PRINT Serial
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <Servo.h>

// BLYNK STUFF //
char auth[] = "4KGHmPHdFEAIo2VPFwp-M5a9vba0xstT";
char ssid[] = "Schanda NET";
char pass[] = "35Stephanie";
// END BLYNK STUFF //

Servo myServoA;
Servo myServoB;
Servo myServoC;

// Adjustable variables to get from Blynk App, but declare them here
int servoSpeed = 130; // 0=CCW full speed, 90 = zero speed, 180 = CW full speed
unsigned long rotationTime = 3.3 * 1000; // how long servos run per cycle
unsigned long pumpOnTime = 15 * 60 * 1000; // how long pump runs
unsigned long pumpOffTime = 15 * 60 * 1000; //* 60 * 1000; // "rest" time

//Misc variables
unsigned long previousTime = 0;
String pumpState = "Off";

//Liquid level variables //
int rawLiquidReading;
float liquidLevel;
const long lowEtapeValue = 590;
const long highEtapeValue = 740;

// Pins Per the ESP8266 NodeMCU
const byte servoPinA = 14;
const byte servoPinB = 12;
const byte servoPinC = 13;
const byte liquidLevelPin = A0; //provides voltage between 0-1024, higher number = more resistance = more liquid
const byte relayPin = 15; //when relay turns on, 12v pump runs

void setup() {

  pinMode(liquidLevelPin, INPUT);
  pinMode(relayPin, OUTPUT);

  Serial.begin(9600);

  Blynk.begin(auth, ssid, pass);

}

void loop() {

  Blynk.run();

  unsigned long currentTime = millis();
  static unsigned long previousTime = currentTime; //previousTime = currentTime first time through loop, then ignored
  static byte state = 1; //assigns state = 1 first time through loop, then ignored

  /* There are 3 states:
      1 - ROTATE THEN STOP
      2 - PUMP RUN THEN OFF
      3 - TAKE LIQUID MEASUREMENT THEN "REST"
  */

  /**** BEGINNING STATE 1 - ROTATE THEN STOP****/

  if ((currentTime - previousTime >= rotationTime)  && (state == 1)) { // do once rotation time is exceeded
    servoStopThenDetach(); // turn off servos
    state = 2;
    previousTime = currentTime;
  }

  else if ( (state == 1) && (!myServoA.attached()) ) { // if servos aren't already attached, attach and activate them
    servoAttachThenRotate();
  }

  /**** END STATE 1 ****/

  /**** BEGINNING STATE 2 - PUMP RUN THEN OFF ****/

  if ((currentTime - previousTime >= pumpOnTime)  && (state == 2)) {
    digitalWrite(relayPin, LOW); // Pump Off
    pumpState = "Off";
    state = 3;
    previousTime = currentTime;
  }

  else if ( (state == 2) && (pumpState = "Off")) {
    digitalWrite(relayPin, HIGH); // Pump On
    pumpState = "On";
  }

  /**** END STATE 2 ****/
  /**** BEGINNING STATE 3 - TAKE LIQUID MEASUREMENT THEN "REST" ****/

  if ((currentTime - previousTime >= pumpOffTime)  && (state == 3)) {

    rawLiquidReading = analogRead(liquidLevelPin);
    Serial.print("Raw Liquid Level: ");
    Serial.print(rawLiquidReading);

    liquidLevel = map(rawLiquidReading, lowEtapeValue, highEtapeValue, 0, 100);
    Serial.print(" / Liquid Remaining: ");
    Serial.print(liquidLevel);
    Serial.println("%.");

    Blynk.virtualWrite(V5, rawLiquidReading); //Push liquid level data to V5 in App
    Blynk.virtualWrite(V6, liquidLevel); //Push liquid level data to V6 in App

    state = 1;

    previousTime = currentTime;

  }
  /**** END STATE 3 ****/


} // end void loop

////////////////////////////////////// Functions //////////////////////////////////////

void servoStopThenDetach () {

  myServoA.write(90);
  myServoB.write(90);
  myServoC.write(90);
  myServoA.detach();
  myServoB.detach();
  myServoC.detach();

}

void servoAttachThenRotate () {

  myServoA.attach(servoPinA);
  myServoB.attach(servoPinB);
  myServoC.attach(servoPinC);
  myServoA.write(servoSpeed);
  myServoB.write(servoSpeed);
  myServoC.write(servoSpeed);

}


// This function will be called every time Slider Widget in Blynk app writes values to the Virtual Pin 1
BLYNK_WRITE(V1)
{
  servoSpeed = param.asInt(); // assigning incoming value from pin V1 to a variable
  // You can also use:
  Serial.print("Servo Speed is now: ");
  Serial.println(servoSpeed);
}

// This function will be called every time Slider Widget in Blynk app writes values to the Virtual Pin 2
BLYNK_WRITE(V2)
{
  rotationTime = 1000 * param.asFloat(); // assigning incoming value from pin V2 to a variable, then convert to MS
  // You can also use:
  Serial.print("Rotation time is now: ");
  Serial.println(rotationTime);
}

// This function will be called every time Slider Widget in Blynk app writes values to the Virtual Pin 3
BLYNK_WRITE(V3)
{
  pumpOnTime =  1000 * 60 * param.asFloat(); // assigning incoming value from pin V3 to a variable, then convert to MS
  // You can also use:
  Serial.print("Pump ON time is now: ");
  Serial.println(pumpOnTime);
}

// This function will be called every time Slider Widget in Blynk app writes values to the Virtual Pin 4
BLYNK_WRITE(V4)
{
  pumpOffTime =  1000 * 60 * param.asFloat(); // assigning incoming value from pin V4 to a variable, then convert to MS
  // You can also use:
  Serial.print("Pump OFF time is now: ");
  Serial.println(pumpOffTime);
}

// This function will be called when device initially connects to blynk app
BLYNK_CONNECTED()
{
  // Synchronizes values on connection
  Blynk.syncVirtual(V1);
  Blynk.syncVirtual(V2);
  Blynk.syncVirtual(V3);
  Blynk.syncVirtual(V4);
}
