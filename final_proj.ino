#include <CapacitiveSensor.h>
#include <Adafruit_NeoPixel.h>

const long numReadings = 50;

int sensorPin1 = 0;
#define LED_PIN     7
#define AUDIO_PIN A4 
#define NUM_LEDS    58


long readings[numReadings];
long index = 0;
long total = 0;
long average = 0;

int leds[NUM_LEDS];

CapacitiveSensor cs_3_2 = CapacitiveSensor(4, 2);
Adafruit_NeoPixel strand = Adafruit_NeoPixel(NUM_LEDS, LED_PIN, NEO_GRB + NEO_KHZ800);  //LED strand objetcs

uint8_t visual = 0;   //Holds the current visual being displayed.
uint8_t volume = 0;   //Holds the volume level read from the sound detector.

float sensorValue = 0, filteredSignal = 0,
    filteredSignalValues[] = {3.4, 3.1, 2.7, 2.4, 2.1, 1.7, 1.3, 0.9, 0.4};

int inputval=0; //variable for values to be read from A1
int outval =0; //variable for output value from A1

void setup() {
  cs_3_2.set_CS_AutocaL_Millis(0xFFFFFFFF);

  Serial.begin(9600);

  for (int thisReading = 0; thisReading < numReadings; thisReading++) readings[thisReading] = 0;

  for (int thisLed = 0; thisLed < NUM_LEDS; thisLed++) {
    pinMode(leds[thisLed], OUTPUT);
  } pinMode(sensorPin1, INPUT);

  strand.begin(); //Initialize the LED strand object.
  strand.show();  //Show a blank strand, just to get the LED's ready for use.
}

void loop() {
  long total1 = cs_3_2.capacitiveSensor(30);

  int top;

  top = analogRead(sensorPin1);

  Serial.print("top");
  Serial.print("\t");
  Serial.println(top);

  int topmap;

  topmap = map(top, 0, 1023, 50, 200);
  Serial.print("Topmap");
    Serial.print("\t");
    Serial.println(topmap);


  // subtract the last reading:
  total = total - readings[index];
  // read from the sensor:
  readings[index] = total1;
  // add the reading to the total:
  total = total + readings[index];
  // advance to the next position in the array:
  index = index + 1;

  // if we're at the end of the array...
  if (index >= numReadings) {
    // ...wrap around to the beginning:
    index = 0;
  }

  // calculate the average:
  average = total / numReadings;
  
  // send it to the computer as ASCII digits
  Serial.print("average");
  Serial.print("\t");
  Serial.println(average);     
  
   int sensorReading = average;
  
   int ledLevel = map(sensorReading, 0, topmap, 0, NUM_LEDS);
  
    /*Serial.print("ledLevel");
    Serial.print("\t");
    Serial.println(ledLevel);*/
  
    // Sound

  volume = (float) analogRead(AUDIO_PIN) * (5.0 / 1024.0);
  FilterSignal(volume);

  /*Serial.print("volume");
    Serial.print("\t");
    Serial.println(volume);*/

  for (int thisLed = 0; thisLed < NUM_LEDS; thisLed++) {

    if (thisLed < ledLevel) {
      strand.clear();
    }

    else {
      //ChooseColor(thisLed, filteredSignal);
      strand.setPixelColor(thisLed, 200, 200, 200);
        strand.show();
    }
  }

}

void FilterSignal(float sensorSignal) {

  filteredSignal = (0.945 * filteredSignal) + (0.0549 * sensorSignal);

     /*Serial.print("filteredSignal");
    Serial.print("\t");
    Serial.println(filteredSignal);*/

}

void ChooseColor(int thisLed, float filteredSignal) {

  if (filteredSignal > filteredSignalValues[0]) {
    RGBColor(thisLed, 0, 0, 255);
    //Blue
  } else if (filteredSignal <= filteredSignalValues[0] && filteredSignal > filteredSignalValues[1]) { // high volumes
    //Azure
    RGBColor(thisLed, 0, 255, 255);
  } else if (filteredSignal <= filteredSignalValues[1] && filteredSignal > filteredSignalValues[2]) {
    RGBColor(thisLed, 0, 127, 255);
    //Cyan
  } else if (filteredSignal <= filteredSignalValues[2] && filteredSignal > filteredSignalValues[3]) {
    RGBColor(thisLed, 0, 255, 127);
    //Aqua marine
  } else if (filteredSignal <= filteredSignalValues[3] && filteredSignal > filteredSignalValues[4]) {
    RGBColor(thisLed, 0, 255, 0);
    //Green
  } else if (filteredSignal <= filteredSignalValues[4] && filteredSignal > filteredSignalValues[5]) {
    RGBColor(thisLed, 255, 255, 0);
    //Yellow
  } else if (filteredSignal <= filteredSignalValues[5] && filteredSignal > filteredSignalValues[6]) {
    RGBColor(thisLed, 255, 0, 255);
    //Magenta
  } else if (filteredSignal <= filteredSignalValues[6] && filteredSignal > filteredSignalValues[7]) {
    RGBColor(thisLed, 255, 0, 127);
    //Rose
  } else if (filteredSignal <= filteredSignalValues[7] && filteredSignal > filteredSignalValues[8]) { // low volumes
    RGBColor(thisLed, 255, 127, 0);
    //Orange
  } else if (filteredSignal <= filteredSignalValues[8]) {
    RGBColor(thisLed, 255, 0, 0);
    //Red
  } else {
    RGBColor(thisLed, 0, 0, 255);
    //Default: Blue
  }
}

void RGBColor(int thisLed, int Rcolor, int Gcolor, int Bcolor) {

  strand.setPixelColor(thisLed, Rcolor, Gcolor, Bcolor);
  strand.show();
} 
