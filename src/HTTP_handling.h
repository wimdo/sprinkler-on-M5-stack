


String trimHTTPrequest(String dataString) {
  int beginPositie = 0;
  int eindPositie = 0;
  beginPositie = dataString.indexOf("?", 0) + 1;
  eindPositie =  dataString.indexOf(" ", beginPositie);
  dataString = dataString.substring(beginPositie, eindPositie) + "&";
  return dataString;
}


int checkHTTPSettingsProgram(String dataString) {
  int beginPositie = 0;
  int eindPositie = 0;
  int middenPositie = 0;
  int keuzeProgramma = 0;
  Serial.print("Check program settings request ");
  Serial.println(dataString);
  while (beginPositie < dataString.length()) {
    eindPositie = dataString.indexOf("&", beginPositie);
    middenPositie = dataString.indexOf("=", beginPositie);
    String fieldString = dataString.substring(beginPositie, middenPositie);
    String subString =  dataString.substring(middenPositie + 1, eindPositie);
    beginPositie = eindPositie + 1;/*
    Serial.print ("loop ");
    Serial.print (beginPositie);
    Serial.print(" ");
    Serial.print (eindPositie);
    Serial.print(" ");
    Serial.print(fieldString);
    Serial.print("   ");
    Serial.println(subString);
    */
    if (fieldString == "PROGRAM") {
      keuzeProgramma = subString.toInt();
    }
    if (fieldString == "STURING") {
      SprinklerProgram[keuzeProgramma].actief = (boolean) subString.toInt() ;
    }
    if (fieldString == "TIME") {
      SprinklerProgram[keuzeProgramma].uurStart = subString.substring(0,subString.indexOf("%3A")).toInt()  ;
      SprinklerProgram[keuzeProgramma].minStart = subString.substring(subString.indexOf("%3A")+3,subString.length() ).toInt() ;
    }
    for (int i = 0; i < 7; i++) {
      if (fieldString == String(dayOfWeek[i])) {
        SprinklerProgram[keuzeProgramma].dayToRun[i] = (boolean) subString.toInt() ;
      }
    }
    for (int i = 0; i < 16; i++) {
      if (fieldString == "VALVE" + String(i)) {
        SprinklerProgram[keuzeProgramma].valve[i] = subString.toInt() ;
      }
      if (fieldString == "TIJD" + String(i)) {
        SprinklerProgram[keuzeProgramma].valveTime[i] = subString.toInt();
      }
    }
  }
  writeProgramFile(keuzeProgramma);
  return keuzeProgramma;

}


int checkHTTPSettingsProgramKeuze(String dataString) {
  
  int middenPositie = 0;
  int programKeuze = 0;
  Serial.print("Check HTTP request ");
  Serial.println(dataString);
  middenPositie = dataString.indexOf("=", 0);
  String fieldString = dataString.substring(0, middenPositie);
  String subString =  dataString.substring(middenPositie + 1, dataString.length());
  Serial.println(fieldString);
  Serial.println(subString);
  if (fieldString == "PROGRAM") {
    programKeuze = subString.toInt();
  }
  Serial.println(programKeuze);
  return programKeuze;
}


void checkHTTPSettingsSprinkler(String dataString) {
  int beginPositie = 0;
  int eindPositie = 0;
  int middenPositie = 0;
  Serial.print("Check HTTP request");
  Serial.print(dataString);
  beginPositie = 0;
  while (beginPositie < dataString.length()) {
    eindPositie = dataString.indexOf("&", beginPositie);
    middenPositie = dataString.indexOf("=", beginPositie);
    String fieldString = dataString.substring(beginPositie, middenPositie);
    String subString =  dataString.substring(middenPositie + 1, eindPositie);
    if (fieldString == "MODUS") {
      mySprinkler.modus = (boolean) subString.toInt();
    } else if (fieldString == "DEBUG") {
      sprinkler.debugMode = (boolean) subString.toInt();
    }else if (fieldString == "PAUZETIME") {
      mySprinkler.pauzeTime = subString.toInt();
    } else if (fieldString == "PUMPTIME") {
      mySprinkler.pumpTime = subString.toInt();
    } else {
      for (int i = 1; i < 9; i++) {
        if (fieldString == "NAME" + String(i)) {
          while (subString.indexOf("+") >= 0) {
            subString.replace("+", " ");
          }
          subString.toCharArray(mySprinkler.valve[i].valveName, subString.length() + 1);
        } else if (fieldString == "PERCENTAGE" + String(i)) {
          mySprinkler.valve[i].percentage = subString.toInt();
        } else if (fieldString == "STURING" + String(i)) {
          mySprinkler.valve[i].withPump = (boolean) subString.toInt();
        }
      }
    }

    beginPositie = eindPositie + 1;
  }
  writeMySprinklerFile();
  controleerProgramma(RTCtime.Hours, RTCtime.Minutes, RTCDate.WeekDay); //controleerProgramma(now.hour(),now.minute(),now.dayOfTheWeek());
  checkRelaisSettingsOnTime(RTCtime.Hours, RTCtime.Minutes);
  //checkRelaisSettingsTempOnTime();
}


