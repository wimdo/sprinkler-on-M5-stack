
void valveSettingsChange(int spriteX,int spriteY, int spriteWidth, int spriteHeight,int localValve){
  char *valveSettings_table[] = {"Naam", "werking", "Met Pomp"};
  int localPercentageTime = mySprinkler.valve[localValve].percentage;
  int localWithPump = mySprinkler.valve[localValve].withPump;
  char buffer[15];
  boolean refreshSprite = true;
  boolean valueChanged[3] ={false,false,false};

  touchButton localButton[3];
  touchButton dataButton[3];
  touchButton escapeButton =(touchButton) {0,272,118,30,DARKGREY,BLACK,"ESC"};
  touchButton saveButton =(touchButton) {120,272,118,30,DARKGREY,BLACK,"SAVE"};
  localButton[0] =(touchButton) {2,9+0*33,116,30,DARKGREY,BLACK,"Naam"};
  localButton[1] =(touchButton) {2,9+1*33,116,30,DARKGREY,BLACK,"Met pomp"};
  localButton[2] =(touchButton) {2,9+2*33,116,30,DARKGREY,BLACK,"perc aan"};
  dataButton[0] =(touchButton) {120,9+0*33,116,30,DARKGREY,BLACK,mySprinkler.valve[localValve+1].valveName};
  dataButton[1] =(touchButton) {120,9+1*33,116,30,DARKGREY,BLACK,janee_table[localWithPump]};
  dataButton[2] =(touchButton) {120,9+2*33,116,30,DARKGREY,BLACK,String(localPercentageTime)};

  spr.deleteSprite();
  spr.setColorDepth(8);
  spr.createSprite(spriteWidth,spriteHeight);
  refreshSprite = true;
  long previousMillis = millis();
  while (1)
  {
    if (refreshSprite) {
      spr.fillRect(0,0,spriteWidth,spriteHeight, BLACK);
      drawInfoBoxSprite(&spr,240,271,1,"CHANGE VALVE SETTINGS");
      for (int i = 0; i < 3 ; i++)
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
      return;
    } 
    M5.update();
    int coordinateY = M5.Touch.point[0].y;  
    int coordinateX = M5.Touch.point[0].x;
    int keuze =-1;  
    for (int i = 0; i < 3 ; i++)
    {
      if (checkTouchButtonSprite(&localButton[i], spriteX, spriteY,coordinateX, coordinateY) || checkTouchButtonSprite(&dataButton[i], spriteX, spriteY,coordinateX, coordinateY) ){
        previousMillis = millis();
        valueChanged[i]=true;
      }
    }
    for (int i = 0; i < 3 ; i++)
    {
      if (valueChanged[i]){
        localButton[i].text.toCharArray(buffer,localButton[i].text.length());
        switch (i){
          case 0:
            // naam wijzigen 
            break;
          case 1:
            keuze=localMenuTouchBoxSprite (50, 50, localWithPump,janee_table,2,buffer);
            if ((keuze !=buttonNone)&&(keuze!=mySprinkler.valve[localValve].withPump)){
             localWithPump=keuze;
              dataButton[1].text=janee_table[localWithPump];
              dataButton[1].textColor=WHITE;
              localButton[1].textColor=WHITE;              
            } else {
              dataButton[1].textColor=BLACK;
              localButton[1].textColor=BLACK;
            }
            break;
          case 2:
            keuze =getalTouchBoxSprite (50, 50, localPercentageTime, 0, 100, 10,buffer);
            if ((keuze !=buttonNone)&&(keuze!=mySprinkler.valve[localValve].percentage)){
              localPercentageTime =keuze;
              dataButton[1].text=String(localPercentageTime);
              dataButton[1].textColor=WHITE;
              localButton[1].textColor=WHITE;              
            } else {
              dataButton[1].textColor=BLACK;
              localButton[1].textColor=BLACK;
            }
            break;
        }
        valueChanged[i]=false;
        refreshSprite = true;
      }
    }
    if (checkTouchButtonSprite(&saveButton, spriteX, spriteY,coordinateX, coordinateY)){
      for (int i = 0; i < 3 ; i++)
      {
        if (dataButton[i].textColor=WHITE){
          Serial.println("values saved");
          break;
        }
      }
      spr.deleteSprite();
      return ;
    } 
    if (checkTouchButtonSprite(&escapeButton, spriteX, spriteY,coordinateX, coordinateY)){
      spr.deleteSprite();
      return ;
    }


  }
}

