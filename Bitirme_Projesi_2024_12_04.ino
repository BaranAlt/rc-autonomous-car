// IR sensör pinleri
#define LEFT_SENSOR A0  // Sol sensör
#define RIGHT_SENSOR A1 // Sağ sensör

// Motor sürücü pinleri
#define LEFT_MOTOR_FORWARD 5
#define LEFT_MOTOR_BACKWARD 6
#define RIGHT_MOTOR_FORWARD 9
#define RIGHT_MOTOR_BACKWARD 10
#define ENA 3  // Sol motor için PWM pin
#define ENB 11 // Sağ motor için PWM pin

// LED pinleri
#define LED1 2
#define LED2 3
#define LED3 4
#define LED4 7

// Bluetooth modülü pinleri
#define HC05_TX 10
#define HC05_RX 11

// Sensör eşik değeri
#define THRESHOLD 500   // Eşik değeri, sensör değerlerinin beyaz veya siyah olduğunu belirler

// Başlangıç hızı (PWM değeri)
int speed = 70;

void setup() {
  // Sensör pinlerini giriş olarak ayarla
  pinMode(LEFT_SENSOR, INPUT);
  pinMode(RIGHT_SENSOR, INPUT);

  // Motor pinlerini çıkış olarak ayarla
  pinMode(LEFT_MOTOR_FORWARD, OUTPUT);
  pinMode(LEFT_MOTOR_BACKWARD, OUTPUT);
  pinMode(RIGHT_MOTOR_FORWARD, OUTPUT);
  pinMode(RIGHT_MOTOR_BACKWARD, OUTPUT);

  // PWM pinlerini çıkış olarak ayarla
  pinMode(ENA, OUTPUT);  
  pinMode(ENB, OUTPUT);  

  // LED pinlerini çıkış olarak ayarla
  pinMode(LED1, OUTPUT);
  pinMode(LED2, OUTPUT);
  pinMode(LED3, OUTPUT);
  pinMode(LED4, OUTPUT);

  // Bluetooth modülünü başlat
  Serial.begin(9600);
  Serial.println("Bluetooth Hazır");
}

void loop() {
  // Bluetooth üzerinden gelen komutları kontrol et
  if (Serial.available()) {
    char command = Serial.read();
    handleBluetoothCommand(command);
  }

  // Şerit izleme mantığı
  int leftValue = analogRead(LEFT_SENSOR);  // Sol sensör verisi
  int rightValue = analogRead(RIGHT_SENSOR); // Sağ sensör verisi

  // Seri monitörde sensör değerlerini yazdır
  Serial.print("Left: ");
  Serial.print(leftValue);
  Serial.print(" | Right: ");
  Serial.println(rightValue);

  // LED'leri sensör durumuna göre kontrol et
  digitalWrite(LED1, leftValue > THRESHOLD ? HIGH : LOW);  // Sol sensör beyazsa LED1'i yak
  digitalWrite(LED2, rightValue > THRESHOLD ? HIGH : LOW); // Sağ sensör beyazsa LED2'yi yak
  digitalWrite(LED3, leftValue <= THRESHOLD ? HIGH : LOW);  // Sol sensör siyahsa LED3'ü yak
  digitalWrite(LED4, rightValue <= THRESHOLD ? HIGH : LOW); // Sağ sensör siyahsa LED4'ü yak

  // Şerit izleme mantığı:
  if (leftValue > THRESHOLD && rightValue > THRESHOLD) {
    // Şerit ortada, ileri hareket
    moveForward();
  } else if (leftValue <= THRESHOLD && rightValue > THRESHOLD) {
    // Sol sensör kayboldu, sağa dön
    turnRight();
  } else if (rightValue <= THRESHOLD && leftValue > THRESHOLD) {
    // Sağ sensör kayboldu, sola dön
    turnLeft();
  } else {
    // Şerit kayboldu, dur
    stopMotors();
  }

  delay(10); // Döngüyü yavaşlat
}

// Bluetooth komutlarını işleme
void handleBluetoothCommand(char command) {
  switch (command) {
    case 'F':
      moveForward();
      break;
    case 'B':
      moveBackward();
      break;
    case 'L':
      turnLeft();
      break;
    case 'R':
      turnRight();
      break;
    case 'S':
      stopMotors();
      break;
    default:
      Serial.println("Bilinmeyen Komut");
      break;
  }
}

void moveForward() {
  analogWrite(ENA, speed);  // Sol motor hızı
  analogWrite(ENB, speed);  // Sağ motor hızı
  digitalWrite(LEFT_MOTOR_FORWARD, HIGH);
  digitalWrite(RIGHT_MOTOR_FORWARD, HIGH);
  digitalWrite(LEFT_MOTOR_BACKWARD, LOW);
  digitalWrite(RIGHT_MOTOR_BACKWARD, LOW);
}

void moveBackward() {
  analogWrite(ENA, speed);  // Sol motor hızı
  analogWrite(ENB, speed);  // Sağ motor hızı
  digitalWrite(LEFT_MOTOR_FORWARD, LOW);
  digitalWrite(RIGHT_MOTOR_FORWARD, LOW);
  digitalWrite(LEFT_MOTOR_BACKWARD, HIGH);
  digitalWrite(RIGHT_MOTOR_BACKWARD, HIGH);
}

void turnLeft() {
  analogWrite(ENA, speed / 2); // Sol motoru yavaşlat
  analogWrite(ENB, speed);
  digitalWrite(LEFT_MOTOR_FORWARD, LOW);
  digitalWrite(RIGHT_MOTOR_FORWARD, HIGH);
  digitalWrite(LEFT_MOTOR_BACKWARD, LOW);
  digitalWrite(RIGHT_MOTOR_BACKWARD, LOW);
}

void turnRight() {
  analogWrite(ENA, speed);
  analogWrite(ENB, speed / 2); // Sağ motoru yavaşlat
  digitalWrite(LEFT_MOTOR_FORWARD, HIGH);
  digitalWrite(RIGHT_MOTOR_FORWARD, LOW);
  digitalWrite(LEFT_MOTOR_BACKWARD, LOW);
  digitalWrite(RIGHT_MOTOR_BACKWARD, LOW);
}

void stopMotors() {
  analogWrite(ENA, 0);  // Sol motoru durdur
  analogWrite(ENB, 0);  // Sağ motoru durdur
  digitalWrite(LEFT_MOTOR_FORWARD, LOW);
  digitalWrite(RIGHT_MOTOR_FORWARD, LOW);
  digitalWrite(LEFT_MOTOR_BACKWARD, LOW);
  digitalWrite(RIGHT_MOTOR_BACKWARD, LOW);
}
