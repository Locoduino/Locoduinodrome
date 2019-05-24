
#include "Feux.h"

////////////// DoTest //////////

const String titreTest[]={
  "Aiguille 0 ",  //0 sous-menus Min, Max
  "Aiguille 1 ",  //1 sous-menus Min, Max
  "Feu # Sat #",  //2 sous-menus 0 à 9 (10 signaux)
  };
const int maxMenu = 3;

const String nomFeu[16]= {"Vl", "A", "S", "C", "R", "RR", "M", "Cv", "Vlc", "Ac", "Sc", "Rc", "RRc", "Mc", "D", "E"};
int indexFeu = 0;
unsigned int typeFeu[16] = {Vl, A, S, C, R, RR, M, Cv, Vlc, Ac, Sc, Rc, RRc, Mc, D, E};
int FeuSat[10] = {4,3,7,5,0,1,6,2,3,4};   // n° de satellite en fonction du N° de feu

int menuTest = 0;
int sousmenuTest = 0;
int etatTest = 0;           // gestion encodeur 
int noSat = 0;
int noAig = 0;              // numero d'aiguille 0..1
int noSig = 0;              // numero signal 0..9
const int maxSignaux = 9;

//-----------------------------------------------
void displayTitre() {
  lcd.setCursor(0, 0);
  lcd.print(titreTest[menuTest]);
  switch (menuTest) { 
    case 0:
    case 1:
    // aiguilles 0 et 1 sous-menus Min, Max
    switch (sousmenuTest) {
      case 0:
      lcd.print("Min");
      break;
      case 1:
      lcd.print("Max");
      break;
    }
    break;
    case 2:
    //"Feu # Sat #  ",     //2 sous-menus 0 à 9 (10 signaux)
    lcd.print("   ");
    lcd.setCursor(4, 0);
    noSig = sousmenuTest;
    lcd.print(noSig);
    lcd.setCursor(10, 0);  //
    noSat = FeuSat[noSig];
    lcd.print(noSat);
    lcd.setCursor(0, 1);         ;            // 2 eme ligne
    lcd.print("Type ");
    break;
  }
}

//-----------------------------------------------
void testAiguille(boolean s) {
  sendAiguille(menuTest, s);
  lcd.setCursor(0, 1);            // 2 eme ligne
  if (s) {
    lcd.print("Droit  ");
  } else {
    lcd.print("Devie  ");
  }
}

//-----------------------------------------------
void testFeu() {
  lcd.setCursor(0, 1);         ;            // 2 eme ligne
  lcd.print("Type ");
  lcd.print(nomFeu[indexFeu]);
  lcd.print("    ");
  SignalWrapper::setSignalState(noSig, typeFeu[indexFeu]);      
}

//-----------------------------------------------
void initTest()
{
  menuTest = 0;
  sousmenuTest = 0;
  etatTest = 0;
  lcd.clear();
  displayTitre();
  initDisplayCapteurs();                    // onglet LCD.h
  testAiguille(sousmenuTest);
}

//-----------------------------------------------
void doTest() { 
  // positionne menuTest et sousmenuTest et fait le test correspondant à chaque changement
  // menuTest et sousmenuTest définissent les tests elementaires
  // etatTest définit l'emploi de l'encodeur
  // etatTest = 0 : juste la selection de test par l'encodeur
  int u = lireEncodeur();
  // rotation de l'encodeur pour choix du test
  if ((u != 0) && (etatTest == 0 )) { 
    if (u > 0) {              // +1 encodeur
      if (menuTest == 2) {    // choix parmi 9 signaux
        if (sousmenuTest < 9) {
          sousmenuTest++;
        }
      }
      if (menuTest <= 1) {    // aiguilles 0 et 1
        sousmenuTest++;
        if (sousmenuTest > 1) {
          menuTest++;         // passage au test signaux
          sousmenuTest = 0;
        }
        if (menuTest <= 1) {
          testAiguille(sousmenuTest); 
        }
      }
    }
    if (u < 0) {              // -1 encodeur
      if (menuTest <= 1) {    // aiguille
        if (sousmenuTest > 0) {
          sousmenuTest--;
        } else {
          if (menuTest > 0) {
            menuTest--;
            sousmenuTest = 1;
          }
        }
        testAiguille(sousmenuTest);
      }
      if (menuTest == 2) {    // feux
        if (sousmenuTest > 0) {
          sousmenuTest--;
        } else { 
          menuTest = 1;       // passage en test aiguille
          sousmenuTest = 0;
          testAiguille(sousmenuTest);
        }
      }
    }        
    displayTitre();           // met à jour noSig pour testFeu()
  }
  if ((u != 0) && (etatTest == 2)) { // choix type de feu
    if (u > 0) {  // augmenter
      if (indexFeu < 15) indexFeu++;
    } else {
      if (indexFeu > 0) indexFeu--;
    }
    testFeu();
  }
  // un appui sur le bouton de l'encodeur change etatTest en fonction du contexte défini par etatTest et menuTest
  if (SWenc.update()) {
    if ( SWenc.fell() ) {     // appui sur bouton encodeur
      switch (etatTest) {
      case 0:
      if (menuTest == 2) {   // passage en choix type de feu
        etatTest = 2;
        indexFeu=0;           // Vl
        displayTitre();
        testFeu();
      }
      break;
      case 2:               // fin du choix type de feux
      etatTest = 0;
      lcd.clear();
      displayTitre();       // retour a l'etat précédent du choix test
      initDisplayCapteurs();    // rafraichissement LCD
      break;
      }
    }
  }
}
