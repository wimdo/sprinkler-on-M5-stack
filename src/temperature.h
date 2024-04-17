
void readTemperature()
{

  if (temperature[0].present)
  {
    temperature[0].value = temperatuur.getTempC(&temperature[0].deviceAddress[0]);
    if (temperature[0].value != DEVICE_DISCONNECTED_C)
    {
      if (temperature[0].value > temperature[0].valueMax)
        temperature[0].valueMax = temperature[0].value;
      if (temperature[0].value < temperature[0].valueMin)
        temperature[0].valueMin = temperature[0].value;
    }
  }
  if (temperature[1].present)
  {
    temperature[1].value = temperatuur.getTempC(&temperature[1].deviceAddress[0]);
    if (temperature[1].value != DEVICE_DISCONNECTED_C)
    {
      if (temperature[1].value > temperature[1].valueMax)
        temperature[1].valueMax = temperature[1].value;
      if (temperature[1].value < temperature[1].valueMin)
        temperature[1].valueMin = temperature[1].value;
    }
  }
  temperatuur.requestTemperatures();
}

void setupTemperature()
{
  temperatuur.begin();
  oneWire.reset_search();
  if (myServer.sensorToggle)
  {
    temperature[1].present = oneWire.search((&temperature[1].deviceAddress[0]));
    temperature[0].present = oneWire.search((&temperature[0].deviceAddress[0]));
  }
  else
  {
    temperature[0].present = oneWire.search((&temperature[0].deviceAddress[0]));
    temperature[1].present = oneWire.search((&temperature[1].deviceAddress[0]));
  }
  sprintf(temperature[0].deviceAddressName,"%02hhX%02hhX%02hhX%02hhX%02hhX%02hhX%02hhX%02hhX",temperature[0].deviceAddress[0],temperature[0].deviceAddress[1],temperature[0].deviceAddress[2],temperature[0].deviceAddress[3],temperature[0].deviceAddress[4],temperature[0].deviceAddress[5],temperature[0].deviceAddress[6],temperature[0].deviceAddress[7]);
  sprintf(temperature[1].deviceAddressName,"%02hhX%02hhX%02hhX%02hhX%02hhX%02hhX%02hhX%02hhX",temperature[1].deviceAddress[0],temperature[1].deviceAddress[1],temperature[1].deviceAddress[2],temperature[1].deviceAddress[3],temperature[1].deviceAddress[4],temperature[1].deviceAddress[5],temperature[1].deviceAddress[6],temperature[1].deviceAddress[7]);
  Serial.printf("SYSTEM : temperature sensor 1 :%s\n",temperature[0].deviceAddressName);
  Serial.printf("SYSTEM : temperature sensor 1 :%s\n",temperature[1].deviceAddressName);
  temperature[0].valueMin = 100;
  temperature[0].valueMax = -100;
  temperature[1].valueMin = 100;
  temperature[1].valueMax = -100;
  temperatuur.setWaitForConversion(false);
  temperatuur.requestTemperatures();
  delay(800);
  readTemperature();
}