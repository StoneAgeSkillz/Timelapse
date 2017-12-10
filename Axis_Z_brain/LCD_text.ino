void popiskyKroku(int x, int y) {
  GLCD.setColor(VGA_WHITE);
  GLCD.setBackColor(VGA_BLACK);
  GLCD.setFont(BigFont);
  GLCD.print("W", x - 1, y);
  GLCD.print("Z", x, y + 32);
  GLCD.print("Y", x, y + 64);
  GLCD.print("X", x, y + 96);
}

String dodejNuly(unsigned long int vstup) {
  String vystup;
  if (vstup < 10) {
    vystup = "000000" + String(vstup, DEC);
  }
  else if (vstup < 100) {
    vystup = "00000" + String(vstup, DEC);
  }
  else if (vstup < 1000) {
    vystup = "0000" + String(vstup, DEC);
  }
  else if (vstup < 10000) {
    vystup = "000" + String(vstup, DEC);
  }
  else if (vstup < 100000) {
    vystup = "00" + String(vstup, DEC);
  }
  else if (vstup < 1000000) {
    vystup = "0" + String(vstup, DEC);
  }
  else {
    vystup = String(vstup);
  }

  return vystup;
}

String dodejMezery(unsigned long int vstup) {
  String vystup;
  if (vstup > 0 and vstup < 10) {
    vystup = "   " + String(vstup, DEC);
  }
  else if (vstup < 100) {
    vystup = "  " + String(vstup, DEC);
  }
  else if (vstup < 1000) {
    vystup = " " + String(vstup, DEC);
  }
  else {
    vystup = "0";
  }

  return vystup;
}

void vypis_DHT(int x, int y, float teplota, float vlhkost, float teplotni_index) {
  //teplota = round(teplota*10.0)/10.0;
  //vlhkost = round(vlhkost*10.0)/10.0;
  //teplotni_index = round(teplotni_index*10.0)/10.0;
  GLCD.print(String(teplota) + "C", x, y);
  GLCD.print(String(vlhkost) + "%", x, y + 10);
  //GLCD.print("(" + String(teplotni_index) + "C)", x-10, y + 20);
}

