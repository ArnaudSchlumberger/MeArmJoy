#include <Servo.h>

#define potenMiddle A0 //Definition des entrees analogiques correspondantes aux differents servos
#define potenLeft A1
#define potenRight A2
#define boutonClaw A3

float maxSpeed = 5.0; //Valeur d'incrémentation maximale des angles

////////////////////////////////Definition de la classe pivot pour piloter les servos left, right et middle
class pivot {
  public:
    //Methodes
    void initialisation(unsigned char analogPin, int angleMin, int angleMax, int angleInitial) {
      pinPot = analogPin;
      angMin = angleMin;
      angMax = angleMax;
      angle = angleInitial;
    }
    void suiviPot() {
      int dAng = analogRead(pinPot);
      dAng = map(dAng, 0, 1023, -maxSpeed, maxSpeed);
      angle = angle + dAng;
      angle = constrain(angle, angMin, angMax); //Limitation : angMin, angMax
    }
    int angle; //Normalement, tous les attributs doivent etre privés, mais là pas le choix pour le moment.
  private:
    //Attributs
    unsigned char pinPot;
    int angMax;
    int angMin;
};
///////////////////////////////FIN DEF CLASSE

//Variables pour servo CLAW
bool etat = false, dernierEtat = false;
unsigned long temps, tempsRebond = 10;
bool ouvert = false;

class pince {
  public:
    void initPince(unsigned char analogPin, int angleInitial, bool etatInit = false, bool dernierEtatInit = false) {
      etat = etatInit;
      dernierEtat = dernierEtatInit;
      pinBouton = analogPin;
      angle = angleInitial;
      temps = millis() - tempsRebond;
    }
    void pilotePince() {
      //Pilotage par bouton
      dernierEtat = etat;
      etat = digitalRead(pinBouton);
      if (!dernierEtat && etat && millis() - temps >= tempsRebond) {
        ouvert = !ouvert;
      }
    }
    bool ouvert = false;
  private:
    bool etat;
    bool dernierEtat;
    unsigned long temps;
    unsigned long tempsRebond = 10;
    int angle;
    unsigned char pinBouton;

};

//Ci-dessous :  Intervalles de valeurs corrects pour les differents servos
/*
  claw.write(100); //0-100
  left.write(90); //90-180
  right.write(45); //45-130
  middle.write(0); //0-165
*/

//Initialisation des classes
pivot LEFT;
pivot RIGHT;
pivot MIDDLE;

pince CLAW;
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
  pinMode(potenMiddle, INPUT);//Init potenMid pour lire tension en sortie du potentiometre 1
  pinMode(potenLeft, INPUT); //Init potentiometre 2
  pinMode(potenRight, INPUT); //Init potentio 3

  //Initialisation attributs de classe
  LEFT.initialisation(potenLeft, 90, 180, 90);
  RIGHT.initialisation(potenRight, 45, 130, 45);
  MIDDLE.initialisation(potenMiddle, 0, 165, 0);

  CLAW.initPince(boutonClaw,0);
}


void loop() {
  LEFT.suiviPot();
  RIGHT.suiviPot();
  MIDDLE.suiviPot();
  CLAW.pilotePince();

  left.write(LEFT.angle); //Voila pourquoi les angles sont publics: melange de classe servo et pivot
  right.write(RIGHT.angle);
  middle.write(MIDDLE.angle);

  if (CLAW.ouvert == true) {  //l'attribut booleen ouvert est public car necessaire directement dans la loop
    claw.write(0);
  }
  else {
    claw.write(100);
  }


  delay(100); //Delai pour permettre aux servos de suivre la commande
}