int valveSettings(int spriteX,int spriteY, int spriteWidth, int spriteHeight)
{
  boolean refreshSprite = true;
  int menuItems = 8;
  touchButton localButton[menuItems];
  touchButton escapeButton =(touchButton) {0,272,240,30,DARKGREY,BLACK,"ESCAPE"};
  for (int i = 0; i < menuItems ; i++)
  {
    localButton[i] =(touchButton) {2,9+i*33,236,30,DARKGREY,BLACK,mySprinkler.valve[i+1].valveName};
  }
  spr.deleteSprite();
  spr.setColorDepth(8);
  spr.createSprite(spriteWidth,spriteHeight);

  long previousMillis = millis();
  while (1)
  {
    if (refreshSprite) {
      spr.fillRect(0,0,spriteWidth,spriteHeight, BLACK);
      drawInfoBoxSprite(&spr,240,271,1,"VALVE SETTINGS");
      for (int i = 0; i < menuItems ; i++)
      {
        drawTouchButtonSprite(&spr,&localButton[i],2,1); 
      }
      drawTouchButtonSprite(&spr,&escapeButton,2,1); 
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
        if (checkTouchButtonSprite(&localButton[i], spriteX, spriteY, coordinateX, coordinateY)){
          previousMillis = millis();
          valveSettingsChange(0,19,240,320,i);
          refreshSprite= true;
        }
      }
      if (checkTouchButtonSprite(&escapeButton, spriteX, spriteY,coordinateX, coordinateY)){
        spr.deleteSprite();
        return buttonNone;
      }
    } 
  }
}



