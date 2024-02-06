
void writeMyServerFile(){
    Serial.println("saving config");
    JsonObject obj = doc.to<JsonObject>();
    doc["refreshRate"] = myServer.refreshRate;
    doc["sensorToggle"] = myServer.sensorToggle;
    doc["connectToWIFI"] = myServer.connectToWIFI;
    doc["connectToMQTT"] = myServer.connectToMQTT;
    doc["serverMQTT"] = myServer.serverMQTT;
    doc["mqtt_port"] = myServer.port;
    //doc["mqtt_user"] = myServer.username;
    //doc["mqtt_pass"] = myServer.passwordMQTT;
    doc["mqtt_topic"] = myServer.topicPublish;
    doc["timeRequestURL"] = myServer.timeRequestURL;
    serializeJson(doc, myPayload);
    Serial.println(myPayload);
    String fileName = "/myServer.json";
    Serial.print("write ");
    Serial.println(fileName);
    File file = SPIFFS.open(fileName, "w");
    file.print(myPayload);
    file.close();
    doc.clear();
}

void readMyServerFile()
{
    int lenght = 0;
    String fileName = "/myServer.json";
    if (SPIFFS.exists(fileName))
    {
       File file = SPIFFS.open(fileName, "r");
      if (file) {
        while(file.available()){
            myPayload[lenght] = file.read();
            lenght++;
        }
        myPayload[lenght] ='\0';
        Serial.print("myServer.json : ");
        Serial.println (myPayload); //use for debug
      }
    }
    DeserializationError error = deserializeJson(doc, myPayload,lenght);
    if (error) {
        Serial.print(F("deserializeJson() failed: "));
        Serial.println(error.f_str());
    } else {
        myServer.refreshRate= doc["refreshRate"]  ;
        myServer.sensorToggle= doc["sensorToggle"];
        myServer.connectToWIFI = doc["connectToWIFI"]  ;
        myServer.connectToMQTT = doc["connectToMQTT"] ;
        strcpy(myServer.serverMQTT,doc["serverMQTT"]| "default");
        myServer.port = doc["mqtt_port"];
        strcpy(myServer.topicPublish,doc["mqtt_topic"]| "garden/publish2");
        strcpy(myServer.timeRequestURL,doc["timeRequestURL"] |"default");
    }
    setupTemperature();
}


void reWriteMyServerFile()
{
    //myServer = (connection) { 5, 0,1, 1,  "192.168.0.121", 1883, "garden/publish2","be.pool.ntp.org"};
    myServer = (connection) { 15, 0,1, 1,  "192.168.0.121", 1883, "garden/publish2","be.pool.ntp.org"};
    writeMyServerFile();
}

void writeMySprinklerFile(){
    Serial.println("saving Spinkler File");
    JsonObject obj = doc.to<JsonObject>();
    doc["modus"]=mySprinkler.modus;
    doc["pumpTime"]=mySprinkler.pumpTime;
    doc["pauzeTime"]=mySprinkler.pauzeTime;
    for (int i =0; i<9;i++ ){
        doc["valveName"][i] = mySprinkler.valve[i].valveName;
        doc["percentage"][i] = mySprinkler.valve[i].percentage;
        doc["with Pump"][i] = mySprinkler.valve[i].withPump;
    }
    serializeJson(doc, myPayload);
    Serial.println(myPayload);
    String fileName = "/mySprinkler.json";
    Serial.print("write ");
    Serial.println(fileName);
    File file = SPIFFS.open(fileName, "w");
    file.print(myPayload);
    file.close();
    doc.clear();
}


void reWriteMySprinklerFile()
{
    mySprinkler.modus = 0;
    mySprinkler.pumpTime = 15;
    mySprinkler.pauzeTime = 15;
    mySprinkler.valve[0] = (valveSpec){"         ", 100, 0};
    mySprinkler.valve[1] = (valveSpec){"ZIJKANT F", 100, 1}; //1
    mySprinkler.valve[2] = (valveSpec){"VOORAAN  ", 100, 1}; //2
    mySprinkler.valve[3] = (valveSpec){"ACHTERAAN", 100, 1}; //3
    mySprinkler.valve[4] = (valveSpec){"GROENTEN ", 100, 1}; //4
    mySprinkler.valve[5] = (valveSpec){"ZIJKANT M", 100, 1}; //5
    mySprinkler.valve[6] = (valveSpec){"VRIJ 1   ", 100, 1};
    mySprinkler.valve[7] = (valveSpec){"VRIJ 2   ", 100, 1};
    mySprinkler.valve[8] = (valveSpec){"POMPEN   ", 100, 1};
    writeMySprinklerFile();
}

