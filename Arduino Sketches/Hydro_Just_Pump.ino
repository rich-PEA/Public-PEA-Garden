unsigned long pumpOnTime = 15 * 60 * 1000; // how long pump runs in milliseconds
unsigned long pumpOffTime = 15 * 60 * 1000; // how long pump "rests" in milliseconds

const byte relayPin = 2; // the pin our relay is attached to

void setup() {

pinMode(relayPin, OUTPUT); // set our relay pinmode to output

}

void loop() {

digitalWrite(relayPin, HIGH); // Pump ON
delay(pumpOnTime); // delay for pump run time
digitalWrite(relayPin, LOW); // Pump OFF
delay(pumpOffTime); // delay for pump "rest" time

}
