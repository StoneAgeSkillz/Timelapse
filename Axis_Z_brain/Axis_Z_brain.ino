#include <DS3231.h> //RTC
#include <Wire.h> // I2C/OneWire
#include <UTFT.h> // TFT LCD
#include <UTouch.h>// TFT Touch
#include <UTFT_Geometry.h> // TFT pokročilá geometrie
#include <EasyTransfer.h> // komunikační knihovna
#include "noty.h" //piezo
#include "DHT.h" // čidlo DHT22

// EASY TRANSFER //////////////////////////////////////////////////////////////////////////
EasyTransfer ET_Rx, ET_Tx;

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
RECEIVE_DATA_STRUCTURE rx_data;
SEND_DATA_STRUCTURE tx_data;
int uzel = 0, typ_dat = 0;
long data_1 = 0, data_2 = 0;


// TOUCHSCREEN ///////////////////////////////////////////////////////////////////////////////
// Initialize display ////////////////////////////////////////////////////////////////////////
// ------------------
// Set the pins to the correct ones for your development board
// -----------------------------------------------------------
// Standard Arduino Uno/2009 Shield            : <display model>,19,18,17,16
// Standard Arduino Mega/Due shield            : <display model>,38,39,40,41
// CTE TFT LCD/SD Shield for Arduino Due       : <display model>,25,26,27,28
// Teensy 3.x TFT Test Board                   : <display model>,23,22, 3, 4
// ElecHouse TFT LCD/SD Shield for Arduino Due : <display model>,22,23,31,33
//
// Remember to change the model parameter to suit your display module!
UTFT    GLCD(ITDB32S, 38, 39, 40, 41);

// Initialize touchscreen ///////////////////////////////////////////////////////////////////
// ----------------------
// Set the pins to the correct ones for your development board
// -----------------------------------------------------------
// Standard Arduino Uno/2009 Shield            : 15,10,14, 9, 8
// Standard Arduino Mega/Due shield            :  6, 5, 4, 3, 2
// CTE TFT LCD/SD Shield for Arduino Due       :  6, 5, 4, 3, 2
// Teensy 3.x TFT Test Board                   : 26,31,27,28,29
// ElecHouse TFT LCD/SD Shield for Arduino Due : 25,26,27,29,30
//
UTouch  myTouch( 6, 5, 4, 3, 2);

UTFT_Geometry GEO(&GLCD);

extern uint8_t BigFont[];
extern uint8_t SmallFont[];
extern uint8_t SevenSegNumFont[];

// RTC //////////////////////////////////////////////////////////////////////////////////////
DS3231 RTC;
bool stoleti = false;
bool h12; // dvanactihodinovy format
bool pm; // AM/PM
byte alarm_day, alarm_hour, alarm_minute, alarm_second, alarm_bits;
bool alarm_dy, alarm_12h, alarm_pm;

String jmena_dni[] = {"ne", "po", "ut", "st", "ct", "pa", "so"};

// PIEZO ////////////////////////////////////////////////////////////////////////////////////
#define PIEZO 12

// DHT22 ////////////////////////////////////////////////////////////////////////////////////
#define DHTPIN 13
#define DHTTYPE DHT22   // DHT 22  (AM2302), AM2321
DHT dht(DHTPIN, DHTTYPE);
float vlhkost;
float teplota;
float teplotni_index;
bool obnov_dht = true;
int interval_obnoveni_dht = 2; // obnoví každých interval_obnoveni_dht vteřin, kde interval_obnoveni_dht je modulo pro výpočet

//JOYSTICK - ADC ////////////////////////////////////////////////////////////////////////////
#define ADC_x A2  // Analog input pin na kterem je joystick 1 - osax
#define ADC_y A1  // Analog input pin na kterem je joystick 1 - osay
#define JOY_BUTTON A0 // joystick - tlačítko

int joystick_value_x = 0;        // hodnoty čtené z ADC (joystick)
int joystick_value_y = 0;

bool joystick_button = false;

