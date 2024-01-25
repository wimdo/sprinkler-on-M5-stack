#include <M5Tough.h>
#include "Core2_Sounds.h"


#include <WebServer.h>
#include "SPIFFS.h"
#include <WiFi.h>
#include <HTTPClient.h>
#include <PubSubClient.h>
#include <WiFiManager.h>
#include "OTA.h"
#include "time.h"
#include <OneWire.h>
#include <DallasTemperature.h>
#include <ArduinoJson.h>
#include <sunset.h>


#define VERSION "Version 23/01/24"  
#define nameprefix "HOME"
#define moduletype "SPRINKLER"

#define LATITUDE        51.17554
#define LONGITUDE        2.29546
#define DST_OFFSET      1

const uint8_t wifi_OK[] PROGMEM = {
    0xF0, 0x00, 0x08, 0x00, 0xE4, 0x00, 0x12, 0x00, 0xCA, 0x00, 0x2A, 0x00};
const uint8_t wifi_NOK[] PROGMEM = {
    0xF0, 0x40, 0x08, 0x40, 0xE4, 0x40, 0x12, 0x40, 0xCA, 0x00, 0x2A, 0x40};

TFT_eSprite spr = TFT_eSprite(&M5.Lcd);
TFT_eSprite box = TFT_eSprite(&M5.Lcd);

WiFiClient client;
WiFiClient clientMQTT;
WiFiServer server(80); // Set web server port number to 80
String HTTPrequest;    // Variable to store the HTTP request
unsigned long currentTime = millis();
unsigned long previousTime = 0;
const long timeoutTime = 2000;
char fullhostname[20];
IPAddress myIP ;

#define commandPrefix "cmnd"
#define dataPrefix "data"
#define infoPrefix "info"
char myPayload[1024];
char myTopic[64];
String myPayloadString = ""; // a string for incoming text
PubSubClient clientEsp(clientMQTT);

StaticJsonDocument<2048> doc;

RTC_TimeTypeDef RTCtime;
RTC_DateTypeDef RTCDate;

SunSet sun;

#define ONE_WIRE_BUS 26
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature temperatuur(&oneWire);

#define RelayI2C 0x39
#define SprinklerI2C 0x20
//#define RelayI2C 0x38
//#define SprinklerI2C 0x38

// sprinkler states
#define modeAutorun 1
#define modeManueel 0
#define Wacht 1
#define Start 2
#define Loopt 3
#define Pauze 4
#define End 5

// program defined buttons
#define buttonNone -1
#define button1 4
#define button2 1
#define button3 3
#define button4 2

#define ON 1
#define OFF 0
typedef struct
{
  int lastSecond;
  int previousMinute;
  boolean checkSecond;
} clockInfo;
static clockInfo clockData;

typedef struct
{
  int staat;
  int runtimeLeft;
  int runtimeValveLeft;
  int runtimeTotal;
  int valveTimeCounter;
  int programCounter;
  int valveSelected = 0;
  int sliderStateValve = 0;
  int sliderStateRelais = 0;
  int hourNext;
  int minuteNext;
  int sunrise;
  int sunset;
  boolean pumpOn;
  boolean valveOn;
  boolean timeSetByNTP;
  boolean keyboardInput;
  int keyboardHold = 0;
  int buttonPressed;
  boolean debugMode;
  boolean updateSprinklerSlider;
  boolean updateRelaisSlider;
  boolean sendData;
} sprinklerData;
sprinklerData sprinkler; // Spec

typedef struct
{
  boolean present;
  uint8_t deviceAddress[8];
  char deviceAddressName[20];
  float value;
  float valueMin = 100;
  float valueMax = -100;
} temp;
temp temperature[2];

typedef struct
{
  char valveName[10]; //?? 09/08 voor 9 bytes gaat het niet
  int percentage;
  boolean withPump;
} valveSpec;

typedef struct
{
  int modus;
  int pumpTime;
  int pauzeTime;
  valveSpec valve[9];    
} sprinklerSpec;
sprinklerSpec mySprinkler;

typedef struct
{
  boolean actief;
  int uurStart;
  int minStart;
  boolean dayToRun[7];
  int valve[16];
  int valveTime[16];
} sprinklerProgram;
sprinklerProgram SprinklerProgram[5];
sprinklerProgram SprinklerProgramRun;
/*
typedef struct
{
  boolean actief;
  int relais;
  char control[14]; // enum maken
  boolean state;
  int data1;
  int data2;
  int data3;
  int data4;
} relaisProg;
relaisProg relaisProgram[16];
*/

typedef struct
{
  char relaisName[10]; //?? 09/08 voor 9 bytes gaat het niet
  //boolean stateAtStart;
  boolean state;
  boolean actief;
  int control;
  int data1;
  int data2;
  int data3;
  int data4;
} relaisSpec;
relaisSpec relais[8];

struct connection
{
  int refreshRate;
  boolean sensorToggle;
  boolean connectToWIFI;
  boolean connectToMQTT;
  char serverMQTT[32];
  int port;
  char topicPublish[32];
  char timeRequestURL[80];
};
struct connection myServer;

typedef struct
{
  int x;
  int y;
  int widht;
  int height;
  int backgroundColor;
  int textColor;
  String text;
} touchButton;

Button sprinklerZone (0,19+68,118,68+68);
Button relaisZone (120,19+68,120,68+68);
Button b1(0,290,119,30);
Button b2(120,290,120,30);

