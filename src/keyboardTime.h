
void readTime()
{
  M5.Rtc.GetTime(&RTCtime); //Gets the time in the real-time clock.  获取实时时钟内的时间
  M5.Rtc.GetDate(&RTCDate);
  clockData.checkSecond = false;
  if (sprinkler.keyboardHold > 0)
  {
    sprinkler.keyboardHold--;
  }
  ArduinoOTA.handle();
  showTimeBar();
}

void setupTime() {
  if (WiFi.status() == WL_CONNECTED) {
    const char *ntpServer = myServer.timeRequestURL;
    Serial.printf("WIFI : ntp time request @ %s\n",ntpServer);
    int timeZone = 3600;
    int daylightOffset=3600;
    configTime(timeZone, daylightOffset, ntpServer);
    struct tm timeInfo;
    if (getLocalTime(&timeInfo)) {
      RTCtime.Hours   = timeInfo.tm_hour;
      RTCtime.Minutes = timeInfo.tm_min;
      RTCtime.Seconds = timeInfo.tm_sec;
      M5.Rtc.SetTime(&RTCtime);
      RTCDate.WeekDay = timeInfo.tm_wday;
      RTCDate.Month = timeInfo.tm_mon + 1;
      RTCDate.Date = timeInfo.tm_mday;
      RTCDate.Year = timeInfo.tm_year + 1900;
      M5.Rtc.SetDate(&RTCDate);
      sprinkler.timeSetByNTP= true;
    } else {
      sprinkler.timeSetByNTP= false;
    } 
  }
  readTime(); 
}

void calculateSolarTime(){
  sun.setPosition(LATITUDE, LONGITUDE, DST_OFFSET);
  sun.setCurrentDate(RTCDate.Year, RTCDate.Month, RTCDate.Date);
  sprinkler.sunrise = static_cast<int>(sun.calcSunrise());
  sprinkler.sunset = static_cast<int>(sun.calcSunset());
  clockData.previousMinute = RTCtime.Minutes;
  boolean summertime = false;
  if ((RTCDate.Month>3) || (RTCDate.Month<11)){
    if (RTCDate.Month==3){
      if(RTCDate.Date < 8 ){
        summertime = false;
      } else if (RTCDate.Date > 14 ){
        summertime = true;  
      } else {
        int secondSunday=RTCDate.Date-RTCDate.WeekDay;
        if( secondSunday < 8 ){
          secondSunday += 7;
        } 
        if( RTCDate.Date > secondSunday ) {
          summertime = true;
        }  
      }
    } else if (RTCDate.Month==11){
      if(RTCDate.Date > 7 ){
        summertime = false;
      } else {
        int firstSunday =RTCDate.Date-RTCDate.WeekDay;
        if( firstSunday < 1 ){
          firstSunday += 7;
        }
        if( RTCDate.Date < firstSunday) {
          summertime = true;
        } 
      }
    } else {
      summertime = true;
    }
  }
  if (summertime){
    sprinkler.sunrise=sprinkler.sunrise+60;
    sprinkler.sunset=sprinkler.sunset+60;
  }
  Serial.printf("SYSTEM : Time: %d:%d, Sunrise: %d:%d, Sunset %d:%d\n", RTCtime.Hours,RTCtime.Minutes,(sprinkler.sunrise/60), (sprinkler.sunrise%60), (sprinkler.sunset/60), (sprinkler.sunset%60));
}


int keyboardScan()
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
      return button2;
    } else if (b3.wasPressed()){
      soundsBeep(1000, 100, 1);
      return button3;
    }  else if (b4.wasPressed()){
      soundsBeep(1000, 100, 1);
      return button4;
    }
  }
  return 0;
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
    } else if (b3.wasPressed()){
      if (buttonString3.length()>0){
        soundsBeep(1000, 100, 1);
        return button3;
      }
    }  else if (b4.wasPressed()){
      if (buttonString4.length()>0){
        soundsBeep(1000, 100, 1);
        return button4;
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
      } else if (b3.wasPressed()){
        soundsBeep(1000, 100, 1);
        return button2;
      }  else if (b4.wasPressed()){
        soundsBeep(1000, 100, 1);
        return button2;
      }
    }
    return 0;
  }
}