void readMySpriklerFile(){
    int lenght = 0;
    String fileName = "/mySprinkler.json";
    if (SPIFFS.exists(fileName))
    {
       File file = SPIFFS.open(fileName, "r");
      if (file) {
        while(file.available()){
            myPayload[lenght] = file.read();
            lenght++;
        }
        myPayload[lenght] ='\0';
        Serial.print("mySprinkler.json : ");
        Serial.println (myPayload); //use for debug
      }
    }
    DeserializationError error = deserializeJson(doc, myPayload,lenght);
    if (error) {
        Serial.print(F("deserializeJson() failed: "));
        Serial.println(error.f_str());
    } else {
        mySprinkler.modus=doc["modus"];
        mySprinkler.pumpTime=doc["pumpTime"];
        mySprinkler.pauzeTime=doc["pauzeTime"];
        for (int i =0; i<9;i++ ){
             strcpy(mySprinkler.valve[i].valveName,doc["valveName"][i]| "sprinkler");
             mySprinkler.valve[i].percentage=doc["percentage"][i];
             mySprinkler.valve[i].withPump=doc["with Pump"][i];
        } 
 
    }
}


void readProgramFile(){
    for (int program = 1; program < 5; program++)
    {
        int lenght = 0;
        String fileName = "/program" + String(program) + ".json";
        if (SPIFFS.exists(fileName))
        {
            File file = SPIFFS.open(fileName, "r");
            if (file) {
                while(file.available()){
                    myPayload[lenght] = file.read();
                    lenght++;
                }
                myPayload[lenght] ='\0';
                Serial.print(fileName);
                Serial.print(" : ");
                Serial.println (myPayload); //use for debug
            }
        }
        DeserializationError error = deserializeJson(doc, myPayload,lenght);
        if (error) {
            Serial.print(F("deserializeJson() failed: "));
            Serial.println(error.f_str());
        } else {
            SprinklerProgram[program].actief=doc["actief"] ;
            SprinklerProgram[program].uurStart=doc["uurStart"] ;
            SprinklerProgram[program].minStart=doc["minStart"] ;
            for (int i =0; i<7;i++ ){
                SprinklerProgram[program].dayToRun[i]=doc["dayToRun"][i] ;
            }
            for (int i =0; i<16;i++ ){
                SprinklerProgram[program].valve[i]=doc["valve"][i] ;
                SprinklerProgram[program].valveTime[i]=doc["valveTime"][i] ;
            }   
        }
    }
}

void writeProgramFile(int programma){
    Serial.println("programs");
    JsonObject obj = doc.to<JsonObject>();
    doc["actief"] = SprinklerProgram[programma].actief;
    doc["uurStart"] = SprinklerProgram[programma].uurStart;
    doc["minStart"] = SprinklerProgram[programma].minStart;
    for (int i =0; i<7;i++ ){
        doc["dayToRun"][i] = SprinklerProgram[programma].dayToRun[i];
    }
    for (int i =0; i<16;i++ ){
        doc["valve"][i] = SprinklerProgram[programma].valve[i];
        doc["valveTime"][i] = SprinklerProgram[programma].valveTime[i];
    }
    serializeJson(doc, myPayload);
    Serial.println(myPayload);
    String fileName = "/program" + String(programma) + ".json";
    Serial.print("write ");
    Serial.println(fileName);
    File file = SPIFFS.open(fileName, "w");
    file.print(myPayload);
    file.close();
    doc.clear();
}

