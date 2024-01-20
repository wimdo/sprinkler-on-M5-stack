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

int timeSettingTouch(){
  int spriteX = 0;
  int spriteY = 19;
  int spriteWidth = 240;
  int spriteHeight = 320;
  int localHour = RTCtime.Hours;
  int localMin = RTCtime.Minutes;
  int localSec = RTCtime.Seconds;
  int localYear = RTCDate.Year;
  int localMonth = RTCDate.Month;
  int localDay = RTCDate.Date;
  char buffer[15];
  boolean refreshSprite = true;
  boolean valueChanged[6] ={false,false,false,false,false,false};

  touchButton localButton[6];
  touchButton dataButton[6];
  for (int i = 0; i < 6 ; i++)
  {
    localButton[i] =(touchButton) {2,9+i*33,126,30,DARKGREY,BLACK,tijdManueel_table[i]};
    dataButton[i]= (touchButton) {130,9+i*33,90,30,DARKGREY,BLACK,""};
  }
  dataButton[0].text=String(localHour);
  dataButton[1].text=String(localMin);
  dataButton[2].text=String(localSec);
  dataButton[3].text=String(localYear);
  dataButton[4].text= String(localMonth);
  dataButton[5].text=String(localDay);
  //char *tijdManueel_table[] = {"Uur", "Min", "Sec","Dag", "Maand", "Jaar"};
  touchButton escapeButton =(touchButton) {0,272,118,30,DARKGREY,BLACK,"ESC"};
  touchButton saveButton =(touchButton) {120,272,118,30,DARKGREY,BLACK,"SAVE"};
  spr.setColorDepth(8);
  spr.createSprite(spriteWidth,spriteHeight);
  long previousMillis = millis();
  while (1)
  {
    if (refreshSprite) {
      spr.fillRect(0,0,spriteWidth,spriteHeight, BLACK);
      drawInfoBoxSprite(&spr,240,271,1,"TIJD EN DATUM");
      for (int i = 0; i < 6 ; i++)
      {
        drawTouchButtonSprite(&spr,&localButton[i],2,1); 
        drawTouchButtonSprite(&spr,&dataButton[i],2,1); 
      }
      drawTouchButtonSprite(&spr,&escapeButton,2,1); 
      drawTouchButtonSprite(&spr,&saveButton,2,1);
      spr.pushSprite(spriteX,spriteY);
      refreshSprite =false;
    }
    if (clockData.checkSecond) readTime();
    if ((millis() - previousMillis) > 10000){
      spr.deleteSprite();
      return buttonNone;
    } 
    M5.update();
    int coordinateY = M5.Touch.point[0].y;  
    int coordinateX = M5.Touch.point[0].x;
    int keuze =-1;  
    for (int i = 0; i < 6 ; i++)
    {
      if (checkTouchButtonSprite(&localButton[i], spriteX, spriteY,coordinateX, coordinateY) || checkTouchButtonSprite(&dataButton[i], spriteX, spriteY,coordinateX, coordinateY) ){
        previousMillis = millis();
        valueChanged[i]=true;
      }
    }
    //char *tijdManueel_table[] = {"Uur", "Min", "Sec","Dag", "Maand", "Jaar"};
    for (int i = 0; i < 6 ; i++)
    {
      if (valueChanged[i]){
        localButton[i].text.toCharArray(buffer,localButton[i].text.length());
        switch (i){
          case 0:
            keuze =getalTouchBoxSprite (50, 50, localHour, 0, 23, 1,buffer);
            if ((keuze !=buttonNone)&&(keuze!=mySprinkler.pumpTime)){
              localHour=keuze;
              dataButton[0].text=String(localHour);
              dataButton[0].textColor=WHITE;
              localButton[0].textColor=WHITE;
            } else {
              dataButton[0].textColor=BLACK;
              localButton[0].textColor=BLACK;
            }
            break;
          case 1:
            keuze =getalTouchBoxSprite (50, 50, localMin, 0, 59, 1,buffer);
            if ((keuze !=buttonNone)&&(keuze!=localMin)){
              localMin=keuze;
              dataButton[1].text=String(localMin); 
              dataButton[1].textColor=WHITE;
              localButton[1].textColor=WHITE;              
            } else {
              dataButton[1].textColor=BLACK;
              localButton[1].textColor=BLACK;
            }
            break;
          case 2:
            keuze =getalTouchBoxSprite (50, 50, localSec, 0, 59, 1,buffer);
            if ((keuze !=buttonNone)&&(keuze!=localSec)){
              localSec = keuze;
              dataButton[2].text=String(localSec);
              dataButton[2].textColor=WHITE;
              localButton[2].textColor=WHITE;
            } else {
              dataButton[2].textColor=BLACK;
              localButton[2].textColor=BLACK;
            }
            break;
          case 3:
            keuze =getalTouchBoxSprite (50, 50, localYear, 2020, 2040, 1,buffer);  
            if ((keuze !=buttonNone)&&(keuze!=localYear)){
              localYear=keuze;
              dataButton[3].text=String(localYear);
              dataButton[3].textColor=WHITE;
              localButton[3].textColor=WHITE;
            } else {
              dataButton[3].textColor=BLACK;
              localButton[3].textColor=BLACK;
            }
            break;
          case 4:
            keuze =getalTouchBoxSprite (50, 50, localMonth, 1, 12, 1,buffer);  
            if ((keuze !=buttonNone)&&(keuze!=localMonth)){
              localMonth=keuze;
              dataButton[3].text=String(localMonth);
              dataButton[3].textColor=WHITE;
              localButton[3].textColor=WHITE;
            } else {
              dataButton[3].textColor=BLACK;
              localButton[3].textColor=BLACK;
            }          
            break;
          case 5:
            keuze =getalTouchBoxSprite (50, 50, localDay, 0, daysInMonth[localMonth - 1], 1,buffer);
            if ((keuze !=buttonNone)&&(keuze!=localDay)){
              localDay=keuze;
              dataButton[5].text=String(localDay);
              dataButton[5].textColor=WHITE;
              localButton[5].textColor=WHITE; 
            }  else {
              dataButton[5].textColor=BLACK;
              localButton[5].textColor=BLACK;
            }
            break;
        }
        valueChanged[i]=false;
        refreshSprite = true;
      }
    }
    if (checkTouchButtonSprite(&saveButton, spriteX, spriteY,coordinateX, coordinateY)){
      for (int i = 0; i < 6 ; i++)
      {
        if (dataButton[i].textColor=WHITE){

          Serial.println("values saved");
          break;
        }
      }
      spr.deleteSprite();
      return buttonNone;
    } 
    if (checkTouchButtonSprite(&escapeButton, spriteX, spriteY,coordinateX, coordinateY)){
      spr.deleteSprite();
      return buttonNone;
    }


  }
}

