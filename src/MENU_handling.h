int valveSettingsSelectie(int localValve)
{

  int localPercentageTime = mySprinkler.valve[localValve].percentage;
  int localWithPump = mySprinkler.valve[localValve].withPump;
  char localValveArray[15];
  int widthLocationData = 100;
  sprintf(&localValveArray[0], "%s", sprinklerName_table[localValve]);
  while (1)
  {
    drawInfoBox(0,19,272,240,1, "VALVE SETTINGS");
    snprintf(&data_table[0][0], 10, "%s", localValveArray);
    snprintf(&data_table[1][0], 8, "%d", localPercentageTime);
    snprintf(&data_table[2][0], 8, "%s", janee_table[localWithPump]);
    int keuze = inputDropbox(1, widthLocationData, valveSettings_table, 3);
    switch (keuze)
    {
    case -1:
      return keuze;
      break;
    case 0:
      localValve = stringDropBox(widthLocationData, keuze, localValveArray);
      if (localValve == -1)
        return -1;
      break;
    case 1:
      localPercentageTime = getalDropDown(widthLocationData, keuze, localPercentageTime, 50, 100, 5);
      if (localPercentageTime == -1)
        return -1;
      break;
    case 2:
      localWithPump = localDropbox(localWithPump, widthLocationData, keuze, janee_table, sizeof(janee_table) / sizeof(*janee_table));
      if (localWithPump == -1)

        return -1;

      break;
    }
    drawInfoBox(0,19,272,240,1, "VALVE SETTINGS");
    snprintf(&data_table[0][0], 10, "%s", localValveArray);
    snprintf(&data_table[1][0], 8, "%d", localPercentageTime);
    snprintf(&data_table[2][0], 8, "%s", janee_table[localWithPump]);
    keuze = inputDropbox(0, widthLocationData, valveSettings_table, 3);
    keuze = keyboardButtonBar("ESC", "UP", "DOWN","SAVE");
    switch (keuze)
    {
    case buttonNone:
      return keuze;
      break;
    case 0:
      return keuze;
      break;
    case button1:
      return buttonNone;
      break;
    case button2:
      break;
    case button3: 
      break;
    case button4:
      Serial.print(" valve gegevens wijzigen");
      mySprinkler.valve[localValve].percentage = localPercentageTime;
      mySprinkler.valve[localValve].withPump = localWithPump;
      snprintf(&mySprinkler.valve[localValve].valveName[0], 10, "%s", localValveArray); // naam copieren
      writeMySprinklerFile();
      return 0;
      break;
    }
  }
}

