// Capteurs Ultrasoniques
#define TRIG_PIN_CENTER 10
#define ECHO_PIN_CENTER 11

#define TRIG_PIN_LEFT 8
#define ECHO_PIN_LEFT 9

#define TRIG_PIN_RIGHT 12
#define ECHO_PIN_RIGHT 13

// Moteurs
int enA = 6;
int in1 = 7;
int in2 = 5;  // Moteur gauche
int enB = 3;
int in3 = 4;
int in4 = 2;  // Moteur droit
              // Moteur droit
// Paramètres généraux
const unsigned long US_PERIOD = 100;  // Période entre les lectures (ms)
const int MAX_DISTANCE = 100;         // Distance maximale (cm)
const int TARGET_DISTANCE = 35;       // Distance seuil pour déclencher une action (cm)
int maxSpeed = 255;                   // Vitesse maximale des moteurs

// Variables de distance
int distanceCenter = MAX_DISTANCE;
int distanceLeft = MAX_DISTANCE;
int distanceRight = MAX_DISTANCE;

// Variables d'état
String action;

void setup() {

  // Configuration des broches capteurs
  pinMode(TRIG_PIN_CENTER, OUTPUT);
  pinMode(ECHO_PIN_CENTER, INPUT);

  pinMode(TRIG_PIN_LEFT, OUTPUT);
  pinMode(ECHO_PIN_LEFT, INPUT);

  pinMode(TRIG_PIN_RIGHT, OUTPUT);
  pinMode(ECHO_PIN_RIGHT, INPUT);

  // Configuration des broches moteurs
  pinMode(enA, OUTPUT);
  pinMode(enB, OUTPUT);
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  pinMode(in3, OUTPUT);
  pinMode(in4, OUTPUT);

  // Initialisation Serial
  Serial.begin(9600);
  delay(4000);
  //forward(150);
   //delay(750);
}

void loop() {
  // Lire les distances de chaque capteur
  distanceCenter = measureDistance(TRIG_PIN_CENTER, ECHO_PIN_CENTER);
  distanceLeft = measureDistance(TRIG_PIN_LEFT, ECHO_PIN_LEFT);
  distanceRight = measureDistance(TRIG_PIN_RIGHT, ECHO_PIN_RIGHT);

  // Affichage des distances dans le Serial
  Serial.print("Gauche: ");
  Serial.print(distanceLeft);
  Serial.print(" cm | Centre: ");
  Serial.print(distanceCenter);
  Serial.print(" cm | Droite: ");
  Serial.print(distanceRight);
  Serial.println(" cm");

  // Logique d'attaque et de recherche
  attackLogic();
}

// Fonction pour mesurer la distance d'un capteur
int measureDistance(int trigPin, int echoPin) {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  long duration = pulseIn(echoPin, HIGH, MAX_DISTANCE * 58);
  int distance = duration * 0.034 / 2;  // Convertir en cm

  if (distance > MAX_DISTANCE) distance = MAX_DISTANCE;
  if (distance <= 0) distance = MAX_DISTANCE;

  return distance;
}

// Logique pour attaquer ou rechercher
void attackLogic() {
  if (distanceCenter <= TARGET_DISTANCE) {
    forward(maxSpeed);
    action = "Attaquer!";
    delay(100);
  } else if (distanceLeft <= TARGET_DISTANCE) {
    while (1) {
      turnLEFTT(255);
      measureDistance(TRIG_PIN_CENTER, ECHO_PIN_CENTER);
      if (measureDistance(TRIG_PIN_CENTER, ECHO_PIN_CENTER) <= TARGET_DISTANCE) {
        break;
      }
    }

    action = "Recentrage (gauche)";
  } else if (distanceRight <= TARGET_DISTANCE) {
    while (1) {
      turnRight(255);
      measureDistance(TRIG_PIN_CENTER, ECHO_PIN_CENTER);
      if (measureDistance(TRIG_PIN_CENTER, ECHO_PIN_CENTER) <= TARGET_DISTANCE) {
        break;
      }
    }


    action = "Recentrage (droite)";
  } else {

    cherch(160);  // Recherche lente
    action = "Recherche...";
  }
 if (distanceLeft <= 5 && 1 <= distanceLeft) {
    while (1) {
      turnLEFTT(255);
      measureDistance(TRIG_PIN_CENTER, ECHO_PIN_CENTER);
      if (measureDistance(TRIG_PIN_CENTER, ECHO_PIN_CENTER) <= TARGET_DISTANCE) {
        break;
      }
    }

    action = "Recentrage (gauche)";
  }
  if (distanceRight <= 5 && 1 <= distanceRight) {
    while (1) {
      turnRight(255);
      measureDistance(TRIG_PIN_CENTER, ECHO_PIN_CENTER);
      if (measureDistance(TRIG_PIN_CENTER, ECHO_PIN_CENTER) <= TARGET_DISTANCE) {
        break;
      }
    }


    action = "Recentrage (droite)";
 }
  Serial.println(action);
}

// Fonctions de contrôle des moteurs
void forward(int ajri) {
  digitalWrite(in1, LOW);
  digitalWrite(in2, HIGH);
  digitalWrite(in3, LOW);
  digitalWrite(in4, HIGH);

  analogWrite(enA, ajri);
  analogWrite(enB, ajri);
}

void turnRight(int tempSpeed) {
  digitalWrite(in1, LOW);
  digitalWrite(in2, HIGH);
  digitalWrite(in3, HIGH);
  digitalWrite(in4, LOW);

  analogWrite(enA, tempSpeed);
  analogWrite(enB, tempSpeed);
}

void turnLEFTT(int tempSpeed) {
  digitalWrite(in1, HIGH);
  digitalWrite(in2, LOW);
  digitalWrite(in3, LOW);
  digitalWrite(in4, HIGH);

  analogWrite(enA, tempSpeed);
  analogWrite(enB, tempSpeed);
}
void cherch(int temp) {
  digitalWrite(in1, LOW);
  digitalWrite(in2, HIGH);
  digitalWrite(in3, HIGH);
  digitalWrite(in4, LOW);

  analogWrite(enA, temp);
  analogWrite(enB, temp);
}
