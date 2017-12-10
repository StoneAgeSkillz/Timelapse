;void pulse() {
  digitalWrite(STEP, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(1);              // wait for a second
  digitalWrite(STEP, LOW);    // turn the LED off by making the voltage LOW
  delay(1);

}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void nastavSmer(int smer) {
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
    case (1):   digitalWrite(MS1, LOW);   digitalWrite(MS2, LOW);   digitalWrite(MS3, LOW);  break; // Full Step       MS1:L MS2:L MS3:L
    case (2):   digitalWrite(MS1, HIGH);  digitalWrite(MS2, LOW);   digitalWrite(MS3, LOW);  break; // Half Step       MS1:H MS2:L MS3:L
    case (4):   digitalWrite(MS1, LOW);   digitalWrite(MS2, HIGH);  digitalWrite(MS3, LOW);  break; // Quarter Step    MS1:L MS2:H MS3:L
    case (8):   digitalWrite(MS1, HIGH);  digitalWrite(MS2, HIGH);  digitalWrite(MS3, LOW);  break; // Eighth Step     MS1:H MS2:H MS3:L
    case (16):  digitalWrite(MS1, HIGH);  digitalWrite(MS2, HIGH);  digitalWrite(MS3, HIGH); break; // Sixteenth Step  MS1:H MS2:H MS3:H
  }
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void vynuluj() {
  nastavSmer(0); //CW
  nastavPresnost(1); // full step
  
  for (long i = 0; i < max_kroky; i++) { // dojeď k mikrospínači
    if (digitalRead(ENDSW) == HIGH) {
      pulse();
    }
    else break;
  }

  //odskočení pro vypnutí mikrospínače
  nastavSmer(1);

  for (int i = 0; i < 400; i++) {
    if (digitalRead(ENDSW) == LOW) {
      pulse(); //udelej krok
    }
    else break;
  }

  // zpřesnit otáčení

  nastavPresnost(16); // half step == 1/2 ==> delitel == 2, quarter step == 1/4 ==> delitel == 4...

  //kalibrace sepnutí mikrospínače
  nastavSmer(0);

  for (int i = 0; i < 200; i++) {
    if (digitalRead(ENDSW) == HIGH) {
      pulse(); //udelej krok
    }
    else {
      akt_kroky = 0;// sem ukladat spočtene kroky, vynulovat
      break;
    }
  }
}

void zjistiMaxKroky() {

  vynuluj();

  nastavSmer(1); //CCW
  
  while(digitalRead(HALL) == HIGH){
   pulse(); //udelej krok 
   akt_kroky++;
  }
//  for (akt_kroky; akt_kroky <= max_kroky; akt_kroky++) {
//    if (digitalRead(HALL) == HIGH) {
//      pulse(); //udelej krok
//    }
//    else break;
//  }

  max_kroky = akt_kroky; //nastavení rozmezí otačení osy
  EEPROMWritelong(0, max_kroky);
  odesliData(0, 4, max_kroky, akt_kroky);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void otocNa(int pozice) {
  if (digitalRead(HALL) == HIGH and akt_kroky >= 0 and akt_kroky < pozice) {
    for (akt_kroky; akt_kroky < pozice; akt_kroky++) {
      nastavSmer(1);
      pulse();

    }
  }
  else if (digitalRead(ENDSW) == HIGH and akt_kroky <= max_kroky and akt_kroky > pozice) {
    for (akt_kroky; akt_kroky > pozice; akt_kroky--) {
      nastavSmer(0);
      pulse();
    }
  }
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void zmenUhel(int uhel) {  
  float rozpeti = abs(min_uhel) + max_uhel;
  float uhel_na_krok = rozpeti / max_kroky;
  float kam_jit = (uhel + (rozpeti - 90)) / uhel_na_krok;

  nastavPresnost(16);
  otocNa(floor(kam_jit));
}

