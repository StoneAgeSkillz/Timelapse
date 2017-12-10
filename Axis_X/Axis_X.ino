#include <EEPROM.h> // knivna pro manipulaci s EEPROM
#include <OneWire.h> // one wire pro teplotní čidlo
#include <DallasTemperature.h>
#include <SoftwareSerial.h> // nutní pro UNO, má jen jeden serial a ten používám pro debug
#include <SoftEasyTransfer.h> // komunikační 

// DEFINICE PINŮ ///////////////////////////////////////////////////////////////////////////////////////////
#define STEP 10
#define MS1 13
#define MS2 11
#define MS3 9
#define DIR 8
#define EN 12

#define ENDSW A5 // mikrospínač
#define HALL A3 // mag. čidlo

#define TEMP A4  // teplotní čidlo

#define Rx 7 // serial Rx
#define Tx 6 // serial Tx

#define OSA 0 // X = 0, Y = 1, Z = 2, W = 3, ovládání = 4, foťák = 5

OneWire oneWire(TEMP);
DallasTemperature sensors(&oneWire);
DeviceAddress insideThermometer;
#define TMP_PREC 12 //přesnost 9 nebo 12

// EasyTransfer ///////////////////////////////////////////////////////////////////////////////////////////
SoftwareSerial softSerial(Rx, Tx);
SoftEasyTransfer ET_Rx, ET_Tx;

struct RECEIVE_DATA_STRUCTURE {
  //put your variable definitions here for the data you want to receive
  //THIS MUST BE EXACTLY THE SAME ON THE OTHER ARDUINO
  int ktery_uzel; // ovladani, pojezd, X ,Y, Z, fotak
  int typ_dat;  // ovládání     teplota       kalibrace   spec.funkce     fotak
  long data_1;      // delitel      teplota       0           ktera funkce    funkce
  long data_2;      // kamOtocit    ktere čidlo   0           0/1 = zap/vyp   vyp/zap
};

struct SEND_DATA_STRUCTURE {
  //put your variable definitions here for the data you want to receive
  //THIS MUST BE EXACTLY THE SAME ON THE OTHER ARDUINO
  int ktery_uzel; // ovladani, pojezd, X ,Y, Z, fotak
  int typ_dat;  // ovládání     teplota       kalibrace   spec.funkce     fotak
  long data_1;      // delitel      teplota       0           ktera funkce    funkce
  long data_2;      // kamOtocit    ktere čidlo   0           0/1 = zap/vyp   vyp/zap
};

//dejme skupině dat jméno
RECEIVE_DATA_STRUCTURE  et_rx_data;
SEND_DATA_STRUCTURE     et_tx_data;


// pomocné proměnné ///////////////////////////////////////////////////////////////////////////////////////
int smer_otaceni = 0; // 0=CW
long max_kroky = 15000;
long akt_kroky = 0;
int min_uhel = -25;
int max_uhel = 85;

void setup() {
  Serial.begin(112500);
  softSerial.begin(9600);

  //zapni softEasyTransfer
  ET_Rx.begin(details(et_rx_data), &softSerial);
  ET_Tx.begin(details(et_tx_data), &softSerial);

  // ovládání
  pinMode(STEP, OUTPUT);
  pinMode(MS1,  OUTPUT);
  pinMode(MS2,  OUTPUT);
  pinMode(MS3,  OUTPUT);
  pinMode(DIR,  OUTPUT);
  pinMode(EN,   OUTPUT);
  pinMode(ENDSW, INPUT_PULLUP);
  pinMode(HALL, INPUT_PULLUP);

  digitalWrite(STEP, LOW);
  digitalWrite(MS1, LOW);
  digitalWrite(MS2, LOW);
  digitalWrite(MS3, LOW);
  digitalWrite(DIR, LOW);
  digitalWrite(EN, LOW);

  sensors.begin(); // teplotní čidlo
  if (!sensors.getAddress(insideThermometer, 0)) Serial.println("Unable to find address for Device 0");
  sensors.setResolution(insideThermometer, TMP_PREC);

  //načtení max. kroků z EEPROM
  /*if (EEPROMReadlong(0) > 10000 and EEPROMReadlong(0) < 15000) {
    max_kroky = EEPROMReadlong(0);
    vynuluj();
    zmenUhel(0);
  }
  else {
    zjistiMaxKroky();
    zmenUhel(0);
  }*/

  zjistiMaxKroky();
}

void loop() {
  //otocNa(random(max_kroky));
  //zmenUhel(random(110)-25);

  sensors.requestTemperatures();
  printTemperature(insideThermometer);

  if (ET_Rx.receiveData()) {
    if (et_rx_data.ktery_uzel == OSA) {
      if (et_rx_data.typ_dat == 0) {
        if (et_rx_data.data_1 != 0) {
          nastavPresnost(abs(et_rx_data.data_1));
          if (et_rx_data.data_1 > 0) {
            nastavSmer(1);
          }
          if (et_rx_data.data_1 < 0) {
            nastavSmer(0);
          }
          pulse();
        }
      }
    }
  }
}
