// ===== MOTOR PINS =====
#define ENA 9    // RIGHT motor
#define ENB 10   // LEFT motor
#define IN1 5
#define IN2 4
#define IN3 3
#define IN4 2

// ===== SENSOR PINS =====
int sensorPins[5] = {A0, A1, A2, A3, A4};
int s[5];

// ===== PID =====
float Kp = 39;
float Kd = 45;

int error = 0, previous_error = 0;

int baseRight = 190;
int baseLeft  = 98;

int leftSpeed, rightSpeed;

void setup() {
  Serial.begin(9600);

  pinMode(ENA, OUTPUT);
  pinMode(ENB, OUTPUT);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
}

void loop() {

  // ===== SENSOR READ =====
  for (int i = 0; i < 5; i++) {
    int val = analogRead(sensorPins[i]);

    if (val < 500) s[i] = 1;   // BLACK
    else s[i] = 0;
  }

  // ===== PATTERN BASED ERROR =====
  if (s[0] && !s[1] && !s[2] && !s[3] && !s[4]) error = -4;
  else if (s[0] && s[1] && !s[2]) error = -3;
  else if (!s[0] && s[1] && !s[2]) error = -2;
  else if (!s[1] && s[2] && !s[3]) error = 0;   // 🔥 CENTER LOCK
  else if (!s[2] && s[3] && !s[4]) error = 2;
  else if (!s[2] && s[3] && s[4]) error = 3;
  else if (!s[0] && !s[1] && !s[2] && !s[3] && s[4]) error = 4;

  // ===== PID =====
  int derivative = error - previous_error;
  int PID = (Kp * error) + (Kd * derivative);
  previous_error = error;

  // ===== MOTOR SPEED =====
  rightSpeed = baseRight - PID;
  leftSpeed  = baseLeft  + PID;

  // LIMIT
  rightSpeed = constrain(rightSpeed, 0, 255);
  leftSpeed  = constrain(leftSpeed, 0, 255);

  moveMotor(leftSpeed, rightSpeed);

  // DEBUG
  Serial.print("Error: ");
  Serial.println(error);
}

// ===== MOTOR =====
void moveMotor(int left, int right) {
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);

  analogWrite(ENA, right);
  analogWrite(ENB, left);
}
