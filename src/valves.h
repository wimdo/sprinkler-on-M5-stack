
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
      Serial.print("I2C device found at address 0x");
      if (address<16)
        Serial.print("0");
      Serial.print(address,HEX);
      Serial.println("  !");
 
      nDevices++;
    }
    else if (error==4)
    {
      Serial.print("Unknown error at address 0x");
      if (address<16)
        Serial.print("0");
      Serial.println(address,HEX);
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
      Serial.print("I2C device found at address 0x");
      if (address<16)
        Serial.print("0");
      Serial.print(address,HEX);
      Serial.println("  !");
 
      nDevices++;
    }
    else if (error==4)
    {
      Serial.print("Unknown error at address 0x");
      if (address<16)
        Serial.print("0");
      Serial.println(address,HEX);
    }    
  }
  if (nDevices == 0)
    Serial.println("No I2C devices found\n");
  else
    Serial.println("done\n");
}



void disableRelais()
{
  sprinkler.sliderStateRelais = 0;
  Wire.requestFrom(RelayI2C, 1);
  if (Wire.available()) //If the request is available
  {
    Wire.beginTransmission(RelayI2C);
    Wire.write(~sprinkler.sliderStateRelais);
    Wire.endTransmission();
  }
  screen.updateRelaisSlider = true;
  clockData.sendData = true;
}

void switchRelais(byte relais, boolean state)
{
  bitWrite(sprinkler.sliderStateRelais,relais,state);
  Wire.requestFrom(RelayI2C, 1);
  if (Wire.available()) //If the request is available
  {
    Wire.beginTransmission(RelayI2C);
    Wire.write((byte)~sprinkler.sliderStateRelais);
    Wire.endTransmission();
  }
  screen.updateRelaisSlider = true; 
  clockData.sendData = true;
  Serial.println(sprinkler.sliderStateRelais,BIN);
}

void switchDakraam(boolean state){
  if (state==OPEN){
    bitWrite(sprinkler.sliderStateRelais,6,1);
    bitWrite(sprinkler.sliderStateRelais,7,1);
    sprinkler.sliderStateDakraam=OPEN;   
    Serial.println("dakraam open");
  } else{
    bitWrite(sprinkler.sliderStateRelais,6,1);
    bitWrite(sprinkler.sliderStateRelais,7,0);
    sprinkler.sliderStateDakraam=CLOSE;   
    Serial.println("dakraam dicht");
  }
  Wire.requestFrom(RelayI2C, 1);
  if (Wire.available()) //If the request is available
  {
    Wire.beginTransmission(RelayI2C);
    Wire.write((byte)~sprinkler.sliderStateRelais);
    Wire.endTransmission();
  }
  screen.updateRelaisSlider = true; 
  clockData.sendData = true;
}

void disableDakraamRelais(){
    bitWrite(sprinkler.sliderStateRelais,6,0);
    bitWrite(sprinkler.sliderStateRelais,7,0);
    Wire.requestFrom(RelayI2C, 1);
    if (Wire.available()) //If the request is available
    {
      Wire.beginTransmission(RelayI2C);
      Wire.write((byte)~sprinkler.sliderStateRelais);
      Wire.endTransmission();
    }
    screen.updateRelaisSlider = true; 
    clockData.sendData = true;
    Serial.println("disable dakraam relais");
}

