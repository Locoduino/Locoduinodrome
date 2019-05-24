//-----------------------------------------------
void initDisplayCapteurs() {
  // lignes 2 et 3 affichent l'état des capteurs satellites
  lcd.setCursor(0, 2);  // 3e ligne
  lcd.print("Zones :             ");
  lcd.setCursor(0, 3);  // 4e ligne
  lcd.print("Detec :             "); 
}

//-------------------------------------------------------------------
void displayCapteurs(unsigned long canId, unsigned char *buf)
{
  /* 
   * Ligne x : 01234567890123456789
   * Ligne 2 : Test courant 
   * Ligne 2 : Zones : 
   * Ligne 3 : Detec :
   */
  switch (canId) 
  {
  case 0x10: // Z0,P2,P4
    lcd.setCursor(8, 2);  // 3e ligne col 8+0
    lcd.print(bitRead(buf[0],2) ? '0' : '.');
    lcd.setCursor(10, 3); // 4e ligne col 8+2
    lcd.print(bitRead(buf[0],1) ? '2' : '.');
    lcd.setCursor(12, 3); // 4e ligne col 8+4
    lcd.print(bitRead(buf[0],0) ? '4' : '.');
  break;
  case 0x11: // Z1,P3,P5
    lcd.setCursor(9, 2);  // 3e ligne col 8+1
    lcd.print(bitRead(buf[0],2) ? '1' : '.');
    lcd.setCursor(11, 3); // 4e ligne col 8+3
    lcd.print(bitRead(buf[0],1) ? '3' : '.');
    lcd.setCursor(13, 3); // 4e ligne col 8+5
    lcd.print(bitRead(buf[0],0) ? '5' : '.');
  break;
  case 0x12: // Z2
    lcd.setCursor(10, 2);  // 3e ligne col 8+2
    lcd.print(bitRead(buf[0],2) ? '2' : '.');
  break;
  case 0x13: // Z3,P7,P8
    lcd.setCursor(11, 2);  // 3e ligne col 8+3
    lcd.print(bitRead(buf[0],2) ? '3' : '.');
    lcd.setCursor(15, 3);// 4e ligne col 8+7
    lcd.print(bitRead(buf[0],1) ? '7' : '.');
    lcd.setCursor(16, 3); // 4e ligne col 8+8
    lcd.print(bitRead(buf[0],0) ? '8' : '.');
  break;
  case 0x14: // Z6,P6,P9
    lcd.setCursor(14, 2);  // 3e ligne col 8+6
    lcd.print(bitRead(buf[0],2) ? '6' : '.');
    lcd.setCursor(14, 3); // 4e ligne col 8+6
    lcd.print(bitRead(buf[0],1) ? '6' : '.');
    lcd.setCursor(17, 3); // 4e ligne col 8+9
    lcd.print(bitRead(buf[0],0) ? '9' : '.');
  break;
  case 0x15: // Z4,P1
    lcd.setCursor(12, 2);  // 3e ligne col 8+4
    lcd.print(bitRead(buf[0],2) ? '4' : '.');
    lcd.setCursor(9, 3); // 4e ligne col 8+1
    lcd.print(bitRead(buf[0],1) ? '1' : '.');
  break;
  case 0x16: // Z5
    lcd.setCursor(13, 2);  // 3e ligne col 8+5
    lcd.print(bitRead(buf[0],2) ? '5' : '.');
    break;
  case 0x17: // P0
    lcd.setCursor(8, 3); // 4e ligne col 8+0
    lcd.print(bitRead(buf[0],1) ? '0' : '.');
    break;
  default:
    lcd.setCursor(19, 2);
    lcd.print('?');                              // Id reçu inconnu
  break;
  }
}
//-------------------------------------------------------------------
