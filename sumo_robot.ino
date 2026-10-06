// CortexANis
// Sumo Robot sketch commenté
// Matériel: Arduino Nano, 3x HC-SR04 (centre, gauche, droite), pilote L298N
// Ce fichier contient les définitions de broches, la logique de recherche/attaque
// et les fonctions de contrôle des moteurs. Les commentaires expliquent chaque
// section pour faciliter la compréhension et la maintenance.

// ===== Capteurs Ultrasoniques (HC-SR04) =====
// Chaque capteur utilise une broche TRIG et une broche ECHO
#define TRIG_PIN_CENTER 10
#define ECHO_PIN_CENTER 11

#define TRIG_PIN_LEFT 8
#define ECHO_PIN_LEFT 9

#define TRIG_PIN_RIGHT 12
#define ECHO_PIN_RIGHT 13

// ===== Pilotage des moteurs (L298N) =====
// enA/enB: broches PWM pour régler la vitesse
// in1..in4: broches de direction pour chaque moteur via L298N
int enA = 6;   // PWM moteur gauche
int in1 = 7;   // direction A1
int in2 = 5;   // direction A2
int enB = 3;   // PWM moteur droit
int in3 = 4;   // direction B1
int in4 = 2;   // direction B2

// ===== Paramètres généraux =====
const unsigned long US_PERIOD = 100;  // Période entre lectures ultrason (ms)
const int MAX_DISTANCE = 100;         // Distance max mesurée (cm)
const int TARGET_DISTANCE = 35;       // Seuil pour déclencher l'attaque (cm)
int maxSpeed = 255;                   // Vitesse maximale (0-255 PWM)

// ===== Variables d'état et mesures =====
// Distances mesurées (initialisées à la valeur max)
int distanceCenter = MAX_DISTANCE;
int distanceLeft = MAX_DISTANCE;
int distanceRight = MAX_DISTANCE;

// Chaîne pour afficher l'action courante dans le Serial
String action;

void setup() {

  // Configuration des broches capteurs: TRIG en OUTPUT, ECHO en INPUT
  pinMode(TRIG_PIN_CENTER, OUTPUT);
  pinMode(ECHO_PIN_CENTER, INPUT);

  pinMode(TRIG_PIN_LEFT, OUTPUT);
  pinMode(ECHO_PIN_LEFT, INPUT);

  pinMode(TRIG_PIN_RIGHT, OUTPUT);
  pinMode(ECHO_PIN_RIGHT, INPUT);

  // Configuration des broches moteurs en sortie
  pinMode(enA, OUTPUT);
  pinMode(enB, OUTPUT);
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  pinMode(in3, OUTPUT);
  pinMode(in4, OUTPUT);

  // Initialisation du moniteur série pour debug
  Serial.begin(9600);
  delay(4000); // délai pour laisser le moniteur série se stabiliser

  // Option: départ en avant court pour vérifier les moteurs
  // forward(150); delay(750);
}

void loop() {
  // Lire les distances de chaque capteur HC-SR04
  distanceCenter = measureDistance(TRIG_PIN_CENTER, ECHO_PIN_CENTER);
  distanceLeft = measureDistance(TRIG_PIN_LEFT, ECHO_PIN_LEFT);
  distanceRight = measureDistance(TRIG_PIN_RIGHT, ECHO_PIN_RIGHT);

  // Affichage des distances pour diagnostic (moniteur série)
  Serial.print("Gauche: "); Serial.print(distanceLeft);
  Serial.print(" cm | Centre: "); Serial.print(distanceCenter);
  Serial.print(" cm | Droite: "); Serial.print(distanceRight);
  Serial.println(" cm");

  // Appeler la logique principale qui décide du mouvement
  attackLogic();

  // Petite pause pour limiter la fréquence des mesures
  delay(US_PERIOD);
}

// ===== Mesure de distance à l'aide d'un HC-SR04 =====
// trigPin: broche TRIG du capteur (OUTPUT)
// echoPin: broche ECHO du capteur (INPUT)
// Renvoie la distance en cm (bounded par MAX_DISTANCE)
int measureDistance(int trigPin, int echoPin) {
  // Envoyer une impulsion de 10µs sur TRIG
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  // Lire la durée du signal sur ECHO (timeout basé sur MAX_DISTANCE)
  long duration = pulseIn(echoPin, HIGH, MAX_DISTANCE * 58);
  int distance = duration * 0.034 / 2;  // conversion µs -> cm

  // Saturation des valeurs invalides
  if (distance > MAX_DISTANCE) distance = MAX_DISTANCE;
  if (distance <= 0) distance = MAX_DISTANCE;

  return distance;
}

