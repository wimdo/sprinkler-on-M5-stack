
void readTime()
{
  M5.Rtc.GetTime(&RTCtime); //Gets the time in the real-time clock.  获取实时时钟内的时间
  M5.Rtc.GetDate(&RTCDate);
  clockData.checkSecond = false;
  if (screen.keyboardHold > 0)
  {
    screen.keyboardHold--;
  }
  ArduinoOTA.handle();
  showTimeBar();
}

void setTimezone(String timezone){
  Serial.printf("SYSTEM : Setting Timezone to %s\n",timezone.c_str());
  setenv("TZ",timezone.c_str(),1);  //  Now adjust the TZ.  Clock settings are adjusted to show the new local time
  tzset();
}


void setupTime() {
  if (WiFi.status() == WL_CONNECTED) {
    //const char *ntpServer = myServer.timeRequestURL;
    //Serial.printf("WIFI : ntp time request @ %s\n",ntpServer);
    //int timeZone = 3600;
    //int daylightOffset=3600;
    //configTime(timeZone, daylightOffset, ntpServer);
    struct tm timeInfo;
    configTime(0, 0, myServer.timeRequestURL);
    setTimezone("CET-1CEST,M3.5.0,M10.5.0/3");
    Serial.printf("WIFI : Ntp time request @ %s\n",myServer.timeRequestURL);
    if (getLocalTime(&timeInfo)) {
      Serial.println(&timeInfo, "SYSTEM : %A, %B %d %Y %H:%M:%S zone %Z %z ");
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
      clockData.timeSetByNTP= false;
    } 
  }
  readTime(); 
}

void calculateSolarTime(){
  SunSet sun;
  sun.setPosition(LATITUDE, LONGITUDE, DST_OFFSET);
  sun.setCurrentDate(RTCDate.Year, RTCDate.Month, RTCDate.Date);
  clockData.sunrise = static_cast<int>(sun.calcSunrise());
  clockData.sunset = static_cast<int>(sun.calcSunset());
  //clockData.previousMinute = RTCtime.Minutes;
  if (!clockData.timeSetByNTP){
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
