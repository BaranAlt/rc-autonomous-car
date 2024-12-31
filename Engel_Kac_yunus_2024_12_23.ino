int sure = 0;
int mesafe = 0;

const int trig_pin = 3; 
const int echo_pin = 2;  
const int sag_enable = 6;    // ENABLE B (Sağ motor hız kontrolü)
const int sag_ileri = 8;     // INPUT 3 (Sağ motor ileri kontrolü)
const int sag_geri = 7;      // INPUT 4 (Sağ motor geri kontrolü)
const int sol_ileri = 10;    // INPUT 1 (Sol motor ileri kontrolü)
const int sol_geri = 9;      // INPUT 2 (Sol motor geri kontrolü)
const int sol_enable = 11;   // ENABLE A (Sol motor hız kontrolü)

void setup() {
  pinMode(trig_pin, OUTPUT); 
  pinMode(echo_pin, INPUT ); 
  pinMode(sag_ileri, OUTPUT); 
  pinMode(sag_geri, OUTPUT);
  pinMode(sol_ileri, OUTPUT);
  pinMode(sol_geri, OUTPUT);
  pinMode(sag_enable, OUTPUT);
  pinMode(sol_enable, OUTPUT);

  Serial.begin(9600); 

  Serial.println("Bluetooth modülü bağlanmayı bekliyor...");
}

void loop() {
  // Ultrasonik sensörle mesafe ölçümü
  digitalWrite(trig_pin, HIGH); 
  delayMicroseconds(10);   
  digitalWrite(trig_pin, LOW);
  sure = pulseIn(echo_pin, HIGH);
  mesafe = (sure / 2) / 29.1;  // Mesafeyi cm cinsinden hesapla
  Serial.print("Mesafe: ");
  Serial.println(mesafe);

  // Engelden kaçma mekanizması
  if (mesafe < 20) {  // Mesafe 20 cm'den küçükse
    dur();
    delay(500);
    geri();
    delay(500);
    solaDon();
    delay(500);
  } else {  // Mesafe 20 cm'den büyükse
    ileri();
  }

  // Bluetooth kontrolü
  if (Serial.available()) {    // Bluetooth'tan gelen veri var mı?
    char veri = Serial.read(); // Veriyi oku
    Serial.print("Bluetooth veri: ");
    Serial.println(veri);

    if (veri == 'F') ileri();       // 'F' ile ileri git
    else if (veri == 'R') sagaDon(); // 'R' ile sağa dön
    else if (veri == 'L') solaDon(); // 'L' ile sola dön
    else if (veri == 'B') geri();    // 'B' ile geri git
    else if (veri == 'S') dur();     // 'S' ile dur
  }
}

// Araç hareket fonksiyonları
void ileri() {
  digitalWrite(sag_ileri, HIGH);
  digitalWrite(sag_geri, LOW);
  digitalWrite(sol_ileri, HIGH);
  digitalWrite(sol_geri, LOW);
  analogWrite(sag_enable, 255); // Maksimum hız
  analogWrite(sol_enable, 255);
}

void geri() {
  digitalWrite(sag_ileri, LOW);
  digitalWrite(sag_geri, HIGH);
  digitalWrite(sol_ileri, LOW);
  digitalWrite(sol_geri, HIGH);
  analogWrite(sag_enable, 200); // Orta hız
  analogWrite(sol_enable, 200);
}

void solaDon() {
  digitalWrite(sag_ileri, HIGH);
  digitalWrite(sag_geri, LOW);
  digitalWrite(sol_ileri, LOW);
  digitalWrite(sol_geri, HIGH);
  analogWrite(sag_enable, 200); // Orta hız
  analogWrite(sol_enable, 200);
}

void sagaDon() {
  digitalWrite(sag_ileri, LOW);
  digitalWrite(sag_geri, HIGH);
  digitalWrite(sol_ileri, HIGH);
  digitalWrite(sol_geri, LOW);
  analogWrite(sag_enable, 200); // Orta hız
  analogWrite(sol_enable, 200);
}

void dur() {
  digitalWrite(sag_ileri, LOW);
  digitalWrite(sag_geri, LOW);
  digitalWrite(sol_ileri, LOW);
  digitalWrite(sol_geri, LOW);
  analogWrite(sag_enable, 0);
  analogWrite(sol_enable, 0);
}