int joy_nula_x = 0; // pro uložení nulové pozice po kalibraci
int joy_nula_y = 0;


// POMOCNE PROMENNE ////////////////////////////////////////////////////////////////////////
// nastaveni pro osy (step, o kolik/kam)
int chod_x[] = {0, 0, 0, 0}; // 1 == Full step, 2 = 1/2 step... (1; 2; 4; 8; 16)
int chod_y = 1; // 1 == Full step, 2 = 1/2 step... (1; 2; 4; 8; 16)
int microstep_x = 1;

bool refresh_LCD = true; // cele LCD
bool refresh_LCD_tlacitka = true; // jen tlacitka
bool refresh_LCD_kroky = true; //jen hodnoty kroků

int vyber_osy = 0; //ktera osa se ma ovladat + označení tlačítka
int vyber_osy_prev = vyber_osy; //ktera osa se ovladala, nutne pro překreslení tlačítka zpět na nepoužité

unsigned long int kroky_osy[] = {0, 0, 0, 0}; //W, Z, Y, X
unsigned long int kroky_osy_max[] = {0, 0, 0, 0}; //soft limit W, Z, Y, X

String popisky_osy[] = {"X", "Y", "Z", "W"};

int typ = 0;
int jdi_na = 0;


// TOUCH ///////////////////////////////////////////////////////////////////////////////////
int touch_x, touch_y;
#define TOUCH_DELAY 2 // prodleva reakce na zmáčknutí TFT, v cyklech
unsigned int touch_delay = 0;
boolean touch_press = false;

int joystick_limity[] = {20, 50, 70, 85, 93, 200}; //hranice nastavení rychlosti 10~25 = 1/16; 25~40 = 1/8...
int microstep_x_predchozi = 0; // pomocna promenna pro rozeznání pohybu joysticku
int data_y_predchozi = 0;

// POLOHY TLACITEK NA TFT //////////////////////////////////////////////////////////////////
#define CUDL_OSA_W_x 1
#define CUDL_OSA_W_y 177
#define CUDL_OSA_W_hrana 62

#define CUDL_OSA_X_x 193
#define CUDL_OSA_X_y 177
#define CUDL_OSA_X_hrana 62

#define CUDL_OSA_Y_x 129
#define CUDL_OSA_Y_y 177
#define CUDL_OSA_Y_hrana 62

#define CUDL_OSA_Z_x 65
#define CUDL_OSA_Z_y 177
#define CUDL_OSA_Z_hrana 62

#define CUDL_SETINGS_x 257
#define CUDL_SETINGS_y 177
#define CUDL_SETINGS_hrana 62


void setup() {
  // initializace komunikace s PC

  Serial.begin(115200);

  // EasyTransfer serial
  Serial1.begin(9600);
  //start the library, pass in the data details and the name of the serial port. Can be Serial, Serial1, Serial2, etc.
  ET_Rx.begin(details(rx_data), &Serial1);
  ET_Tx.begin(details(tx_data), &Serial1);

  // joystick tlačíko
  pinMode(JOY_BUTTON, INPUT);
  digitalWrite(JOY_BUTTON, HIGH);

  // I2C
  Wire.begin();

  // DHT
  dht.begin();

  // TFT LCD
  GLCD.InitLCD();
  GLCD.clrScr();
  // TOUCH
  myTouch.InitTouch();
  myTouch.setPrecision(PREC_MEDIUM);

  kalibrujJSTICK();

  vykresliPozadi();
  vykresliTlacitko( CUDL_OSA_Z_x,   CUDL_OSA_Z_y,   CUDL_OSA_Z_hrana, "Z", false);
  vykresliTlacitko( CUDL_OSA_Y_x,   CUDL_OSA_Y_y,   CUDL_OSA_Y_hrana, "Y", false);
  vykresliTlacitko( CUDL_OSA_X_x,   CUDL_OSA_X_y,   CUDL_OSA_X_hrana, "X", false);
  vykresliTlacitko( CUDL_OSA_W_x,   CUDL_OSA_W_y,   CUDL_OSA_W_hrana, "W", false);
  vykresliTlacitko( CUDL_SETINGS_x, CUDL_SETINGS_y, CUDL_SETINGS_hrana, "SET", false);

  //vykresliKruhovouSipku(100, 100, 25, true);

}