void dakraamManueel(boolean state){
    if (state == OPEN){
      relais[6].data5=relais[6].data2;
      switchDakraam(OPEN); 
      // automatisch verloop stoppen
    } else {
      relais[6].data5=2;
      switchDakraam(CLOSE); 
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
        Serial.print("dakraam teller ");
        Serial.println(relais[6].data5);
        if (relais[6].data5==relais[6].data2-2){
          disableDakraamRelais();
        }
        if (relais[6].data5==2){
          switchDakraam(CLOSE);
           
        }
        if (relais[6].data5==0){
          disableDakraamRelais();
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
}

void switchValveRelay(byte relay, boolean state)
{
  sprinkler.sliderStateValve = 0B10000000; 
  bitWrite(sprinkler.sliderStateValve, relay-1, state);
  Wire.requestFrom(SprinklerI2C, 1);
  if (Wire.available()) //If the request is available
  {
    Wire.beginTransmission(SprinklerI2C);
    Wire.write(~sprinkler.sliderStateValve);
    Wire.endTransmission();
  }
  screen.updateSprinklerSlider = true;
  clockData.sendData = true;
}

void switchPump(boolean state)
{
  if (state ==1){
    screen.keyboardHold=2;
    sprinkler.pumpOn = true;
  } else {
    sprinkler.pumpOn = false;
  }
  switchValveRelay(8, state);
}


void switchValve(byte valve, boolean state)
{
  if (state ==1){
    screen.keyboardHold=1;
    sprinkler.valveOn = true;
  } else {
    sprinkler.valveOn = false;
  }
  switchValveRelay(valve, state); 
}

void disableValves()
{
  sprinkler.sliderStateValve = B10000000;
  Wire.requestFrom(SprinklerI2C, 1);
  if (Wire.available()) //If the request is available
  {  
    Wire.beginTransmission(SprinklerI2C);
    Wire.write(~sprinkler.sliderStateValve);
    Wire.endTransmission();
    Wire.requestFrom(SprinklerI2C, 1);
  }
  screen.updateSprinklerSlider = true;
  clockData.sendData = true;
  sprinkler.valveOn =false;
}

void ControleValves()
{
  if (sprinkler.runtimeLeft != 0)
  {
    if (sprinkler.runtimeValveLeft > mySprinkler.pauzeTime)
    {
      if (sprinkler.valveOn == false) switchValve(sprinkler.valveSelected, 1);
      sprinkler.runtimeValveLeft--;
    }
    else
    {
      if (sprinkler.valveOn == true) switchValve(sprinkler.valveSelected, 0);
      // hier controleren als de volgend de pomp nodig heeft en ander starten
      sprinkler.valveSelected = 0;
      if (sprinkler.runtimeValveLeft == 0)
      {
        sprinkler.programCounter++;
        sprinkler.valveSelected = SprinklerProgramRun.valve[sprinkler.programCounter];
        sprinkler.runtimeValveLeft = SprinklerProgramRun.valveTime[sprinkler.programCounter] * (60 - 59 * sprinkler.debugMode);
      }
      sprinkler.runtimeValveLeft--;
    }
    sprinkler.runtimeLeft--;
  }
  else
  {
    sprinkler.staat = End;
    sprinkler.valveTimeCounter = 0;
    sprinkler.valveSelected = 0;
    if (screen.keyboardInput ==false){
      drawButtonBar2("MENU",BLACK,"SETTINGS",BLACK);
    }
  }
}

void sprinklerStop(){
  disableValves();
  sprinkler.staat = End;
  sprinkler.valveTimeCounter = 0;
  sprinkler.valveSelected = 0;
  if (screen.keyboardInput ==false){
    drawButtonBar2("MENU",BLACK,"SETTINGS",BLACK);
  }
}

void printSprinklerStatus()
{

  Serial.print(sprinkler.staat);
  Serial.print("  ");
  Serial.print(sprinkler.runtimeLeft);
  Serial.print("  ");
  Serial.print(sprinkler.runtimeValveLeft);
  Serial.print("  ");
  Serial.print(sprinkler.runtimeTotal);
  Serial.print("  ");
  Serial.print(sprinkler.programCounter);
  Serial.print("  ");
  Serial.print(sprinkler.valveSelected);
  Serial.println("  ");

  //sprintf(serialArray, "pumptTime: %02d  pauzeTime: %02d   modus:%01d", mySprinkler.pumpTime, mySprinkler.pauzeTime, mySprinkler.modus);
}

void laadProgramma(byte programma)
{
  Serial.print(programma);
  Serial.println(" : copy naar SprinklerProgramRun ");
  SprinklerProgramRun = SprinklerProgram[programma];

  sprinkler.programCounter = 0;
  sprinkler.runtimeTotal = 0;
  for (byte i = 0; i < 16; i++)
  {
    sprinkler.runtimeTotal = sprinkler.runtimeTotal + SprinklerProgramRun.valveTime[i] * (60 - 59 * sprinkler.debugMode);
  }
  sprinkler.runtimeLeft = sprinkler.runtimeTotal;
  sprinkler.valveSelected = SprinklerProgramRun.valve[0];
  sprinkler.runtimeValveLeft = SprinklerProgramRun.valveTime[0] * (60 - 59 * sprinkler.debugMode);
  sprinkler.valveTimeCounter = 0;
  sprinkler.staat = Start;
}

void checkSprinklerStatus()
{
  switch (sprinkler.staat)
  {
  case Start: 
    if (sprinkler.pumpOn == false) switchPump(1);
    //showSprinklerStatus();
    if (sprinkler.valveTimeCounter == mySprinkler.pumpTime)
    {
      sprinkler.staat = Loopt;
      sprinkler.valveTimeCounter = 0;
    }
    else
      sprinkler.valveTimeCounter++;
    break;
  case Loopt:
    ControleValves();
    break;
  case End:
    disableValves();
    switchPump(0);
    sprinkler.staat = Wacht;
    break;
  default:
    break;
  }
}

void setupValves()
{
  disableValves();
  switchPump(0);
}

void controleerProgramma(byte uur, byte minuten, byte weekday)
{
  if (sprinkler.staat == Wacht && mySprinkler.modus == modeAutorun)
  {
    int timeInMin =1440;
    for (byte i = 1; i < 5; i++)
    {
      if (SprinklerProgram[i].actief == 1)
      {
        if (SprinklerProgram[i].dayToRun[weekday-1] == 1)
        {
          if ((SprinklerProgram[i].uurStart*60+SprinklerProgram[i].minStart) > (uur*60+minuten)) {
            
            if ((SprinklerProgram[i].uurStart*60+SprinklerProgram[i].minStart)<timeInMin){
              timeInMin= SprinklerProgram[i].uurStart*60+SprinklerProgram[i].minStart;
            }          
          }
          if ((SprinklerProgram[i].uurStart == uur) && (SprinklerProgram[i].minStart == minuten))
          {
            laadProgramma(i);
          }
        }
      }
    }
    clockData.hourNext = (timeInMin - timeInMin%60) / 60;
    clockData.minuteNext = timeInMin%60;
  }

}
