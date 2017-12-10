void pulse() {
  digitalWrite(STEP, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(1);              // wait for a second
  digitalWrite(STEP, LOW);    // turn the LED off by making the voltage LOW
  delay(1);

}

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

/*void zjistiMaxKroky() {
  Serial.println("zjistiMaxKroky();");

  akt_kroky = 0;

  nastavSmerOtaceni(0); //CW
  nastavPresnost(2); // half step

  for (long i = 0; i < max_kroky; i++) { // dojeď k mikrospínači
    if (digitalRead(HALL_A) == HIGH) {
      pulse();
    }
    else{  Serial.println("dojel k HALL_Au"); break;}
  }

  //odskočení pro vypnutí HALL_Au
    nastavSmerOtaceni(1);
    nastavPresnost(16);
    for (int i = 0; i < 400; i++) {
     if (digitalRead(HALL_A) == LOW) {
       pulse();
     }
     else Serial.println("odskocil od HALL_Au"); break;
    }

    //zjištění šířky HALL_Au
    nastavSmerOtaceni(0);
    for (int i = 0; i < 2000; i++) {
     if (digitalRead(HALL_A) == LOW) {
       pulse(); //udelej krok
       hall_A_zona++;
     }
     else Serial.println("HALL_A zona: " + String(hall_A_zona, DEC)); break;
     
    }

    // zjištění počtu kroků na 360°
    for (int i = 0; i < max_kroky; i++) {
     if (digitalRead(HALL_A) == HIGH) {
       pulse(); //udelej krok
       akt_kroky++;
     }
     else {
       Serial.println("akt kroky: " + String(akt_kroky, DEC));
       max_kroky = hall_A_zona + akt_kroky;
       Serial.println("max kroky: " + String(max_kroky, DEC));
       digitalWrite(EN, HIGH);
       break;
     }
    }

    // zpřesnit otáčení
    int delitel = 16; // half step == 1/2 ==> delitel == 2, quarter step == 1/4 ==> delitel == 4
    nastavPresnost(delitel);
  
}*/
