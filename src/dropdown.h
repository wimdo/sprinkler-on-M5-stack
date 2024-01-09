

int localDropbox(int positie, int WidthPosition, int DepthPosition, char *menuTable[], int rijen)
{
    int lengte = 0;
    M5.Lcd.setTextSize(2);
    while (menuTable[0][lengte] != '\0') lengte++;
    DepthPosition = DepthPosition * 18 + 27;
    int WidthDropbox = 2 + lengte * 12 + 2;
    int DepthDropbox = 2 + rijen * 18 + 1;
    M5.Lcd.fillRect(0 + WidthPosition, DepthPosition, WidthDropbox, DepthDropbox, BLACK);
    M5.Lcd.drawRect(0 + WidthPosition, DepthPosition, WidthDropbox, DepthDropbox, DARKGREY);
    for (int i = 0; i < rijen; i++)
    {
        M5.Lcd.setCursor(2 + WidthPosition, 2 + DepthPosition + i * 18);
        M5.Lcd.print(menuTable[i]);
    }
    int positiePrevious = positie;
    while (1)
    {
        M5.Lcd.setTextSize(2);
        M5.Lcd.setCursor(2 + WidthPosition, 2 + DepthPosition + positiePrevious * 18);
        M5.Lcd.setTextColor(DARKGREY, BLACK);
        M5.Lcd.print(menuTable[positiePrevious]);
        M5.Lcd.setCursor(2 + WidthPosition, 2 + DepthPosition + positie * 18);
        M5.Lcd.setTextColor(WHITE, DARKGREY);
        M5.Lcd.print(menuTable[positie]);
        M5.Lcd.setTextColor(WHITE, BLACK);
        positiePrevious = positie; 
        int keuze = keyboardButtonBar("ESC", "DOWN", "UP","SLCT");
        switch (keuze)
        {
        case buttonNone:
            return buttonNone;
            break;
        case button1:
            return buttonNone;
            break;
        case button2:
            (positie < rijen - 1) ? positie++ : positie = 0;
            break;
        case button4:
            return positie;
            break;
        case button3:
            (positie == 0) ? positie = rijen - 1 : positie--;
            break;
        }
    }
    return 0;
    
}



int inputDropbox(boolean withSelect, int widthPositionData, char *menuTable[], int rijen)
{
    int positie = 0;   
    while (1)
    {
        M5.Lcd.setTextSize(2);
        for (int i = 0; i < rijen; i++)
        {
            M5.Lcd.setCursor(2, 27 + i * 18);
            if (positie == i)
            {
                if (withSelect)
                {
                    M5.Lcd.setTextColor(WHITE, DARKGREY);
                    M5.Lcd.print(menuTable[i]);
                }
                else
                {
                    M5.Lcd.print(menuTable[i]);
                }
            }
            else
            {
                M5.Lcd.setTextColor(DARKGREY, BLACK);
                M5.Lcd.print(menuTable[i]);
            }
            M5.Lcd.setCursor(widthPositionData, 27 + i * 18);
            M5.Lcd.print(&data_table[i][0]);
        }

        if (!withSelect)
        {
            return positie;
            break;
        }
        int keuze = keyboardButtonBar("ESC", "DOWN", "UP","SLCT");
        switch (keuze)
        {
        case buttonNone:
            return buttonNone;
            break;
        case button1:
            return buttonNone;
            break;
        case button2:
            (positie < rijen - 1) ? positie++ : positie = 0;
            break;
        case button4:
            return positie;
            break;
        case button3:
            (positie == 0) ? positie = rijen - 1 : positie--;
            break;
        }
    }
    return 0;
    
}

