void piezoBeep(int pocet_pipnuti, int nota){
  for (int i = 0; i < pocet_pipnuti; i++) {

      // to calculate the note duration, take one second divided by the note type.
      //e.g. quarter note = 1000 / 4, eighth note = 1000/8, etc.
      int noteDuration = 150;
      tone(PIEZO, nota, noteDuration);

      // to distinguish the notes, set a minimum time between them.
      // the note's duration + 30% seems to work well:
      int pauseBetweenNotes = noteDuration * 1.15;
      delay(pauseBetweenNotes);
      // stop the tone playing:
      noTone(PIEZO);
    }
}

