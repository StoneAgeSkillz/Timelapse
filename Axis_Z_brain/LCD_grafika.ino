void vykresliPozadi() {
  /*GLCD.setColor(VGA_YELLOW);
    GLCD.drawPixel(0, 0);
    GLCD.setColor(VGA_RED);
    GLCD.drawPixel(319, 239);*/
  GLCD.setColor(VGA_WHITE);
  GLCD.setFont(BigFont);
  GLCD.setBackColor(VGA_BLACK);

  GLCD.drawLine(0, 12, 319, 12);
  GLCD.setColor(VGA_BLUE);
  GLCD.fillRect(0, 13, 319, 173);
  GLCD.setColor(VGA_BLACK);
  GLCD.fillRect(1, 15, 220, 171);

  for (int i = 0; i < 5 ; i++) {
    GLCD.fillRect(223, 14 + 32 * i, 253, 44 + 32 * i);
    GLCD.fillRect(255, 14 + 32 * i, 317, 44 + 32 * i);
  }

  GLCD.setColor(VGA_WHITE);
  for (int i = 0; i < 5 ; i++) {
    GLCD.drawRect(223, 14 + 32 * i, 253, 44 + 32 * i);
    if (i == 0) {
      GLCD.print(popisky_osy[i], 230, 23 + 32 * i); // korekce polohy W (pouze vizual, x-1)
    }
    else {
      GLCD.print(popisky_osy[i], 231, 23 + 32 * i);
    }
    GLCD.drawRect(255, 14 + 32 * i, 317, 44 + 32 * i);
  }
  GLCD.drawRect(1, 14, 220, 171);
  GLCD.drawLine(0, 174, 319, 174);

  GLCD.setFont(BigFont);
  GLCD.setBackColor(VGA_BLACK);
  //popiskyKroku(231, 25);
}

//--> tlačítka //////////////////////////////////////////////////////////////////////////////////////////////////
void okrajeTlacitek(int x, int y, int hrana, bool zvyrazni)
{
  hrana = hrana - 1; // oprava hodnoty, začínáme nulou

  GLCD.setColor( 92, 92 , 231);
  GLCD.fillRoundRect(x, y, x + hrana, y + hrana);
  GLCD.setColor(VGA_WHITE);
  //GLCD.setColor(0, 0, 227);
  GLCD.drawRoundRect(x, y, x + hrana, y + hrana);

  if (zvyrazni == false) {
    GLCD.setColor(VGA_BLUE);
    GLCD.fillRoundRect(x + 3, y + 3, x + hrana - 2, y + hrana - 2);
    GLCD.setColor(VGA_WHITE);
    GLCD.drawRoundRect(x + 3, y + 3, x + hrana - 2, y + hrana - 2);
    GLCD.setBackColor(VGA_BLUE);
  }
  else {
    GLCD.setColor(VGA_RED);
    GLCD.fillRoundRect(x + 3, y + 3, x + hrana - 2, y + hrana - 2);
    GLCD.setColor(VGA_WHITE);
    GLCD.drawRoundRect(x + 3, y + 3, x + hrana - 2, y + hrana - 2);
    GLCD.setBackColor(VGA_RED);
  }
}

void vykresliTlacitko(int x, int y, int hrana, String text, bool zvyrazni) {
  okrajeTlacitek(x, y, hrana, zvyrazni);
  GLCD.setFont(BigFont);
  GLCD.print(text, x + 5, y + 5);
  refresh_LCD_tlacitka = false;
}
//<-- tlačítka /////////////////////////////////////////////////////////////////////////////////////////////

//--> osy //////////////////////////////////////////////////////////////////////////////////////////////////
void vykresliOsuW(int x, int y, unsigned long int kroky) {
  int procenta = floor((float)kroky / ((float)kroky_osy_max[0] / 100.0  )); //max kroky/delka baru

  int prev_proc = 111;
  int delka_baru = 200;

  float nasobic = (float)delka_baru / (float)100 ;
  float vykresli = floor(nasobic * procenta);

  if (procenta != prev_proc) {
    GLCD.setColor(VGA_RED);
    GLCD.fillRect(x + 1, y + 1, x + (int)vykresli, y + 4);
    if (vykresli < delka_baru - 1) {
      GLCD.setColor(VGA_BLACK);
      GLCD.fillRect(x + (int)vykresli + 1, y + 1, x + (int)vykresli + 2, y + 4);
    }
    GLCD.setColor(VGA_WHITE);
    GLCD.drawRect(x, y, x + delka_baru, y + 5);

    for (int i = 1; i < 10; i++) {
      GLCD.drawLine(x + 20 * i, y + 1, x + 20 * i, y + 4);
    }
  }
  prev_proc = procenta;
}