int sprinklerSettingTouch(){
  int spriteX = 0;
  int spriteY = 19;
  int spriteWidth = 240;
  int spriteHeight = 320;
  int pumpTimeKeuze = mySprinkler.pumpTime;
  int pauzeTimeKeuze = mySprinkler.pauzeTime;
  int modusKeuze = mySprinkler.modus;
  int debugKeuze = sprinkler.debugMode;
  int valveKeuze = 0;
  int wisselKeuze = myServer.sensorToggle;
  char buffer[15];
  boolean refreshSprite = true;
  boolean valueChanged[6] ={false,false,false,false,false,false};

  touchButton localButton[6];
  touchButton dataButton[6];
  for (int i = 0; i < 6 ; i++)
  {
    localButton[i] =(touchButton) {2,9+i*33,126,30,DARKGREY,BLACK,sprinklerSettings_table[i]};
    dataButton[i]= (touchButton) {130,9+i*33,90,30,DARKGREY,BLACK,""};
  }
  dataButton[0].text=String(pumpTimeKeuze);
  dataButton[1].text=String(pauzeTimeKeuze);
  dataButton[2].text=auto_table[modusKeuze];
  dataButton[3].text=onoff_table[debugKeuze];
  dataButton[4].text= "";
  dataButton[5].text=janee_table[wisselKeuze];

  touchButton escapeButton =(touchButton) {0,272,118,30,DARKGREY,BLACK,"ESC"};
  touchButton saveButton =(touchButton) {120,272,118,30,DARKGREY,BLACK,"SAVE"};
  spr.setColorDepth(8);
  spr.createSprite(spriteWidth,spriteHeight);
  long previousMillis = millis();
  while (1)
  {
    if (refreshSprite) {
      spr.fillRect(0,0,spriteWidth,spriteHeight, BLACK);
      drawInfoBoxSprite(&spr,240,271,1,"INSTELLINGEN");
      for (int i = 0; i < 6 ; i++)
      {
        drawTouchButtonSprite(&spr,&localButton[i],2,1); 
        drawTouchButtonSprite(&spr,&dataButton[i],2,1); 
      }
      drawTouchButtonSprite(&spr,&escapeButton,2,1); 
      drawTouchButtonSprite(&spr,&saveButton,2,1);
      spr.pushSprite(spriteX,spriteY);
      refreshSprite =false;
    }
    if (clockData.checkSecond) readTime();
    if ((millis() - previousMillis) > 10000){
      spr.deleteSprite();
      return buttonNone;
    } 
    M5.update();
    int coordinateY = M5.Touch.point[0].y;  
    int coordinateX = M5.Touch.point[0].x;
    int keuze =-1;  
    for (int i = 0; i < 6 ; i++)
    {
      if (checkTouchButtonSprite(&localButton[i], spriteX, spriteY,coordinateX, coordinateY) || checkTouchButtonSprite(&dataButton[i], spriteX, spriteY,coordinateX, coordinateY) ){
        previousMillis = millis();
        valueChanged[i]=true;
      }
    }
    for (int i = 0; i < 6 ; i++)
    {
      if (valueChanged[i]){
        localButton[i].text.toCharArray(buffer,localButton[i].text.length());
        switch (i){
          case 0:
            keuze =getalTouchBoxSprite (50, 50, pumpTimeKeuze, 0, 20, 1,buffer);
            if ((keuze !=buttonNone)&&(keuze!=mySprinkler.pumpTime)){
              pumpTimeKeuze=keuze;
              dataButton[0].text=String(pumpTimeKeuze);
              dataButton[0].textColor=WHITE;
              localButton[0].textColor=WHITE;
            } else {
              dataButton[0].textColor=BLACK;
              localButton[0].textColor=BLACK;
            }
            break;
          case 1:
            keuze =getalTouchBoxSprite (50, 50, pauzeTimeKeuze, 0, 20, 1,buffer);
            if ((keuze !=buttonNone)&&(keuze!=mySprinkler.pauzeTime)){
              pauzeTimeKeuze=keuze;
              dataButton[1].text=String(pauzeTimeKeuze); 
              dataButton[1].textColor=WHITE;
              localButton[1].textColor=WHITE;              
            } else {
              dataButton[1].textColor=BLACK;
              localButton[1].textColor=BLACK;
            }
            break;
          case 2:
            keuze=localMenuTouchBoxSprite (50, 50, modusKeuze,auto_table,2,buffer);  
            if ((keuze !=buttonNone)&&(keuze!=mySprinkler.modus)){
              modusKeuze = keuze;
              dataButton[2].text=auto_table[modusKeuze];
              dataButton[2].textColor=WHITE;
              localButton[2].textColor=WHITE;
            } else {
              dataButton[2].textColor=BLACK;
              localButton[2].textColor=BLACK;
            }
            break;
          case 3:
            keuze=localMenuTouchBoxSprite (50, 50, debugKeuze,onoff_table,2,buffer);  
            if ((keuze !=buttonNone)&&(keuze!=sprinkler.debugMode)){
              debugKeuze=keuze;
              dataButton[3].text=onoff_table[debugKeuze];
              dataButton[3].textColor=WHITE;
              localButton[3].textColor=WHITE;
            } else {
              dataButton[3].textColor=BLACK;
              localButton[3].textColor=BLACK;
            }
            break;
          case 4:
            break;
          case 5:
            keuze=localMenuTouchBoxSprite (50, 50, wisselKeuze,janee_table,2,buffer);
            if ((keuze !=buttonNone)&&(keuze!=myServer.sensorToggle)){
              wisselKeuze=keuze;
              dataButton[5].text=janee_table[wisselKeuze];
              dataButton[5].textColor=WHITE;
              localButton[5].textColor=WHITE; 
            }  else {
              dataButton[5].textColor=BLACK;
              localButton[5].textColor=BLACK;
            }
            break;
        }
        valueChanged[i]=false;
        refreshSprite = true;
      }
    }
    if (checkTouchButtonSprite(&saveButton, spriteX, spriteY,coordinateX, coordinateY)){
      for (int i = 0; i < 6 ; i++)
      {
        if (dataButton[i].textColor=WHITE){
          mySprinkler.pumpTime = pumpTimeKeuze;
          mySprinkler.pauzeTime = pauzeTimeKeuze;
          mySprinkler.modus = modusKeuze;
          sprinkler.debugMode = debugKeuze;
          myServer.sensorToggle = wisselKeuze;
          writeMySprinklerFile();
          writeMyServerFile();
          controleerProgramma(RTCtime.Hours, RTCtime.Minutes, RTCDate.WeekDay); 
          checkRelaisOnTime(RTCtime.Hours, RTCtime.Minutes);
          checkRelaisTempOnTime();
          Serial.println("values saved");
          break;
        }
      }
      spr.deleteSprite();
      return buttonNone;
    } 
    if (checkTouchButtonSprite(&escapeButton, spriteX, spriteY,coordinateX, coordinateY)){
      spr.deleteSprite();
      return buttonNone;
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
  boolean refreshSprite = true;
  int sprinklerKeuze = -1;
  int sprinklerKeuzePrevious = -1;
  int sprinklerTime = 0;
  int spriteX = 0;
  int spriteY = 19;
  int spriteWidth = 240;
  int spriteHeight = 320;

  touchButton sprinklerButton[8];
  touchButton sprinklerTimeButton =(touchButton) {120,9,80,30,BLACK,BLACK,"x min"};
  touchButton escapeButton =(touchButton) {0,272,118,30,DARKGREY,BLACK,"ESCAPE"};
  touchButton startButton =(touchButton) {120,272,118,30,DARKGREY,DARKGREY,"START"};
  for (int i = 0; i < 8 ; i++)
  {
    sprinklerButton[i] =(touchButton) {2,9+i*33,116,30,DARKGREY,BLACK,sprinklerName_table[i+1]};
  }
  spr.setColorDepth(8);
  spr.createSprite(spriteWidth,spriteHeight);
  long previousMillis = millis();
  while (1)
  {
    if (refreshSprite) {
      spr.fillRect(0,0,spriteWidth,spriteHeight, BLACK);
      drawInfoBoxSprite(&spr,240,271,1,"SPRINKLER SELECTIE");
      for (int i = 0; i < 8 ; i++)
      {
        drawTouchButtonSprite(&spr,&sprinklerButton[i],2,1); 
      }
      drawTouchButtonSprite(&spr,&escapeButton,2,1); 
      drawTouchButtonSprite(&spr,&startButton,2,1);
      drawTouchButtonSprite(&spr,&sprinklerTimeButton,2,1);    
      spr.pushSprite(spriteX,spriteY);
      refreshSprite =false;
    }
    if (clockData.checkSecond) readTime();
    if ((millis() - previousMillis) > 10000){
      spr.deleteSprite();
      return buttonNone;
    }
    M5.update();  
    if ( M5.Touch.changed ){ 
      int coordinateY = M5.Touch.point[0].y;
      int coordinateX = M5.Touch.point[0].x;
      int keuze =-1;
      for (int i = 0; i < 8; i++){
        if (checkTouchButtonSprite(&sprinklerButton[i], spriteX, spriteY, coordinateX, coordinateY)){
          soundsBeep(1000, 100, 1);
          previousMillis = millis();
          if (!sprinklerSelected){
            sprinklerKeuze = i;
            sprinklerKeuzePrevious = i;
            sprinklerSelected=true;
            showTimeSelect = true;
            sprinklerButton[sprinklerKeuze].textColor=WHITE;
            drawTouchButtonSprite(&spr,&sprinklerButton[sprinklerKeuze],2,1);
            sprinklerTimeButton.y = 9+(sprinklerKeuze)*33; 
            sprinklerTimeButton.textColor = BLACK;
            sprinklerTimeButton.backgroundColor = DARKGREY;    
            drawTouchButtonSprite(&spr,&sprinklerTimeButton,2,1);   
            spr.pushSprite(spriteX,spriteY);
          } else {
            soundsBeep(1000, 100, 1);
            sprinklerKeuze = i;
            if (sprinklerKeuze !=sprinklerKeuzePrevious){
              sprinklerButton[sprinklerKeuze].textColor=WHITE;
              sprinklerButton[sprinklerKeuzePrevious].textColor=BLACK;
              drawTouchButtonSprite(&spr,&sprinklerButton[sprinklerKeuze],2,1);
              drawTouchButtonSprite(&spr,&sprinklerButton[sprinklerKeuzePrevious],2,1);
              sprinklerTimeButton.y = 9+(sprinklerKeuzePrevious)*33;      
              hideTouchButtonSprite(&spr,&sprinklerTimeButton);
              sprinklerTimeButton.y = 9+(sprinklerKeuze)*33;   
              drawTouchButtonSprite(&spr,&sprinklerTimeButton,2,1);   
              spr.pushSprite(spriteX,spriteY);
              sprinklerKeuzePrevious=sprinklerKeuze;
              //showTimeSelect = true;
            } else {
              showTimeSelect = true;
            }
          }  
        }
      }
      if (showTimeSelect){
        sprinklerTime =getalTouchBoxSprite (50, 50, sprinklerTime, 0, 60, 5,sprinklerName_table[sprinklerKeuze+1]);
        if (sprinklerTime ==buttonNone){
          return buttonNone;
        } else if (sprinklerTime==0){
          previousMillis = millis();
          timeSelected = false;
          sprinklerTimeButton.y = 9+(sprinklerKeuze)*33;
          sprinklerTimeButton.text=String(sprinklerTime)+" min";
          sprinklerTimeButton.textColor= BLACK;
          startButton.textColor=DARKGREY;
        } else {
          previousMillis = millis();
          timeSelected = true;
          sprinklerTimeButton.y = 9+(sprinklerKeuze)*33;
          sprinklerTimeButton.text=String(sprinklerTime)+" min";
          sprinklerTimeButton.textColor= WHITE;
          startButton.textColor=BLACK;
        }
        showTimeSelect = false;
        refreshSprite = true;
      }
      if (sprinklerSelected && timeSelected) {
        if (checkTouchButtonSprite(&sprinklerTimeButton, spriteX, spriteY,coordinateX, coordinateY)){
          previousMillis = millis();
          showTimeSelect = true;
        }
        if (checkTouchButtonSprite(&startButton, spriteX, spriteY,coordinateX, coordinateY)){
          SprinklerProgram[0].valve[0]=sprinklerKeuze+1;
          SprinklerProgram[0].valveTime[0]=sprinklerTime;  
          laadProgramma(0);
          keuze = 0;
          spr.deleteSprite();
          return keuze;
        }
      } 
      if (checkTouchButtonSprite(&escapeButton, spriteX, spriteY,coordinateX, coordinateY)){
          spr.deleteSprite();
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
    //keuze = uurSelectie();
    keuze = timeSettingTouch();
    return keuze;
    break;
  case 1:
    //keuze = datumSelectie();
    keuze = timeSettingTouch();
    return keuze;
    break;
  case 2:
    //keuze = sprinklerSettingsSelectie();
    keuze = sprinklerSettingTouch();
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