void relaisSettingsChange(int spriteX,int spriteY, int spriteWidth, int spriteHeight,int localRelais){
  char *localButtonTable[] = {"Naam","Actief", "Programma","data1","data2","data3","data4"};
  // {"none","time", "sunrise", "sunset","day","night", "temp on"};
  // naam, stateAtStart, state, actief, control, data1, data2, data3, data 4
  int menuItems = 7;
  int localActief=relais[localRelais].actief;
  int localControl=relais[localRelais].control;
  int localData1=relais[localRelais].data1;
  int localData2=relais[localRelais].data2;
  int localData3=relais[localRelais].data3;
  int localData4=relais[localRelais].data4;
  char buffer[15];

  boolean refreshSprite = true;
  boolean valueChanged[7] ={false,false,false,false, false, false,false};
  touchButton localButton[7];
  touchButton dataButton[7];
  touchButton escapeButton =(touchButton) {0,272,118,30,DARKGREY,BLACK,"ESC"};
  touchButton saveButton =(touchButton) {120,272,118,30,DARKGREY,BLACK,"SAVE"};
  for (int i = 0; i < menuItems ; i++){
    localButton[i] =(touchButton) {2,9+i*33,116,30,DARKGREY,BLACK,localButtonTable[i]};
  }
  dataButton[0] =(touchButton) {120,9+0*33,116,30,DARKGREY,BLACK,relais[localRelais].relaisName};
  dataButton[1] =(touchButton) {120,9+1*33,116,30,DARKGREY,BLACK,janee_table[localActief]};
  dataButton[2] =(touchButton) {120,9+2*33,116,30,DARKGREY,BLACK,program_Table[localControl]};
  dataButton[3] =(touchButton) {120,9+3*33,116,30,DARKGREY,BLACK,String(localData1)};
  dataButton[4] =(touchButton) {120,9+4*33,116,30,DARKGREY,BLACK,String(localData2)};
  dataButton[5] =(touchButton) {120,9+5*33,116,30,DARKGREY,BLACK,String(localData3)};
  dataButton[6] =(touchButton) {120,9+6*33,116,30,DARKGREY,BLACK,String(localData4)};
  spr.deleteSprite();
  spr.setColorDepth(8);
  spr.createSprite(spriteWidth,spriteHeight);
  refreshSprite = true;
  long previousMillis = millis();
  while (1)
  {
    if (refreshSprite) {
      spr.fillRect(0,0,spriteWidth,spriteHeight, BLACK);
      drawInfoBoxSprite(&spr,240,271,1,"CHANGE RELAIS SETTINGS");
      switch (localControl){
        // programTable[] = {"none","time", "sunrise", "sunset","day","night", "temp on"};
        case 0:
          menuItems = 3;
          break; 
        case 1:
          menuItems = 7;
          localButton[3].text ="Uur start";
          localButton[4].text ="Min start";
          localButton[5].text ="Uur stop";
          localButton[6].text ="Min stop";
          break;
        case 2:
          menuItems = 5;
          localButton[3].text ="Uur stop";
          localButton[4].text ="Min stop";
          break;
        case 3:
          menuItems = 5;
          localButton[3].text ="Uur stop";
          localButton[4].text ="Min stop";
          break;          
        case 4:
          menuItems = 3;
          break;
        case 5:
          menuItems = 3;
          break;  
        case 6:
          menuItems = 4;
          localButton[3].text ="temp";
          break; 
      }
      for (int i = 0; i < menuItems ; i++)
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
      return;
    } 
    M5.update();
    int coordinateY = M5.Touch.point[0].y;  
    int coordinateX = M5.Touch.point[0].x;
    int keuze =-1;  
    for (int i = 0; i < menuItems ; i++)
    {
      if (checkTouchButtonSprite(&localButton[i], spriteX, spriteY,coordinateX, coordinateY) || checkTouchButtonSprite(&dataButton[i], spriteX, spriteY,coordinateX, coordinateY) ){
        previousMillis = millis();
        valueChanged[i]=true;
      }
    }
    for (int i = 0; i < menuItems ; i++)
    {
      if (valueChanged[i]){
        localButton[i].text.toCharArray(buffer,localButton[i].text.length());
        switch (i){
          case 0:
            // naam wijzigen 
            break;
          case 1: // actief localActief
            keuze=localMenuTouchBoxSprite (50, 50, localActief,onoff_table,2,buffer);  
            if ((keuze !=buttonNone)&&(keuze!=localActief)){
              localActief=keuze;
              dataButton[i].text=onoff_table[localActief];
              dataButton[i].textColor=WHITE;
              localButton[i].textColor=WHITE;
            } else {
              dataButton[i].textColor=BLACK;
              localButton[i].textColor=BLACK;
            }
            break;
          case 2: // control
            keuze=localMenuTouchBoxSprite (0, 0, localControl,program_Table,7,buffer);  
            if ((keuze !=buttonNone)&&(keuze!=localControl)){
              localControl=keuze;
              dataButton[i].text=program_Table[localControl];
              dataButton[i].textColor=WHITE;
              localButton[i].textColor=WHITE;
            } else {
              dataButton[i].textColor=BLACK;
              localButton[i].textColor=BLACK;
            }
            break;
          case 3: {// bij temperatuur is er een verschil in de min max waarden
            int minVal = 0;
            int maxVal = 23;
            if (localControl ==6){
              minVal = 10;
              maxVal =30;
            }
            keuze =getalTouchBoxSprite (50, 50, localData1, minVal, maxVal, 1,buffer);
            if ((keuze !=buttonNone)&&(keuze!=localData1)){
              localData1 = keuze;
              dataButton[i].text=String(localData1);
              dataButton[i].textColor=WHITE;
              localButton[i].textColor=WHITE;
            } else {
              dataButton[i].textColor=BLACK;
              localButton[i].textColor=BLACK;
            }
            break;
          }  
          case 4:
            keuze =getalTouchBoxSprite (50, 50, localData2, 0, 59, 1,buffer);
            if ((keuze !=buttonNone)&&(keuze!=localData2)){
              localData2 = keuze;
              dataButton[i].text=String(localData2);
              dataButton[i].textColor=WHITE;
              localButton[i].textColor=WHITE;
            } else {
              dataButton[i].textColor=BLACK;
              localButton[i].textColor=BLACK;
            }
            break;
          case 5:
            keuze =getalTouchBoxSprite (50, 50, localData3, 0, 23, 1,buffer);
            if ((keuze !=buttonNone)&&(keuze!=localData3)){
              localData3 = keuze;
              dataButton[i].text=String(localData3);
              dataButton[i].textColor=WHITE;
              localButton[i].textColor=WHITE;
            } else {
              dataButton[i].textColor=BLACK;
              localButton[i].textColor=BLACK;
            }
            break;
          case 6:
            keuze =getalTouchBoxSprite (50, 50, localData4, 0, 59, 1,buffer);
            if ((keuze !=buttonNone)&&(keuze!=localData4)){
              localData4 = keuze;
              dataButton[i].text=String(localData4);
              dataButton[i].textColor=WHITE;
              localButton[i].textColor=WHITE;
            } else {
              dataButton[i].textColor=BLACK;
              localButton[i].textColor=BLACK;
            }
            break;
        }
        valueChanged[i]=false;
        refreshSprite = true;
      }
    }
    if (checkTouchButtonSprite(&saveButton, spriteX, spriteY,coordinateX, coordinateY)){
      for (int i = 0; i < menuItems ; i++)
      {
        if (dataButton[i].textColor=WHITE){
            relais[localRelais].actief=localActief;
            relais[localRelais].control=localControl;
            relais[localRelais].data1=localData1;
            relais[localRelais].data2=localData2;
            relais[localRelais].data3=localData3;
            relais[localRelais].data4=localData4;
            writeRelaisSpecFile();
            startRelaisProgram();
            break;
        }
      }
      spr.deleteSprite();
      return ;
    } 
    if (checkTouchButtonSprite(&escapeButton, spriteX, spriteY,coordinateX, coordinateY)){
      spr.deleteSprite();
      return ;
    }
  }
}



