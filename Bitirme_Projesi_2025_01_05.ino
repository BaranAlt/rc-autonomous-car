// Sensör pinleri
#define LEFT_SENSOR A0    // Şerit sensörü - sol
#define RIGHT_SENSOR A1   // Şerit sensörü - sağ
const int TRIG_PIN = 12;   // Ultrasonik sensör
const int ECHO_PIN = 13;

// Motor pinleri
const int SAG_ENABLE = 3;
const int SAG_ILERI = 5;
const int SAG_GERI = 6;
const int SOL_ILERI = 9;
const int SOL_GERI = 10;
const int SOL_ENABLE = 11;

// LED pinleri
#define LED1 2
#define LED2 3
#define LED3 4
#define LED4 7

// Sabitler
#define THRESHOLD 500     // Şerit algılama eşiği
#define OBSTACLE_DIST 20  // Engel mesafesi (cm)
#define MOTOR_HIZ 70     // Normal hız
#define MAX_HIZ 70       // Maksimum hız

int sure = 0;
int mesafe = 0;

void setup() {
  // Sensör pinleri
  pinMode(LEFT_SENSOR, INPUT);
  pinMode(RIGHT_SENSOR, INPUT);
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  
  // Motor pinleri
  pinMode(SAG_ILERI, OUTPUT);
  pinMode(SAG_GERI, OUTPUT);
  pinMode(SOL_ILERI, OUTPUT);
  pinMode(SOL_GERI, OUTPUT);
  pinMode(SAG_ENABLE, OUTPUT);
  pinMode(SOL_ENABLE, OUTPUT);
  
  // LED pinleri
  pinMode(LED1, OUTPUT);
  pinMode(LED2, OUTPUT);
  pinMode(LED3, OUTPUT);
  pinMode(LED4, OUTPUT);
  
  Serial.begin(9600);
  Serial.println("Sistem hazır");
}

void loop() {
  // Mesafe ölçümü
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);
  sure = pulseIn(ECHO_PIN, HIGH);
  mesafe = (sure / 2) / 29.1;
  
  // Şerit takibi veya engel kontrolü
  if (mesafe < OBSTACLE_DIST) {
    engeldenKac();
  } else {
    seritTakibi(); // Şerit takibi fonksiyonu
  }

  // Bluetooth kontrolü
  if (Serial.available()) {
    char veri = Serial.read();
    switch(veri) {
      case 'F': ileri(); break;
      case 'B': geri(); break;
      case 'L': solaDon(); break;
      case 'R': sagaDon(); break;
      case 'S': dur(); break;
    }
  }
  
  delay(10);
}

void seritTakibi() {
  // Şerit sensör değerleri
  int solSerit = analogRead(LEFT_SENSOR);
  int sagSerit = analogRead(RIGHT_SENSOR);

  // LED göstergeleri
  digitalWrite(LED1, solSerit > THRESHOLD);
  digitalWrite(LED2, sagSerit > THRESHOLD);
  digitalWrite(LED3, solSerit <= THRESHOLD);
  digitalWrite(LED4, sagSerit <= THRESHOLD);

  // Şerit takibi
  if (solSerit > THRESHOLD && sagSerit > THRESHOLD) {
    ileri();
  } else if (solSerit <= THRESHOLD && sagSerit > THRESHOLD) {
    sagaDon();
  } else if (sagSerit <= THRESHOLD && solSerit > THRESHOLD) {
    solaDon();
  } else {
    dur();
  }
}

void engeldenKac() {
  dur(); // Engel algılandığında aracı durdur

  // Engelin kalkmasını beklemek için bir döngü
  while (true) {
    // Mesafeyi tekrar ölç
    digitalWrite(TRIG_PIN, HIGH);
    delayMicroseconds(10);
    digitalWrite(TRIG_PIN, LOW);
    sure = pulseIn(ECHO_PIN, HIGH);
    mesafe = (sure / 2) / 29.1;

    // Engel kalktıysa döngüden çık
    if (mesafe >= OBSTACLE_DIST) {
      break;
    }
    
    delay(10); // Küçük bir gecikme
  }
}

void ileri() {
  digitalWrite(SAG_ILERI, HIGH);
  digitalWrite(SAG_GERI, LOW);
  digitalWrite(SOL_ILERI, HIGH);
  digitalWrite(SOL_GERI, LOW);
  analogWrite(SAG_ENABLE, MAX_HIZ);
  analogWrite(SOL_ENABLE, MAX_HIZ);
}

void geri() {
  digitalWrite(SAG_ILERI, LOW);
  digitalWrite(SAG_GERI, HIGH);
  digitalWrite(SOL_ILERI, LOW);
  digitalWrite(SOL_GERI, HIGH);
  analogWrite(SAG_ENABLE, MOTOR_HIZ);
  analogWrite(SOL_ENABLE, MOTOR_HIZ);
}

void solaDon() {
  digitalWrite(SAG_ILERI, HIGH);
  digitalWrite(SAG_GERI, LOW);
  digitalWrite(SOL_ILERI, LOW);
  digitalWrite(SOL_GERI, HIGH);
  analogWrite(SAG_ENABLE, MOTOR_HIZ);
  analogWrite(SOL_ENABLE, MOTOR_HIZ);
}

void sagaDon() {
  digitalWrite(SAG_ILERI, LOW);
  digitalWrite(SAG_GERI, HIGH);
  digitalWrite(SOL_ILERI, HIGH);
  digitalWrite(SOL_GERI, LOW);
  analogWrite(SAG_ENABLE, MOTOR_HIZ);
  analogWrite(SOL_ENABLE, MOTOR_HIZ);
}

void dur() {
  digitalWrite(SAG_ILERI, LOW);
  digitalWrite(SAG_GERI, LOW);
  digitalWrite(SOL_ILERI, LOW);
  digitalWrite(SOL_GERI, LOW);
  analogWrite(SAG_ENABLE, 0);
  analogWrite(SOL_ENABLE, 0);
}