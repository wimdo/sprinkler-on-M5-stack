
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
    //myServer = (connection) { 5, 0,1, 1,  "192.168.0.121", 1883, "garden/publish2","http://worldtimeapi.org/api/timezone/Europe/Brussels"};
    myServer = (connection) { 5, 0,0, 0,  "192.168.0.121", 1883, "garden/publish2","http://worldtimeapi.org/api/timezone/Europe/Brussels"};
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
    mySprinkler.valve[0] = (valveSpec){
        "         ", 100, 0};

    mySprinkler.valve[1] = (valveSpec){
        "ZIJKANT F", 100, 1}; //1

    mySprinkler.valve[2] = (valveSpec){
        "VOORAAN  ", 100, 1}; //2

    mySprinkler.valve[3] = (valveSpec){
        "ACHTERAAN", 100, 1}; //3

    mySprinkler.valve[4] = (valveSpec){
        "GROENTEN ", 100, 1}; //4

    mySprinkler.valve[5] = (valveSpec){
        "ZIJKANT M", 100, 1}; //5

    mySprinkler.valve[6] = (valveSpec){
        "VRIJ 1   ", 100, 1};

    mySprinkler.valve[7] = (valveSpec){
        "VRIJ 2   ", 100, 1};

    mySprinkler.valve[8] = (valveSpec){
        "POMPEN   ", 100, 1};
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
        sprinklerName_table = (char**)malloc(9*sizeof(char*));
        for (int i = 0; i < 9; i++){
        sprinklerName_table[i] = (char*)malloc(strlen(&mySprinkler.valve[i].valveName[0])+1);
        strncpy(sprinklerName_table[i], &mySprinkler.valve[i].valveName[0], strlen(&mySprinkler.valve[i].valveName[0])+1);
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


void readRelaisProgramFile(){
    int lenght = 0;
    String fileName = "/relaisprogram.json";
    if (SPIFFS.exists(fileName))
    {
       File file = SPIFFS.open(fileName, "r");
      if (file) {
        while(file.available()){
            myPayload[lenght] = file.read();
            lenght++;
        }
        myPayload[lenght] ='\0';
        Serial.print("relaisprogram.json : ");
        Serial.println (myPayload); //use for debug
      }
    }
    DeserializationError error = deserializeJson(doc, myPayload,lenght);
    if (error) {
        Serial.print(F("deserializeJson() failed: "));
        Serial.println(error.f_str());
    } else {
        for (int i =0; i<16;i++ ){
            relaisProgram[i].actief=doc["actief"][i];
            relaisProgram[i].relais=doc["relais"][i];
            strcpy(relaisProgram[i].control,doc["control"][i]| "default");
            relaisProgram[i].state=doc["state"][i];
            relaisProgram[i].data1=doc["data1"][i];
            relaisProgram[i].data2=doc["data2"][i];
            relaisProgram[i].data3=doc["data3"][i];
            relaisProgram[i].data4=doc["data4"][i];
        } 
    }         
}


void writeRelaisProgramFile(){
    Serial.println("programs");
    JsonObject obj = doc.to<JsonObject>();
    for (int i =0; i<16;i++ ){
        doc["actief"][i]=relaisProgram[i].actief;
        doc["relais"][i]=relaisProgram[i].relais;
        doc["control"][i]=relaisProgram[i].control;
        doc["state"][i]=relaisProgram[i].state;
        doc["data1"][i]=relaisProgram[i].data1;
        doc["data2"][i]=relaisProgram[i].data2;
        doc["data3"][i]=relaisProgram[i].data3;
        doc["data4"][i]=relaisProgram[i].data4;
    }
    serializeJson(doc, myPayload);
    Serial.println(myPayload);
    String fileName = "/relaisprogram.json";
    Serial.print("write ");
    Serial.println(fileName);
    File file = SPIFFS.open(fileName, "w");
    file.print(myPayload);
    file.close();
    doc.clear();
}


void reWriteRelaisProgramFile(){
    relaisProgram[0]= (relaisProg){1,1,"sunset",1,23,55,0,0};
    relaisProgram[1]= (relaisProg){1,2,"sunset",1,23,55,0,0};
    relaisProgram[2]= (relaisProg){0,3,"time",0,18,0,23,55};
    relaisProgram[3]= (relaisProg){0,4,"time",0,14,0,20,0};
    relaisProgram[4]= (relaisProg){0,5,"sunset",0,8,0,0,0};
    relaisProgram[5]= (relaisProg){0,6,"night",0,0,0,0,0};
    relaisProgram[6]= (relaisProg){0,7,"day",0,0,0,0,0};
    relaisProgram[7]= (relaisProg){1,3,"temp",1,25,0,0,0};
    relaisProgram[8]= (relaisProg){0,1,"default",1,0,0,0,0};
    relaisProgram[9]= (relaisProg){0,1,"default",1,0,0,0,0};
    relaisProgram[10]= (relaisProg){0,1,"default",1,0,0,0,0};
    relaisProgram[11]= (relaisProg){0,1,"default",1,0,0,0,0};
    relaisProgram[12]= (relaisProg){0,1,"default",1,0,0,0,0};
    relaisProgram[13]= (relaisProg){0,1,"default",1,0,0,0,0};
    relaisProgram[14]= (relaisProg){0,1,"default",1,0,0,0,0};
    relaisProgram[15]= (relaisProg){0,1,"default",1,0,0,0,0};
    writeRelaisProgramFile(); 
}


void writeRelaisSpecFile(){

}

void reWriteRelaisSpecFile(){
    relais[0]= (relaisSpec){"Vijgen  ",0,0};
    relais[1]= (relaisSpec){"Appel   ",0,0};
    relais[2]= (relaisSpec){"Serre   ",0,0};
    relais[3]= (relaisSpec){"Relais 4",0,0};
    relais[4]= (relaisSpec){"Relais 5",0,0};
    relais[5]= (relaisSpec){"Relais 6",0,0};
    relais[6]= (relaisSpec){"Relais 7",0,0};
    relais[7]= (relaisSpec){"Relais 8",0,0};
    writeRelaisSpecFile();
}

void readReleaisSpecFile(){
    reWriteRelaisSpecFile();
    relaisName_table = (char**)malloc(9*sizeof(char*));
    for (int i = 0; i < 9; i++){
        relaisName_table[i] = (char*)malloc(strlen(&relais[i].relaisName[0])+1);
        strncpy(relaisName_table[i], &relais[i].relaisName[0], strlen(&relais[i].relaisName[0])+1);
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
    reWriteRelaisProgramFile();
    reWriteMySprinklerFile();
    reWriteMyServerFile();
    
    for (int i = 1; i < 5; i++){
       reWriteProgramFile(i); 
    } 
      
    readMyServerFile();
    readProgramFile();
    readMySpriklerFile(); 
    readReleaisSpecFile();
    readRelaisProgramFile();
  }