void checkStringContentServer(String dataString) {

  int beginPositie = dataString.indexOf("=", 0);
  int eindPositie = dataString.length();
  String fieldString = dataString.substring(0, beginPositie);
  String subString =  dataString.substring(beginPositie + 1, eindPositie);

 if (fieldString == "intervalServer") {
    myServer.refreshRate = subString.toInt();
  } else if (fieldString == "sensorToggle") {
    myServer.sensorToggle = (boolean) subString.toInt(); 
  } else if (fieldString == "connectWIFI") {
    myServer.connectToWIFI = (boolean) subString.toInt();
  } else if (fieldString == "connectMQTT") {
    myServer.connectToMQTT = (boolean) subString.toInt();
  } else if (fieldString == "serverMQTT") {
    subString.toCharArray(myServer.serverMQTT, subString.length() + 1);
  } else if (fieldString == "portMQTT") {
    myServer.port = subString.toInt();
  } else if (fieldString == "topicMQTT") {
    while (subString.indexOf("%2F") >= 0) {
      subString.replace("%2F", "/");
    }
    subString.toCharArray(myServer.topicPublish, subString.length() + 1);
  } else if (fieldString == "timeRequestURL") {
    while (subString.indexOf("%3A") >= 0) {
      subString.replace("%3A", ":");
    }
    while (subString.indexOf("%2F") >= 0) {
      subString.replace("%2F", "/");
    }
    subString.toCharArray(myServer.timeRequestURL, subString.length() + 1); 
  }
}

void checkHTTPSettingsServer(String dataString) {

  int beginPositie = 0;
  int eindPositie = 0;
  int lengteHTTPrequest = 0;
  Serial.print("Check HTTP request ");
  Serial.println(dataString);
  lengteHTTPrequest = dataString.length();
  beginPositie = 0;
  for (int teller = 0; teller < lengteHTTPrequest; teller++) {
    eindPositie = dataString.indexOf("&", beginPositie);
    if (eindPositie != -1) {
      String subString = dataString.substring(beginPositie, eindPositie);
      Serial.println(subString);
      checkStringContentServer(subString);
      beginPositie = eindPositie + 1;
    }
  }
  writeMyServerFile();
  setupTemperature();
  setupTime();
}

void HTTPrequestCheck() {
  if (HTTPrequest.indexOf("GET /program.php?") >= 0) {
    Serial.println(F("program request received"));
  }
  else if (HTTPrequest.indexOf("GET /settingsServer.php") >= 0) {
    checkHTTPSettingsServer(trimHTTPrequest(HTTPrequest));
    myServerSettingsPage();
  }
  else if (HTTPrequest.indexOf("GET /settingsServer") >= 0) {
    myServerSettingsPage();
  }
  else if (HTTPrequest.indexOf("GET /settingsSprinkler.php") >= 0) {
    checkHTTPSettingsSprinkler(trimHTTPrequest(HTTPrequest));
    mySprinklerSettingsPage();
  }
  else if (HTTPrequest.indexOf("GET /settingsTime.php") >= 0) {
    //Tijd aanpassen
    mySprinklerSettingsPage();
  }
  else if (HTTPrequest.indexOf("GET /settingsSprinkler") >= 0) {
    mySprinklerSettingsPage();
  }
  else if (HTTPrequest.indexOf("GET /settingsValve.php") >= 0) {
    checkHTTPSettingsSprinkler(trimHTTPrequest(HTTPrequest));
    valveSettingsPage();
  }
  else if (HTTPrequest.indexOf("GET /settingsProgramKeuze.php") >= 0) {
    programSettingsPage(checkHTTPSettingsProgramKeuze(trimHTTPrequest(HTTPrequest)));

  }
  else if (HTTPrequest.indexOf("GET /settingsProgram.php") >= 0) {
    Serial.println("check gegevens programma");
    programSettingsPage(checkHTTPSettingsProgram(trimHTTPrequest(HTTPrequest)));
  }

  else if (HTTPrequest.indexOf("GET /settingsValve") >= 0) {
    valveSettingsPage();
  }
  else if (HTTPrequest.indexOf("GET /main") >= 0) {
    webServerMainPage(client);
  }
  else if (HTTPrequest.indexOf("GET /favicon.ico") >= 0) {
    webServerMainPage(client);
  }
  else if (HTTPrequest.indexOf("GET /") >= 0) {
    webServerMainPage(client);
  }
}
void HTPPcheckClient() {
  Serial.println("New Client.");          // print a message out in the serial port
  String currentLine = "";                // make a String to hold incoming data from the client
  currentTime = millis();
  previousTime = currentTime;
  while (client.connected() && currentTime - previousTime <= timeoutTime) { // loop while the client's connected
    currentTime = millis();
    if (client.available()) {
      currentLine = client.readStringUntil('\r');
      char c = client.read();
      if (c == '\n') {
        if (currentLine.length() == 0) {
          Serial.print("HTTP-request :");
          Serial.println(HTTPrequest);
          client.println("HTTP/1.1 200 OK");
          client.println("Content-type:text/html");
          client.println("Connection: close");
          client.println();
          HTTPrequestCheck();
          HTTPrequest = "";
          client.stop();
          Serial.println("Client disconnected.");
          Serial.println("");
        } else {
          if (HTTPrequest == "") {
            HTTPrequest = currentLine;
          }
          currentLine = "";
        }
      }
    }
  }
}

