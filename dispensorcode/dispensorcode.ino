#define TRIG_PIN_WATER 19
#define ECHO_PIN_WATER 21
#define TRIG_PIN_HAND 5
#define ECHO_PIN_HAND 18
#define RELAY_PIN 12 // volt input right,volt ouput middle
#define WATER_TIN_HEIGHT 30 // height of the water tin in cm
#define RoomNumber 3

#define BLYNK_TEMPLATE_ID "TMPL3fitKZ86f"
#define BLYNK_TEMPLATE_NAME "blink"
#define BLYNK_AUTH_TOKEN "FxX4EmSWgiKbILsngBK5iJnsBITH4pHh"

#include <WiFi.h>
#include <BlynkSimpleEsp32.h>

char auth[] = BLYNK_AUTH_TOKEN;
char ssid[] = "Aasrith";
char pass[] = "12345678";
int value0 = 0;
int value1;

void setup() {
  Serial.begin(115200);
  pinMode(TRIG_PIN_WATER, OUTPUT);
  pinMode(ECHO_PIN_WATER, INPUT);
  pinMode(TRIG_PIN_HAND, OUTPUT);
  pinMode(ECHO_PIN_HAND, INPUT);
  pinMode(RELAY_PIN, OUTPUT);
  digitalWrite(RELAY_PIN, LOW);
  WiFi.begin(ssid, pass);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("WiFi connected");
  Blynk.begin(auth, ssid, pass);
}

float measureDistance(int trigPin, int echoPin) {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  float duration = pulseIn(echoPin, HIGH);
  float distance = (duration * 0.034) / 2;
  return distance;
}

void loop() {

  Blynk.run();
  float distanceWater = measureDistance(TRIG_PIN_WATER, ECHO_PIN_WATER);

  float distanceHand = measureDistance(TRIG_PIN_HAND, ECHO_PIN_HAND);
  float waterLevel = (((13 - distanceWater)/13)*100);

  Serial.println("Water Level: ");
  Serial.println(waterLevel);


  if (waterLevel < 15) {
    Serial.println("Low Water Level");
    digitalWrite(RELAY_PIN, LOW);
  } else {
    if (distanceHand <= 10) {
      Serial.println("Hand Detected");
      digitalWrite(RELAY_PIN, HIGH);
    } else {
      Serial.println("Hand Not Detected");
      digitalWrite(RELAY_PIN, LOW);
    }
  }
  Blynk.virtualWrite(V1, waterLevel);
  Blynk.syncVirtual(V1);
  Blynk.syncVirtual(V0);

  if (value1 <= 15 && value0 == 0)
  {
    Blynk.virtualWrite(V0, RoomNumber);
    Blynk.logEvent( "low_water", (String("Water Level is ")+value1));
    Serial.println("Alert");

  }
  else if (value1 > 10 && value0 != 0)
  {
    Blynk.virtualWrite(V0, 0);
  }

  delay(500);
}

BLYNK_WRITE(V1)
{
  value1 = param.asInt(); // Get value as integer
}

BLYNK_WRITE(V0)
{
  value0 = param.asInt(); // Get value as integer
}