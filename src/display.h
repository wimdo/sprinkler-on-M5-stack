
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

boolean checkTouchButton(touchButton *a, int x, int y){
  if (x >= a->x && x <= a->x+a->widht){
    if(y >= a->y && y <= a->y+a->height){
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
  int base =292;
  M5.Lcd.fillRect(0, base, 240, 30, BLACK);  
  M5.Lcd.fillRect(0, base, 58, 30, DARKGREY);
  M5.Lcd.fillRect(60, base, 58, 30, DARKGREY);
  M5.Lcd.fillRect(120, base, 58, 30, DARKGREY);
  M5.Lcd.fillRect(180, base, 60, 30, DARKGREY);
  M5.Lcd.setTextSize(2);
  M5.Lcd.setTextColor(color1);
  M5.lcd.setCursor((56-buttonString1.length()*12)/2,base+6);
  M5.Lcd.print(buttonString1); 
  M5.Lcd.setTextColor(color2);
  M5.lcd.setCursor(60+(56-buttonString2.length()*12)/2,base+6);
  M5.Lcd.print(buttonString2); 
  M5.Lcd.setTextColor(color3);
  M5.lcd.setCursor(120+(56-buttonString3.length()*12)/2,base+6);
  M5.Lcd.print(buttonString3); 
  M5.Lcd.setTextColor(color4);
  M5.lcd.setCursor(180+(56-buttonString4.length()*12)/2,base+6);
  M5.Lcd.print(buttonString4); 
}

void drawButtonBar2(String buttonString1,int color1, String buttonString2,int color2){
  int base = 292;
  M5.Lcd.fillRect(0, base, 240, 30, BLACK);  
  M5.Lcd.fillRect(0, base, 56+4+56+2, 30, DARKGREY);
  M5.Lcd.fillRect(120, base, 56+4+56+4, 30, DARKGREY);
  M5.Lcd.setTextSize(2);
  M5.Lcd.setTextColor(color1);
  M5.lcd.setCursor((56+4+56-buttonString1.length()*12)/2,base+6);
  M5.Lcd.print(buttonString1); 
  M5.Lcd.setTextColor(color2);
  M5.lcd.setCursor(120+(56+4+56-buttonString2.length()*12)/2,base+6);
  M5.Lcd.print(buttonString2); 
}

void outlineMainscreen()
{
  drawInfoBox(0,19,68,240,1, "INFO");
  drawInfoBox(0,19+68,68+68,118,1, "SPRINKLER"); // 87
  drawInfoBox(120,19+68,68+68,120,1, "RELAIS"); // 155
  drawInfoBox(0,19+68+68+68,68,118,1, "SERRE"); // 223
  drawInfoBox(120,19+68+68+68,68,120,1, "BUITEN");  
  if (sprinkler.staat == Wacht){
    drawButtonBar2("MENU",BLACK,"SETTINGS",BLACK);
  } else {
    drawButtonBar2("MENU",BLACK,"STOP",RED);
  }
  
}


void showRelaisSliderStatus(){
  int bitCompare = 7;
  int columnPos =120;
  for (int i = 0; i < 8; i++)  {
    M5.Lcd.fillRoundRect(columnPos+3,94+i*15+1, 10, 12, 4, bitRead(sprinkler.sliderStateRelais, bitCompare)? GREEN : BLACK);
    M5.Lcd.fillRoundRect(columnPos+3+10,94+i*15+1, 10, 12, 4, bitRead(sprinkler.sliderStateRelais, bitCompare)? BLACK : RED);      
    bitCompare--; 
  }
}


void showRelaySlider()
{
  int bitCompare = 7;
  M5.Lcd.setTextColor(DARKGREY);
  M5.Lcd.setTextSize(1); 
  int columnPos =120;
  for (int i = 0; i < 8; i++)  {
    M5.Lcd.drawRoundRect(columnPos+2, 94+i*15, 22, 14, 4, DARKGREY); 
    M5.Lcd.setCursor(columnPos+25, 94+i*15+4);
    M5.Lcd.print(relais[7-i].relaisName);
    bitCompare--; 
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
  int bitCompare = 7;
  for (int i = 0; i < 8; i++)  {
    M5.Lcd.fillRoundRect(columnPos+3,94+i*15+1, 10, 12, 4, bitRead(sprinkler.sliderStateValve, bitCompare)? GREEN : BLACK);
    M5.Lcd.fillRoundRect(columnPos+3+10,94+i*15+1, 10, 12, 4, bitRead(sprinkler.sliderStateValve, bitCompare)? BLACK : RED);      
    bitCompare--; 
  }
    //showSprinkerSliderTime();
}

// pomp zit op 8 !
void showSprinklerSlider()
{
  M5.Lcd.setTextColor(DARKGREY,BLACK);
  M5.Lcd.setTextSize(1);
  int columnPos =0;
  int bitCompare = 7;
  for (int i = 0; i < 8; i++)  {
    M5.Lcd.drawRoundRect(columnPos+2, 94+i*15, 22, 14, 4, DARKGREY);  
    M5.Lcd.setCursor(columnPos+25, 94+i*15+4);
    M5.Lcd.print(mySprinkler.valve[7-i+1].valveName);
    bitCompare--; 
  }
  showSprinklerSliderStatus();
  //showSprinkerSliderTime();
}

void showButtonBar(String buttonString1, String buttonString2, String buttonString3)
{

}

void showSprinklerStatus()
{
  int basePos = 0;
  int columnPos =0;
  M5.Lcd.setTextColor(DARKGREY,BLACK); 
  if (sprinkler.updateSprinklerSlider)
  {
    showSprinklerSlider();
    sprinkler.updateSprinklerSlider = false;
  }
  if (sprinkler.updateRelaisSlider)
  {
    showRelaySlider();
    sprinkler.updateRelaisSlider = false;
  }
  M5.Lcd.setCursor(2, 54);
  M5.Lcd.printf("Op %d:%d, Onder %d:%d", (sprinkler.sunrise/60), (sprinkler.sunrise%60), (sprinkler.sunset/60), (sprinkler.sunset%60));
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
          if (sprinkler.hourNext == 24) {M5.Lcd.print("Start : morgen        ");}
          else {M5.Lcd.printf("Start om %02d:%02d          ", sprinkler.hourNext, sprinkler.minuteNext);} 
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
  M5.Lcd.drawXBitmap(230, 4, wifi_OK, 10, 7, (WiFi.status()==WL_CONNECTED)? DARKGREEN : RED); 
  if (!sprinkler.keyboardInput)
  {
    showSprinklerStatus();
  }
}



void setupDisplay()
{
  M5.Lcd.setRotation(2);
  drawButtonBar2("MENU",BLACK,"SETTINGS",BLACK);
}


