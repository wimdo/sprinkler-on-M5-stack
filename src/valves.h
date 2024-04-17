/*

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
*/


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
  Serial.println("VALVE : disable valves");
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