int relaisSettings(int spriteX,int spriteY, int spriteWidth, int spriteHeight)
{
  boolean refreshSprite = true;
  int menuItems = 7;
  touchButton localButton[menuItems];
  touchButton escapeButton =(touchButton) {0,272,240,30,DARKGREY,BLACK,"ESCAPE"};
  for (int i = 0; i < menuItems ; i++)
  {
    localButton[i] =(touchButton) {2,9+i*33,236,30,DARKGREY,BLACK,relais[i].relaisName};
  }
  spr.setColorDepth(8);
  spr.createSprite(spriteWidth,spriteHeight);

  long previousMillis = millis();
  while (1)
  {
    if (refreshSprite) {
      spr.fillRect(0,0,spriteWidth,spriteHeight, BLACK);
      drawInfoBoxSprite(&spr,240,271,1,"RELAIS SETTINGS");
      for (int i = 0; i < menuItems ; i++)
      {
        drawTouchButtonSprite(&spr,&localButton[i],2,1); 
      }
      drawTouchButtonSprite(&spr,&escapeButton,2,1); 
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
      for (int i = 0; i < menuItems; i++){
        if (checkTouchButtonSprite(&localButton[i], spriteX, spriteY, coordinateX, coordinateY)){
          previousMillis = millis();
          relaisSettingsChange(0,19,240,320,i);
        }
      }
      if (checkTouchButtonSprite(&escapeButton, spriteX, spriteY,coordinateX, coordinateY)){
        spr.deleteSprite();
        return buttonNone;
      }
    } 
  }
}


