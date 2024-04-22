void I2Cscan()
{
  byte error, address;
  int nDevices;

  Serial.println("Scanning Wire");
  nDevices = 0;
  for(address = 1; address < 127; address++ )
  {
    Wire.beginTransmission(address);
    error = Wire.endTransmission();
 
    if (error == 0)
    {
      Serial.printf("I2C device found at address 0x%X",address);
      nDevices++;
    }
    else if (error==4)
    {
      Serial.printf("Unknown error at address 0x%X",address);
    }    
  }
  if (nDevices == 0)
    Serial.println("No I2C devices found\n");
  else
    Serial.println("done\n");
nDevices = 0;
Serial.println("Scanning Wire1");
  for(address = 1; address < 127; address++ )
  {
    Wire1.beginTransmission(address);
    error = Wire1.endTransmission();
 
    if (error == 0)
    {
      Serial.printf("I2C device found at address 0x%X",address);
      nDevices++;
    }
    else if (error==4)
    {
      Serial.printf("Unknown error at address 0x%X",address);
    }    
  }
  if (nDevices == 0)
    Serial.println("No I2C devices found\n");
  else
    Serial.println("done\n");
}

void switchRelaisToSliderState(){
  Wire.requestFrom(RelayI2C, 1);
  if (Wire.available()) //If the request is available
  {
    Wire.beginTransmission(RelayI2C);
    Wire.write((byte)~relaisBoard.sliderStateRelais);
    Wire.endTransmission();
  }
  screen.updateRelaisSlider = true; 
  clockData.sendData = true;
  //Serial.println(relaisBoard.sliderStateRelais,BIN);  
}

void disableRelais()
{
  Serial.println("RELAY : disable relais");
  relaisBoard.sliderStateRelais = 0;
  switchRelaisToSliderState();
}

void switchRelais(byte relais, boolean state)
{
  bitWrite(relaisBoard.sliderStateRelais,relais,state);
  switchRelaisToSliderState();
}

void switchDakraam(boolean state){
  relaisBoard.dakraamState = waitForDirection;
  relaisBoard.dakraamTime = dakraamTimePreset;
  relaisBoard.sliderStateDakraam =state; 
}

void dakraamManueel(boolean state){
  relais[6].actief=false; //als het manueel geopend wordt vervalt het programma voor de rest van de dag. 
  switchDakraam(state);
}


void initDakraam(){

  if (relais[6].actief){
    if (temperature[0].value >=  relais[6].data1){
      relais[6].data5=relais[6].data2;
      switchDakraam(OPEN); 
      Serial.println("RELAY : dakraam open");
    } else {
      switchDakraam(CLOSE);
    Serial.println("RELAY : dakraam dicht");
    }
  }       
}

void checkDakraam(){
  if (relais[6].actief){
    if (temperature[0].value >=  relais[6].data1){
      if (relais[6].data5 > 0){
        relais[6].data5=relais[6].data2;
      } else {
        relais[6].data5=relais[6].data2;
        switchDakraam(OPEN); 
      }
    }         
  }
}

void checkRelaisSettingsTemp(){
  for (int i=0; i<6;i++ ){
    if (relais[i].actief){
      if (relais[i].control==6){
        if (temperature[0].value >=  relais[i].data1){
          if (relais[i].data5 > 0){
            // als waarde >0 is loopt het programma, enkel de waarde opnieuw op max zetten. 
            relais[i].data5=relais[i].data2;
          } else {
            // als waarde 0 is, loopt het programma nog niet, waarde op max zetten en relais aan zetten
            relais[i].data5=relais[i].data2;
            switchRelais(i, ON); 
          }
        }         
      } 
    }
  }
  checkDakraam();
}


void checkRelaisSettingsOnTime(int hour, int min){
  int minuteCount = hour*60+min;
  for (int i=0; i<6;i++ ){
    //// programTable[] = {"none","time", "sunrise", "sunset","day","night", "temp on"};
    if (relais[i].actief){
      if (relais[i].control==1){ 
          if (relais[i].data1==hour && relais[i].data2==min){
              Serial.printf("RELAY : %d:%d switch relay %d state %s\n",hour, min,i,"on");
              switchRelais(i, ON);
          }
          if (relais[i].data3==hour && relais[i].data4==min){
              Serial.printf("RELAY : %d:%d switch relay %d state %s\n",hour, min,i,"off");
              switchRelais(i, OFF);
          }
      } else if (relais[i].control==2){
          if (minuteCount==clockData.sunrise){
              Serial.printf("RELAY : %d:%d switch relay %d state %s\n",hour, min,i,"on");
              switchRelais(i, ON);
          }
          if (relais[i].data1==hour && relais[i].data2==min){
              Serial.printf("RELAY : %d:%d switch relay %d state %s\n",hour, min,i,"off");
              switchRelais(i, OFF);
          }
      } else if (relais[i].control==3){
          if (minuteCount==clockData.sunset){
              Serial.printf("RELAY : %d:%d switch relay %d state %s\n",hour, min,i,"on");
              switchRelais(i, ON);
          }
          if (relais[i].data1==hour && relais[i].data2==min){
              Serial.printf("RELAY : %d:%d switch relay %d state %s\n",hour, min,i,"off");
              switchRelais(i, OFF);
          }
      } else if (relais[i].control==4){
          if (minuteCount==clockData.sunrise){
              Serial.printf("RELAY : %d:%d switch relay %d state %s\n",hour, min,i,"on");
              switchRelais(i, ON);
          }
          if (minuteCount==clockData.sunset){
              Serial.printf("RELAY : %d:%d switch relay %d state %s\n",hour, min,i,"off");
              switchRelais(i, OFF);
          }
      } else if(relais[i].control==5){
          Serial.println("night");
      } else if (relais[i].control==6){
        if (relais[i].data5>0){  // om het programma correct te laten starten, waarde
          relais[i].data5--;
          if (relais[i].data5==0){
            switchRelais(i,OFF);  
          }
        } 
      } 
    }  
  }
  if (relais[6].actief){
    if (relais[6].data5>0){  // om het programma correct te laten starten, waarde
      relais[6].data5--;
      if (relais[6].data5 == 0) {
        switchDakraam(CLOSE);
      }
    }         
  } 
 

}


void startRelaisProgram(){
  for (int hour =0; hour<RTCtime.Hours;hour++ ){
    for (int min =0; min<60;min++ ){
      checkRelaisSettingsOnTime(hour,min);
    }    
  } 
  for (int min =0; min<=RTCtime.Minutes;min++ ){
    checkRelaisSettingsOnTime(RTCtime.Hours,min);
  } 
  initDakraam(); // controle op temperatuur als het te laag is dan ook opdracht om te sluiten. 
}
