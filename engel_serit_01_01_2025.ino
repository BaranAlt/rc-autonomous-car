// Sensör pinleri
#define LEFT_SENSOR A0    // Şerit sensörü - sol
#define RIGHT_SENSOR A1   // Şerit sensörü - sağ
const int TRIG_PIN = 12;   // Ultrasonik sensör
const int ECHO_PIN = 13;

// Motor pinleri
const int SAG_ENABLE = 6;
const int SAG_ILERI = 8;
const int SAG_GERI = 7;
const int SOL_ILERI = 10;
const int SOL_GERI = 9;
const int SOL_ENABLE = 11;

// LED pinleri tanıttım gene sonra eklersek diye 
#define LED1 2
#define LED2 3
#define LED3 4
#define LED4 7

// Sabitler (hız biraz yüksek gibi) 
#define THRESHOLD 500     // Şerit algılama eşiği
#define OBSTACLE_DIST 20  // Engel mesafesi (cm)
#define MOTOR_HIZ 200    // Normal hız
#define MAX_HIZ 255      // Maksimum hız

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
// tekrarı önlemek için serit takibi bir fonksiyon yarattım
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
  dur();
  delay(1000);
  
  // Sola dön
  solaDon();
  delay(1000);
  
  // Şerit takibi ile ilerleme
  while (true) {
    seritTakibi(); // Şerit takibi fonksiyonunu tekrar kullanıyoruz

    // Mesafe tekrar kontrol edilerek döngüden çıkış sağlanabilir
    digitalWrite(TRIG_PIN, HIGH);
    delayMicroseconds(10);
    digitalWrite(TRIG_PIN, LOW);
    sure = pulseIn(ECHO_PIN, HIGH);
    mesafe = (sure / 2) / 29.1;

    if (mesafe >= OBSTACLE_DIST) {
      break; // Engel kalmadıysa döngüden çık
    }

    delay(10); // Küçük bir gecikme ekleniyor
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
