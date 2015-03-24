#define MAX_BEAN_SLEEP 0xFFFFFFFF

#define SLEEP_INTERVAL 10000     //heartbeet every 10 seconds

long timeSinceOn = 0;            //inrement this by one ever SLEEP_INTERVAL milliseconds
unsigned int touches[500];      //at which minutes was this touched
int touchIndex = 0;              //index for touches  
char writeBuffer[10];            //buffer for writing data

bool justTouched = false;

void touch() {
  justTouched = true;
}

void setMessage(String message) {
  char charBuffer[message.length()];
  message.toCharArray(charBuffer,message.length());
  
  uint8_t sBuffer[message.length()];
  
  for (int i=0; i<message.length(); i++) {
    sBuffer[i] = message.charAt(i);
  }
  Bean.setScratchData(1, sBuffer, sizeof(sBuffer));
}

void showTouches() {
  unsigned int timeOn = (timeSinceOn / 6);
  setMessage(String(timeOn));
  for (int i=0; i<touchIndex; i++) {
    setMessage(String(touches[i]));  
  }
}

void setup() {
  Serial.begin(9600);
  pinMode(0, INPUT);
}

void loop() {
  Bean.attachChangeInterrupt(0,touch);
  if (justTouched == false) {
    Bean.setLed(10,0,0);
    Bean.sleep(100);
    Bean.setLed(0,0,0);
    timeSinceOn++;
    Bean.sleep(SLEEP_INTERVAL - 100);
  }
  if ((justTouched == true)) {
    unsigned int minutesSinceOn = (timeSinceOn / 6);
    if (touches[touchIndex] != minutesSinceOn) {
      touchIndex++;
      touches[touchIndex] = minutesSinceOn;
    }
    justTouched = false;
    Bean.detachChangeInterrupt(0);
    Bean.setLed(0,10,0);
    Bean.sleep(500);
    Bean.setLed(0,0,0);
    showTouches();
    Bean.sleep(SLEEP_INTERVAL - 100);
  }
}
