////////////// DoConfig //////////

bool gModeConfig = false;    // mode test
int valeurEncodeur = 0;      // valeur absolue (virtuelle) de l'encodeur
int gnumConfig = 0;          // numero de config
bool gModeSWConfig = false;  // choix ou true : execution
int satConfig = 0;            // choix du satellite (0..7)
int ledConfig = 0;           // choix de la led (0..9)

int sMin = 1300;    // servo min
int sMax = 1700;    // servo max
// pour les conversions entre 1 float et 4 bytes
union sVitType {float f; unsigned char b[sizeof(float)];} ;
sVitType sVit;      // servo vitesse
uint8_t sLum = 255; // intensité
uint8_t sAlt = 255; // temps d'allumage
uint8_t sFad = 255; // temps d'extinction
uint8_t sOnt = 255; // temps allumé
uint8_t sPer = 255; // periode


const String titreConfig[]={
  "Servo Max    ",  //0
  "Servo Min    ",  //1
  "Servo Vitesse",  //2
  "Choix Led    ",  //3
  "Led Maxi     ",  //4
  "Led T. allume",  //5
  "Led T. eteint",  //6
  "Led T. on    ",  //7
  "Led Periode  ",  //8
  "Fin config   "}; //9
;
///////////////// initialisations /////////////////
  
void initConfig()    // menu principal
{
  lcd.clear();  // 1e ligne
  lcd.print("Config Satellite ");
  lcd.print(satConfig); // satellite en cours de configuration
  while (1) {
    int enc = lireEncodeur();
    if (enc > 0) satConfig++; 
    if (enc < 0) satConfig--;
    if (satConfig < 0) {satConfig = 0;}
    if (satConfig > 7) {satConfig = 7;}
    lcd.setCursor(17, 0);
    lcd.print(satConfig); // satellite en cours de configuration
    SWenc.update();
    if (SWenc.fell()) break;
    if ( SWmode.update() ) {        // test de la clé T/C
      if ( SWmode.fell() ) {
        gModeConfig = false;
        initTest();
        return;   
      } 
    }
  }
  gModeSWConfig = false;// mode choix fonction
  lcd.setCursor(0, 1);  // 2e ligne
  lcd.print(titreConfig[gnumConfig]);
}



///////////////// configuration /////////////////
/*
 * Mode opératoire
 * Choisir un parametre de configuration en tournant le bouton de l'encodeur (9 cas possibles)
 * Appuyer sur le bouton de l'encodeur pour passer en mode modification du parametre sélectionné
 * Tourner le bouton de l'encodeur pour changer la valeur du parametre
 * Un message Can de configuration est envoyé au satellite à chaque cran de l'encodeur
 * Appuyer sur le bouton de l'encodeur pour terminer le mode modification du parametre sélectionné
 * Une message Can de fin de configuration est envoyé au satellite
 */
