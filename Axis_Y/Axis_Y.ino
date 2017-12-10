
#define STEP 13
#define MS1 A2
#define MS2 A1
#define MS3 A0
#define DIR 12
#define EN A3

#define HALL 10

// pomocné proměnné
int smer_otaceni = 0; // 0=CW
long max_kroky = 50000;
long akt_kroky = 0;
int hall_zona = 0;
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
  pinMode(HALL, INPUT_PULLUP);

  digitalWrite(STEP, LOW);
  digitalWrite(MS1, LOW);
  digitalWrite(MS2, LOW);
  digitalWrite(MS3, LOW);
  digitalWrite(DIR, LOW);
  digitalWrite(EN, LOW);

  zjistiMaxKroky();
  nastavPresnost(2);
}

void loop() {  

}
