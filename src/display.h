void drawInfoBox(int xposBox,int yposBox,int heightBox,int widhtBox,int textSize, char title[]){
  int lengteTitle =strlen(title)*6*textSize;
  M5.Lcd.fillRect(xposBox,yposBox,  widhtBox, heightBox, BLACK); 
  M5.Lcd.drawRect(xposBox,yposBox+4,  widhtBox, heightBox-4, DARKGREY);  
  if (lengteTitle !=0){
    int positieTitle= (widhtBox-lengteTitle)/2; 
    M5.Lcd.fillRect(xposBox+positieTitle-1, yposBox, lengteTitle+1, 8*textSize, BLACK);
    M5.Lcd.setCursor(xposBox+positieTitle, yposBox);
    M5.Lcd.setTextColor(DARKGREY);
    M5.Lcd.setTextSize(textSize); 
    M5.Lcd.printf(title); 
  }
}

void drawInfoBoxSprite(TFT_eSprite *sprite,int widhtBox,int heightBox, int textSize, char title[]){
  int lengteTitle =strlen(title)*6*textSize;
  sprite->fillRect(0,0,  widhtBox, heightBox, BLACK); //BLACK
  sprite->drawRect(0,4,  widhtBox, heightBox-4, DARKGREY);  //DARKGEY
  if (lengteTitle !=0){
    int positieTitle= (widhtBox-lengteTitle)/2; 
    sprite->fillRect(positieTitle-1, 0, lengteTitle+1, 8*textSize, BLACK);//BLACK
    sprite->setCursor(positieTitle, 0);
    sprite->setTextColor(DARKGREY);//DARKGREY
    sprite->setTextSize(textSize); 
    sprite->printf(title); 
  }
}

void hideTouchButton(touchButton *a){
  M5.Lcd.fillRoundRect(a->x, a->y, a->widht, a->height,8,BLACK);
}

void hideTouchButtonSprite(TFT_eSprite *sprite,touchButton *a){
  sprite->fillRoundRect(a->x, a->y, a->widht, a->height,8,BLACK);
}

void drawTouchButton(touchButton *a, int textSize, boolean centerText){
  M5.Lcd.fillRoundRect(a->x, a->y, a->widht, a->height,8,a->backgroundColor);
  M5.Lcd.setTextColor(a->textColor);
  M5.Lcd.setTextSize(textSize);
  int ypos = a->y+ (a->height-textSize*8)/2;
  int xpos =a->x+2;
  if (centerText){
    xpos= a->x+(a->widht-(a->text.length()*(textSize*6)))/2;
  } 
  M5.lcd.setCursor(xpos,ypos);
  M5.Lcd.print(a->text); 
}

void drawTouchButtonSprite(TFT_eSprite *sprite,touchButton *a, int textSize, boolean centerText){
  sprite->fillRoundRect(a->x, a->y, a->widht, a->height,8,a->backgroundColor);
  sprite->setTextColor(a->textColor);
  sprite->setTextSize(textSize);
  int ypos = a->y+ (a->height-textSize*8)/2;
  int xpos =a->x+2;
  if (centerText){
    xpos= a->x+(a->widht-(a->text.length()*(textSize*6)))/2;
  } 
  sprite->setCursor(xpos,ypos);
  sprite->print(a->text); 
}


boolean checkTouchButton(touchButton *a, int x, int y){
  if (x >= a->x && x <= a->x+a->widht){
    if(y >= a->y && y <= a->y+a->height){
      soundsBeep(1000, 100, 1);
      return 1;
    }
  }
  return 0;
}

boolean checkTouchButtonSprite(touchButton *a, int spriteX, int spriteY, int x, int y){
  if (x >= a->x+spriteX && x <= a->x+a->widht+spriteX){
    if(y >= a->y+spriteY && y <= a->y+a->height+spriteY){
      soundsBeep(1000, 100, 1);
      return 1;
    }
  }
  return 0;
}

void printTouchButton(touchButton *a, int x, int y){
  Serial.print(x);
  Serial.print(":");
  Serial.print(a->x);
  Serial.print("-");
  Serial.print(a->x+a->widht);
  Serial.print("   ");
  Serial.print(y);
  Serial.print(":");
  Serial.print(a->y);
  Serial.print("-");
  Serial.println(a->y+a->height);
}

void clearButtonBar(){
M5.Lcd.fillRect(0, 292, 240, 30, BLACK); 
} 

