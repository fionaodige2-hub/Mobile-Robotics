
int motor1PWM   = 37;   // AENABLE
int motor1Phase = 38;   // APHASE

int motor2PWM   = 39;   // BENABLE
int motor2Phase = 20;   // BPHASE

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
  GoStop(1000);
  GoForward(3000,50,50);
  GoStop(0);
  GoBackward(3000,50,50);
  GoStop(0);
}

void GoForward(int time,int speed1,int speed2)
  {
  digitalWrite(motor1Phase, HIGH);
  digitalWrite(motor2Phase, HIGH);
  analogWrite(motor1PWM, speed1); // set speed of motor
  analogWrite(motor2PWM, speed2);
  Serial.println("Forward"); // Display motor direction
  delay(time);
  }
void GoBackward(int time,int speed1,int speed2)
 {
  digitalWrite(motor1Phase, LOW);
  digitalWrite(motor2Phase, LOW);
  analogWrite(motor1PWM, speed1); // set speed of motor
  analogWrite(motor2PWM, speed2);
  Serial.println("Yeild"); // Display motor direction
  delay(time);
  }
void GoStop(int time)
{
  analogWrite(motor1PWM, 0); // set speed of motor
  analogWrite(motor2PWM, 0);
  delay(time);
}

int AnalogValue[5] = {0,0,0,0,0};
int AnalogPin[5] = {4,5,6,7,15}; // keep 8 free for tone O/P music
void set()
{
// put your setup code here, to run once:
Serial.begin(9600);
}
void loop1()
{
// put your main code here, to run repeatedly:
int i;
for (i=0;i<5;i++)
{
AnalogValue[i]=analogRead(AnalogPin[i]);
Serial.print(AnalogValue[i]); // This prints the actual analog sensor reading
Serial.print("\t"); //tab over on screen
if(i==4)
{
Serial.println(""); //carriage return
delay(600); // display new set of readings every 600mS
}
}
}
