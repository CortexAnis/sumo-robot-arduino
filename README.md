# Sumo Robot

Projet Arduino: sketch principal `sumo_robot.ino`.

## Description
Ce dépôt contient le code pour le robot de combat "Robot_fight".

## Contenu
- `sumo_robot.ino` : sketch Arduino principal.

## Installer / Utilisation
1. Ouvrir `sumo_robot.ino` dans l'IDE Arduino.
2. Programmer la carte (sélectionner le modèle et le port).

## Explication du code

Ce dépôt contient le sketch principal `sumo_robot.ino` pour le robot de sumo.

Structure générale du code :

- **Déclarations / constantes :** définition des broches (moteurs, capteurs), constantes de vitesse et de seuils.
- **`setup()` :** initialise les broches, les communications série et toute calibration requise (capteurs, encodeurs).
- **`loop()` :** boucle principale qui lit les capteurs, décide du comportement (détection d'adversaire, évitement de bord) et commande les moteurs.
- **Fonctions utilitaires :** par exemple `readSensors()` pour lire capteurs infrarouges/ultrasons, `drive(left,right)` pour piloter les moteurs, `attack()` et `avoidEdge()` pour les stratégies.

Comportement attendu :

- Le robot surveille les capteurs frontaux pour localiser l'adversaire et se dirige vers lui.
- Il utilise des capteurs de bord (ou capteurs de luminosité) pour détecter le bord de l'arène et reculer/éviter.
- Les décisions de déplacement combinent lectures de capteurs et règles simples (ex. si adversaire à gauche -> tourner à gauche).

Pour toute précision (schéma de brochage, valeurs de seuil recommandées ou ajout d'images), dites-moi ce que vous souhaitez que j'ajoute au `README`.

## Matériel utilisé

- **Carte:** Arduino Nano
- **Capteurs:** 3 × HC-SR04 (un centre, deux latéraux)
- **Pilote moteurs:** L298N
- **Moteurs:** 2x moteurs à courant continu (avec roues)
- **Alimentation:** batterie LiPo/pack adapté au moteur

## Exemple de brochage (suggestion)

- HC-SR04 centre: `Trig` D2, `Echo` D3
- HC-SR04 gauche: `Trig` D4, `Echo` D5
- HC-SR04 droite: `Trig` D6, `Echo` D7
- L298N: IN1..IN4 sur D8..D11, ENA/ENB via PWM D5/D6 (adapter selon votre carte)
- Alimentation moteurs connectée au `Vmotor` du L298N, GND commun

Adaptez les broches selon vos connexions réelles sur l'Arduino Nano.

## Comportement et algorithme

1. Lire les distances des trois HC-SR04 en permanence.
2. Si le capteur central détecte un adversaire sous un seuil (ex. < 30 cm), basculer en mode "attaque" et avancer droit vers l'adversaire.
3. Si le capteur central ne détecte rien mais qu'un capteur latéral détecte l'adversaire, effectuer une manœuvre d'alignement pour placer l'adversaire devant le capteur central :
	- Si adversaire à gauche -> tourner légèrement à gauche jusqu'à ce que la distance du capteur gauche augmente et que le capteur central détecte.
	- Si adversaire à droite -> procédure symétrique.
4. Une fois central détecté, accélérer l'approche et maintenir capteurs latéraux pour corriger la trajectoire si l'adversaire se décale.
5. (Optionnel) Utiliser des capteurs de bord pour éviter de sortir de l'arène et basculer en rétrogradation si un bord est détecté.

Ce comportement permet au robot de se recentrer (grâce aux deux capteurs latéraux) puis d'attaquer lorsque le capteur central confirme la présence en face à face.

## Résultat

Ce robot a remporté la première place lors d'une compétition de robotique organisée par la branche étudiante IEEE de l'École Polytechnique d'Alger.