void drawButtonBar4(String buttonString1, int color1,String buttonString2,int color2, String buttonString3,int color3,String buttonString4,int color4){
  touchButton Button1 =(touchButton) {0,292,58,30,DARKGREY,color1,buttonString1};
  touchButton Button2 =(touchButton) {60,292,58,30,DARKGREY,color2,buttonString2};
  touchButton Button3 =(touchButton) {120,292,58,30,DARKGREY,color3,buttonString3};
  touchButton Button4 =(touchButton) {180,292,58,30,DARKGREY,color4,buttonString4};
  drawTouchButton(&Button1,2,1); 
  drawTouchButton(&Button2,2,1);
  drawTouchButton(&Button3,2,1); 
  drawTouchButton(&Button4,2,1);
}

void drawButtonBar2(String buttonString1,int color1, String buttonString2,int color2){
  touchButton Button1 =(touchButton) {0,292,118,30,DARKGREY,color1,buttonString1};
  touchButton Button2 =(touchButton) {120,292,118,30,DARKGREY,color2,buttonString2};
  drawTouchButton(&Button1,2,1); 
  drawTouchButton(&Button2,2,1);
}

void outlineMainscreen()
{
  M5.Lcd.clear();
  drawInfoBox(0,19,68,240,1, "INFO");
  drawInfoBox(0,19+68,68+68,118,1, "SPRINKLER"); // 87
  drawInfoBox(120,19+68,68+68-38,120,1, "RELAIS"); // 155
  drawInfoBox(120,19+68+68+68-38,38,120,1, "DAKRAAM");
  drawInfoBox(0,19+68+68+68,68,118,1, "SERRE"); // 223
  drawInfoBox(120,19+68+68+68,68,120,1, "BUITEN");  
  if (sprinkler.staat == Wacht){
    drawButtonBar2("MENU",BLACK,"SETTINGS",BLACK);
  } else {
    drawButtonBar2("MENU",BLACK,"STOP",RED);
  }
  
}


void showRelaisSliderStatus(){
  int columnPos =120;
  for (int i = 0; i < 6; i++)  {
    M5.Lcd.fillRoundRect(columnPos+3,94+i*15+1, 10, 12, 4, bitRead(sprinkler.sliderStateRelais, i)? BLACK : RED);
    M5.Lcd.fillRoundRect(columnPos+3+10,94+i*15+1, 10, 12, 4, bitRead(sprinkler.sliderStateRelais, i)? GREEN : BLACK);      
  }
}


void showRelaySlider()
{
  M5.Lcd.setTextColor(DARKGREY);
  M5.Lcd.setTextSize(1); 
  int columnPos =120;
  for (int i = 0; i < 6; i++)  {
    M5.Lcd.drawRoundRect(columnPos+2, 94+i*15, 22, 14, 4, DARKGREY); 
    M5.Lcd.setCursor(columnPos+25, 94+i*15+4);
    M5.Lcd.print(relais[i].relaisName);
  }
  showRelaisSliderStatus();
}

void showSprinkerSliderTime(){
  M5.Lcd.setTextColor(DARKGREY,BLACK);
  M5.Lcd.setTextSize(1);
  int columnPos =0;
  int bitCompare = 7;
  for (int i = 0; i < 8; i++)  {
    if (bitRead(sprinkler.sliderStateValve, bitCompare)){
        M5.Lcd.setCursor(columnPos+25+56, 94+i*15+4);
        M5.Lcd.printf("%03d:%02d",(sprinkler.runtimeValveLeft - sprinkler.runtimeValveLeft % 60) / 60,sprinkler.runtimeValveLeft % 60);  
    }
    bitCompare--; 
  }

}

void showSprinklerSliderStatus(){
  int columnPos =0;
  for (int i = 0; i < 8; i++)  {
    M5.Lcd.fillRoundRect(columnPos+3,98+i*15+1, 10, 12, 4, bitRead(sprinkler.sliderStateValve, i)? BLACK : RED);
    M5.Lcd.fillRoundRect(columnPos+3+10,98+i*15+1, 10, 12, 4, bitRead(sprinkler.sliderStateValve, i)? GREEN : BLACK);      
  }
}