int timeSetting(int spriteX,int spriteY, int spriteWidth, int spriteHeight){
  char *tijdManueel_table[] = {"Uur", "Min", "Sec","Dag", "Maand", "Jaar"};
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
          RTCtime.Hours   = localHour;
          RTCtime.Minutes = localMin;
          RTCtime.Seconds = localSec;
          M5.Rtc.SetTime(&RTCtime);
          RTCDate.Month = localMonth;
          RTCDate.Date = localDay;
          RTCDate.Year = localYear;
          M5.Rtc.SetDate(&RTCDate);
          startRelaisProgram();
          controleerProgramma(RTCtime.Hours, RTCtime.Minutes, RTCDate.WeekDay);
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




void sprinklerSelectie(int spriteX,int spriteY, int spriteWidth, int spriteHeight){
  
  boolean sprinklerSelected = false;
  boolean timeSelected = false;  // de 8 knoppen laten zien
  boolean showTimeSelect = false;
  boolean refreshSprite = true;
  int sprinklerKeuze = -1;
  int sprinklerKeuzePrevious = -1;
  int sprinklerTime = 0;
  touchButton localButton[8];
  touchButton sprinklerTimeButton =(touchButton) {120,9,80,30,BLACK,BLACK,"x min"};
  touchButton escapeButton =(touchButton) {0,272,118,30,DARKGREY,BLACK,"ESCAPE"};
  touchButton startButton =(touchButton) {120,272,118,30,DARKGREY,DARKGREY,"START"};
  for (int i = 0; i < 8 ; i++)
  {
    localButton[i] =(touchButton) {2,9+i*33,116,30,DARKGREY,BLACK,mySprinkler.valve[i+1].valveName};
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
        drawTouchButtonSprite(&spr,&localButton[i],2,1); 
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
      return;
    }
    M5.update();  
    if ( M5.Touch.changed ){ 
      int coordinateY = M5.Touch.point[0].y;
      int coordinateX = M5.Touch.point[0].x;
      int keuze =-1;
      for (int i = 0; i < 8; i++){
        if (checkTouchButtonSprite(&localButton[i], spriteX, spriteY, coordinateX, coordinateY)){
          soundsBeep(1000, 100, 1);
          previousMillis = millis();
          if (!sprinklerSelected){
            sprinklerKeuze = i;
            sprinklerKeuzePrevious = i;
            sprinklerSelected=true;
            showTimeSelect = true;
            localButton[sprinklerKeuze].textColor=WHITE;
            drawTouchButtonSprite(&spr,&localButton[sprinklerKeuze],2,1);
            sprinklerTimeButton.y = 9+(sprinklerKeuze)*33; 
            sprinklerTimeButton.textColor = BLACK;
            sprinklerTimeButton.backgroundColor = DARKGREY;    
            drawTouchButtonSprite(&spr,&sprinklerTimeButton,2,1);   
            spr.pushSprite(spriteX,spriteY);
          } else {
            soundsBeep(1000, 100, 1);
            sprinklerKeuze = i;
            if (sprinklerKeuze !=sprinklerKeuzePrevious){
              localButton[sprinklerKeuze].textColor=WHITE;
              localButton[sprinklerKeuzePrevious].textColor=BLACK;
              drawTouchButtonSprite(&spr,&localButton[sprinklerKeuze],2,1);
              drawTouchButtonSprite(&spr,&localButton[sprinklerKeuzePrevious],2,1);
              sprinklerTimeButton.y = 9+(sprinklerKeuzePrevious)*33;      
              hideTouchButtonSprite(&spr,&sprinklerTimeButton);
              sprinklerTimeButton.y = 9+(sprinklerKeuze)*33;   
              drawTouchButtonSprite(&spr,&sprinklerTimeButton,2,1);   
              spr.pushSprite(spriteX,spriteY);
              sprinklerKeuzePrevious=sprinklerKeuze;
            } else {
              showTimeSelect = true;
            }
          }  
        }
      }
      if (showTimeSelect){
        sprinklerTime =getalTouchBoxSprite (50, 50, sprinklerTime, 0, 60, 5,mySprinkler.valve[sprinklerKeuze+1].valveName);
        if (sprinklerTime ==buttonNone){
          return;
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
          return;
        }
      } 
      if (checkTouchButtonSprite(&escapeButton, spriteX, spriteY,coordinateX, coordinateY)){
          spr.deleteSprite();
          return;
      }
    } 
  }
}

int programmaSelectie(int spriteX,int spriteY, int spriteWidth, int spriteHeight)
{
  /*
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
  */
}

int wifiOptions(int spriteX,int spriteY, int spriteWidth, int spriteHeight){
  Serial.println("Wifi opties nog uitwerken");
  return 0;
}


int resetOptions(int spriteX,int spriteY, int spriteWidth, int spriteHeight){
  Serial.println("reeset opties nog uitwerken");
  return 0;
}