void vykresli_osovy_kriz(int x, int y, int delka_car) {
  GLCD.setColor(VGA_RED);
  GLCD.drawLine(x + 32, y, x + delka_car, y); // Y
  GLCD.drawLine(x + 15, y - 17, x + 15, y - delka_car); // X

  ikona_fotak(x - 10, y - 15);

  GLCD.setColor(VGA_RED);
  GLCD.drawLine(x + 15, y, x + 15 - delka_car / 2, y + delka_car / 2); // Z

  /*GLCD.setColor(VGA_WHITE);
  if (chod_x[1] != 0) {// Z
    if (chod_x[1] > 0) {
      smazKruhovouSipku(x + 5 - delka_car / 2, y - 5 + delka_car / 2, 10, 0);
      vykresliKruhovouSipku(x + 5 - delka_car / 2, y - 5 + delka_car / 2, 10, 1);
    }
    else {
      smazKruhovouSipku(x + 5 - delka_car / 2, y - 5 + delka_car / 2, 10, 1);
      vykresliKruhovouSipku(x + 5 - delka_car / 2, y - 5 + delka_car / 2, 10, 0);
    }
  }
  else {
    smazKruhovouSipku(x + 5 - delka_car / 2, y - 5 + delka_car / 2, 10, 0);
    smazKruhovouSipku(x + 5 - delka_car / 2, y - 5 + delka_car / 2, 10, 1);
  }

  if (chod_x[2] != 0) {// Y
    if (chod_x[2] > 0) {
      smazKruhovouSipku(x - 5 + delka_car, y - 9, 10, 1);
      vykresliKruhovouSipku(x - 5 + delka_car, y - 9, 10, 0);
    }
    else {
      smazKruhovouSipku(x - 5 + delka_car, y - 9, 10, 0);
      vykresliKruhovouSipku(x - 5 + delka_car, y - 9, 10, 1);
    }
  }
  else {
    smazKruhovouSipku(x - 5 + delka_car, y - 9, 10, 0);
    smazKruhovouSipku(x - 5 + delka_car, y - 9, 10, 1);
  }*/

}
//<-- osy ////////////////////////////////////////////////////////////////////////////////////////////////////

//--> šipky //////////////////////////////////////////////////////////////////////////////////////////////////
void vykresliKruhovouSipku(int x, int y, int polomer, bool smer) {
  GLCD.setColor(VGA_WHITE);

  if (smer == true) {
    GLCD.drawLine(x - 1, y + 1 + polomer, x + 1, y + 1 + polomer);
    GLCD.drawLine(x - 2, y + 2 + polomer, x + 2, y + 2 + polomer);
    GLCD.drawLine(x - 3, y + 3 + polomer, x + 3, y + 3 + polomer);
    GEO.drawArc(x + polomer, y + polomer, polomer, 0, 270); // x, y, poloměr, odkud, kam (,tloušťka)
  }
  else
  {
    GLCD.drawLine(x - 1, y - 2 + polomer, x + 1, y - 2 + polomer);
    GLCD.drawLine(x - 2, y - 3 + polomer, x + 2, y - 3 + polomer);
    GLCD.drawLine(x - 3, y - 4 + polomer, x + 3, y - 4 + polomer);
    GEO.drawArc(x + polomer, y + polomer, polomer, -90, 180); // x, y, poloměr, odkud, kam (,tloušťka)
  }
}
void smazKruhovouSipku(int x, int y, int polomer, bool smer) {
  GLCD.setColor(VGA_BLACK);

  if (smer == true) {
    GLCD.drawLine(x - 1, y + 1 + polomer, x + 1, y + 1 + polomer);
    GLCD.drawLine(x - 2, y + 2 + polomer, x + 2, y + 2 + polomer);
    GLCD.drawLine(x - 3, y + 3 + polomer, x + 3, y + 3 + polomer);
    GEO.drawArc(x + polomer, y + polomer, polomer, 0, 270); // x, y, poloměr, odkud, kam (,tloušťka)
  }
  else
  {
    GLCD.drawLine(x - 1, y - 2 + polomer, x + 1, y - 2 + polomer);
    GLCD.drawLine(x - 2, y - 3 + polomer, x + 2, y - 3 + polomer);
    GLCD.drawLine(x - 3, y - 4 + polomer, x + 3, y - 4 + polomer);
    GEO.drawArc(x + polomer, y + polomer, polomer, -90, 180); // x, y, poloměr, odkud, kam (,tloušťka)
  }
}
//<-- šipky //////////////////////////////////////////////////////////////////////////////////////////////////