int sprinklerSettingsSelectie()
{
  int pumpTimeKeuze = mySprinkler.pumpTime;
  int pauzeTimeKeuze = mySprinkler.pauzeTime;
  int modusKeuze = mySprinkler.modus;
  int debugKeuze = sprinkler.debugMode;
  int valveKeuze = 0;
  int wisselKeuze = myServer.sensorToggle;
  int widthLocationData = 130;
  while (1)
  {
    drawInfoBox(0,19,272,240,1, "INSTELLINGEN");
    snprintf(&data_table[0][0], 8, "%d", pumpTimeKeuze);
    snprintf(&data_table[1][0], 8, "%d", pauzeTimeKeuze);
    snprintf(&data_table[2][0], 8, "%s", auto_table[modusKeuze]);
    snprintf(&data_table[3][0], 8, "%s", onoff_table[debugKeuze]);
    snprintf(&data_table[4][0], 8, "%s", sprinklerName_table[valveKeuze]);
    snprintf(&data_table[5][0], 8, "%s", janee_table[wisselKeuze]);
    int keuze = inputDropbox(1, widthLocationData, sprinklerSettings_table, 6);
    switch (keuze)
    {
    case -1:
      return keuze;
      break;
    case 0:
      pumpTimeKeuze = getalDropDown(widthLocationData, keuze, pumpTimeKeuze, 0, 15, 1);
      if (pumpTimeKeuze == -1)
        return -1;
      break;
    case 1:
      pauzeTimeKeuze = getalDropDown(widthLocationData, keuze, pauzeTimeKeuze, 0, 15, 1);
      if (pauzeTimeKeuze == -1)
        return -1;
      break;
    case 2:
      modusKeuze = localDropbox(modusKeuze, widthLocationData, keuze, auto_table, sizeof(auto_table) / sizeof(*auto_table));
      if (modusKeuze == -1)
        return -1;
      break;
    case 3:
      debugKeuze = localDropbox(debugKeuze, widthLocationData, keuze, onoff_table, sizeof(onoff_table) / sizeof(*onoff_table));
      if (debugKeuze == -1)
        return -1;
      break;
    case 4:
      valveKeuze = localDropbox(valveKeuze, widthLocationData, keuze, sprinklerName_table, 9);
      if (valveKeuze > 0)
      {
        valveKeuze = valveSettingsSelectie(valveKeuze);
        if (valveKeuze == -1)
          return -1;
        break;
      }
      else if (valveKeuze == 0)
        break;
      else
        return -1;
      break;
    case 5:
      wisselKeuze = localDropbox(wisselKeuze, widthLocationData, keuze, janee_table, sizeof(janee_table) / sizeof(*janee_table));
      if (wisselKeuze == -1)
        return -1;
      break;
    }
    drawInfoBox(0,19,272,240,1, "INSTELLINGEN");
    snprintf(&data_table[0][0], 8, "%d", pumpTimeKeuze);
    snprintf(&data_table[1][0], 8, "%d", pauzeTimeKeuze);
    snprintf(&data_table[2][0], 8, "%s", auto_table[modusKeuze]);
    snprintf(&data_table[3][0], 8, "%s", onoff_table[debugKeuze]);
    snprintf(&data_table[4][0], 8, "%s", sprinklerName_table[valveKeuze]);
    snprintf(&data_table[5][0], 8, "%s", janee_table[wisselKeuze]);
    keuze = inputDropbox(0, widthLocationData, sprinklerSettings_table, 6);
    keuze = keyboardButtonBar("ESC", "UP", "DOWN","SAVE");
    switch (keuze)
    {
    case buttonNone:
      return keuze;
      break;
    case 0:
      return keuze;
      break;
    case button1:
      return buttonNone;
      break;
    case button2:
      break;
    case button3: 
      break;
    case button4:
      mySprinkler.pumpTime = pumpTimeKeuze;
      mySprinkler.pauzeTime = pauzeTimeKeuze;
      mySprinkler.modus = modusKeuze;
      sprinkler.debugMode = debugKeuze;
      myServer.sensorToggle = wisselKeuze;
      writeMySprinklerFile();
      writeMyServerFile();
      controleerProgramma(RTCtime.Hours, RTCtime.Minutes, RTCDate.WeekDay); //controleerProgramma(now.hour(),now.minute(),now.dayOfTheWeek());
      checkRelaisOnTime(RTCtime.Hours, RTCtime.Minutes);
      checkRelaisTempOnTime();
      return 0;
      break;
    }
  }
}

int relaisSelectie()
{
  int localRelais = 0;
  int localTime = 15;
  int widthLocationData = 120;
  while (1)
  {
    drawInfoBox(0,19,272,240,1, "RELAIS SELECTIE");
    snprintf(&data_table[0][0], 10, "%s", relaisName_table[localRelais]);
    snprintf(&data_table[1][0], 10, "%d", localTime);
    int keuze = inputDropbox(1, widthLocationData, sprinklerManueel_table, 2);
    switch (keuze)
    {
    case -1:
      return keuze;
      break;
    case 0:
      localRelais = localDropbox(localRelais, widthLocationData, keuze, relaisName_table, 9);
      if (localRelais == -1)
        return -1;
      break;
    case 1:
      localTime = getalDropDown(widthLocationData, keuze, localTime, 5, 60, 1);
      if (localTime == -1)
        return -1;
      break;
    }
    drawInfoBox(0,19,272,240,1, "RELAIS SELECTIE");
    snprintf(&data_table[0][0], 10, "%s", relaisName_table[localRelais]);
    snprintf(&data_table[1][0], 10, "%d", localTime);
    keuze = inputDropbox(0, widthLocationData, sprinklerManueel_table, 2);
    if (localRelais > 0)
    {
      keuze = keyboardButtonBar("ESC", "UP", "DOWN","START");
    }
    else
    {
      keuze = keyboardButtonBar("ESC", "UP", "DOWN","");
    }
    switch (keuze)
    {
    case buttonNone:
      return keuze;
      break;
    case 0:
      return keuze;
      break;
    case button1:
      return buttonNone;
      break;
    case button2:
      break;
    case button3: 
      break;
    case button4:
      if (localRelais > 0)
      {
        //SprinklerProgram[0].valve[0]=localSprinkler;
        //SprinklerProgram[0].valveTime[0]=localTime;  
        //laadProgramma(0);
        keuze = 0;
        return keuze;
      } 
      break;
    }
  }
}

