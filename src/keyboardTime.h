
void readTime()
{
  M5.Rtc.GetTime(&RTCtime); //Gets the time in the real-time clock.  获取实时时钟内的时间
  M5.Rtc.GetDate(&RTCDate);
  clockData.checkSecond = false;
  showTimeBar();
  if (screen.keyboardHold > 0)
  {
    screen.keyboardHold--;
  }
  if (relaisBoard.dakraamState != idle){
    switch (relaisBoard.dakraamState){
      case waitForDirection : // wachtloop 1seconde
        relaisBoard.dakraamState=chooseDirection;
        relaisBoard.dakraamTime--;
        break;
      case chooseDirection : // richting bepalen
        if (relaisBoard.sliderStateDakraam==OPEN){
          bitWrite(relaisBoard.sliderStateRelais,7,1); // direction relais aan
        } else {
          bitWrite(relaisBoard.sliderStateRelais,7,0); // direction relais uit
        };   
        switchRelaisToSliderState();
        relaisBoard.dakraamState=activatePower;
        relaisBoard.dakraamTime--;
        break;
      case activatePower : // wachtloop 1 seconde
        bitWrite(relaisBoard.sliderStateRelais,6,1); // power relais aan
        switchRelaisToSliderState();
        relaisBoard.dakraamState=motorRunning;
        relaisBoard.dakraamTime--;
        break;
      case motorRunning : // aftellen tot einde
        if (relaisBoard.dakraamTime ==0){
          bitWrite(relaisBoard.sliderStateRelais,6,0); // power relais uit
          bitWrite(relaisBoard.sliderStateRelais,7,0); // direction relais uit
          switchRelaisToSliderState();
          relaisBoard.dakraamState = idle;
        } else{
          relaisBoard.dakraamTime--;
        }
        break;
    }
    //Serial.printf("dakraam tijd %d dakraam %d status %d\n",relaisBoard.dakraamTime,relaisBoard.sliderStateDakraam,relaisBoard.dakraamState);
  }
  ArduinoOTA.handle();
}

void setTimezone(String timezone){
  Serial.printf("SYSTEM : Setting Timezone to %s\n",timezone.c_str());
  setenv("TZ",timezone.c_str(),1);  //  Now adjust the TZ.  Clock settings are adjusted to show the new local time
  tzset();
}

boolean checkDST(){
  M5.Rtc.GetDate(&RTCDate);
  if (RTCDate.Month < 3 || RTCDate.Month > 10)  return false; 
  if ((RTCDate.Month>3) && (RTCDate.Month<11))  return true;
  if (RTCDate.Month==3){
    if (RTCDate.WeekDay==7 && RTCDate.Date>=25){
      return true;
    } else {
      return false;
    }
  }
  if (RTCDate.Month==11){
    if (RTCDate.WeekDay==7 && RTCDate.Date>=25){
      return false;
    } else {
      return true;
    }
  }
  return false;
}

void setupTime() {
  if (WiFi.status() == WL_CONNECTED) {
    struct tm timeInfo;
    configTime(0, 0, myServer.timeRequestURL);
    setTimezone("CET-1CEST,M3.5.0,M10.5.0/3");
    Serial.printf("WIFI : Ntp time request @ %s\n",myServer.timeRequestURL);
    if (getLocalTime(&timeInfo)) {
      Serial.println(&timeInfo, "SYSTEM : %A, %B %d %Y %H:%M:%S zone %Z %z ");
      Serial.print("zomertijd / wintertijd Ntp: ");
      Serial.println(clockData.isDST);
      clockData.isDST=checkDST();
      Serial.print("zomertijd / wintertijd berekend: ");
      Serial.println(clockData.isDST);
      RTCtime.Hours   = timeInfo.tm_hour;
      RTCtime.Minutes = timeInfo.tm_min;
      RTCtime.Seconds = timeInfo.tm_sec;
      M5.Rtc.SetTime(&RTCtime);
      RTCDate.WeekDay = timeInfo.tm_wday;
      RTCDate.Month = timeInfo.tm_mon + 1;
      RTCDate.Date = timeInfo.tm_mday;
      RTCDate.Year = timeInfo.tm_year + 1900;
      M5.Rtc.SetDate(&RTCDate);
      clockData.timeSetByNTP= true;
    } else {
      M5.Rtc.GetTime(&RTCtime); 
      M5.Rtc.GetDate(&RTCDate);
      clockData.timeSetByNTP= false;
      clockData.isDST=checkDST();
      Serial.printf("SYSTEM : zomertijd / wintertijd berekend: %d\n",clockData.isDST);
    } 
    
  } else {  
      M5.Rtc.GetTime(&RTCtime); 
      M5.Rtc.GetDate(&RTCDate);
      clockData.timeSetByNTP= false;
      clockData.isDST=checkDST();
      Serial.printf("SYSTEM : zomertijd / wintertijd berekend: %d\n",clockData.isDST);
  }
  readTime();
}

void calculateSolarTime(){
  SunSet sun;
  sun.setPosition(LATITUDE, LONGITUDE, DST_OFFSET);
  M5.Rtc.GetDate(&RTCDate);
  sun.setCurrentDate(RTCDate.Year, RTCDate.Month, RTCDate.Date);
  clockData.sunrise = static_cast<int>(sun.calcSunrise());
  clockData.sunset = static_cast<int>(sun.calcSunset());
  if (!clockData.timeSetByNTP){
    if (clockData.isDST){
      clockData.sunrise=clockData.sunrise+60;
      clockData.sunset=clockData.sunset+60;
    }
  }
  Serial.printf("SYSTEM : Sunrise: %d:%d, Sunset %d:%d\n", (clockData.sunrise/60), (clockData.sunrise%60), (clockData.sunset/60), (clockData.sunset%60));
}


int keyboardButtonBar(String buttonString1, String buttonString2, String buttonString3,String buttonString4)
{
  drawButtonBar4(buttonString1,BLACK,buttonString2,BLACK,buttonString3,BLACK,buttonString4,BLACK);
  long previousMillis = millis();
  while (1)
  {
    if (clockData.checkSecond)
      readTime();
    if ((millis() - previousMillis) > 10000)
      return buttonNone;
    M5.update();  
    if (b1.wasPressed()) {
      if (buttonString1.length()>0){
        soundsBeep(1000, 100, 1);
        return button1;    
      }
    } else if (b2.wasPressed()){
      if (buttonString2.length()>0){
        soundsBeep(1000, 100, 1);
        return button2;
      }
    } 
  }
  return 0;
 }

int keyboardButtonBar2(String buttonString1, String buttonString2)
{
  drawButtonBar2(buttonString1, BLACK,buttonString2,BLACK);
  while (1)
  {
    long previousMillis = millis();
    while (1)
    {
      if (clockData.checkSecond)
        readTime();
      if ((millis() - previousMillis) > 10000)
        return buttonNone;
      M5.update();  
      if (b1.wasPressed()) {
        soundsBeep(1000, 100, 1);
        return button1;
      } else if (b2.wasPressed()){
        soundsBeep(1000, 100, 1);
        return button1;
      } 
    }
    return 0;
  }
}
