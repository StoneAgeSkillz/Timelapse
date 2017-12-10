#include <OneWire.h> // one wire pro teplotní čidlo
#include <SoftwareSerial.h> // nutné pro UNO, má jen jeden serial a ten používám pro debug
#include <SoftEasyTransfer.h> // komunikační knihovna

#define STEP A1
#define MS1 A4
#define MS2 A3
#define MS3 A2
#define DIR A0
#define EN A5

#define TEMP 10 // teplotní čidlo

// nastavení a proměnné komunikace
SoftwareSerial softSerial(3, 4); // Rx, Tx
SoftEasyTransfer ETRx, ETTx;


struct RECEIVE_DATA_STRUCTURE {
  //put your variable definitions here for the data you want to receive
  //THIS MUST BE EXACTLY THE SAME ON THE OTHER ARDUINO
  int ktery_uzel; // ovladani, pojezd, X ,Y, Z, fotak
  int typ_dat;  // ovládání     teplota       kalibrace   spec.funkce     fotak
  int data_1;      // delitel      teplota       0           ktera funkce    funkce
  int data_2;      // kamOtocit    ktere čidlo   0           0/1 = zap/vyp   vyp/zap
};

struct SEND_DATA_STRUCTURE {
  //put your variable definitions here for the data you want to receive
  //THIS MUST BE EXACTLY THE SAME ON THE OTHER ARDUINO
  int ktery_uzel; // ovladani, pojezd, X ,Y, Z, fotak
  int typ_dat;  // ovládání     teplota       kalibrace   spec.funkce     fotak
  int data_1;      // delitel      teplota       0           ktera funkce    funkce
  int data_2;      // kamOtocit    ktere čidlo   0           0/1 = zap/vyp   vyp/zap
};

//dejme skupině dat jméno
RECEIVE_DATA_STRUCTURE  rx_data;
SEND_DATA_STRUCTURE     tx_data;


// nastaveni pro osy (step, o kolik/kam)
int chod = 0; // 1 == Full step, 2 = 1/2 step... (1; 2; 4; 8; 16)

int microstep = 1;


unsigned long int kroky_osy = 0;
unsigned long int kroky_osy_max = 3000000;

int typ = 0;
int jdi_na = 0;


void setup() {
  Serial.begin(112500);
  softSerial.begin(9600);

  //zapni softEasyTransfer
  ETRx.begin(details(rx_data), &softSerial);
  ETTx.begin(details(tx_data), &softSerial);


  // ovládání
  pinMode(STEP, OUTPUT);
  pinMode(MS1,  OUTPUT);
  pinMode(MS2,  OUTPUT);
  pinMode(MS3,  OUTPUT);
  pinMode(DIR,  OUTPUT);
  pinMode(EN,   OUTPUT);
  digitalWrite(STEP, LOW);
  digitalWrite(MS1, LOW);
  digitalWrite(MS2, LOW);
  digitalWrite(MS3, LOW);
  digitalWrite(DIR, HIGH);
  digitalWrite(EN, LOW);

  //zjistiMaxKroky();
}
void loop() {

  if (ETRx.receiveData()) {
    if (rx_data.ktery_uzel == 0) { // 0 == osa W
      Serial.println(String(rx_data.ktery_uzel, DEC) + ": " + String(rx_data.typ_dat, DEC) + " - data1: " + String(rx_data.data_1, DEC) + " / data2: " + String(rx_data.data_2, DEC));
      chod = rx_data.data_1;
    }
    delay(5);
  }

  if (rx_data.typ_dat == 0) {
    if (chod != 0) {
      if (chod < 0) {
        for (int i = 1; i <= 16; i = i * 2 ) {
          if (((kroky_osy) - (16 / i)) < kroky_osy_max) {
            if (i >= abs(chod)) {
              microstep = -i;
            }
            else {
              microstep = chod;
            }

            break;
          }
        }
      }

      if (chod > 0) {
        for (int i = 1; i <= 16; i = i * 2 ) {
          if ((kroky_osy + (16 / i)) <= kroky_osy_max) {
            if (i >= abs(chod)) {
              microstep = i;
            }
            else {
              microstep = chod;
            }
            break;
          }
        }
      }

      if (kroky_osy + 16 / microstep <= kroky_osy_max) {

        if (chod > 0) {
          nastavSmerOtaceni(0);  //CW
        }
        else {
          nastavSmerOtaceni(1); //CCW
        }

        nastavPresnost(abs(microstep));
        pulse();
        
        kroky_osy = kroky_osy + 16 / microstep;        

        Serial.println("chod: " + String(16 / chod, DEC) + " => kroky osy: " + String(kroky_osy, DEC) + " +( " + String(16 / microstep) + " ) / data2: " + String(rx_data.data_2, DEC));
      }
    }
    else {
      microstep = 0;
    }
  }
}
