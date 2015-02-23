#include <RunningMedian.h>
#include <SPI.h>
#include <ADXL362.h>

 //the sensitivity required to acknowledge a "move" event.  Will be replaced with accelerometer
 //interrupts

#define sensitivity 200

/*Define some running median things*/
RunningMedian xLong = RunningMedian(50);
RunningMedian xShort = RunningMedian(10);
RunningMedian yLong = RunningMedian(50);
RunningMedian yShort = RunningMedian(10);
RunningMedian zLong = RunningMedian(50);
RunningMedian zShort = RunningMedian(10);

int x, y, z; //variables to store the accelerometer movements

ADXL362 xl;

int16_t temp;
int16_t XValue, YValue, ZValue, Temperature;

void setup(){
  Serial.begin(9600);
  xl.begin(10);                   // Setup SPI protocol, issue device soft reset
  xl.beginMeasure();              // Switch ADXL362 to measure mode  	
  Serial.println("Start Demo: Simple Read");
}

void loop(){
  xl.readXYZTData(XValue, YValue, ZValue, Temperature);
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
  
  if (change > sensitivity) {
    Serial.println(change);
  }
  delay(1);
}

