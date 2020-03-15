#include <Servo.h>

Servo myservo;

const int sw = 8;
const int servo = 2;

const int piezo = A2;
const int led = 5;
const int thresholdHigh = 80;
const int thresholdLow = 30;

int closeDoor = 130, openDoor = 30;

int sw_value;
int secretCounter = 0;
int knock = 0;
int knockTime = 10000;

const int secretKnockLength = 3;
const int secretKnock[secretKnockLength] = {1,0,0};

void setup() {
  Serial.begin(9600);
  pinMode(led, OUTPUT);
  myservo.attach(servo);
  myservo.write(closeDoor);
}

void loop() {
  digitalWrite(led, HIGH);
  if (digitalRead(sw) == 1) {
    sw_value = 1;
    delay(500);
  }
  if (sw_value == 1) {
    for(int i=0; i<2; i++) {
      digitalWrite(led, HIGH);
      delay(150);
      digitalWrite(led, LOW);
      delay(150);
    }
    Serial.println("Please Knock");
    long stt = millis();
    while (millis() < (stt + knockTime)) {
      knock = analogRead(piezo);
      String knockstr = String(knock);
      if (knock >= thresholdHigh) {
        if (secretKnock[secretCounter] == 1) {
          secretCounter++;
          Serial.println(knockstr + " Correct");
        } 
        else {
          secretCounter = 0;
          Serial.println(knockstr + " Fail");
        }
        delay(100);
      }
      else if (knock >= thresholdLow) {
        if (secretKnock[secretCounter] == 0) {
          secretCounter++;
          Serial.println(knockstr + " Correct");
        }
        else {
          secretCounter = 0;
          Serial.println(knockstr + " Fail");
        }
        delay(100);
      }
    }
    //digitalWrite(led, LOW);
    Serial.println("Time's up");
    delay(500);
  }
  if (sw_value == 1) {
    if (secretCounter == (secretKnockLength)) {
      Serial.println("Unlocked");
      ledUnlock();
      doorUnlock();
    } else {
    Serial.println("Invalid Knock");
    digitalWrite(led, LOW);
    delay(1000);
    }
  }
  delay(1000);
  sw_value = 0;
  secretCounter = 0;
}

void ledUnlock() {
  for (int i=0; i<5; i++) {
    digitalWrite(led, HIGH);
    delay(200);
    digitalWrite(led, LOW);
    delay(200);
  }
}

void doorUnlock() {
  myservo.write(openDoor);
  delay(3000);
  myservo.write(closeDoor);
}