void relaisSelectie(int spriteX,int spriteY, int spriteWidth, int spriteHeight)
{
  boolean refreshSprite = true;
  int menuItems = 7;
  touchButton localButton[menuItems];
  touchButton escapeButton =(touchButton) {0,272,240,30,DARKGREY,BLACK,"ESCAPE"};
  for (int i = 0; i < menuItems ; i++)
  {
    localButton[i] =(touchButton) {2,9+i*33,236,30,DARKGREY,BLACK,relais[i].relaisName};
  }
  spr.setColorDepth(8);
  spr.createSprite(spriteWidth,spriteHeight);

  long previousMillis = millis();
  while (1)
  {
    if (refreshSprite) {
      spr.fillRect(0,0,spriteWidth,spriteHeight, BLACK);
      drawInfoBoxSprite(&spr,240,271,1,"RELAIS SELECTIE");
      for (int i = 0; i < menuItems ; i++)
      {
        drawTouchButtonSprite(&spr,&localButton[i],2,1); 
      }
      drawTouchButtonSprite(&spr,&escapeButton,2,1); 
      spr.pushSprite(spriteX,spriteY);
      refreshSprite =false;
    }
    if (clockData.checkSecond) readTime();
    if ((millis() - previousMillis) > 10000){
      spr.deleteSprite();
      return;
    }
    M5.update();  
    if ( M5.Touch.changed ){ 
      int coordinateY = M5.Touch.point[0].y;
      int coordinateX = M5.Touch.point[0].x;
      int keuze =-1;
      for (int i = 0; i < 8; i++){
        if (checkTouchButtonSprite(&localButton[i], spriteX, spriteY, coordinateX, coordinateY)){
          previousMillis = millis();
          // naar relais specifiek programma gaan
        }
      }
      if (checkTouchButtonSprite(&escapeButton, spriteX, spriteY,coordinateX, coordinateY)){
        spr.deleteSprite();
        return;
      }
    } 
  }
}

void settingsMenu(int spriteX,int spriteY, int spriteWidth, int spriteHeight){
  char *settingsMenu_table[] = {"Pomp tijd", "Wacht tijd", "Modus", "Debug", "Wissel T","Tijd","Valve info","Relais info"};
  int pumpTimeKeuze = mySprinkler.pumpTime;
  int pauzeTimeKeuze = mySprinkler.pauzeTime;
  int modusKeuze = mySprinkler.modus;
  int debugKeuze = sprinkler.debugMode;
  int valveKeuze = 0;
  int wisselKeuze = myServer.sensorToggle;
  char buffer[15];
  boolean refreshSprite = true;
  boolean valueChanged[8] ={false,false,false,false,false,false};

  touchButton localButton[8];
  touchButton dataButton[8];
  for (int i = 0; i < 8 ; i++)
  {
    localButton[i] =(touchButton) {2,9+i*33,146,30,DARKGREY,BLACK,settingsMenu_table[i]};
    dataButton[i]= (touchButton) {150,9+i*33,90,30,DARKGREY,BLACK,""};
  }
  dataButton[0].text=String(pumpTimeKeuze);
  dataButton[1].text=String(pauzeTimeKeuze);
  dataButton[2].text=auto_table[modusKeuze];
  dataButton[3].text=onoff_table[debugKeuze];
  dataButton[4].text=janee_table[wisselKeuze];
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
      for (int i = 0; i < 8 ; i++)
      {
        drawTouchButtonSprite(&spr,&localButton[i],2,1); 
        if (i<5) drawTouchButtonSprite(&spr,&dataButton[i],2,1); 
      }
      drawTouchButtonSprite(&spr,&escapeButton,2,1); 
      drawTouchButtonSprite(&spr,&saveButton,2,1);
      spr.pushSprite(spriteX,spriteY);
      refreshSprite =false;
    }
    if (clockData.checkSecond) readTime();
    if ((millis() - previousMillis) > 10000){
      spr.deleteSprite();
      return;
    } 
    M5.update();
    int coordinateY = M5.Touch.point[0].y;  
    int coordinateX = M5.Touch.point[0].x;
    int keuze =-1;  
    for (int i = 0; i < 8 ; i++)
    {
      if (checkTouchButtonSprite(&localButton[i], spriteX, spriteY,coordinateX, coordinateY) || checkTouchButtonSprite(&dataButton[i], spriteX, spriteY,coordinateX, coordinateY) ){
        previousMillis = millis();
        valueChanged[i]=true;
      }
    }
    for (int i = 0; i < 8 ; i++)
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
            keuze=localMenuTouchBoxSprite (50, 50, wisselKeuze,janee_table,2,buffer);
            if ((keuze !=buttonNone)&&(keuze!=myServer.sensorToggle)){
              wisselKeuze=keuze;
              dataButton[4].text=janee_table[wisselKeuze];
              dataButton[4].textColor=WHITE;
              localButton[4].textColor=WHITE; 
            }  else {
              dataButton[4].textColor=BLACK;
              localButton[4].textColor=BLACK;
            }
            break;
          case 5: // tijd
            keuze= timeSetting(0,19,240,320);
            return ;
            break;
          case 6: // valve
            keuze= valveSettings(0,19,240,320);
            return ;
            break;
          case 7:  // relais
            keuze= relaisSettings(0,19,240,320);
            return ;
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
          break;
        }
      }
      spr.deleteSprite();
      return ;
    } 
    if (checkTouchButtonSprite(&escapeButton, spriteX, spriteY,coordinateX, coordinateY)){
      spr.deleteSprite();
      return ;
    }
  }
}


