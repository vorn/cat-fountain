#include <DS3231.h>

#define IR_SENS 7
#define LED 4
#define RELAY 11

#define SDA A4
#define SCL A5

#define RELAY_ON LOW
#define RELAY_OFF HIGH

DS3231 rtc(SDA, SCL);

long offTime;
long del;

// the setup function runs once when you press reset or power the board
void setup() {
  // initialize digital pin 13 as an output.
  pinMode(LED, OUTPUT);
  pinMode(IR_SENS, INPUT);
  pinMode(RELAY, OUTPUT);
  digitalWrite(RELAY, HIGH);
  
  Serial.begin(9600);
  rtc.begin();

  offTime = 0;
  del = 0;

  //rtc.setDate(25,4,2016);
  //rtc.setTime(11,03,15);
}

// the loop function runs over and over again forever
void loop() {
  int val = 0;
  val = digitalRead(IR_SENS);
  ////digitalWrite(LED, val == HIGH ? 1 : 0);
  //digitalWrite(RELAY, val == HIGH ? LOW : HIGH);
  //delay(val == HIGH ? 120000 : 200);
  //Serial.print(rtc.getDateStr());
  //Serial.print(" ");
  //Serial.print(rtc.getTimeStr());
  //Serial.print("\n");
  if (offTime) {
    long unixTime = rtc.getUnixTime(rtc.getTime());
    if (unixTime > offTime)  {
      digitalWrite(RELAY, RELAY_OFF);
      offTime = 0; 
      delay(2000); 
    }
  } else {
      /*
      Sched:
      - no op 10pm-7am
      - 5min 10am - 6pm
      - 2min 6pm - 10pm, 7am - 10am 
  
      */
    if (val == HIGH) {
      
      Time t = rtc.getTime();
      long unixTime = rtc.getUnixTime(t);
      uint8_t hr = t.hour;
      if (hr >= 10 && hr < 18) {
        del = 5 * 60;
        digitalWrite(RELAY, RELAY_ON);
        offTime = unixTime + del;
      } else if (hr >= 18 && hr < 22 || hr >= 7 && hr < 10) {
        del = 2 * 60;
        digitalWrite(RELAY, RELAY_ON);
        offTime = unixTime + del;
      }
    }

  }
  delay(100);
}

