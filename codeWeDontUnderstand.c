// Motor pin definitions (CHANGE if needed)
int motor1PWM   = 37;   // AENABLE
int motor1Phase = 38;   // APHASE

int motor2PWM   = 39;   // BENABLE
int motor2Phase = 20;   // BPHASE


// -------------------- Line sensors --------------------
int AnalogValue[5] = {0,0,0,0,0};
int AnalogPin[5]   = {4,5,6,7,15};   // keeping 8 free for music 

// Threshold: decide "white line" vs "black background"
// Guide example: white ~ <250, black ~ >3800 
int THRESH = 1500;  // <-- CALIBRATE 
// Simple speed settings (you will tune these)
int BASE_L = 200;
int BASE_R = 200;

// Proportional steering gain (tune)
int KP = 60;

// Median filter buffers (last 5 samples per sensor) for robustness 
int buf[5][5] = {0};
int bufIdx = 0;


// Return median of 5 numbers (helper for filtering)
int median5(int a, int b, int c, int d, int e) {
  int x[5] = {a,b,c,d,e};
  // simple sort (5 items)
  for (int i=0;i<5;i++){
    for (int j=i+1;j<5;j++){
      if (x[j] < x[i]) { int t=x[i]; x[i]=x[j]; x[j]=t; }
    }
  }
  return x[2];
}

void setup()
{
  Serial.begin(9600);

  pinMode(motor1PWM, OUTPUT);
  pinMode(motor1Phase, OUTPUT);
  pinMode(motor2PWM, OUTPUT);
  pinMode(motor2Phase, OUTPUT);

}

void loop()
{
  // 1) Read sensors (raw), store into 5-sample buffers
  for (int i=0; i<5; i++) {
    int raw = analogRead(AnalogPin[i]);
    buf[i][bufIdx] = raw;
  }
  bufIdx = (bufIdx + 1) % 5;

  // 2) Filter each sensor with median of last 5 (more robust than single readings) :contentReference[oaicite:6]{index=6}
  for (int i=0; i<5; i++) {
    AnalogValue[i] = median5(buf[i][0], buf[i][1], buf[i][2], buf[i][3], buf[i][4]);
  }

  // 3) Convert to "on white line?" bits.
  //    We assume WHITE line gives LOWER reading than BLACK (as per guide) :contentReference[oaicite:7]{index=7}
  int L2 = (AnalogValue[0] < THRESH) ? 1 : 0;  // far left
  int L1 = (AnalogValue[1] < THRESH) ? 1 : 0;  // left
  int C  = (AnalogValue[2] < THRESH) ? 1 : 0;  // centre
  int R1 = (AnalogValue[3] < THRESH) ? 1 : 0;  // right
  int R2 = (AnalogValue[4] < THRESH) ? 1 : 0;  // far right

  // 4) Compute an "error" (negative = line is left, positive = line is right)
  //    Weighted sum approach (covers lots of truth-table cases cleanly) :contentReference[oaicite:8]{index=8}
  int seen = L2 + L1 + C + R1 + R2;

  int error = (-2 * L2) + (-1 * L1) + (0 * C) + (1 * R1) + (2 * R2);

  // If multiple sensors see the line at once, average it
  if (seen > 0) error = error / seen;
  if (L2) error = -2;
  if (R2) error =  2;


  // 5) Turn that error into motor speeds (proportional control)
  int turn = (KP * error * (BASE_L + BASE_R)) / 220; 

  int MAX_TURN = (BASE_L + BASE_R) / 2;   // dynamic: bigger base allows bigger turn
  turn = constrain(turn, -MAX_TURN, MAX_TURN);

  int leftSpeed  = BASE_L - turn;
  int rightSpeed = BASE_R + turn;

  // Clamp to valid PWM range
  if (leftSpeed  < 0)   leftSpeed = 0;
  if (rightSpeed < 0)   rightSpeed = 0;
  if (leftSpeed  > 255) leftSpeed = 255;
  if (rightSpeed > 255) rightSpeed = 255;

  // 6) Handle "lost line" case (no sensors detect line)
  //    Do a gentle search in the last known direction.
  static int lastError = 0;
  if (seen == 0) {
    if (lastError >= 0) {
      // last saw line on right -> rotate right gently
      leftSpeed  = 120;
      rightSpeed = 60;
    } else {
      // last saw line on left -> rotate left gently
      leftSpeed  = 60;
      rightSpeed = 120;
    }
  } else {
    lastError = error;
  }

  // 7) Drive forward with adjusted speeds (no long delay, keep looping fast)
  GoForward(0, leftSpeed, rightSpeed);

  delay(3); // small control loop delay (10–20ms is typical)
}


void GoForward(int time,int speed1,int speed2)
{
  digitalWrite(motor1Phase, HIGH);
  digitalWrite(motor2Phase, HIGH);
  analogWrite(motor1PWM, speed1);
  analogWrite(motor2PWM, speed2);
  if (time > 0) delay(time);
}

void GoBackward(int time,int speed1,int speed2)
{
  digitalWrite(motor1Phase, LOW);
  digitalWrite(motor2Phase, LOW);
  analogWrite(motor1PWM, speed1);
  analogWrite(motor2PWM, speed2);
  if (time > 0) delay(time);
}

void GoStop(int time)
{
  analogWrite(motor1PWM, 0);
  analogWrite(motor2PWM, 0);
  delay(time);
}