#include "HX711.h"
#include <Servo.h> // Biblioteca pentru servo

// Definire pini HX711
const int LOADCELL_DOUT_PIN = A1;
const int LOADCELL_SCK_PIN = A0;

// Definire pin fotorezistor
const int photoresistorPin = A3;

// Definire pini LED-uri
const int led1Pin = 8;

// Senzor distanta
const int trigPin = 13;  // Pinul Trig
const int echoPin = 12;  // Pinul Echo

// Servo
Servo motorServo; // Obiect pentru servo
const int servoPin = 11; // Pinul PWM la care este conectat servo-ul

// Creare obiect HX711
HX711 scale;

// Factor de calibrare (valoare inițială ajustabilă)
float calibration_factor = -7050;

bool calibrareFinalizata = false;

void setup() {
  Serial.begin(9600);
  if (!calibrareFinalizata) {
    Serial.println("HX711 Demo");
    Serial.println("------------------");

    // Inițializare HX711
    scale.begin(LOADCELL_DOUT_PIN, LOADCELL_SCK_PIN);
    if (!scale.is_ready()) {
      Serial.println("Eroare: HX711 nu este gata. Verifică conexiunile!");
      while (1); // Stop execuția
    }

    // Resetare la zero
    Serial.println("Golește cântarul, apoi apasă o tastă pentru a continua.");
    waitForUserInput();
    scale.tare();

    Serial.println("Cântarul a fost resetat la zero.");

    // Calibrare manuală
    Serial.println("Pune 1000 grame pe cântar, apoi apasă o tastă pentru a continua.");
    waitForUserInput();
    float raw_units = scale.get_units(10);
    calibration_factor = raw_units / 1000.0;
    scale.set_scale(calibration_factor);

    Serial.print("Calibrare completă. Factor de calibrare: ");
    Serial.println(calibration_factor);

    calibrareFinalizata = true; // Setează flag-ul pentru calibrare

   pinMode(trigPin, OUTPUT); // Setăm pinul Trig ca OUTPUT
   pinMode(echoPin, INPUT); // Setăm pinul Echo ca INPUT
   Serial.println("Senzor dist gata!");
  }

  // Continuă inițializarea
  pinMode(led1Pin, OUTPUT);
  //initializareSenzor();
  motorServo.attach(servoPin);
    Serial.println("Servo gata!");
}

void loop() {
  // Citire greutate
  float weight = scale.get_units(10);
  Serial.print("Greutate: ");
  Serial.print(weight);
  Serial.println(" grame");

  // Citire valoare fotorezistor
  int lightLevel = analogRead(photoresistorPin);
  Serial.print("Nivelul de lumină: ");
  Serial.println(lightLevel);

  // Control LED-uri în funcție de luminozitate
  controlLEDs(lightLevel);

  // Afisare distanta
  float distanta = masoaraDistanta(); // Apelăm funcția de măsurare a distanței
  afiseazaDistanta(distanta); // Afișăm distanța

  // Servo
  if(distanta < 10 && weight < 20) {
    motorServo.write(100);
  } else if (weight >= 20) {
    motorServo.write(70);
  }

  delay(500);
}

// Funcție pentru controlul LED-urilor în funcție de nivelul de lumină
void controlLEDs(int lightLevel) {
  if (lightLevel > 600) { // Luminozitate ridicată
    digitalWrite(led1Pin, LOW); // LED-uri stinse
  } else { // Luminozitate scăzută
    digitalWrite(led1Pin, HIGH); // LED-uri aprinse
  }
}

// Funcție pentru așteptarea inputului utilizatorului
void waitForUserInput() {
  while (Serial.available() > 0) { // Curăță buffer-ul serial
    Serial.read();
  }
  while (!Serial.available()) { // Așteaptă o tastă
    delay(10);
  }
  while (Serial.available() > 0) { // Asigură-te că buffer-ul este din nou curat
    Serial.read();
  }
}


/*void initializareSenzor() {
  pinMode(trigPin, OUTPUT); // Setăm pinul Trig ca OUTPUT
  pinMode(echoPin, INPUT); // Setăm pinul Echo ca INPUT
  Serial.println("Senzor gata!");
}*/

// Funcții senzor distanță
float masoaraDistanta() {
  // Trimit un impuls de 10µs către Trig
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  
  // Citim durata semnalului de pe Echo (în microsecunde)
  long durata = pulseIn(echoPin, HIGH);

  // Calculăm distanța în centimetri
  float distanta = durata * 0.034 / 2;
  return distanta; // Returnăm distanța calculată
}

// Funcție pentru afișarea distanței
void afiseazaDistanta(float distanta) {
  Serial.print("Distanța: ");
  Serial.print(distanta);
  Serial.println(" cm");
}
