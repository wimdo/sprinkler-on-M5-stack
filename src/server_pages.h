void webServerMainPage(WiFiClient client) {

  char infoLine0 [100];
  client.println("<!DOCTYPE html>");
  client.println("<html>");
  client.println("<head>");
  client.println("<title>Temperaturecontrol server settings page</title>");
  client.println("<meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">"); // auto scale for mobile or website
  client.println("<meta http-equiv=\"refresh\" content=\"" + String(myServer.refreshRate) + "\">");
  client.println("<style>html { font-family: monospace; display: inline-block; margin: 0px auto; text-align: left;}");
  client.println("form  { display: table;      }");
  client.println(" p    { display: table-row;  }");
  client.println("label { display: table-cell; }");
  client.println("input { display: table-cell; }");
  client.println(".button { background-color: DarkGrey; border: 2px; margin: 2px;padding: 5px; width: 320px ;color: white; text-align: center;text-decoration: none; font-size: 20px; cursor: pointer;}");
  client.println("type=\"text/css\" #btn_submit{width:120px;} #formbox {width:320px; margin:auto 0; text-align: left;}#formbox2 {width:320px; margin:auto 0; text-align: center;}");
  client.println("</style>");
  client.println("</head>");
  client.println("<body>");

 
  switch (sprinkler.staat)
  {
  case Start:
    sprintf(infoLine0,"Start na   %d/%d", sprinkler.valveTimeCounter, mySprinkler.pumpTime );
    break;
  case Loopt:
    sprintf(infoLine0 , " %s %02d:%02d/%02d:%02d", (&mySprinkler.valve[sprinkler.valveSelected].valveName), (sprinkler.runtimeValveLeft - sprinkler.runtimeValveLeft % 60) / 60, sprinkler.runtimeValveLeft % 60, (sprinkler.runtimeLeft - sprinkler.runtimeLeft % 60) / 60, sprinkler.runtimeLeft % 60);
    
    break;
  case Wacht:
    if (mySprinkler.modus == modeAutorun)
    {
      if (sprinkler.debugMode)
      {
        sprintf(infoLine0,"Auto Debugmode        ");
      }
      else
      {
        if (clockData.hourNext == 24)
        {
          sprintf(infoLine0,"Start : morgen        ");
        }
        else
        {
          sprintf(infoLine0,"Start om %02d:%02d          ", clockData.hourNext, clockData.minuteNext);
        }
      }
    }
    else
    {
      (sprinkler.debugMode) ? sprintf(infoLine0,"Manueel Debugmode    " ): sprintf(infoLine0,"Manueel              ");
    }

    break;
  default:
    break;
  }
  
  
  client.println("<div id=\"formbox2\"><h1>Sprinkler</h1>");
  client.printf("<div id=\"formbox2\"><h> %s </h></div>", infoLine0);
  client.println("<br><hr></div>");
  if (sprinkler.staat > Wacht)
  {
    client.println("<p><a href=\"stopSprinkler\"><button class=\"button\">STOP</button></a></p>");
  }


  client.println("<div id=\"formbox2\"><h1>Relais</h1>");
    // alle relais info
  client.println("<br><hr></div>");
  

  if (temperature[0].value != DEVICE_DISCONNECTED_C) {
        client.printf("<div id=\"formbox2\"><h1> Serre : %s&deg </h1></div>", String(temperature[0].value, 1));
  } else {
        client.printf("<div id=\"formbox2\"><h1> Serre : %s&deg </h1></div>", "----");
  }
  client.printf("<div id=\"formbox2\"><h>  Min :%s  Max :%s</h></div>", String(temperature[0].valueMin, 1),String(temperature[0].valueMax, 1));
 
 if (temperature[1].value != DEVICE_DISCONNECTED_C) {
        client.printf("<div id=\"formbox2\"><h1> Buiten : %s&deg </h1></div>", String(temperature[1].value, 1));
  } else {
        client.printf("<div id=\"formbox2\"><h1> Buiten : %s&deg </h1></div>", "----");
  }
  client.printf("<div id=\"formbox2\"><h>  Min :%s  Max :%s</h></div>", String(temperature[1].valueMin, 1),String(temperature[1].valueMax, 1));
  client.println("<br><hr>");
  
  client.println("<p><a href=\"settingsSprinkler\"><button class=\"button\">SPRINKLER SETTINGS</button></a></p>");
  client.println("<p><a href=\"settingsServer\"><button class=\"button\">SERVER SETTINGS</button></a></p>");
  client.println("<p><a href=settingsValve\"\"><button class=\"button\">VALVES SETTINGS</button></a></p>");
  client.println("<p><a href=settingsRelais\"\"><button class=\"button\">RELAIS SETTINGS</button></a></p>");

  client.println("</body>");
  client.println("</html>");
  client.println();

}


void programSettingsPage(int keuzeProgramma) {
  Serial.print(" programSetingsPage with");
  Serial.println(keuzeProgramma);
  client.println("<!DOCTYPE html>");
  client.println("<html>");
  client.println("<head>");
  client.println("<title>Sprinkler valve settings page</title>");
  client.println("<meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">"); // auto scale for mobile or website
  client.println("<style>html { font-family: monospace; display: inline-block; margin: 0px auto; text-align: left;}");
  client.println("form  { display: table;      }");
  client.println("p     { display: table-row;  }");
  client.println("label { display: table-cell; }");
  client.println("input { display: table-cell; }");
  client.println(".button { background-color: DarkGrey; border: 2px; margin: 2px;padding: 5px; width: 320px ;color: white; text-align: center;text-decoration: none; font-size: 20px; cursor: pointer;}");
  client.println("type=\"text/css\" #btn_submit{width:120px;} #formbox {width:320px; margin:auto 0; text-align: left;}#formbox2 {width:320px; margin:auto 0; text-align: center;}");
  client.println("</style>");
  client.println("</head>");
  client.println("<body>");
  client.println("<form action=\"/settingsProgram.php\" method=\"get\">");
  client.println("<div id=\"formbox2\">");
  client.println("<p><label for=\"PROGRAM\">Programma : </label>");
  client.println("<input type=\"text\"  name=\"PROGRAM\"   value=\"" + String(keuzeProgramma) + "\" readonly><br><br></p>");
  // programma actief?
  //keuzeProgramma--;
  client.println("<div id=\"formbox\">");
  client.println("<p><label for=\"STURING\">Actief :</label>");
  client.println("<select name=\"STURING\" >");
  if (SprinklerProgram[keuzeProgramma].actief ) {
    client.println("  <option value=\"1\" selected>Yes</option>");
    client.println("  <option value=\"0\" >No</option>");
  } else {
    client.println("  <option value=\"1\" >Yes</option>");
    client.println("  <option value=\"0\" selected>No</option>");
  }
  client.println("</select>");
  //uur
  String valueString = "";
  if (SprinklerProgram[keuzeProgramma].uurStart < 10) {
    valueString = "0" + String(SprinklerProgram[keuzeProgramma].uurStart) + ":";
  } else {
    valueString = String(SprinklerProgram[keuzeProgramma].uurStart) + ":";
  }
  if (SprinklerProgram[keuzeProgramma].minStart < 10) {
    valueString = valueString + "0" + String(SprinklerProgram[keuzeProgramma].minStart) ;
  } else {
    valueString = valueString + String(SprinklerProgram[keuzeProgramma].minStart) ;

  }
  client.println("<label for=\"TIME\">Tijd : </label>");
  client.println("<input type=\"time\"  name=\"TIME\"   value=" + valueString + "><br><br></p>");
  client.println("</div>");
  //dag lopen
  client.println("<div id=\"formbox\">");
  client.println("<p>");
  for (int t = 0; t < 7; t++) {
    valueString = String(dayOfWeek[t]);
    if (SprinklerProgram[keuzeProgramma].dayToRun[t] == 1) {
      client.println("<input checked type=\"checkbox\" name=\""  + valueString + "\"value=\"1\"><label for=\"" + valueString +  "\"> " + valueString + " </label>");
    } else {
      client.println("<input type=\"checkbox\" name=\""  + valueString + "\"value=\"1\"><label for=\"" + valueString +  "\"> " + valueString + " </label>");
    }
  }
  client.println("<br><br></p></div>");
  //cyclus en tijd
  client.println("<div id=\"formbox\">");
  for (int i = 0; i < 16; i++) {
    client.println("<p><label for=\"VALVE\">" + String(i + 1) + " :</label>");
    client.println("<select name=\"VALVE" + String(i) + "\" >");
    if (SprinklerProgram[keuzeProgramma].valve[i] == 0 ) {
      client.println("  <option value=\"" + String(0) + "\" selected>" + "Geen Valve" + "</option>");
    } else {
      client.println("  <option value=\"" + String(0) + "\" >" + "Geen Valve" + "</option>");
    }
    for (int t = 1; t < 9; t++) {
      if (SprinklerProgram[keuzeProgramma].valve[i] == t) {
        client.println("  <option value=\"" + String(t) + "\" selected>" + String(mySprinkler.valve[t].valveName) + "</option>");
      } else  {
        client.println("  <option value=\"" + String(t) + "\" >" + String(mySprinkler.valve[t].valveName) + "</option>");
      }
    }
    client.println("</select>");
    client.println("<label for=\"TIJD\">Tijd </label>");
    valueString = String(SprinklerProgram[keuzeProgramma].valveTime[i]);
    client.println("<input type=\"number\"  name=\"TIJD" + String(i) + "\" min=\"0\" max=\"40\"  value=" + valueString + ">");
    client.println("<br><br></p>");
  }
  //client.println("<div id=\"formbox\">");
  client.println("</div>");
  client.println("<div id=\"formbox2\">");
  client.println("<input type=\"submit\" value=\"Change\"   >");
  client.println("<hr>");
  client.println("</div>");
  client.println("</form>");
  client.println("<br>");
  client.println("<p><a href=main\"\"><button class=\"button\">BACK</button></a></p>");
  client.println("</body>");
  client.println("</html>");
  client.println();
}


void valveSettingsPage() {
  client.println("<!DOCTYPE html>");
  client.println("<html>");
  client.println("<head>");
  client.println("<title>Sprinkler valve settings page</title>");
  client.println("<meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">"); // auto scale for mobile or website
  client.println("<style>html { font-family: monospace; display: inline-block; margin: 0px auto; text-align: left;}");
  client.println("form  { display: table;      }");
  client.println("p     { display: table-row;  }");
  client.println("label { display: table-cell; }");
  client.println("input { display: table-cell; }");
  client.println(".button { background-color: DarkGrey; border: 2px; margin: 2px;padding: 5px; width: 320px ;color: white; text-align: center;text-decoration: none; font-size: 20px; cursor: pointer;}");
  client.println("type=\"text/css\" #btn_submit{width:120px;} #formbox {width:320px; margin:auto 0; text-align: left;}#formbox2 {width:320px; margin:auto 0; text-align: center;}");
  client.println("</style>");
  client.println("</head>");

  client.println("<body>");
  client.println("<form action=\"/settingsValve.php\" method=\"get\">");
  client.println("<div id=\"formbox\">");
  for (int i = 1; i < 9; i++) {
    String valueString = "value=\"" + String(mySprinkler.valve[i].valveName) + "\"";
    client.println("<p><label for=\"NAME\"><u><b>VALVE " + String(i) + "</b></u></label><input type=\"text\" name=\"NAME" + String(i) + "\" maxlength=\"10\"" + valueString + "><br><br></p>");
    client.println("<p><label for=\"PERCENTAGE\">percentage </label>");
    valueString = String(mySprinkler.valve[i].percentage);
    client.println("<input type=\"number\"  name=\"PERCENTAGE" + String(i) + "\" min=\"0\" max=\"100\"  value=" + valueString + ">");
    client.println("<label for=\"STURING\">Pompen:</label>");
    client.println("<select name=\"STURING" + String(i) + "\" >");
    if (mySprinkler.valve[i].withPump ) {
      client.println("  <option value=\"1\" selected>Yes</option>");
      client.println("  <option value=\"0\" >No</option>");
    } else {
      client.println("  <option value=\"1\" >Yes</option>");
      client.println("  <option value=\"0\" selected>No</option>");
    }
    client.println("</select><br><br></p>");
    client.println("<hr><br>");
  }
  client.println("</div>");
  client.println("<div id=\"formbox2\">");
  client.println("<input type=\"submit\" value=\"Change\"   >");
  client.println("<hr>");
  client.println("</div>");
  client.println("</form>");
  client.println("<br>");
  client.println("<p><a href=main\"\"><button class=\"button\">BACK</button></a></p>");
  client.println("</body>");
  client.println("</html>");
  client.println();
}



void mySprinklerSettingsPage() {
  //DateTime timeNow = rtc.now();
  String valueString;

  client.println("<!DOCTYPE html>");
  client.println("<html>");
  client.println("<head>");
  client.println("<title>Sprinkler settings page</title>");
  client.println("<meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">"); // auto scale for mobile or website
  client.println("<style>html { font-family: monospace; display: inline-block; margin: 0px auto; text-align: left;}");
  client.println("form  { display: table;      }");
  client.println(" p    { display: table-row;  }");
  client.println("label { display: table-cell; }");
  client.println("input { display: table-cell; }");
  client.println(".button { background-color: DarkGrey; border: 2px; margin: 2px;padding: 5px; width: 320px ;color: white; text-align: center;text-decoration: none; font-size: 20px; cursor: pointer;}");
  client.println("type=\"text/css\" #btn_submit{width:120px;} #formbox {width:320px; margin:auto 0; text-align: left;}#formbox2 {width:320px; margin:auto 0; text-align: center;}");
  client.println("</style>");
  client.println("</head>");
  //body
  client.println("<body>");
  client.println("<form action=\"/settingsSprinkler.php\" method=\"get\">");
  client.println("<div id=\"formbox2\">");
  client.println("<h1>Sprinkler settings</h1>");
  client.println("</div>");
  // automatisch of manueel
  client.println("<div id=\"formbox\">");
  client.println("<p><label for=\"MODUS\">Modus:</label>");
  client.println("<select name=\"MODUS\" >");
  if (mySprinkler.modus == modeAutorun) {
    client.println("  <option value=\"1\" selected>Automatisch</option>");
    client.println("  <option value=\"0\" >Manueel</option>");
  } else {
    client.println("  <option value=\"1\" >Automatisch</option>");
    client.println("  <option value=\"0\" selected>Manueel</option>");
  }
  client.println("</select><br><br></p>");
  // debug

  client.println("<p><label for=\"DEBUG\">Debug:</label>");
  client.println("<select name=\"DEBUG\" >");
  if (sprinkler.debugMode == 1) {
    client.println("  <option value=\"1\" selected>On</option>");
    client.println("  <option value=\"0\" >Off</option>");
  } else {
    client.println("  <option value=\"1\" >On</option>");
    client.println("  <option value=\"0\" selected>Off</option>");
  }
  client.println("</select><br><br></p>");
  // pauze time
  valueString = String(mySprinkler.pauzeTime);
  client.println("<p><label for=\"PAUZETIME\">Pauze tijd (s): </label>");
  client.println("<input type=\"number\"  name=\"PAUZETIME\" min=\"2\" max=\"30\"  value=" + valueString + "><br><br></p>");
  // pump time
  valueString = String(mySprinkler.pumpTime);
  client.println("<p><label for=\"PUMPTIME\">Pomp tijd (s): </label>");
  client.println("<input type=\"number\"  name=\"PUMPTIME\" min=\"2\" max=\"30\"  value=" + valueString + "><br><br></p>");
  client.println("</div>");
  client.println("<div id=\"formbox2\">");
  client.println("<input type=\"submit\" value=\"Change\"   >");
  client.println("<hr>");
  client.println("</div>");
  client.println("</form>");
  // tijd
  client.println("<form action=\"/settingsTime.php\" method=\"get\">");
  client.println("<div id=\"formbox2\">");
  client.println("<h1>Tijd</h1>");
  client.println("</div>");
  if (!clockData.timeSetByNTP) {
    client.println("<div id=\"formbox\">");
    valueString = String(RTCtime.Hours) + ":" + String(RTCtime.Minutes); //+":"+String(timeNow.second()); //value="09:00"
    client.println("<p><label for=\"TIME\">Tijd: </label>");
    client.println("<input type=\"time\"  name=\"TIME\"   value=" + valueString + "><br><br></p>");
    valueString = String(RTCDate.Year) + "-" + String(RTCDate.Month) + "-" + String(RTCDate.Date);
    client.println("<p><label for=\"DATE\">Datum: </label>");
    client.println("<input type=\"date\"  name=\"DATE\"   value=" + valueString + "><br><br></p>");
    client.println("</div>");
    client.println("<div id=\"formbox2\">");
    client.println("<input type=\"submit\" value=\"Change\"   >");
    client.println("<hr>");
    client.println("</div>");
  } else {
    client.println("<div id=\"formbox\">");
    client.println("<h3>Time set by URL</h3>");
    client.println("<hr>");
    client.println("</div>");
  }
  client.println("</form>");
  client.println("<br>");

  // Programma
  client.println("<div id=\"formbox2\">");
  client.println("<h1>Program settings</h1>");
  client.println("</div>");
  client.println("<form action=\"/settingsProgramKeuze.php\" method=\"get\">");
  client.println("<div id=\"formbox\">");
  valueString = String(0);
  client.println("<p><label for=\"PROGRAM\">Wijzig programma: </label>");
  client.println("<input type=\"number\"  name=\"PROGRAM\" min=\"0\" max=\"4\"  value=" + valueString + "><br><br></p>");
  client.println("</div>");
  client.println("<div id=\"formbox2\">");
  client.println("<input type=\"submit\" value=\"Change\"   >");
  client.println("<hr>");
  client.println("</div>");
  client.println("</form>");
  // buttons onderaan
  client.println("<br>");
  client.println("<p><a href=main\"\"><button class=\"button\">BACK</button></a></p>");
  client.println("</body>");
  client.println("</html>");
  client.println();
}

void myServerSettingsPage() {

  client.println("<!DOCTYPE html>");
  client.println("<html>");
  client.println("<head>");
  client.println("<title>Temperaturecontrol server settings page</title>");
  client.println("<meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">"); // auto scale for mobile or website
  client.println("<style>html { font-family: monospace; display: inline-block; margin: 0px auto; text-align: left;}");
  client.println("form  { display: table;      }");
  client.println(" p    { display: table-row;  }");
  client.println("label { display: table-cell; }");
  client.println("input { display: table-cell; }");
  client.println(".button { background-color: DarkGrey; border: 2px; margin: 2px;padding: 5px; width: 320px ;color: white; text-align: center;text-decoration: none; font-size: 20px; cursor: pointer;}");
  client.println("type=\"text/css\" #btn_submit{width:120px;} #formbox {width:320px; margin:auto 0; text-align: left;}#formbox2 {width:320px; margin:auto 0; text-align: center;}");
  client.println("</style>");
  client.println("</head>");
  client.println("<body>");
  client.println("<form action=\"/settingsServer.php\" method=\"get\">");
  // ACCESPOINT
  client.println("<div id=\"formbox2\">");
  client.println("<h1>Server</h1>");
  client.println("</div>");

  client.println("<div id=\"formbox\">");
  client.println("<p><label for=\"intervalServer\">Refresh rate (s): </label>");
  client.println("<input type=\"number\"  name=\"intervalServer\" min=\"2\" max=\"30\"  value=" + String(myServer.refreshRate) + "><br><br></p>");

  client.println("<p><label for=\"S1\">Sensor Toggle:</label>");
  client.println("<select name=\"sensorToggle\" >");
  if (myServer.sensorToggle) {
    client.println("  <option value=\"1\" selected>Yes</option>");
    client.println("  <option value=\"0\" >No</option>");
  } else {
    client.println("  <option value=\"1\" >Yes</option>");
    client.println("  <option value=\"0\" selected>No</option>");
  }
  client.println("</select><br><br></p>"); 

  client.println("</div>");
  client.println("<div id=\"formbox2\">");
  client.println("<input type=\"submit\" value=\"Change\">");
  client.println("</div>");
  client.println("<hr>");
  // WIFI
  client.println("<div id=\"formbox2\">");
  client.println("<h1>WIFI</h1>");
  client.println("</div>");
  client.println("<div id=\"formbox\">");
  client.println("<p><label for=\"S1\">Connect:</label>");
  client.println("<select name=\"connectWIFI\" >");
  if (myServer.connectToWIFI) {
    client.println("  <option value=\"1\" selected>Yes</option>");
    client.println("  <option value=\"0\" >No</option>");
  } else {
    client.println("  <option value=\"1\" >Yes</option>");
    client.println("  <option value=\"0\" selected>No</option>");
  }
  client.println("</select><br><br></p>");
  client.println("</div>");
  client.println("<div id=\"formbox2\">");
  client.println("<input type=\"submit\" value=\"Change\">");
  client.println("</div>");
  client.println("<hr>");
  // MQTT
  client.println("<div id=\"formbox2\">");
  client.println("<h1>MQTT</h1>");
  client.println("</div>");
  client.println("<div id=\"formbox\">");
  client.println("<p><label for=\"connectMQTT\">Connect:</label>");
  client.println("<select name=\"connectMQTT\" >");
  if (myServer.connectToMQTT) {
    client.println("  <option value=\"1\" selected>Yes</option>");
    client.println("  <option value=\"0\" >No</option>");
  } else {
    client.println("  <option value=\"1\" >Yes</option>");
    client.println("  <option value=\"0\" selected>No</option>");
  }
  client.println("</select><br><br></p>");
  client.println("<p><label for=\"serverMQTT\">Server: </label>");
  client.println("<input type=\"text\" name=\"serverMQTT\" value=" + String(myServer.serverMQTT) + "><br><br></p>");
  client.println("<p><label for=\"portMQTT\">Port: </label>");
  client.println("<input type=\"number\" name=\"portMQTT\" value=" + String(myServer.port) + "><br><br></p>");
  client.println("<p><label for=\"topicMQTT\">Topic: </label>");
  client.println("<input type=\"text\" name=\"topicMQTT\" value=" + String(myServer.topicPublish) + "><br><br></p>");
  client.println("</div>");
  client.println("<div id=\"formbox2\">");
  client.println("<input type=\"submit\" value=\"Change\"   >");
  client.println("</div>");
  client.println("<hr>");
  // Time
  client.println("<div id=\"formbox2\">");
  client.println("<h1>Time request URL</h1>");
  client.println("</div>");
  client.println("<div id=\"formbox\">");
  client.println("<p><label for=\"timeRequestURL\">URL: </label>");
  client.println("<input type=\"text\" name=\"timeRequestURL\" value=" + String(myServer.timeRequestURL) + "><br><br></p>");
  client.println("</div>");
  client.println("<div id=\"formbox2\">");
  client.println("<input type=\"submit\" value=\"Change\"   >");
  client.println("</div>");
  client.println("<hr>");
  client.println("</form>");
  client.println("<br>");
  client.println("<p><a href=\"main\"><button class=\"button\">BACK</button></a></p>");
  client.println("</body>");
  client.println("</html>");
  client.println();
}
