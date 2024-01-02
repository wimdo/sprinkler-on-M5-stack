void mqttSendIPAdress()
{
    myIP = WiFi.localIP();
    sprintf (myPayload,"{\"IP\":\"%u.%u.%u.%u\",\"Module\":\"%s\",\"Hostname\":\"%s\",\"time\":\"%u\"}", myIP[0],myIP[1],myIP[2],myIP[3],moduletype,fullhostname,millis());
    sprintf (myPayload,"%u.%u.%u.%u", myIP[0],myIP[1],myIP[2],myIP[3]);
    JsonObject obj = doc.to<JsonObject>();
    doc["IP"]= myPayload;
    doc["Module"]= moduletype;
    doc["Version"]= VERSION;
    doc["Hostname"]= fullhostname;
    doc["Time"]= millis();
    for (int i = 0; i < 8; i++) {
      doc["Relais"][i]= bitRead(sprinkler.sliderStateRelais,i);
    }
    for (int i = 0; i < 8; i++) {
      doc["Valve"][i]= bitRead(sprinkler.sliderStateValve,i);
    }
    serializeJson(doc, myPayload);
    sprintf (myTopic,"%s/%s",infoPrefix, &myServer.topicPublish[0]);
    //sprintf (myTopic,"%s/%s",infoPrefix, &myServer.topic[0]);
    clientEsp.publish(myTopic, myPayload,1);
    Serial.printf("MQTT : %s = %s\n", myTopic, myPayload ); 
}



bool connectMQTT()
{
  byte errorCountConnection = 0;
  while (!clientEsp.connected())
  {
    if (clientEsp.connect(fullhostname))
    { 
      mqttSendIPAdress();
      sprintf (myTopic,"%s/%s",commandPrefix, &myServer.topicPublish[0]);
      if (clientEsp.subscribe(myTopic)) { 
        Serial.printf("MQTT : Subscribed to %s\n",myTopic);
      } else {
        Serial.printf("MQTT : Fail to subscribed to %s\n",myTopic);
      }
      return true;
    }
    else
    {
      errorCountConnection++;
      delay(500);
    }
    if (errorCountConnection == 5)
    {
      Serial.println(F("MQTT : Can not connect with MQTT server"));
      return false;
      break;
    }
  }
  return true;
}

void sendData()
{ 
    myPayloadString ="";
    JsonObject obj = doc.to<JsonObject>();
    if (WiFi.status() == WL_CONNECTED)
    {
      if (myServer.connectToMQTT){ 
        if (connectMQTT()) {  
            for (int i = 0; i < 8; i++) {
              doc["Relais"][i]= bitRead(sprinkler.sliderStateRelais,i);
            }
            for (int i = 0; i < 8; i++) {
              doc["Valve"][i]= bitRead(sprinkler.sliderStateValve,i);
            }
            if (temperature[0].present){
              doc["Temp"]["SERRE"]["Id"]= temperature[0].deviceAddressName;
              doc["Temp"]["SERRE"]["Temperature"]= round((double)temperature[0].value*10)/10;
              
            }  
            if (temperature[1].present){
                doc["Temp"]["BUITEN"]["Id"]= temperature[1].deviceAddressName;
                doc["Temp"]["BUITEN"]["Temperature"]= round((double)temperature[1].value*10)/10;
            } 
            if (!doc.isNull() ){
              serializeJson(doc, myPayload);
              sprintf (myTopic,"%s/%s",dataPrefix, &myServer.topicPublish[0]);
              clientEsp.publish(myTopic, myPayload,1);
              Serial.printf("MQTT : %s = %s\n", myTopic, myPayload ); 
            }
          }
        }    
      }
}

void callback(char* topic, byte* payload, unsigned int length) {
    char command[10] = "";
    char status[8]="";
    for (int i = 0; i < length; i++) {
      myPayload[i]=payload[i]; 
    }
    myPayload[length]= 0;
    Serial.printf("MQTT : received [%s] %s\n", topic, myPayload);
    deserializeJson(doc, myPayload, length);
    strcpy(command, doc["command"] | "default");
    if ( strcmp(command,"IP") == 0 ){
      mqttSendIPAdress(); 
    }
    if ( strcmp(command,"SET_RELAY") == 0 ){
        int relais = doc["relais"];
        boolean state = false;
        strcpy(status, doc["state"] | "default");  
        if ( strcmp(status,"ON") == 0 ){
          state = true;
        } else {
          state = false;
        }
        switchRelais(relais, state);
        sendData();
    }
}


