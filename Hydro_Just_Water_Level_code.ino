int rawLiquidReading; //declare our raw data variable in integer form
float liquidLevel; // declare our calculated liquid level, which will be a percentage
const long lowEtapeValue = 590; // this variable corresponds to the reading of the sensor when the reservoir is nearly empty
const long highEtapeValue = 740;  // this variable corresponds to the reading of the sensor when the reservoir is full

const byte liquidLevelPin = A0; //the pin our eTape is connected to

void setup() {

  pinMode(liquidLevelPin, INPUT); // declares the pin our Etape is connected to as something to read

  Serial.begin(9600); // begin serial communication

}

void loop() {



  rawLiquidReading = analogRead(liquidLevelPin); // read the pin, expecting readings between 0 - 1023
  Serial.print("Raw Liquid Level: ");
  Serial.print(rawLiquidReading); // display raw reading

  liquidLevel = map(rawLiquidReading, lowEtapeValue, highEtapeValue, 0, 100); // converts our raw reading into a percentage
  Serial.print(" / Liquid Remaining: ");
  Serial.print(liquidLevel);
  Serial.println("%."); // display the percentage of the liquid remaining in the reservoir

}