void reWriteProgramFile(int programma)
{
    switch (programma)
    {
    case 1:
        SprinklerProgram[1] = (sprinklerProgram){
            1, 20, 30, {1, 1, 1, 1, 1, 1, 1}, {2, 1, 5, 4, 5, 1, 4, 3, 0, 0, 0, 0, 0, 0, 0, 0}, {20, 6, 10, 6, 10, 6, 6, 20, 0, 0, 0, 0, 0, 0, 0, 0}};
        break;
    case 2:
        SprinklerProgram[2] = (sprinklerProgram){
            0, 12, 0, {1, 1, 1, 1, 1, 1, 1}, {1, 2, 3, 4, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {10, 15, 10, 15, 10, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}};
        break;
    case 3:
        SprinklerProgram[3] = (sprinklerProgram){
            0, 13, 15, {0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}};
        break;
    case 4:
        SprinklerProgram[4] = (sprinklerProgram){
            0, 15, 46, {0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}};
        break;
    }
    writeProgramFile(programma);
}


void writeRelaisSpecFile(){
    Serial.println("Relais settings");
    JsonObject obj = doc.to<JsonObject>();
    for (int i =0; i<8;i++ ){
        doc["relaisName"][i] = relais[i].relaisName;
        doc["actief"][i]=relais[i].actief;
        doc["control"][i]=relais[i].control;
        doc["data1"][i]=relais[i].data1;
        doc["data2"][i]=relais[i].data2;
        doc["data3"][i]=relais[i].data3;
        doc["data4"][i]=relais[i].data4;
    }
    serializeJson(doc, myPayload);
    Serial.println(myPayload);
    String fileName = "/relaissettings.json";
    Serial.print("write ");
    Serial.println(fileName);
    File file = SPIFFS.open(fileName, "w");
    file.print(myPayload);
    file.close();
    doc.clear();
}


void reWriteRelaisSpecFile(){
    relais[0]= (relaisSpec){"Vijgen",0,1,3,23,55,0,0};
    relais[1]= (relaisSpec){"Appel",0,1,3,23,55,0,0};
    relais[2]= (relaisSpec){"Serre",0,1,6,23,0,0,0};
    relais[3]= (relaisSpec){"Relais 4",0,1,1,9,55,22,32};
    relais[4]= (relaisSpec){"Relais 5",0,1,2,11,35,0,0};
    relais[5]= (relaisSpec){"Relais 6",0,1,2,19,22,0};
    relais[6]= (relaisSpec){"Dakraam",0,1,0,20,6,0,0};
    relais[7]= (relaisSpec){"Dakraam",0,1,0,20,6,0,0};
    writeRelaisSpecFile();
    // naam, state, actief, control, data1, data2, data3, data 4
    //programTable[] = {"none","time", "sunrise", "sunset","day","night", "temp"};
}

void readRelaisSpecFile(){
    int lenght = 0;
    String fileName = "/relaissettings.json";
    if (SPIFFS.exists(fileName))
    {
       File file = SPIFFS.open(fileName, "r");
      if (file) {
        while(file.available()){
            myPayload[lenght] = file.read();
            lenght++;
        }
        myPayload[lenght] ='\0';
        Serial.print("relaissettings.json : ");
        Serial.println (myPayload); //use for debug
      }
    }
    DeserializationError error = deserializeJson(doc, myPayload,lenght);
    if (error) {
        Serial.print(F("deserializeJson() failed: "));
        Serial.println(error.f_str());
    } else {
        for (int i =0; i<8;i++ ){
            strcpy(relais[i].relaisName, doc["relaisName"][i]| "default");
            relais[i].actief=doc["actief"][i];
            relais[i].control=doc["control"][i];
            relais[i].data1=doc["data1"][i];
            relais[i].data2=doc["data2"][i];
            relais[i].data3=doc["data3"][i];
            relais[i].data4=doc["data4"][i];
        }
    }          
}     


void listFilesInDir(File dir, int numTabs)
{
    while (true)
    {

        File entry = dir.openNextFile();
        if (!entry)
        {
            // no more files in the folder
            break;
        }
        for (uint8_t i = 0; i < numTabs; i++)
        {
            Serial.print('\t');
        }
        Serial.print(entry.name());
        if (entry.isDirectory())
        {
            Serial.println("/");
            listFilesInDir(entry, numTabs + 1);
        }
        else
        {
            // display zise for file, nothing for directory
            Serial.print("\t\t");
            Serial.println(entry.size(), DEC);
        }
        entry.close();
    }
}

void checkFileSystem()
{
    if (!SPIFFS.begin(true))
    {
        Serial.println("An Error has occurred while mounting SPIFFS");
        return;
    }
    Serial.println(F("Inizializing FS..."));
    if (SPIFFS.begin())
    {
        Serial.println(F("SPIFFS mounted correctly."));
    }
    else
    {
        Serial.println(F("!An error occurred during SPIFFS mounting"));
    }

    // Get all information of SPIFFS

    unsigned int totalBytes = SPIFFS.totalBytes();
    unsigned int usedBytes = SPIFFS.usedBytes();

    Serial.println("===== File system info =====");

    Serial.print("Total space:      ");
    Serial.print(totalBytes);
    Serial.println("byte");

    Serial.print("Total space used: ");
    Serial.print(usedBytes);
    Serial.println("byte");

    Serial.println();

    // Open dir folder
    File dir = SPIFFS.open("/");
    // List file at root
    listFilesInDir(dir, 1);
}





void loadDataFromFile()
{
    checkFileSystem();
    //reWriteRelaisProgramFile();
    //reWriteRelaisSpecFile();
    //reWriteMySprinklerFile();
    //reWriteMyServerFile();
    //for (int i = 1; i < 5; i++){
    //   reWriteProgramFile(i); 
    //} 
    readMyServerFile();
    readProgramFile();
    readMySpriklerFile(); 
    readRelaisSpecFile();
}