void showSprinklerSlider()
{
  M5.Lcd.setTextColor(DARKGREY,BLACK);
  M5.Lcd.setTextSize(1);
  int columnPos =0;
  for (int i = 0; i < 8; i++)  {
    M5.Lcd.drawRoundRect(columnPos+2, 98+i*15, 22, 14, 4, DARKGREY);  
    M5.Lcd.setCursor(columnPos+25, 98+i*15+4);
    M5.Lcd.print(mySprinkler.valve[i+1].valveName); // pomptext zit op 8 in array!
  }
  showSprinklerSliderStatus();
}

void showSprinklerStatus()
{
  int basePos = 0;
  int columnPos =0;
  M5.Lcd.setTextColor(DARKGREY,BLACK); 
  if (screen.updateSprinklerSlider)
  {
    showSprinklerSlider();
    screen.updateSprinklerSlider = false;
  }
  if (screen.updateRelaisSlider)
  {
    showRelaySlider();
    screen.updateRelaisSlider = false;
  }
  M5.Lcd.setCursor(2, 54);
  M5.Lcd.printf("Op %d:%d, Onder %d:%d", (clockData.sunrise/60), (clockData.sunrise%60), (clockData.sunset/60), (clockData.sunset%60));
  M5.Lcd.setCursor(2, 62);
  M5.Lcd.printf("IP   : %u.%u.%u.%u", myIP[0],myIP[1],myIP[2],myIP[3] );
  M5.Lcd.setCursor(2, 70);
  M5.Lcd.printf("Topic : %s", myServer.topicPublish);
  M5.Lcd.setCursor(2, 26);
  switch (sprinkler.staat)
  {
    case Start:
      M5.Lcd.printf("Start na   %d/%d", sprinkler.valveTimeCounter, mySprinkler.pumpTime);
      break;
    case Loopt:
      M5.Lcd.printf("%s %02d:%02d/%02d:%02d", (&mySprinkler.valve[sprinkler.valveSelected].valveName), (sprinkler.runtimeValveLeft - sprinkler.runtimeValveLeft % 60) / 60, sprinkler.runtimeValveLeft % 60, (sprinkler.runtimeLeft - sprinkler.runtimeLeft % 60) / 60, sprinkler.runtimeLeft % 60);
      break;
    case Wacht:
      if (mySprinkler.modus == modeAutorun)
      {
        if (sprinkler.debugMode) {M5.Lcd.print("Auto Debugmode        ");}
        else
        {
          if (clockData.hourNext == 24) {M5.Lcd.print("Start : morgen        ");}
          else {M5.Lcd.printf("Start om %02d:%02d          ", clockData.hourNext, clockData.minuteNext);} 
        }
      }
      else {(sprinkler.debugMode) ? M5.Lcd.print("Manueel Debugmode    ") : M5.Lcd.print("Manueel              ");}
      break;
    default:
      break;
  }
  for (int i = 0; i < 2; i++)  {
    basePos = 223;
    columnPos = i*120;
    if (temperature[i].present){
      basePos = 223;
      M5.Lcd.setCursor(columnPos+43, basePos+16); 
      M5.Lcd.setTextSize(2);
      M5.Lcd.setTextColor(DARKGREY,BLACK); 
      (temperature[i].value != DEVICE_DISCONNECTED_C) ? M5.Lcd.printf("%.1f", temperature[i].value) : M5.Lcd.print("----");
      M5.Lcd.setTextSize(1);
      M5.Lcd.setCursor(columnPos+3, basePos+40); 
      M5.Lcd.printf("Min :  %.1f", temperature[i].valueMin);
      M5.Lcd.setCursor(columnPos+3, basePos+52); 
      M5.Lcd.printf("Max :  %.1f", temperature[i].valueMax);
    }
  }
  
}  

void showTimeBar()
{
  M5.Lcd.setTextSize(2);
  M5.Lcd.setTextColor(DARKGREY,BLACK); 
  M5.lcd.setCursor(2,1); 
  M5.Lcd.printf("%02d:%02d:%02d %s %02d/%02d",RTCtime.Hours,RTCtime.Minutes,RTCtime.Seconds,dayOfWeek[RTCDate.WeekDay], RTCDate.Date, RTCDate.Month);
  M5.Lcd.setTextSize(1);
  M5.Lcd.drawXBitmap(225, 0, wifiBMP, 16, 16, (WiFi.status()==WL_CONNECTED)? DARKGREEN : RED); 
  if (!screen.keyboardInput)
  {
    showSprinklerStatus();
  }
}



