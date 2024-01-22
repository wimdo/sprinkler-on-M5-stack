

int localMenuTouchBoxSprite (int x, int y, int startWaarde,char *menuTable[],int rijen, char *title){    
    int buttonWidthFull = 140;
    int buttonHeight = 30;
    int spacing = 2;
    int edge =1;
    int header = 0;
    int boxX= edge+spacing+buttonWidthFull+spacing+edge; ;
    int boxY = 0;
    int waardePrevious = startWaarde;
    box.setColorDepth(8);
    if (title !=""){
        boxY = edge+spacing+buttonHeight+spacing+(buttonHeight+spacing)*rijen+buttonHeight+spacing+edge;
        box.createSprite(boxX,boxY);
        touchButton textButton =(touchButton) {edge+spacing,edge+spacing,buttonWidthFull,buttonHeight,BLACK,WHITE,title};
        box.fillRect(0, 0, boxX, boxY, BLACK);
        box.drawRect(0, 0, boxX, boxY, WHITE); 
        drawTouchButtonSprite(&box,&textButton,2,1);
        header=buttonHeight;
        y=y+header;
    } else {
        boxY = edge+spacing+(buttonHeight+spacing)*rijen+buttonHeight+spacing+edge;
        box.createSprite(boxX,boxY);
        box.fillRect(0, 0, boxX, boxY, BLACK);
        box.drawRect(0, 0, boxX, boxY, WHITE); 
    }   
    touchButton localButton[rijen];
    for (int i = 0; i < rijen ; i++)
    {
        if (i == startWaarde){
            localButton[i] =(touchButton) {edge+spacing,edge+spacing+header+(buttonHeight+spacing)*i,buttonWidthFull,buttonHeight,DARKGREY,WHITE,menuTable[i]};   
        } else {
            localButton[i] =(touchButton) {edge+spacing,edge+spacing+header+(buttonHeight+spacing)*i,buttonWidthFull,buttonHeight,DARKGREY,BLACK,menuTable[i]};
        }
    }
    touchButton escapeButton =(touchButton) {edge+spacing,header+spacing+(buttonHeight+spacing)*rijen+spacing,(buttonWidthFull-spacing)/2,buttonHeight,DARKGREY,BLACK,"ESC"};
    touchButton OKButton =(touchButton) {edge+spacing+(buttonWidthFull-spacing)/2+spacing ,header+spacing+(buttonHeight+spacing)*rijen+spacing,(buttonWidthFull-spacing)/2,buttonHeight,DARKGREY,BLACK,"OK"};
    for (int i = 0; i < rijen ; i++)
    {
        drawTouchButtonSprite(&box,&localButton[i],2,1);
    }   
    drawTouchButtonSprite(&box,&escapeButton,2,1);
    drawTouchButtonSprite(&box,&OKButton,2,1);
    box.pushSprite(x,y);
    long previousMillis = millis();
    int waarde = startWaarde;
    while (1)
    {
        if (clockData.checkSecond)
        readTime();
        if ((millis() - previousMillis) > 10000){
            box.deleteSprite();
            return buttonNone;
        }
        M5.update();  
        if ( M5.Touch.changed ){ 
            int coordinateY = M5.Touch.point[0].y;
            int coordinateX = M5.Touch.point[0].x;
            for (int i = 0; i < rijen ; i++)
            {
                if (checkTouchButtonSprite(&localButton[i], x,y,coordinateX, coordinateY)){
                    previousMillis = millis();
                    waarde=i;
                    if (waarde != waardePrevious){
                       localButton[waarde].textColor=WHITE;
                       drawTouchButtonSprite(&box,&localButton[waarde],2,1); 
                       localButton[waardePrevious].textColor=BLACK;
                       drawTouchButtonSprite(&box,&localButton[waardePrevious],2,1); 
                       waardePrevious=waarde;
                       box.pushSprite(x,y); 
                    }
                }
            }  
            if (checkTouchButtonSprite(&escapeButton, x,y,coordinateX, coordinateY)){
                box.deleteSprite();
                return startWaarde;
            }
            if (checkTouchButtonSprite(&OKButton, x,y,coordinateX, coordinateY)){
                box.deleteSprite();
                return waarde;
            }
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

int getalTouchBoxSprite (int x, int y, int startWaarde, int minimum, int maximum, int stap, char *title){

    int offsetY = 0;
    spr.setColorDepth(8);
    if (title !=""){
        box.createSprite(110,130);
        touchButton textButton =(touchButton) {3,5,106,16,BLACK,WHITE,title};
        box.fillRect(0, 0, 110, 130, BLACK);
        box.drawRect(0, 0, 110, 130, WHITE); 
        drawTouchButtonSprite(&box,&textButton,2,1);
        offsetY=20;
        y=y+offsetY;
    } else {
        box.createSprite(110,110);
        box.fillRect(0, 0, 110, 110, BLACK);
        box.drawRect(0, 0, 110, 110, WHITE);
    } 
    touchButton getalButton =(touchButton) {4,offsetY+21,50,30,DARKGREY,BLACK,String(startWaarde)};  
    touchButton upButton =(touchButton) {56,offsetY+5,50,30,DARKGREY,BLACK,"UP"};
    touchButton downButton =(touchButton) {56,offsetY+40,50,30,DARKGREY,BLACK,"DOWN"};
    touchButton escapeButton =(touchButton) {4,offsetY+75,50,30,DARKGREY,BLACK,"ESC"};
    touchButton OKButton =(touchButton) {56,offsetY+75,50,30,DARKGREY,BLACK,"OK"};
    drawTouchButtonSprite(&box,&getalButton,2,1);
    drawTouchButtonSprite(&box,&upButton,2,1); 
    drawTouchButtonSprite(&box,&downButton,2,1);
    drawTouchButtonSprite(&box,&escapeButton,2,1);
    drawTouchButtonSprite(&box,&OKButton,2,1);
    box.pushSprite(x,y);
    long previousMillis = millis();
    int waarde = startWaarde;
    while (1)
    {
        if (clockData.checkSecond)
        readTime();
        if ((millis() - previousMillis) > 10000){
            box.deleteSprite();
            return buttonNone;
        }
        M5.update();  
        if ( M5.Touch.changed ){ 
            int coordinateY = M5.Touch.point[0].y;
            int coordinateX = M5.Touch.point[0].x;
            boolean checkButton;
            if (checkButton = checkTouchButtonSprite(&upButton, x,y,coordinateX, coordinateY)){
                previousMillis = millis();
                if (waarde < maximum) waarde = waarde + stap;
                getalButton.text=String(waarde);
                                drawTouchButtonSprite(&box,&getalButton,2,1);   
                box.pushSprite(x,y); 
            }
            if (checkButton = checkTouchButtonSprite(&downButton, x,y,coordinateX, coordinateY)){
                previousMillis = millis();
                if (waarde > minimum) waarde = waarde - stap;
                getalButton.text=String(waarde);
                                drawTouchButtonSprite(&box,&getalButton,2,1);   
                box.pushSprite(x,y);
            }
            if (checkButton = checkTouchButtonSprite(&escapeButton, x,y,coordinateX, coordinateY)){
                box.deleteSprite();
                return startWaarde;
            }
            if (checkButton = checkTouchButtonSprite(&OKButton, x,y,coordinateX, coordinateY)){
                box.deleteSprite();
                return waarde;
            }
        }
    }
}

