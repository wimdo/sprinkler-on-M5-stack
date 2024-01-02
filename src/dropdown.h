

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
        case buttonEscape:
            return buttonNone;
            break;
        case buttonLeft:
            (positie < rijen - 1) ? positie++ : positie = 0;
            break;
        case buttonCenter:
            return positie;
            break;
        case buttonRight:
            (positie == 0) ? positie = rijen - 1 : positie--;
            break;
        }
    }
    return 0;
    
}

int menuDropbox(char *menuTable[], int rijen)
{
    int positie = 0;
    int positiePrevious = 0;
    M5.Lcd.setTextSize(2);
    M5.Lcd.setTextColor(DARKGREY, BLACK);
    for (int i = 0; i < rijen; i++)
    {
        M5.Lcd.setCursor(10, 27 + i * 18);
        M5.Lcd.printf("%-18s", menuTable[i]);
    }
    while (1)
    {
        M5.Lcd.setTextSize(2);
        M5.Lcd.setCursor(10, 27 + positiePrevious * 18);
        M5.Lcd.setTextColor(DARKGREY, BLACK);
        M5.Lcd.printf("%-18s ", menuTable[positiePrevious]);
        M5.Lcd.setCursor(10, 27 + positie * 18);
        M5.Lcd.setTextColor(WHITE, DARKGREY);
        M5.Lcd.printf("%-18s ", menuTable[positie]);
        positiePrevious = positie;
        int keuze = keyboardButtonBar("ESC", "DOWN", "UP","SLCT");
        switch (keuze)
        {
            case buttonNone:
                return buttonNone;
                break;
            case buttonEscape:
                return buttonNone;
                break;
            case buttonLeft:
                (positie < rijen - 1) ? positie++ : positie = 0;
                break;
            case buttonCenter:
                return positie;
                break;
            case buttonRight:
                (positie == 0) ? positie = rijen - 1 : positie--;
                break;
        }
    }
    return 0;
    
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
        case buttonEscape:
            return buttonNone;
            break;
        case buttonUp:
            (keyboardLocation < sizeof(keyboard) - 2) ? keyboardLocation++ : keyboardLocation = 0;
            localChangeArray[localChangeArrayPosition] = keyboard[keyboardLocation];
            break;
        case buttonDown:
            (localChangeArrayPosition < lengteArray) ? localChangeArrayPosition++ : localChangeArrayPosition = 0;
            keyboardLocation = 0;
            while (localChangeArray[localChangeArrayPosition] != keyboard[keyboardLocation])
                keyboardLocation++;
            break;
        case buttonOK:
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
        case buttonEscape:
            return startWaarde;
            break;
        case buttonUp:
            if (waarde > minimum) waarde = waarde - stap;

            break;
        case buttonDown:
            if (waarde < maximum)waarde = waarde + stap;
            break;
        case buttonOK:
            return waarde;
            break;
        }
    }
   return 0;
}