void loop() {
  //--> RTC //////////////////////////////////////////////////////////////////////
  hodiny(10, 0);
  //<-- RTC //////////////////////////////////////////////////////////////////////

  //--> DHT22 //////////////////////////////////////////////////////////////////////
  if (!(RTC.getSecond() % interval_obnoveni_dht) and obnov_dht == true) {
    // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
    vlhkost = dht.readHumidity();
    // Read temperature as Celsius (the default)
    teplota = dht.readTemperature();

    // Check if any reads failed and exit early (to try again).
    if (isnan(vlhkost) || isnan(teplota)) {
      Serial.println("Failed to read from DHT sensor!");
    }
    else {
      // vypočti teplotní index v °C (Fahreheit = false)
      teplotni_index = dht.computeHeatIndex(teplota, vlhkost, false);
      vypis_DHT(264, 147, teplota, vlhkost, teplotni_index);
      obnov_dht = false;
    }
  }
  else if (RTC.getSecond() % interval_obnoveni_dht and obnov_dht == false) {
    obnov_dht = true;
  }
  //<-- DHT22//////////////////////////////////////////////////////////////////////

  //--> JOYSTICK//////////////////////////////////////////////////////////////////////
  joystick_value_x =  (analogRead(ADC_x) / 5);
  //delay(5);

  int joystick_x = joystick_value_x - joy_nula_x;

  // zjisteni vyhnuti joysticku pro nastevení rychlosti posuvu
  for (int i = 0; i < 5; i++) {
    if (joystick_x > joystick_limity[i] and joystick_x <= joystick_limity[(i + 1)] and joystick_x > 0) {
      switch (i) {
        case 0: chod_x[vyber_osy] = 16; break;
        case 1: chod_x[vyber_osy] = 8; break;
        case 2: chod_x[vyber_osy] = 4; break;
        case 3: chod_x[vyber_osy] = 2; break;
        case 4: chod_x[vyber_osy] = 1; break;
      }
    }

    else if (abs(joystick_x) > joystick_limity[i] and abs(joystick_x) <= joystick_limity[(i + 1)] and joystick_x < 0) {
      switch (i) {
        case 0: chod_x[vyber_osy] = -16; break;
        case 1: chod_x[vyber_osy] = -8; break;
        case 2: chod_x[vyber_osy] = -4; break;
        case 3: chod_x[vyber_osy] = -2; break;
        case 4: chod_x[vyber_osy] = -1; break;
      }
    }
  }


  //--> propocty pohybu ////////////////////////////////////////////////////////////////////////////////////////////////
  if (chod_x[vyber_osy] != 0) {
    if (chod_x[vyber_osy] < 0) {
      for (int i = 1; i <= 16; i = i * 2 ) {
        if (((kroky_osy[vyber_osy]) - (16 / i)) < kroky_osy_max[vyber_osy]) {
          if (i >= abs(chod_x[vyber_osy])) {
            microstep_x = -i;
          }
          else {
            microstep_x = chod_x[vyber_osy];
          }

          break;
        }
      }
    }

    if (chod_x[vyber_osy] > 0) {
      for (int i = 1; i <= 16; i = i * 2 ) {
        if ((kroky_osy[vyber_osy] + (16 / i)) <= kroky_osy_max[vyber_osy]) {
          if (i >= abs(chod_x[vyber_osy])) {
            microstep_x = i;
          }
          else {
            microstep_x = chod_x[vyber_osy];
          }
          break;
        }
      }
    }

    if (chod_x[vyber_osy] != 0 and kroky_osy[vyber_osy] + 16 / microstep_x <= kroky_osy_max[vyber_osy]) {
      kroky_osy[vyber_osy] = kroky_osy[vyber_osy] + 16 / microstep_x;
      refresh_LCD_kroky = true;
    }
  }
  else {
    microstep_x = 0;
  }

  //<-- propocty pohybu ////////////////////////////////////////////////////////////////////////////////////////////////

  if (digitalRead(JOY_BUTTON) == LOW) {

    piezoBeep(1, NOTE_C7);
    refresh_LCD_tlacitka = true;

    delay(50);
    if (vyber_osy < 4) {
      vyber_osy++;
    }
    else {
      vyber_osy = 1;
    }
  }
  //<--JYOSTICK//////////////////////////////////////////////////////////////////////

  //--> TOUCH //////////////////////////////////////////////////////////////////////
  if (myTouch.dataAvailable()) {
    myTouch.read();
    touch_x = myTouch.getX();
    touch_y = myTouch.getY();
    touch_press = true;

    // OSA W //
    if ((touch_x >= CUDL_OSA_W_x) && (touch_x <= CUDL_OSA_W_x + CUDL_OSA_W_hrana) && (touch_y >= CUDL_OSA_W_y) && (touch_y <= CUDL_OSA_W_y + CUDL_OSA_W_hrana)) {
      if (touch_press == true && touch_delay < touch_delay)
      {
        touch_delay++;
      }
      else
      {
        touch_delay = 0;
        vyber_osy = 0;
        refresh_LCD_tlacitka = true;
      }
    }
    // OSA X
    else if ((touch_x >= CUDL_OSA_X_x) && (touch_x <= CUDL_OSA_X_x + CUDL_OSA_X_hrana) && (touch_y >= CUDL_OSA_X_y) && (touch_y <= CUDL_OSA_X_y + CUDL_OSA_X_hrana)) {
      if (touch_press == true && touch_delay < touch_delay)
      {
        touch_delay++;
      }
      else
      {
        touch_delay = 0;
        vyber_osy = 1;
        refresh_LCD_tlacitka = true;
      }
    }
    // OSA Y
    else if ((touch_x >= CUDL_OSA_Y_x) && (touch_x <= CUDL_OSA_Y_x + CUDL_OSA_Y_hrana) && (touch_y >= CUDL_OSA_Y_y) && (touch_y <= CUDL_OSA_Y_y + CUDL_OSA_Y_hrana)) {
      if (touch_press == true && touch_delay < touch_delay)
      {
        touch_delay++;
      }
      else
      {
        touch_delay = 0;
        vyber_osy = 2;
        refresh_LCD_tlacitka = true;
      }
    }
    // OSA Z
    else if ((touch_x >= CUDL_OSA_Z_x) && (touch_x <= CUDL_OSA_Z_x + CUDL_OSA_Z_hrana) && (touch_y >= CUDL_OSA_Z_y) && (touch_y <= CUDL_OSA_Z_y + CUDL_OSA_Z_hrana)) {
      if (touch_press == true && touch_delay < touch_delay)
      {
        touch_delay++;
      }
      else
      {
        touch_delay = 0;
        vyber_osy = 3;
        refresh_LCD_tlacitka = true;
      }
    }
  }
  //<-- TOUCH //////////////////////////////////////////////////////////////////////

  //--> obnova grafiky //////////////////////////////////////////////////////////////////////////////////////////////
  if (refresh_LCD == true or refresh_LCD_tlacitka == true or refresh_LCD_kroky == true) {
    if (refresh_LCD_tlacitka == true or refresh_LCD == true) {
      switch (vyber_osy_prev) {// tlačítko původní - předchozí překresli
        case 3: vykresliTlacitko( CUDL_OSA_Z_x,   CUDL_OSA_Z_y,   CUDL_OSA_Z_hrana, "Z", false);      break;
        case 2: vykresliTlacitko( CUDL_OSA_Y_x,   CUDL_OSA_Y_y,   CUDL_OSA_Y_hrana, "Y", false);      break;
        case 1: vykresliTlacitko( CUDL_OSA_X_x,   CUDL_OSA_X_y,   CUDL_OSA_X_hrana, "X", false);      break;
        case 4: vykresliTlacitko( CUDL_OSA_W_x,   CUDL_OSA_W_y,   CUDL_OSA_W_hrana, "W", false);      break;
      }
      switch (vyber_osy) { // tlačítko načerveno
        case 3: vykresliTlacitko( CUDL_OSA_Z_x,   CUDL_OSA_Z_y,   CUDL_OSA_Z_hrana, "Z", true);      break;
        case 2: vykresliTlacitko( CUDL_OSA_Y_x,   CUDL_OSA_Y_y,   CUDL_OSA_Y_hrana, "Y", true);      break;
        case 1: vykresliTlacitko( CUDL_OSA_X_x,   CUDL_OSA_X_y,   CUDL_OSA_X_hrana, "X", true);      break;
        case 4: vykresliTlacitko( CUDL_OSA_W_x,   CUDL_OSA_W_y,   CUDL_OSA_W_hrana, "W", true);      break;
      }
    }

    if (refresh_LCD_kroky == true or refresh_LCD == true) { // kroky
      GLCD.setColor(VGA_WHITE);
      GLCD.setBackColor(VGA_BLACK);
      GLCD.setFont(SmallFont);

      GLCD.print(dodejNuly(kroky_osy[0]), 260, 18);
      GLCD.print("mm " + dodejMezery(prepoctiKrokyNaMM(kroky_osy[0], 200)), 260, 30);

      GLCD.print(dodejNuly(kroky_osy[1]), 260, 50);
      GLCD.print("deg " + dodejMezery(int(prepoctiKrokyNaStupne(kroky_osy[3], kroky_osy_max[3]))), 260, 62);

      GLCD.print(dodejNuly(kroky_osy[2]), 260, 82);
      GLCD.print("deg " + dodejMezery(int(prepoctiKrokyNaStupne(kroky_osy[2], kroky_osy_max[2]))), 260, 94);

      GLCD.print(dodejNuly(kroky_osy[3]), 260, 114);
      GLCD.print("deg " + dodejMezery(int(prepoctiKrokyNaStupne(kroky_osy[1], kroky_osy_max[1]))), 260, 126);

      vykresliOsuW(10, 150, kroky_osy[0]);
      vykresli_osovy_kriz(100, 100, 50);

      refresh_LCD_kroky = false;
    }


    vyber_osy_prev = vyber_osy;
    refresh_LCD = false;
  }
  //<-- obnova grafiky //////////////////////////////////////////////////////////////////////////////////////////////

  //--> Serial com //////////////////////////////////////////////////////////////////////////////////////////////////
  if (typ == 0) { // 0 = man. pohyb
    //Serial.println (String(chod_x, DEC) + " ");
    if (microstep_x != microstep_x_predchozi) {
      tx_data.ktery_uzel = vyber_osy; // osa
      tx_data.typ_dat = typ; // poloha manuáně
      tx_data.data_1 = microstep_x;
      tx_data.data_2 = jdi_na;

      ET_Tx.sendData();
      Serial.println("Tx - uzel: " + String(tx_data.ktery_uzel) + "; typ dat: " + String(tx_data.typ_dat) + "; tx_data.data_1 " + String(tx_data.data_1) + "; tx_data.data_2 " + String(tx_data.data_2));
    }
  }

  if (ET_Rx.receiveData()) {
    
    kroky_osy_max[rx_data.ktery_uzel] = rx_data.data_1;
    kroky_osy[rx_data.ktery_uzel] = rx_data.data_1; 
    refresh_LCD_kroky = true;   
    
    Serial.println("Rx - uzel: " + String(rx_data.ktery_uzel) + "; typ dat: " + String(rx_data.typ_dat) + "; tx_data.data_1 " + String(rx_data.data_1) + "; tx_data.data_2 " + String(rx_data.data_2));
  }

  //<-- Serial com //////////////////////////////////////////////////////////////////////////////////////////////////


  microstep_x_predchozi = microstep_x;
  chod_x[vyber_osy] = 0;
}

