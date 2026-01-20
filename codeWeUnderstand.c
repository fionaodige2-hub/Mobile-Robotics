/**/
// -------- MOTOR PINS (ESP32 + DRV8835 style) --------
const int motor1PWM   = 37;  // left motor speed
const int motor1Phase = 38;  // left motor direction
const int motor2PWM   = 39;  // right motor speed
const int motor2Phase = 20;  // right motor direction

// -------- SENSOR THRESHOLDS (from lab sheet) --------
// White line: reading < 250
// Black background: reading > 3800
const int onWhite = 200;   // treat values < 200 as "white line"
const int onBlack = 2000;  // (kept for completeness, may not be used)

// -------- SENSOR READING VARIABLES --------
int s1, s2, s3, s4, s5;     // sensor values (extreme L .. extreme R)

// -------- BASIC MOVE FUNCTIONS --------
void moveForward(int speedVal) {
  digitalWrite(motor1Phase, HIGH);   // both motors forward
  digitalWrite(motor2Phase, HIGH);
  analogWrite(motor1PWM, speedVal);
  analogWrite(motor2PWM, speedVal);
}

void turnLeft(int speedVal) {
  digitalWrite(motor1Phase, HIGH);
  digitalWrite(motor2Phase, HIGH);
  analogWrite(motor1PWM, speedVal/2);          // left wheel stop
  analogWrite(motor2PWM, speedVal);   // right wheel forward
}

void turnRight(int speedVal) {
  digitalWrite(motor1Phase, HIGH);
  digitalWrite(motor2Phase, HIGH);
  analogWrite(motor2PWM, speedVal/2); 
  analogWrite(motor1PWM, speedVal);   // left wheel forward
}

// -------- HELPER FUNCTIONS --------
bool isOnWhite(int sensorValue) {
  // White line gives small values (< ~250)
  return (sensorValue < onWhite);
}

bool isOnBlack(int sensorValue) {
  // Black background gives large values (> ~3800)
  return (sensorValue > onBlack);
}

// ========================= SETUP =========================
void setup() {
  Serial.begin(9600);

  // motor pins
  pinMode(motor1PWM,   OUTPUT);
  pinMode(motor1Phase, OUTPUT);
  pinMode(motor2PWM,   OUTPUT);
  pinMode(motor2Phase, OUTPUT);

  // sensors on ESP32 pins 4,5,6,7,15
  pinMode(4, INPUT);   // extreme left
  pinMode(5, INPUT);   // left
  pinMode(6, INPUT);   // middle
  pinMode(7, INPUT);   // right
  pinMode(15, INPUT);  // extreme right
}

// ========================= LOOP =========================
void loop() {
  // ---- read sensors ----
  s1 = analogRead(4);   // extreme left
  s2 = analogRead(5);   // left
  s3 = analogRead(6);   // middle
  s4 = analogRead(7);   // right
  s5 = analogRead(15);  // extreme right

  // print for debugging
  Serial.print("extL: ");  Serial.print(s1);
  Serial.print("\tL: ");   Serial.print(s2);
  Serial.print("\tMid: "); Serial.print(s3);
  Serial.print("\tR: ");   Serial.print(s4);
  Serial.print("\textR: ");Serial.println(s5);

  // ---------- DECISION LOGIC ----------
  // We are following a WHITE line on BLACK background.
  // MIDDLE sensor has PRIORITY:
  //  - If middle sees white -> go straight
  //  - Else, use side sensors to turn
  //  - If nothing clearly sees white -> just creep forward

  if (isOnWhite(s3)) {
    // Middle on white -> best case: go straight
    moveForward(150);
  }
  else {
    // Middle NOT on white, use side sensors

    // Left side sees white? (either extreme left or left)
    if (isOnWhite(s1)) {
      turnRight(150);

    }
    if (isOnWhite(s2)) {
      turnRight(100);
    }
    //}
    // Right side sees white? (either right or extreme right)
    //if (isOnWhite(s4) || isOnWhite(s5)) {
    if (isOnWhite(s4)) {
      turnLeft(100);
    }
    if (isOnWhite(s5)) {
      turnLeft(150);
    }
    /*if ({
      // No sensor strongly on white:
      // stay calm and go slowly forward instead of spinning
      moveForward(100);
    }*/
  }

  //delay(20);  // small delay to smooth behaviour
}