int menuTouchbox(char *menuTable[], int rijen){
  touchButton menuButton[8];  
  for (int i = 0; i < rijen ; i++)
    {    
        menuButton[i] =(touchButton) {2,28+i*33,236,30,DARKGREY,BLACK,menuTable[i]};
        drawTouchButton(&menuButton[i],2,1); 
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
      int keuze =-1;
      for (int i = 0; i < 8; i++){
        if (boolean check = checkTouchButton(&menuButton[i], M5.Touch.point[0].x, M5.Touch.point[0].y)){
          soundsBeep(1000, 100, 1);
          return i;
        }
      }
    } 
    if (b1.wasPressed()||b2.wasPressed() || b3.wasPressed() || b4.wasPressed()   ) {
      soundsBeep(1000, 100, 1);
      return buttonNone;
    }
  }
}

int stringDropBox(int WidthPosition, int DepthPosition, char changeArray[])
{
    char keyboard[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ- _0123456789";
    char localChangeArray[20];
    int lengteArray = 0;

    while (changeArray[lengteArray] != '\0')
    {
        localChangeArray[lengteArray] = changeArray[lengteArray];
        lengteArray++;
    }
    localChangeArray[lengteArray] = '\0';
    int keyboardLocation = 0;
    while (localChangeArray[0] != keyboard[keyboardLocation])
        keyboardLocation++;
    
    DepthPosition = 25 + DepthPosition * 18;
    
    int WidthDropbox = 2 + 12 + 12 + lengteArray * 12 + 2;
    int DepthDropbox = 2 + 16 + 2;
    int localChangeArrayPosition = 0;
    while (1)
    {
        M5.Lcd.setTextSize(2);
        M5.Lcd.fillRect(0 + WidthPosition, DepthPosition, WidthDropbox, DepthDropbox, BLACK);
        M5.Lcd.drawRect(0 + WidthPosition, DepthPosition, WidthDropbox, DepthDropbox, WHITE);
        M5.Lcd.setCursor(2 + WidthPosition, 2 + DepthPosition);
        M5.Lcd.print(localChangeArray);
        M5.Lcd.setTextColor(BLACK);
        M5.Lcd.fillRect(1 + WidthPosition + 12 * localChangeArrayPosition, 1 + DepthPosition, 14, 18, WHITE);
        M5.Lcd.setCursor(2 + WidthPosition + 12 * localChangeArrayPosition, 2 + DepthPosition);
        //M5.Lcd.fillRect(1 + WidthPosition + 6 * localChangeArrayPosition, 1 + DepthPosition, 7, 9, WHITE);
        //M5.Lcd.setCursor(2 + WidthPosition + 6 * localChangeArrayPosition, 2 + DepthPosition);
    

        M5.Lcd.print(localChangeArray[localChangeArrayPosition]);
        M5.Lcd.setTextColor(WHITE);

        int keuze = keyboardButtonBar("ESC", ">", "CHNG","OK");
        switch (keuze)
        {
        case buttonNone:
            return buttonNone;
            break;
        case button1:
            return buttonNone;
            break;
        case button2:
            (keyboardLocation < sizeof(keyboard) - 2) ? keyboardLocation++ : keyboardLocation = 0;
            localChangeArray[localChangeArrayPosition] = keyboard[keyboardLocation];
            break;
        case button3:
            (localChangeArrayPosition < lengteArray) ? localChangeArrayPosition++ : localChangeArrayPosition = 0;
            keyboardLocation = 0;
            while (localChangeArray[localChangeArrayPosition] != keyboard[keyboardLocation])
                keyboardLocation++;
            break;
        case button4:
            for (int i = 0; i <= lengteArray; i++)
            {
                changeArray[i] = localChangeArray[i];
                Serial.println(changeArray);
            }
            return 1;
            break;
        }
    }
   return 0;
}

int getalTouchBox (int x, int y, int startWaarde, int minimum, int maximum, int stap, char *title){
        
    if (title !=""){
        touchButton textButton =(touchButton) {x+3,y+5,106,16,BLACK,WHITE,title};
        M5.Lcd.fillRoundRect(x, y, 110, 130,8, BLACK);
        M5.Lcd.drawRoundRect(x, y, 110, 130,8, WHITE); 
        drawTouchButton(&textButton,2,1);
        y=y+20;   
    } else {
        M5.Lcd.fillRoundRect(x, y, 110, 110,8, BLACK);
        M5.Lcd.drawRoundRect(x, y, 110, 110,8, WHITE);
    }
    touchButton getalButton =(touchButton) {x+4,y+21,50,30,DARKGREY,BLACK,String(startWaarde)};
    touchButton upButton =(touchButton) {x+56,y+5,50,30,DARKGREY,BLACK,"UP"};
    touchButton downButton =(touchButton) {x+56,y+40,50,30,DARKGREY,BLACK,"DOWN"};
    touchButton escapeButton =(touchButton) {x+4,y+75,50,30,DARKGREY,BLACK,"ESC"};
    touchButton OKButton =(touchButton) {x+56,y+75,50,30,DARKGREY,BLACK,"OK"};
    drawTouchButton(&getalButton,2,1);
    drawTouchButton(&upButton,2,1); 
    drawTouchButton(&downButton,2,1);
    drawTouchButton(&escapeButton,2,1);
    drawTouchButton(&OKButton,2,1);
    long previousMillis = millis();
    int waarde = startWaarde;
    while (1)
    {
        if (clockData.checkSecond)
        readTime();
        if ((millis() - previousMillis) > 10000)
        return buttonNone;
        M5.update();  
        if ( M5.Touch.changed ){ 
            int coordinateY = M5.Touch.point[0].y;
            int coordinateX = M5.Touch.point[0].x;
            boolean checkButton;
            if (checkButton = checkTouchButton(&upButton, coordinateX, coordinateY)){
                soundsBeep(1000, 100, 1);
                previousMillis = millis();
                if (waarde < maximum) waarde = waarde + stap;
                getalButton.text=String(waarde);
                drawTouchButton(&getalButton,2,1);    
            }
            if (checkButton = checkTouchButton(&downButton, coordinateX, coordinateY)){
                soundsBeep(1000, 100, 1);
                previousMillis = millis();
                if (waarde > minimum) waarde = waarde - stap;
                getalButton.text=String(waarde);
                drawTouchButton(&getalButton,2,1);   
            }
            if (checkButton = checkTouchButton(&escapeButton, coordinateX, coordinateY)){
                soundsBeep(1000, 100, 1); 
                previousMillis = millis();
                return startWaarde;
            }
            if (checkButton = checkTouchButton(&OKButton, coordinateX, coordinateY)){
                soundsBeep(1000, 100, 1); 
                return waarde;
            }
        }
    }
}


int getalDropDown(int WidthPosition, int DepthPosition, int startWaarde, int minimum, int maximum, int stap)

{
    int lengte = 2;
    int waarde = startWaarde;
    if (waarde > 999)
    {
        lengte = 4;
    }
    else if (waarde > 99)
    {
        lengte = 3;
    }
    DepthPosition = 25 + DepthPosition * 18;
    int WidthDropbox = 2 + 12 + 12 + lengte * 12 + 2;
    int DepthDropbox = 2 + 16 + 2;    
    while (1)
    {
        M5.Lcd.setTextSize(2);
        M5.Lcd.setTextColor(WHITE, DARKGREY);
        M5.Lcd.fillRect(0 + WidthPosition, DepthPosition, WidthDropbox, DepthDropbox, DARKGREY);
        M5.Lcd.setCursor(2 + WidthPosition, 2 + DepthPosition);
        M5.Lcd.print(waarde);
        int keuze = keyboardButtonBar("ESC", "+", "-","OK");
        switch (keuze)
        {
        case buttonNone:
            return buttonNone;
            break;
        case button1:
            return startWaarde;
            break;
        case button2:
            if (waarde > minimum) waarde = waarde - stap;

            break;
        case button3:
            if (waarde < maximum)waarde = waarde + stap;
            break;
        case button4:
            return waarde;
            break;
        }
    }
   return 0;
}