char *auto_table[] = {"MANUEEL", "AUTO"};
char *onoff_table[] = {"UIT", "AAN"};
char *janee_table[] = {"NEE", "JA"};
char *program_Table[] = {"none","time", "sunrise", "sunset","day","night", "temp on"};
char *dayOfWeek[] = {"Zo", "Ma", "Di", "Wo", "Do", "Vr", "Za"};
int daysInMonth[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

#include "display.h"
#include "keyboardTime.h"
#include "dropdown.h"
#include "valves.h"
#include "temperature.h"
#include "server_pages.h"
#include "file_handling.h"
#include "MQTT_handling.h"
#include "WIFI_handling.h"
#include "HTTP_handling.h"
#include "MENU_handling.h"
#include "speaker.h"

hw_timer_t *systemTimer = NULL;

void IRAM_ATTR timeCheck()
{
  clockData.checkSecond = true;
}

void setup()
{
  M5.begin(true, false, true, true); 
  //I2Cscan();
  //sprinkler.debugMode = true;
  sprinkler.staat = Wacht;
  sprinkler.keyboardInput = true;
  systemTimer = timerBegin(0, 80, true); // true = count up
  timerAlarmWrite(systemTimer, 1000000, true);
  timerAlarmEnable(systemTimer);
  timerAttachInterrupt(systemTimer, &timeCheck, true);
  SpeakerInit();
  loadDataFromFile();
  setupDisplay();
  outlineMainscreen();  
  setupValves();
  showSprinklerSlider();
  disableRelais();
  showRelaySlider();
  //WiFi.begin("57_home", "wonderfulcurtain962");
  setupHostName();
  myServer.connectToWIFI = false;
  connectWithWiFi();
  calculateSolarTime();
  setupTemperature();
  sprinkler.keyboardInput = false;
  Serial.printf("SYSTEM : /s\n",VERSION);
  M5.Rtc.GetTime(&RTCtime); 
  startRelaisProgram();
  controleerProgramma(RTCtime.Hours, RTCtime.Minutes, RTCDate.WeekDay);
}

void screenTouchCheck()
{
  if (sprinkler.keyboardHold==0){
    M5.update();
    if (b1.wasPressed()) {
      soundsBeep(1000, 100, 1);
      sprinkler.keyboardInput = true;
      mainMenu(0,19,240,320);
      outlineMainscreen();
      sprinkler.updateRelaisSlider = true;
      sprinkler.updateSprinklerSlider = true;
      sprinkler.keyboardInput = false;
    } else if (b2.wasPressed()) {
        if (sprinkler.staat == Wacht){
        soundsBeep(1000, 100, 1);
        sprinkler.keyboardInput = true;
        settingsMenu(0,19,240,320);
        outlineMainscreen();
        sprinkler.updateRelaisSlider = true;
        sprinkler.updateSprinklerSlider = true;
        sprinkler.keyboardInput = false;
        } else {
          sprinklerStop();
        }
    } else if (sprinklerZone.wasPressed()) {
      soundsBeep(1000, 100, 1);
      sprinkler.keyboardInput = true;
      sprinkler.buttonPressed = sprinklerSelectie(0,19,240,320);
      outlineMainscreen();
      sprinkler.updateRelaisSlider = true;
      sprinkler.updateSprinklerSlider = true;
      sprinkler.keyboardInput = false;
    } else if (relaisZone.wasPressed()) {
      soundsBeep(1000, 100, 1);
      sprinkler.keyboardInput = true;
      sprinkler.buttonPressed = relaisSelectie(0,19,240,320);
      outlineMainscreen();
      sprinkler.updateRelaisSlider = true;
      sprinkler.updateSprinklerSlider = true;
      sprinkler.keyboardInput = false;
    }  
  }
}

void loop()
{
  screenTouchCheck();
  if (clockData.checkSecond)
  {
    readTime();
    if ( RTCtime.Minutes != clockData.previousMinute )
    {
      controleerProgramma(RTCtime.Hours, RTCtime.Minutes, RTCDate.WeekDay); //controleerProgramma(now.hour(),now.minute(),now.dayOfTheWeek());
      checkRelaisSettingsOnTime(RTCtime.Hours, RTCtime.Minutes);
      //checkRelaisSettingsTempOnTime();
      if (RTCtime.Minutes == 0 && RTCtime.Hours == 0)
      { 
        disableRelais();
        setupTime();
        calculateSolarTime();
        if (RTCtime.Hours == 0){
          setupTemperature(); // reset min en max waarde van de
        }
      }
        clockData.previousMinute = RTCtime.Minutes;
        if (sprinkler.staat == Wacht)
        {
          if (WiFi.status() != WL_CONNECTED){
            if (clockData.previousMinute%5 == 0){
              connectWithWiFi();
            }
          } else {
            sprinkler.sendData= true;
          }
        } else {
          sprinkler.sendData= true;
        }   
      } 
      checkSprinklerStatus();
      if (sprinkler.sendData) {
        sendData();
        sprinkler.sendData= false;
      } 
      readTemperature();
      checkRelaisSettingsTemp();  
  }
  if (myServer.connectToWIFI){
    if (WiFi.status() == WL_CONNECTED){
      connectMQTT();
      clientEsp.loop();
    }
  }
  client = server.available();
  if (client)
  {
    Serial.println("check client");
    HTPPcheckClient();
  }
}
