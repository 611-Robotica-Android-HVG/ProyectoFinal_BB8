// ESP CONTROL → v, w + 2 servos por incremento

const int btnAdelante  = 12;
const int btnAtras     = 14;
const int btnIzquierda = 27;
const int btnDerecha   = 26;


const int btnServo1Up   = 16;
const int btnServo1Down = 17;
const int btnServo2Up   = 5;
const int btnServo2Down = 18; 

// --------------------
// velocidades robot
// --------------------
float v = 0.0;
float w = 0.0;

float v_base = 0.5;
float w_base = 1.2;

float dv = 0.1;
float dw = 0.2;

float v_max = 2.0;
float v_min = -2.0;
float w_max = 3.0;
float w_min = -3.0;

// --------------------
// SERVOS
// --------------------
int servo1 = 0;
int servo2 = 0;

int dservo = 5;   // incremento grados

int servo_min = -90;
int servo_max = 90;

// --------------------
// flancos
// --------------------
bool prevA = false;
bool prevB = false;
bool prevI = false;
bool prevD = false;

bool prevS1U = false;
bool prevS1D = false;
bool prevS2U = false;
bool prevS2D = false;

void setup() {
  Serial.begin(115200);

  pinMode(btnAdelante, INPUT_PULLUP);
  pinMode(btnAtras, INPUT_PULLUP);
  pinMode(btnIzquierda, INPUT_PULLUP);
  pinMode(btnDerecha, INPUT_PULLUP);

  pinMode(btnServo1Up, INPUT_PULLUP);
  pinMode(btnServo1Down, INPUT_PULLUP);
  pinMode(btnServo2Up, INPUT_PULLUP);
  pinMode(btnServo2Down, INPUT_PULLUP);
}

void loop() {

  // -------------------------
  // lectura botones (activo LOW)
  // -------------------------
  bool adelante  = !digitalRead(btnAdelante);
  bool atras     = !digitalRead(btnAtras);
  bool izquierda = !digitalRead(btnIzquierda);
  bool derecha   = !digitalRead(btnDerecha);

  bool s1_up   = !digitalRead(btnServo1Up);
  bool s1_down = !digitalRead(btnServo1Down);
  bool s2_up   = !digitalRead(btnServo2Up);
  bool s2_down = !digitalRead(btnServo2Down);

  // -------------------------
  // ROBOT v, w
  // -------------------------
  if (adelante && !prevA) v += dv;
  if (atras && !prevB)    v -= dv;

  if (izquierda && !prevI) w += dw;
  if (derecha && !prevD)   w -= dw;

  // -------------------------
  // SERVOS (incremento por click)
  // -------------------------
  if (s1_up && !prevS1U)   servo1 += dservo;
  if (s1_down && !prevS1D) servo1 -= dservo;

  if (s2_up && !prevS2U)   servo2 += dservo;
  if (s2_down && !prevS2D) servo2 -= dservo;

  // -------------------------
  // saturación
  // -------------------------
  if (v > v_max) v = v_max;
  if (v < v_min) v = v_min;

  if (w > w_max) w = w_max;
  if (w < w_min) w = w_min;

  if (servo1 > servo_max) servo1 = servo_max;
  if (servo1 < servo_min) servo1 = servo_min;

  if (servo2 > servo_max) servo2 = servo_max;
  if (servo2 < servo_min) servo2 = servo_min;

  // -------------------------
  // envío a Python
  // formato:
  // v,w,s1,s2
  // -------------------------
  Serial.print(v);
  Serial.print(",");
  Serial.print(w);
  Serial.print(",");
  Serial.print(servo1);
  Serial.print(",");
  Serial.println(servo2);

  // -------------------------
  // guardar estados previos
  // -------------------------
  prevA = adelante;
  prevB = atras;
  prevI = izquierda;
  prevD = derecha;

  prevS1U = s1_up;
  prevS1D = s1_down;
  prevS2U = s2_up;
  prevS2D = s2_down;

  delay(50);
}