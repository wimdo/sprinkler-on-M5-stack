





void configModeCallback (WiFiManager *myWiFiManager) {
  IPAddress myIP = WiFi.softAPIP();
  //outlineStartUpScreen();
  /*
  display.setCursor(2, 26);
  display.print("Setup device on: ");
  display.setCursor(2, 36);
  display.printf("%s", fullhostname);
  display.setCursor(2, 45);
  display.printf("%d.%d.%d.%d", myIP[0],myIP[1],myIP[2],myIP[3]);
  */
}

void reconnectWifiManager(){
  Serial.println(millis());
  if (myServer.connectToWIFI) {  
    //display.setCursor(2, 26);
    //display.print("reconnecting to network");
    WiFiManager wifiManager;
    wifiManager.setConfigPortalTimeout(10);
    boolean result = wifiManager.autoConnect(fullhostname);
    if (result)  {
      if (myServer.connectToMQTT ) {
        Serial.println("MQTT : start");
        clientEsp.setServer(&myServer.serverMQTT[0], myServer.port);
        clientEsp.setCallback(callback);
        connectMQTT();
      }
      setupTime();
      setupOTA();
    } else {
        Serial.println("WIFI : not reconnected");
    }   
  } else {
      WiFi.mode(WIFI_AP);
      WiFi.softAP(fullhostname);
  }
  Serial.println("WIFI : Start webserver");
  server.begin();
  Serial.println(millis());
}


void setupWifiManager(){
  uint16_t maxlen = strlen(nameprefix) + 7;
  uint8_t mac[6];
  WiFi.macAddress(mac);
  snprintf(fullhostname, maxlen, "%s-%02X%02X%02X", nameprefix, mac[3], mac[4], mac[5]);  
  if (myServer.connectToWIFI) { 
    //display.setCursor(2, 26);      
    //display.print("connecting to network");
    WiFiManager wifiManager;  
    wifiManager.setAPCallback(configModeCallback);
    wifiManager.setConfigPortalTimeout(60);
    boolean result = wifiManager.autoConnect(fullhostname);
    if (result)  {
      if (myServer.connectToMQTT ) {
        Serial.println("MQTT : start");
        clientEsp.setServer(&myServer.serverMQTT[0], myServer.port);
        clientEsp.setCallback(callback);
        connectMQTT();
      }
    } else {
        Serial.println("WIFI : not connected");
        myServer.connectToWIFI = false;
    }   
  } else {
      WiFi.mode(WIFI_AP);
      WiFi.softAP(fullhostname);
  }
  setupTime();
  setupOTA();
  Serial.println("WIFI : Start webserver");
  server.begin();
}