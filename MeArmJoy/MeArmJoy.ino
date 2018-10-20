#include <Servo.h>

#define potenMid A0
#define potenLeft A1
#define potenRight A2
#define boutonClaw A3

//Variables pour servo MIDDLE
int AngMid = 0;
float dAngMid;

//Variables pour servo LEFT
int AngLeft = 0;
float dAngLeft;

//Variables pour servo RIGHT
int AngRight = 0;
float dAngRight;

//Variables pour servo CLAW
bool etat = false, dernierEtat = false;
unsigned long temps, tempsRebond = 10;
bool ouvert = false;

float maxSpeed = 5.0; //Valeur d'incrémentation maximale des angles

//Ci-dessous :  Intervalles de valeurs corrects pour les differents servos
/*
  claw.write(100); //0-100
  left.write(90); //90-180
  right.write(45); //45-130
  middle.write(0); //0-165
*/

Servo claw, left, right, middle;
void setup() {
  Serial.begin(9600); //Init serie
  claw.attach(6);  //Init servos
  left.attach(9);
  right.attach(10);
  middle.attach(11);

  claw.write(100); //Init des positions
  left.write(90);
  right.write(45);
  middle.write(0);

  //Init pins
  //(Inutile pour des entrées analogiques...)
  pinMode(potenMid, INPUT);//Init potenMid pour lire tension en sortie du potentiometre 1
  pinMode(potenLeft, INPUT); //Init potentiometre 2
  pinMode(potenRight, INPUT); //Init potentio 3

}


void loop() {
  //////MIDDLE
  //dAngMid = Variation de l'angle AngMid sur une boucle
  dAngMid = analogRead(potenMid); //Lecture potentiometre 1
  dAngMid = map(dAngMid, 0, 1023, -maxSpeed, maxSpeed); //mappage : (0,1023)->(-maxspeed,maxspeed)
  AngMid = AngMid + dAngMid; //Mise a jour de AngMid
  AngMid = constrain(AngMid, 0, 165);//AngMid limité aux valeurs comprises entre 0 et 165
  middle.write(AngMid);
  //////FIN MIDDLE
  
  //////LEFT
  dAngLeft = analogRead(potenLeft);  //Meme fonctionnement que ci-dessus
  dAngLeft = map(dAngLeft, 0, 1023, -maxSpeed, maxSpeed);
  AngLeft = AngLeft + dAngLeft;
  AngLeft = constrain(AngLeft, 90, 180); //Limitation: 90,180
  left.write(AngLeft);
  //////FIN LEFT
  
  //RIGHT
  dAngRight = analogRead(potenRight);
  dAngRight = map(dAngRight, 0, 1023, -maxSpeed, maxSpeed);
  AngRight = AngRight + dAngRight;
  AngRight = constrain(AngRight, 45, 130); //Limitation : 45, 130
  right.write(AngRight);
  delay(100); //Delai pour permettre aux servos de suivre la commande
  //////FIN RIGHT
  
  ///CLAW
  //Pilotage par bouton
  dernierEtat = etat;
  etat = digitalRead(boutonClaw);
  if (!dernierEtat && etat && millis() - temps >= tempsRebond){
    ouvert = !ouvert;
  }
  if(ouvert == true){
    claw.write(0);
  }
  else{
    claw.write(100);
  }
  //FIN CLAW
}

