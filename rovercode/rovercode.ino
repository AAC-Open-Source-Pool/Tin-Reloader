#define LEFT_MOTOR_FORWARD 19
#define LEFT_MOTOR_BACKWARD 21

#define RIGHT_MOTOR_FORWARD 5
#define RIGHT_MOTOR_BACKWARD 18

#define LEFT_SENSOR 23 // GPIO pin for the left IR sensor
#define RIGHT_SENSOR 22 // GPIO pin for the right IR sensor
int k = 1;
bool b1 = false;

#define BLYNK_TEMPLATE_ID "TMPL3fitKZ86f"
#define BLYNK_TEMPLATE_NAME "blink"
#define BLYNK_AUTH_TOKEN "FxX4EmSWgiKbILsngBK5iJnsBITH4pHh"

#include <WiFi.h>
#include <BlynkSimpleEsp32.h>

char auth[] = BLYNK_AUTH_TOKEN;
char ssid[] = "Aasrith";
char pass[] = "12345678";
int value0;

void setup() {
  // Set motor control pins as outputs
  pinMode(LEFT_MOTOR_FORWARD, OUTPUT);
  pinMode(LEFT_MOTOR_BACKWARD, OUTPUT);

  pinMode(RIGHT_MOTOR_FORWARD, OUTPUT);
  pinMode(RIGHT_MOTOR_BACKWARD, OUTPUT);

  // Set IR sensor pins as inputs
  pinMode(LEFT_SENSOR, INPUT);
  pinMode(RIGHT_SENSOR, INPUT);

  Serial.begin(115200);
  
  WiFi.begin(ssid, pass);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("WiFi connected");
  Blynk.begin(auth, ssid, pass);

  // Initialize motors as OFF
  stopMotors();
}

void loop() {
  if (b1==false)
  {
    delay(500);
    Blynk.syncVirtual(V0);
    if (value0 != 0)
    {
      Serial.println("Station: "+value0);
      k = value0;
      b1 = true;
    }
  }
  int leftSensor = digitalRead(LEFT_SENSOR);   // Read left IR sensor
  int rightSensor = digitalRead(RIGHT_SENSOR); // Read right IR sensor

  if (k == 0)
  {
    stopMotors();
    delay(10000)l
    Serial.println("P Stop");
    b1 = false;
  }
  else
  {
    // Line-following logic
    if (leftSensor == LOW && rightSensor == LOW) {
      // Both sensors detect the line - move forward
      moveForward();
    } else if (leftSensor == LOW && rightSensor == HIGH) {
      // Left sensor detects the line - turn left
      turnLeft();
    } else if (leftSensor == HIGH && rightSensor == LOW) {
      // Right sensor detects the line - turn right
      turnRight();
    } else if (leftSensor == HIGH && rightSensor == HIGH) {
      // No line detected - stop
      stopMotors();
      k--;
      Serial.println(k);
      delay(2000);
      moveForward();
      moveForward();
    } else {
      stopMotors();
    }
  }
}

// Motor control functions
void moveForward() {
  digitalWrite(LEFT_MOTOR_FORWARD, LOW);
  digitalWrite(LEFT_MOTOR_BACKWARD, HIGH);
  digitalWrite(RIGHT_MOTOR_FORWARD, HIGH);
  digitalWrite(RIGHT_MOTOR_BACKWARD, LOW);
  Serial.println("forward");

}

void turnLeft() {
  digitalWrite(LEFT_MOTOR_FORWARD, LOW);
  digitalWrite(LEFT_MOTOR_BACKWARD, LOW);
  digitalWrite(RIGHT_MOTOR_FORWARD, HIGH);
  digitalWrite(RIGHT_MOTOR_BACKWARD, LOW);
  Serial.println("left");
}

void turnRight() {
  digitalWrite(LEFT_MOTOR_FORWARD, LOW);
  digitalWrite(LEFT_MOTOR_BACKWARD, HIGH);
  digitalWrite(RIGHT_MOTOR_FORWARD, LOW);
  digitalWrite(RIGHT_MOTOR_BACKWARD, LOW);
  Serial.println("right");
}

void stopMotors() {
  digitalWrite(LEFT_MOTOR_FORWARD, LOW);
  digitalWrite(LEFT_MOTOR_BACKWARD, LOW);
  digitalWrite(RIGHT_MOTOR_FORWARD, LOW);
  digitalWrite(RIGHT_MOTOR_BACKWARD, LOW);
  Serial.println("pause");
}

BLYNK_WRITE(V0)
{
  value0 = param.asInt(); // Get value as integer
}

