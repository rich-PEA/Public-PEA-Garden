// BLYNK STUFF //
#define BLYNK_PRINT Serial
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>

char auth[] = "UNIQUE BLYNK AUTH TOKEN"; //Blynk Auth Token for "Water Level Test"
char ssid[] = "YOUR WIFI NETWORK";
char pass[] = "YOUR WIFI PASSWORD";
// END BLYNK STUFF //

//Misc variables
unsigned long previousTime = 0;
unsigned long sampleTime = 2000; // How often we're going to sample the water level


int rawLiquidReading; //declare our raw data variable in integer form
float liquidLevel; // declare our calculated liquid level, which will be a percentage
const long lowEtapeValue = 590; // this variable corresponds to the reading of the sensor when the reservoir is nearly empty
const long highEtapeValue = 740;  // this variable corresponds to the reading of the sensor when the reservoir is full

const byte liquidLevelPin = A0; //the pin our eTape is connected to

void setup() {

  pinMode(liquidLevelPin, INPUT); // declares the pin our Etape is connected to as something to read

  Serial.begin(9600); // begin serial communication

  Blynk.begin(auth, ssid, pass);

}

void loop() {

  unsigned long currentTime = millis();
  static unsigned long previousTime = currentTime;

  Blynk.run();

  if (currentTime - previousTime >= sampleTime) { // execute code below once sampleTime is exceeded

    rawLiquidReading = analogRead(liquidLevelPin); // read the pin, expecting readings between 0 - 1023
    Serial.print("Raw Liquid Level: ");
    Serial.print(rawLiquidReading); // display raw reading

    liquidLevel = map(rawLiquidReading, lowEtapeValue, highEtapeValue, 0, 100); // converts our raw reading into a percentage
    Serial.print(" / Liquid Remaining: ");
    Serial.print(liquidLevel);
    Serial.println("%."); // display the percentage of the liquid remaining in the reservoir

    Blynk.virtualWrite(V1, rawLiquidReading); //Push liquid level data to V1 in App
    Blynk.virtualWrite(V0, liquidLevel); //Push liquid level data to V0 in App

    previousTime = currentTime;

  }

}

// This function will be called every time the Stepper Widget in Blynk app writes values to the Virtual Pin 2
BLYNK_WRITE(V2)
{
  sampleTime = 1000 * param.asFloat(); // assigning incoming value from pin V2 to a variable, then convert to MS
  // You can also use:
  Serial.print("Sample Time is now: ");
  Serial.println(sampleTime);
}

BLYNK_CONNECTED()
{
  // Synchronizes values on connection
  Blynk.syncVirtual(V2);
}