void mainMenu(int spriteX,int spriteY, int spriteWidth, int spriteHeight){
  char *mainMenu_table[] = {"programma kiezen", "sprinkler kiezen", "relais kiezen", "programma wijzigen","WIFI opties","Reset options"};
  int menuItems = 6;
  boolean refreshSprite = true;
  touchButton localButton[menuItems];
  touchButton escapeButton =(touchButton) {0,272,240,30,DARKGREY,BLACK,"ESCAPE"};
  for (int i = 0; i < menuItems ; i++)
  {
    localButton[i] =(touchButton) {2,9+i*33,236,30,DARKGREY,BLACK,mainMenu_table[i]};
  }
  spr.setColorDepth(8);
  spr.createSprite(spriteWidth,spriteHeight);

  long previousMillis = millis();
  while (1)
  {
    if (refreshSprite) {
      spr.fillRect(0,0,spriteWidth,spriteHeight, BLACK);
      drawInfoBoxSprite(&spr,240,271,1,"HOOFMENU");
      for (int i = 0; i < menuItems ; i++)
      {
        drawTouchButtonSprite(&spr,&localButton[i],2,1); 
      }
      drawTouchButtonSprite(&spr,&escapeButton,2,1); 
      spr.pushSprite(spriteX,spriteY);
      refreshSprite =false;
    }
    if (clockData.checkSecond) readTime();
    if ((millis() - previousMillis) > 10000){
      spr.deleteSprite();
      return;
    }
    M5.update();  
    if ( M5.Touch.changed ){ 
      int coordinateY = M5.Touch.point[0].y;
      int coordinateX = M5.Touch.point[0].x;
      int keuze =-1;
      for (int i = 0; i < menuItems; i++){
        if (checkTouchButtonSprite(&localButton[i], spriteX, spriteY, coordinateX, coordinateY)){
          previousMillis = millis();
          switch (i)
          {
          case 0:
            keuze = programmaSelectie(0,19,240,320);
            spr.deleteSprite();
            return;
            break;
          case 1:
            sprinklerSelectie(0,19,240,320);
            spr.deleteSprite();
            return;
            break;
          case 2:
            relaisSelectie(0,19,240,320);
            spr.deleteSprite();
            return;
            break;
          case 3: // programma wijzigine;
            keuze = programmaSelectie(0,19,240,320);
            spr.deleteSprite();
            return;
            break;
          case 4:
            keuze = wifiOptions(0,19,240,320);
            spr.deleteSprite();
            return;
            break;
          case 5:  
            keuze = resetOptions(0,19,240,320);
            spr.deleteSprite();
            return;
            break;              
          }
        }
      }
      if (checkTouchButtonSprite(&escapeButton, spriteX, spriteY,coordinateX, coordinateY)){
        spr.deleteSprite();
        return;
      }
    } 
  }
}

