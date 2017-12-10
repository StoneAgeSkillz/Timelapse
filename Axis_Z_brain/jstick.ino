void kalibrujJSTICK(){
  //získej analogové hodnoty
  int hodnoty_x = 0;
  int hodnoty_y = 0;

  for (int i = 0; i < 4 ; i++){    
    hodnoty_x = hodnoty_x + analogRead(ADC_x) / 5 ;
    hodnoty_y = hodnoty_y + analogRead(ADC_y) / 5 ;
  }

  joy_nula_x = hodnoty_x/4;
  joy_nula_y = hodnoty_y/4;
}

