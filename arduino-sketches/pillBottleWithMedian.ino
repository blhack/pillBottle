#include <RunningMedian.h>
#include <SPI.h>
#include <ADXL362.h>
#include <boards.h>
#include <RBL_nRF8001.h>


#define sensitivity 50  //the sensitivity required to acknowledge a "move" event.  Will be replaced with accelerometer
#define sampleSpeed 100 //delay between samples.  ADXL362 samples at 100hz.

//Define some running median things
RunningMedian xLong = RunningMedian(100); //median over 10 seconds (at 10hz)
RunningMedian xShort = RunningMedian(10); //median over 1 second (at 10hz)
RunningMedian yLong = RunningMedian(100);
RunningMedian yShort = RunningMedian(10);
RunningMedian zLong = RunningMedian(100);
RunningMedian zShort = RunningMedian(10);

//lastBump is the time (millis()) when the last "touch event" happened
long lastBump = millis();

//ADXL362 related 
int x, y, z; //variables to store the accelerometer movements
ADXL362 xl;
int16_t temp;
int16_t XValue, YValue, ZValue, Temperature;

//NRF8001 related
unsigned char buf[16] = {0};
unsigned char len = 0;

//Buffer for outbound BLE data
int writeBuffer_size = 140;
char writeBuffer[140];

//Array for movements
int movements[100];
int moveIndex = 0;

void setup(){
  ble_set_name("PillBottle");
  ble_begin();
  Serial.begin(9600);
  xl.begin(10);                   // Setup SPI protocol, issue device soft reset
  xl.beginMeasure();              // Switch ADXL362 to measure mode  	
}

void bleTick() {
  setMessage(String(millis()));
  displayMessage();
}

void clearBuffer() {
  for (int i=0; i<writeBuffer_size; i++) {
    writeBuffer[i] = '\0';   
  }
}

void displayMessage() {
  if ( ble_connected() ) {
    for (int i=0; i<writeBuffer_size; i++) {
      if (writeBuffer[i] != '\0') {
        ble_write(writeBuffer[i]);
        Serial.println(writeBuffer[i]);
      }
    }
  }
  ble_do_events();
}

void setMessage(String message) {
  clearBuffer();
  message.toCharArray(writeBuffer,140); 
}

void bleDump() {
  setMessage(String(millis() / 1000)); //The current time on the arduino
  displayMessage();
  
  for (int i=0; i<100; i++) {
    if (movements[i] != '\0') {
      setMessage(String(movements[i]));
      displayMessage();
    }
  } 
}

void readBle() {
  if (ble_available()) {
    if (ble_read() == 1) {
      Serial.println("You asked for data");
      bleDump(); 
    }
  }  
}

void loop(){
  if (!ble_busy()) {
    xl.readXYZTData(XValue, YValue, ZValue, Temperature);
  }
  xLong.add(XValue);
  xShort.add(XValue);
  yLong.add(YValue);
  yShort.add(YValue);
  zLong.add(ZValue);
  zShort.add(ZValue);
 
  int xChange = abs(xLong.getMedian() - xShort.getAverage());
  int yChange = abs(yLong.getMedian() - yShort.getAverage());
  int zChange = abs(zLong.getMedian() - zShort.getAverage());
  int change = xChange + yChange + zChange;
  
  if ((change > sensitivity) && ((millis() - lastBump) > 3000)) {
    lastBump = millis();
    movements[moveIndex] = (millis() / 1000);
    moveIndex++;
    Serial.println("Move!");
  }
  readBle();
  ble_do_events();
  delay(10);
}