// ===== Logique principale: recherche et attaque =====
// Le principe:
// - Si le capteur central voit l'adversaire proche -> attaquer en avant
// - Sinon si un capteur latéral voit l'adversaire -> réaligner le robot
// - Sinon -> chercher (tourner/avancer lentement)
void attackLogic() {
  // Si l'adversaire est détecté devant -> avancer plein gaz
  if (distanceCenter <= TARGET_DISTANCE) {
    forward(maxSpeed);
    action = "Attaquer!";
    delay(100);

  // Si l'adversaire est à gauche -> tourner à gauche jusqu'à le centrer
  } else if (distanceLeft <= TARGET_DISTANCE) {
    while (1) {
      turnLEFTT(255); // tourner rapidement à gauche
      // vérifier si centre voit maintenant l'adversaire
      if (measureDistance(TRIG_PIN_CENTER, ECHO_PIN_CENTER) <= TARGET_DISTANCE) {
        break; // centré -> sortir de la boucle
      }
    }
    action = "Recentrage (gauche)";

  // Si à droite -> tourner à droite jusqu'au centrage
  } else if (distanceRight <= TARGET_DISTANCE) {
    while (1) {
      turnRight(255);
      if (measureDistance(TRIG_PIN_CENTER, ECHO_PIN_CENTER) <= TARGET_DISTANCE) {
        break;
      }
    }
    action = "Recentrage (droite)";

  } else {
    // Aucun adversaire proche -> comportement de recherche
    cherch(160);  // déplacement de recherche (vitesse moyenne)
    action = "Recherche...";
  }

  // Protection supplémentaire: si un capteur latéral est très proche (<5cm)
  // on effectue un recentrage immédiat (prévenir collision ou contact non désiré)
  if (distanceLeft <= 5 && distanceLeft >= 1) {
    while (1) {
      turnLEFTT(255);
      if (measureDistance(TRIG_PIN_CENTER, ECHO_PIN_CENTER) <= TARGET_DISTANCE) {
        break;
      }
    }
    action = "Recentrage (gauche)";
  }
  if (distanceRight <= 5 && distanceRight >= 1) {
    while (1) {
      turnRight(255);
      if (measureDistance(TRIG_PIN_CENTER, ECHO_PIN_CENTER) <= TARGET_DISTANCE) {
        break;
      }
    }
    action = "Recentrage (droite)";
  }

  // Afficher l'action choisie pour debug
  Serial.println(action);
}

// ===== Fonctions de commande des moteurs =====
// Chaque fonction configure les broches de direction puis applique une PWM

// Avancer en mettant les roues dans le même sens
void forward(int ajri) {
  digitalWrite(in1, LOW);
  digitalWrite(in2, HIGH);
  digitalWrite(in3, LOW);
  digitalWrite(in4, HIGH);

  analogWrite(enA, ajri);
  analogWrite(enB, ajri);
}

// Tourner vers la droite (virage sur place ou différentiel)
void turnRight(int tempSpeed) {
  digitalWrite(in1, LOW);
  digitalWrite(in2, HIGH);
  digitalWrite(in3, HIGH);
  digitalWrite(in4, LOW);

  analogWrite(enA, tempSpeed);
  analogWrite(enB, tempSpeed);
}

// Tourner vers la gauche
void turnLEFTT(int tempSpeed) {
  digitalWrite(in1, HIGH);
  digitalWrite(in2, LOW);
  digitalWrite(in3, LOW);
  digitalWrite(in4, HIGH);

  analogWrite(enA, tempSpeed);
  analogWrite(enB, tempSpeed);
}

// Comportement de recherche: avancer/ tourner lentement pour trouver l'adversaire
void cherch(int temp) {
  digitalWrite(in1, LOW);
  digitalWrite(in2, HIGH);
  digitalWrite(in3, HIGH);
  digitalWrite(in4, LOW);

  analogWrite(enA, temp);
  analogWrite(enB, temp);
}
