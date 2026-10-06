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