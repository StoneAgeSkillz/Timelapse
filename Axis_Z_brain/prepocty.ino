int prepoctiKrokyNaMM(unsigned long int akt_pocet_kroku, int pocet_kroku_na_otacku) { // platí pouze pro metrickou závitivou tyč, kde je 1mm/ot
  int mm;
  
  mm = akt_pocet_kroku / (pocet_kroku_na_otacku * 16); // 16 je max rozliseni microstepu, kroky se pocitaji v max microstepu

  return floor(mm);
}

float prepoctiKrokyNaStupne(long int akt_pocet_kroku, int pocet_kroku_na_otacku) { // krokový motor s 1,8° na krok /200 ot
  float stupne;
  
  stupne = ((float)360 / ((float)pocet_kroku_na_otacku)) * (float)akt_pocet_kroku; // 16 je max rozliseni microstepu, kroky se pocitaji v max microstepu

  //Serial.println("(360:"+String(pocet_kroku_na_otacku, DEC)+"*16)*"+String(akt_pocet_kroku, DEC)+"="+String(stupne, DEC));
  return floor(stupne);
}