int sprinklerSelectieTouch(){
  
  boolean sprinklerSelected = false;
  boolean timeSelected = false;  // de 8 knoppen laten zien
  boolean showTimeSelect = false;
  int sprinklerKeuze = -1;
  int sprinklerKeuzePrevious = -1;
  int sprinklerTime = 0;

  touchButton sprinklerButton[8];
  touchButton sprinklerTimeButton =(touchButton) {120+5,127,70,30,DARKGREY,WHITE,"x min"};
  touchButton escapeButton =(touchButton) {0,292,56+4+56+2,30,DARKGREY,BLACK,"ESCAPE"};
  touchButton startButton =(touchButton) {120,292,56+4+56+4,30,DARKGREY,DARKGREY,"START"};

  drawInfoBox(0,19,272,240,1, "SPRINKLER SELECTIE");
  clearButtonBar();
  for (int i = 0; i < 8 ; i++)
  {
    sprinklerButton[i] =(touchButton) {2,28+i*33,116,30,DARKGREY,BLACK,sprinklerName_table[i+1]};
    drawTouchButton(&sprinklerButton[i],2,1); 
  }
  long previousMillis = millis();
  while (1)
  {
    if (clockData.checkSecond)
      readTime();
    if ((millis() - previousMillis) > 10000)
      return buttonNone;
    M5.update();  
    if ( M5.Touch.changed ){ 
      Serial.println( M5.Touch.point[0]);
      int coordinateY = M5.Touch.point[0].y;
      int coordinateX = M5.Touch.point[0].x;
      int keuze =-1;
      boolean checkButton;
      for (int i = 0; i < 8; i++){
        if (checkButton = checkTouchButton(&sprinklerButton[i], coordinateX, coordinateY)){
          soundsBeep(1000, 100, 1);
          previousMillis = millis();
          if (!sprinklerSelected){
            sprinklerKeuze = i;
            sprinklerKeuzePrevious = i;
            sprinklerSelected=true;
            showTimeSelect = true;
            sprinklerButton[sprinklerKeuze].textColor=WHITE;
            drawTouchButton(&sprinklerButton[sprinklerKeuze],2,1);
          } else {
            soundsBeep(1000, 100, 1);
            sprinklerKeuze = i;
            if (sprinklerKeuze !=sprinklerKeuzePrevious){
              sprinklerButton[sprinklerKeuze].textColor=WHITE;
              sprinklerButton[sprinklerKeuzePrevious].textColor=BLACK;
              drawTouchButton(&sprinklerButton[sprinklerKeuze],2,1);
              drawTouchButton(&sprinklerButton[sprinklerKeuzePrevious],2,1);
              sprinklerTimeButton.y = 28+(sprinklerKeuzePrevious+1)*33;      
              hideTouchButton(&sprinklerTimeButton);
              sprinklerKeuzePrevious=sprinklerKeuze;
              showTimeSelect = true;
            }
          }  
        }
      }
      if (showTimeSelect){
        sprinklerTime =getalTouchBox (120, 120, 0, 0, 60, 5,sprinklerName_table[sprinklerKeuze+1]);
        if (sprinklerTime ==buttonNone){
          return buttonNone;
        } else if (sprinklerTime==0){
          timeSelected = false;
        } else {
          timeSelected = true;
          sprinklerTimeButton.y = 28+(sprinklerSelected+1)*33;
          sprinklerTimeButton.text=String(sprinklerTime)+" min";
          drawTouchButton(&sprinklerTimeButton,2,1);
          startButton.textColor=DARKGREY;
          drawTouchButton(&startButton,2,1);  
        }
        showTimeSelect = false;
      }
      if (sprinklerSelected && timeSelected) {
        if (checkButton = checkTouchButton(&startButton, coordinateX, coordinateY)){
          soundsBeep(1000, 100, 1);
          SprinklerProgram[0].valve[0]=sprinklerKeuze+1;
          SprinklerProgram[0].valveTime[0]=sprinklerTime;  
          laadProgramma(0);
          keuze = 0;
          return keuze;
        }
      } 
      if (checkButton = checkTouchButton(&escapeButton, coordinateX, coordinateY)){
          soundsBeep(1000, 100, 1);
          return buttonNone;
      }
    } 
  }
}

