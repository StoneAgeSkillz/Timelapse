void hodiny(int x, int y) {
  GLCD.setBackColor(VGA_BLACK);
  GLCD.setColor(VGA_WHITE);
  GLCD.setFont(SmallFont);

  int hodina  = RTC.getHour(h12, pm);
  int minuta  = RTC.getMinute();
  int sekunda = RTC.getSecond();
  int rok     = RTC.getYear();
  int mesic   = RTC.getMonth(stoleti);
  int den     = RTC.getDate();
  int teplota = RTC.getTemperature();

  String sekunda_str;
  String minuta_str;
  String hodina_str;
  String mesic_str;
  String den_str;

  if (sekunda < 10) {
    sekunda_str = "0" + String(sekunda, DEC);
  }
  else {
    sekunda_str = String(sekunda, DEC);
  }

  if (minuta < 10) {
    minuta_str = "0" + String(minuta, DEC);
  }
  else {
    minuta_str = String(minuta, DEC);
  }

  if (hodina < 10) {
    hodina_str = "0" + String(hodina, DEC);
  }
  else {
    hodina_str = String(hodina, DEC);
  }

  if (mesic < 10) {
    mesic_str = "0" + String(mesic, DEC);
  }
  else {
    mesic_str = String(mesic, DEC);
  }

  if (den < 10) {
    den_str = "0" + String(den, DEC);
  }
  else {
    den_str = String(den, DEC);
  }

  if (stoleti) {
    rok = rok + 100;
  }
  rok = rok + 2000;

  String cas = hodina_str + ":" + minuta_str + ":" + sekunda_str + " "  + den_str + "." + mesic_str + "." + String(rok) + "   " + String(teplota)+"C";

  GLCD.print(cas, x, y);
}