void doConfig()
{
  int u = lireEncodeur();
  if (u == 0) // pas de changement
  {
    SWenc.update();
    if ( SWenc.fell() )
    {
      gModeSWConfig = !gModeSWConfig; // inversion du mode CONFIG -> ENREGISTREMENT et retour MENU
      if (gModeSWConfig == false)     // fin du mode CONFIG -> ENREGISTREMENT
      {
        lcd.setCursor(14, 1);  // 2e ligne
        lcd.print("    ");     // effacement 
        switch (gnumConfig)
        {
        case 0: // Servo Max
        bufS[0]=0x80; // perma servo
        bufS[1]=1;  // aig max
        bufS[2]=(byte)(sMax>>8);
        bufS[3]=(byte)(sMax);
        sendConfig(satConfig, 4);
        break;
        case 1: // Servo Min
        bufS[0]=0x80; // perma servo
        bufS[1]=0;  // aig min
        bufS[2]=(byte)(sMin>>8);
        bufS[3]=(byte)(sMin);
        sendConfig(satConfig, 4);
        break;
        case 2: // Servo Vit
        bufS[0]=0x80; // perma servo
        bufS[1]=2;  // aig vit
        bufS[2]=sVit.b[3];
        bufS[3]=sVit.b[2];
        bufS[4]=sVit.b[1];
        bufS[5]=sVit.b[0];
        sendConfig(satConfig, 6);
        break;
        case 3: // choix de la led
        break;
        case 4: // Led Max 
        bufS[0]=0x81; // perma led
        sendConfig(satConfig, 3);
        break;
        case 5: // Led T. allume
        bufS[0]=0x81; // perma led
        sendConfig(satConfig, 3);
        break;
        case 6: // Led T. eteint
        bufS[0]=0x81; // perma led
        sendConfig(satConfig, 3);
        break;
        case 7: // Led T. On
        bufS[0]=0x81; // perma led
        sendConfig(satConfig, 3);
        break;
        case 8: // Led Period
        bufS[0]=0x81; // perma led
        sendConfig(satConfig, 3);
        break;
        }  
      } else    // passage en mode modif (gModeSWConfig = true) -> SELECTION du TYPE
      { 
        lcd.setCursor(13, 1);  // 2e ligne 
        lcd.print('>');
        switch (gnumConfig)
        {
        case 0:
        lcd.print(sMax);
        break;
        case 1:
        lcd.print(sMin);
        break;
        case 2:
        lcd.print(sVit.f, 2);
        break;
        case 3: // choix de la led 
        lcd.print(ledConfig);
        break;
        case 4:
        lcd.print(sLum); // Max
        break;
        case 5:
        lcd.print(sAlt); // T. allume
        break;
        case 6:
        lcd.print(sFad); // T. eteint
        break;
        case 7:
        lcd.print(sOnt); // T. on
        break;
        case 8:
        lcd.print(sPer); // Period
        break;
        case 9:
        gnumConfig = 0;
        initConfig();    // fin pour ce satellite
        break;
        }
        lcd.print("  ");
      }
    }
  return;
  }
  // cas de la rotation encodeur
  if (u > 0) valeurEncodeur++; 
  if (u < 0) valeurEncodeur--;
  if (gModeSWConfig == false)              // mode CHOIX CONFIG
  {
    gnumConfig = valeurEncodeur;
    if (gnumConfig < 0) {valeurEncodeur = gnumConfig = 0;}
    if (gnumConfig > 9) {valeurEncodeur = gnumConfig = 9;}
    lcd.setCursor(0, 1);  // 2e ligne
    lcd.print(titreConfig[gnumConfig]);
  } else {                                // mode MODIFICATION
    lcd.setCursor(14, 1);  // 2e ligne 
    switch (gnumConfig)
    {
    case 0: // Servo Max
      if (u > 0) sMax++; 
      if (u < 0) sMax--;
      lcd.print(sMax);lcd.print("  ");
      bufS[0]=0; // tempo servo
      bufS[1]=1; // a0 max
      bufS[2]=(byte)(sMax>>8);
      bufS[3]=(byte)(sMax);
      sendConfig(satConfig, 4);
    break;
    
    case 1: // Servo Min
      if (u > 0) sMin++; 
      if (u < 0) sMin--;
      lcd.print(sMin);lcd.print("  ");
      bufS[0]=0; // tempo servo
      bufS[1]=1; // a0 min
      bufS[2]=(byte)(sMin>>8);
      bufS[3]=(byte)(sMin);
      sendConfig(satConfig, 4);
    break;
    
    case 2: // Servo Vit
      if (u > 0) {sVit.f = sVit.f + 0.1;} 
      if (u < 0) {sVit.f = sVit.f - 0.1;}
      lcd.print(sVit.f, 2);lcd.print("  ");
      bufS[0]=0; // tempo servo
      bufS[1]=2; // a0 vitesse
      bufS[2]=sVit.b[3];
      bufS[3]=sVit.b[2];
      bufS[4]=sVit.b[1];
      bufS[5]=sVit.b[0];
      sendConfig(satConfig, 6);
    break;
    
    case 3: // choix de la Led
      if ((u > 0) && (ledConfig < 9)) ledConfig++; 
      if ((u < 0) && (ledConfig > 0)) ledConfig--;
      lcd.print(ledConfig);lcd.print("   ");
    break;  
    
    case 4: // Led Max luminosité
      if ((u > 0) && (sLum < 255)) sLum++; 
      if ((u < 0) && (sLum > 0)) sLum--;
      lcd.print(sLum);lcd.print("  ");
      bufS[0]=1; // reglage led
      bufS[1]= (ledConfig << 3) + 0; // numero de led + intensité
      bufS[2]=sLum;
      sendConfig(satConfig, 3);
    break;
    
    case 5: // Led allumage Time
    if ((u > 0) && (sAlt < 255)) sAlt++; 
      if ((u < 0) && (sAlt > 0)) sAlt--;
      lcd.print(sAlt);lcd.print("  ");
      bufS[0]=1; // reglage led
      bufS[1]= (ledConfig << 3) + 1; // numero de led + intensité
      bufS[2]=sAlt;
      sendConfig(satConfig, 3);
    break;
    
    case 6: // Led extinction Time
    if ((u > 0) && (sFad < 255)) sFad++; 
      if ((u < 0) && (sFad > 0)) sFad--;
      lcd.print(sFad);lcd.print("  ");
      bufS[0]=1; // reglage led
      bufS[1]= (ledConfig << 3) + 2; // numero de led + intensité
      bufS[2]=sFad;
      sendConfig(satConfig, 3);
    break;
    
    case 7: // Led On Time
    if ((u > 0) && (sOnt < 255)) sOnt++; 
      if ((u < 0) && (sOnt > 0)) sOnt--;
      lcd.print(sOnt);lcd.print("  ");
      bufS[0]=1; // reglage led
      bufS[1]= (ledConfig << 3) + 3; // numero de led + intensité
      bufS[2]=sOnt;
      sendConfig(satConfig, 3);
    break;
    
    case 8: // Led Period
    if ((u > 0) && (sPer < 255)) sPer++; 
      if ((u < 0) && (sPer > 0)) sPer--;
      lcd.print(sPer);lcd.print("  ");
      bufS[0]=1; // reglage led
      bufS[1]= (ledConfig << 3) + 4; // numero de led + intensité
      bufS[2]=sPer;
      sendConfig(satConfig, 3);
    break;

    case 9: // fin pour ce satellite
    break;
    }  
  }
}
