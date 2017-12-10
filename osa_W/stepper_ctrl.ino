//void otocNaPozici(long pozice) {
//  for (int i = 0; i < kroky_osy_max; i++) {
//    
//    if (pozice >= 0 and pozice <= kroky_osy_max)
//    {
//      if (pozice < kroky_osy) {
//        nastavSmerOtaceni(0); //CW
//        kroky_osy--;
//        pulse();
//      }
//      else if (pozice > kroky_osy) {
//        nastavSmerOtaceni(1); //CW
//        aktKroky++;
//        pulse();
//      }
//      else break;
//    }
//    Serial.println(String(pozice) + "/" + String(aktKroky));
//  }
//  Serial.println(String(pozice) + "=" + String(aktKroky));
//}

void pulse() {
  digitalWrite(STEP, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(1);              // wait for a second
  digitalWrite(STEP, LOW);    // turn the LED off by making the voltage LOW
  delay(1);

}

/*void zjistiMaxKroky() {
  Serial.println("zjistiMaxKroky();");
  int stateENDSW1A = digitalRead(ENDSW1A);
  int stateENDSW1B = digitalRead(ENDSW1B);
  int stateHALL = digitalRead(HALL);

  nastavSmerOtaceni(1); //CW
  nastavPresnost(1); // full step

  for (long i = 0; i < maxKroky; i++) { // dojeď k mikrospínači
    if (stateENDSW1A == LOW) {
      pulse();
      stateENDSW1A = digitalRead(ENDSW1A);
    }
    else break;
  }

  //odskočení pro vypnutí mikrospínače
  nastavSmerOtaceni(0);

  for (int i = 0; i < 400; i++) {
    if (stateENDSW1A == HIGH) {
      pulse(); //udelej krok
      stateENDSW1A = digitalRead(ENDSW1A);
    }
    else break;
  }

  // zpřesnit otáčení
  int delitel = 16; // half step == 1/2 ==> delitel == 2, quarter step == 1/4 ==> delitel == 4
  nastavPresnost(delitel);

  //kalibrace sepnutí mikrospínače
  nastavSmerOtaceni(1);

  for (int i = 0; i < 200; i++) {
    if (stateENDSW1A == LOW) {
      pulse(); //udelej krok
      stateENDSW1A = digitalRead(ENDSW1A);
    }
    else {
      aktKroky = 0;// sem ukladat spočtene kroky, vynulovat
      break;
    }
  }


  nastavSmerOtaceni(0); //CCW

  stateHALL = digitalRead(HALL);
  for (long i = 0; i < maxKroky; i++) {
    if (stateHALL == HIGH) {
      pulse(); //udelej krok

      stateHALL = digitalRead(HALL);

      aktKroky = aktKroky + 16 / delitel;
    }
    else break;
  }

  Serial.println(String(aktKroky));
  maxKroky = aktKroky; //nastavení rozmezí otačení osy
  aktKroky = 0;
}*/

void nastavSmerOtaceni(int smer) {
  if (smer == 0) {
    digitalWrite(DIR, HIGH);
    //Serial.println("DIR: CW HIGH");
  }
  else {
    digitalWrite(DIR, LOW);
    //Serial.println("DIR: CCW LOW");
  }
}

void nastavPresnost(int delitel) {
  switch (delitel)
  {
    case (1):   digitalWrite(MS1, LOW);   digitalWrite(MS2, LOW);   digitalWrite(MS3, LOW);  break; Serial.println("Full Step");     // Full Step       MS1:L MS2:L MS3:L
    case (2):   digitalWrite(MS1, HIGH);  digitalWrite(MS2, LOW);   digitalWrite(MS3, LOW);  break; Serial.println("1/2 Step");     // Half Step       MS1:H MS2:L MS3:L
    case (4):   digitalWrite(MS1, LOW);   digitalWrite(MS2, HIGH);  digitalWrite(MS3, LOW);  break; Serial.println("1/4 Step");  // Quarter Step    MS1:L MS2:H MS3:L
    case (8):   digitalWrite(MS1, HIGH);  digitalWrite(MS2, HIGH);  digitalWrite(MS3, LOW);  break; Serial.println("1/8 Step");   // Eighth Step     MS1:H MS2:H MS3:L
    case (16):  digitalWrite(MS1, HIGH);  digitalWrite(MS2, HIGH);  digitalWrite(MS3, HIGH); break; Serial.println("1/16 Step");     // Sixteenth Step  MS1:H MS2:H MS3:H
  }
}

