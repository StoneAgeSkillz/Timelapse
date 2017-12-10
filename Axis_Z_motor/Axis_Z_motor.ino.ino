#define STEP 9
#define MS1 12
#define MS2 11
#define MS3 10
#define DIR A0
#define EN 13

#define HALL_A A5
#define HALL_B A4

// pomocné proměnné
int smer_otaceni = 0; // 0=CW
long max_kroky = 500000;
long akt_kroky = 0;
int hall_A_zona = 0;
int i = 0;
int y = 1;

void setup() {
  Serial.begin(112500);

  // ovládání
  pinMode(STEP, OUTPUT);
  pinMode(MS1,  OUTPUT);
  pinMode(MS2,  OUTPUT);
  pinMode(MS3,  OUTPUT);
  pinMode(DIR,  OUTPUT);
  pinMode(EN,   OUTPUT);
  pinMode(HALL_A, INPUT_PULLUP);
  pinMode(HALL_B, INPUT_PULLUP);

  digitalWrite(STEP, LOW);
  digitalWrite(MS1, LOW);
  digitalWrite(MS2, LOW);
  digitalWrite(MS3, LOW);
  digitalWrite(DIR, LOW);
  digitalWrite(EN, LOW);

  //zjistiMaxKroky();
  //nastavPresnost(2);
}

void loop() {
  if (digitalRead(HALL_A) == LOW) {
     Serial.println("HALL");

    if (i == 0) {
       smer_otaceni = ~smer_otaceni;
       nastavPresnost(y);
       i++;
       

       if (y < 16) {
         y += y;
       }
       else{digitalWrite(EN, HIGH);}
     }
     nastavSmerOtaceni(smer_otaceni);
     //digitalWrite(EN, HIGH);
    }
    else {
     i = 0;
    }

  pulse();
}