int programmaSelectie()
{
  int localProgramma = 1;
  int widthLocationData = 120;
  drawInfoBox(0,19,272,240,1, "PROGRAMMA");
  while (1){
    M5.Lcd.setTextSize(2);
    M5.Lcd.setTextColor(DARKGREY, BLACK);
    M5.Lcd.setCursor(10, 27 + 0 * 18);
    M5.Lcd.printf("%s %02d", "PROGRAMMA", localProgramma);
    M5.Lcd.setTextSize(1);
    for (int i = 0; i < 16; i++)
    {
      M5.Lcd.setCursor(10, 27 + 1 * 18+ 4 + i*10);
      M5.Lcd.printf("%s %02d:%02d", (&mySprinkler.valve[SprinklerProgram[localProgramma].valve[i]].valveName), (SprinklerProgram[localProgramma].valveTime[i] % 60) / 60, SprinklerProgram[localProgramma].valveTime[i] % 60);
    }
    int keuze = keyboardButtonBar("ESC", "-", "+","SLCT");
      switch (keuze)
      {
        case buttonNone:
            return buttonNone;
            break;
        case button1:
            return button1;
            break;
        case button2:
            if (localProgramma > 1) localProgramma = localProgramma - 1;
            Serial.println("down");
            break;
        case button3:
            if (localProgramma < 4) localProgramma = localProgramma + 1;
            Serial.println("up");
            break;
        case button4:
            keuze = keyboardButtonBar2("ESC", "START");
            switch (keuze)
            {
              case buttonNone:
                  return buttonNone;
                  break;
              case button1:
                  return buttonNone;
                  break;
              case button2:
                  Serial.println("programma starten");
                  laadProgramma(localProgramma);
                  return buttonNone; 
                  break;
            }
            break;
      }
  }
}

