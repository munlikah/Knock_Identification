#include <WiFi.h>
#include <WiFiClientSecure.h>
#define TokenLine "ZUSD9vA5YR8oNrlaHDkQd5xUMp2BYJNlLtiT1oVB5jw"

const int sw1 = 25;
const int sw2 = 26;
const int led = 5;
int sw_value[2];
int countTime = 5000;

char ssid[] = "WiFi v.2";
char pass[] = "Nikorn_615";

void setup() {
  Serial.begin(9600);
  pinMode(sw1, INPUT);
  pinMode(sw2, INPUT);
  pinMode(led, OUTPUT);

  WiFi.begin(ssid, pass);
  wifiConnect();
}

void loop() {
  digitalWrite(led, HIGH);
  if (digitalRead(sw1) == 1) {
    sw_value[0] = 1;
    delay(500);
  }
  if (sw_value[0] == 1) {
    for(int i=0; i<2; i++) {
      digitalWrite(led, HIGH);
      delay(150);
      digitalWrite(led, LOW);
      delay(150);
    }
    Serial.println("Please Knock");
    long stt = millis();
    while (millis() < (stt + countTime)) {
      if (digitalRead(sw2) == 1)
        sw_value[1] = 1;
    }
    Serial.println("Time's up");
    delay(500);
  }
  if (sw_value[0] == 1) {
    if (sw_value[1] == 1) {
      Serial.println("Unlocked");
      NotifyLine("Mom unlocked the door.");
      ledUnlock();
    } else {
      NotifyLine("Stranger tried to unlock the door...");
      digitalWrite(led, LOW);
      delay(500);
    }
  }
  delay(1000);
  sw_value[0] = 0;
  sw_value[1] = 0;
}

void ledUnlock() {
  for (int i=0; i<5; i++) {
    digitalWrite(led, HIGH);
    delay(200);
    digitalWrite(led, LOW);
    delay(200);
  }
}

void wifiConnect() {
  Serial.print("WiFi Connecting");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.println();
  Serial.println("connected");
}

void NotifyLine(String sent_msg) {
  WiFiClientSecure client;

  if (!client.connect("notify-api.line.me", 443)) {
    Serial.println("Connection failed");
    return;
  }
  
  String msg = String("message=") + sent_msg;
  String req = "";
  req = "POST /api/notify HTTP/1.1\r\nHost: notify-api.line.me\r\nAuthorization: Bearer "
  + String(TokenLine) + "\r\nContent-Type: application/x-www-form-urlencoded\r\nContent-Length: "
  + String(msg.length()) + "\r\n\r\n" + msg;

  Serial.println(msg);
  client.print(req);

  while (client.connected()) {
    String line = client.readStringUntil('\n');
    if (line == "\r")
    {
      Serial.println("Send LINE Notify successful!");
      break;
    }
  }
}