int uurSelectie()
{
  int localHour = RTCtime.Hours;
  int localMin = RTCtime.Minutes;
  int localSec = RTCtime.Seconds;
  int widthLocationData = 120;
  drawInfoBox(0,19,272,240,1, "UUR WIJZIGEN");
  while (1)
  {
    M5.Lcd.setTextSize(2);
    snprintf(&data_table[0][0], 17, "%2d", localHour);
    snprintf(&data_table[1][0], 17, "%2d", localMin);
    snprintf(&data_table[2][0], 17, "%2d", localSec);
    int keuze = inputDropbox(1, widthLocationData, uurmanueel_table, 3);
    switch (keuze)
    {
    case -1:
      return keuze;
      break;
    case 0:
      localHour = getalDropDown(widthLocationData, keuze, localHour, 0, 23, 1);
      if (localHour == -1)
        return -1;
      break;
    case 1:
      localMin = getalDropDown(widthLocationData, keuze, localMin, 0, 59, 1);
      if (localMin == -1)
        return -1;
      break;
    case 2:
      localSec = getalDropDown(widthLocationData, keuze, localSec, 0, 59, 1);
      if (localSec == -1)
        return -1;
      break;
    }
    drawInfoBox(0,19,272,240,1, "UUR WIJZIGEN");
    snprintf(&data_table[0][0], 17, "%2d", localHour);
    snprintf(&data_table[1][0], 17, "%2d", localMin);
    snprintf(&data_table[2][0], 17, "%2d", localSec);
    keuze = inputDropbox(0, widthLocationData, uurmanueel_table, 3);
    keuze = keyboardButtonBar("ESC", "SLCT", "","SAVE");
    switch (keuze)
      {
        case buttonNone:
            return buttonNone;
            break;
        case button1:
            return button1;
            break;
        case button2:
            break;
        case button3:
            break;
        case button4:
            RTCtime.Hours   = localHour;
            RTCtime.Minutes = localMin;
            RTCtime.Seconds = localSec;
            M5.Rtc.SetTime(&RTCtime);
            return 0;
            break;
      }
  }
  return 0;
}
int datumSelectie()
{
  int localYear = RTCDate.Year;
  int localMonth = RTCDate.Month;
  int localDay = RTCDate.Date;
  int widthLocationData = 120;
  drawInfoBox(0,19,272,240,1, "DATUM WIJZIGEN");
  while (1)
  {
    M5.Lcd.setTextSize(2);
    snprintf(&data_table[0][0], 17, "%2d", localYear);
    snprintf(&data_table[1][0], 17, "%2d", localMonth);
    snprintf(&data_table[2][0], 17, "%2d", localDay);
    int keuze = inputDropbox(1, widthLocationData, datummanueel_table, 3);
    switch (keuze)
    {
    case -1:
      return keuze;
      break;
    case 0:
      localYear = getalDropDown(widthLocationData, keuze, localYear, 2020, 2040, 1);
      if (localYear == -1)
        return -1;
      break;
    case 1:
      localMonth = getalDropDown(widthLocationData, keuze, localMonth, 1, 12, 1);
      if (localMonth == -1)
        return -1;
      break;
    case 2:
      localDay = getalDropDown(widthLocationData, keuze, localDay, 1, daysInMonth[localMonth - 1], 1);
      if (localDay == -1)
        return -1;
      break;
    }
    drawInfoBox(0,19,272,240,1, "DATUM WIJZIGEN");
    snprintf(&data_table[0][0], 17, "%2d", localYear);
    snprintf(&data_table[1][0], 17, "%2d", localMonth);
    snprintf(&data_table[2][0], 17, "%2d", localDay);
    keuze = inputDropbox(0, widthLocationData, datummanueel_table, 3);
    keuze = keyboardButtonBar("ESC", "SLCT", "","SAVE");
    switch (keuze)
      {
        case buttonNone:
            return buttonNone;
            break;
        case button1:
            return button1;
            break;
        case button2:
            break;
        case button3:
            break;
        case button4:
            RTCDate.Month = localMonth;
            RTCDate.Date = localDay;
            RTCDate.Year = localYear;
            M5.Rtc.SetDate(&RTCDate);
            return 0;
            break;
      }
  }
  return 0;
}

int resetOptions(){
  drawInfoBox(0,19,272,240,1, "RESET OPTIONS");
  while (1)
  {
    int keuze = keyboardButtonBar("ESC", "", "","");
      switch (keuze)
      {
        case buttonNone:
            return buttonNone;
            break;
        case button1:
            return button1;
            break;
        case button2:
            break;
        case button3:
            break;
        case button4:
            break;
      }
  }
  return 0;
}


int settingsMenu()
{
  drawInfoBox(0,19,272,240,1, "INSTELLINGEN");
  int keuze = menuTouchbox(settingsMenu_table, sizeof(settingsMenu_table) / sizeof(*settingsMenu_table));
  switch (keuze)
  {
  case buttonNone:
    return buttonNone;
    break;
  case 0:
    keuze = uurSelectie();
    return keuze;
    break;
  case 1:
    keuze = datumSelectie();
    return keuze;
    break;
  case 2:
    keuze = sprinklerSettingsSelectie();
    return keuze;
    break;
  case 3:
    keuze = resetOptions();
    return keuze;
    break;
  }
  return 1;
}






int mainMenu()
{
  drawInfoBox(0,19,272,240,1, "HOOFDMENU");
  int keuze = menuTouchbox(mainMenu_table, sizeof(mainMenu_table) / sizeof(*mainMenu_table));
  switch (keuze)
  {
  case buttonNone:
    return buttonNone;
    break;
  case 0:
    keuze = programmaSelectie();
    return keuze;
    break;
  case 1:
    keuze = sprinklerSelectieTouch();
    //keuze = sprinklerSelectie();
    return keuze;
    break;
  case 2:
    keuze = relaisSelectie();
    return keuze;
    break;
  case 3:
    Serial.println("programma wijzigen");
    return 1;
    break;
  case 4: 
    keuze = settingsMenu();
    return 1;
    break;
  }
  return 1;
}
