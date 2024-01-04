#include <M5Tough.h>
#include "Core2_Sounds.h"

//#include <Arduino.h>
//#include "Wire.h"
//#include <SPI.h>

//lijn toegevoegd voor controle github
//lijn toegevoegd voor controle github
//lijn 3 toegevoegd voor controle github
#define VERSION "Version 05/06/23"  
#define nameprefix "HOME"
#define moduletype "SPRINKLER"

#include <WebServer.h>
#include "SPIFFS.h"
#include <WiFi.h>
#include <HTTPClient.h>
#include <PubSubClient.h>
#include "time.h"
#include <OneWire.h>
#include <DallasTemperature.h>
#include <WiFiManager.h>
#include "OTA.h"
#include <ArduinoJson.h>
#include <sunset.h>
SunSet sun;
#define LATITUDE        51.17554
#define LONGITUDE        2.29546
#define DST_OFFSET      1

const uint8_t wifi_OK[] PROGMEM = {
    0xF0, 0x00, 0x08, 0x00, 0xE4, 0x00, 0x12, 0x00, 0xCA, 0x00, 0x2A, 0x00};
const uint8_t wifi_NOK[] PROGMEM = {
    0xF0, 0x40, 0x08, 0x40, 0xE4, 0x40, 0x12, 0x40, 0xCA, 0x00, 0x2A, 0x40};

WiFiClient client;
WiFiClient clientMQTT;
WiFiServer server(80); // Set web server port number to 80
String HTTPrequest;    // Variable to store the HTTP request
unsigned long currentTime = millis();
unsigned long previousTime = 0;
const long timeoutTime = 2000;
char fullhostname[40];
IPAddress myIP ;



PubSubClient clientEsp(clientMQTT);
char myPayload[1024];
char myTopic[64];
String myPayloadString = ""; // a string for incoming text
char serialArray[50];

StaticJsonDocument<2048> doc;

RTC_TimeTypeDef RTCtime;
RTC_DateTypeDef RTCDate;

#define ONE_WIRE_BUS 26
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature temperatuur(&oneWire);

#define commandPrefix "cmnd"
#define dataPrefix "data"
#define infoPrefix "info"

#define RelayI2C 0x39
#define SprinklerI2C 0x20

//#define RelayI2C 0x38
//#define SprinklerI2C 0x38

#define modeAutorun 1
#define modeManueel 0

#define Wacht 1
#define Start 2
#define Loopt 3
#define Pauze 4
#define End 5

#define btnTIMEOUT -1
// program defined buttons
#define buttonNone -1
#define buttonLeft 1
#define buttonDown 1
#define buttonCenter 2
#define buttonOK 2
#define buttonRight 3
#define buttonUp 3
#define buttonEscape 4

#define button1 4
#define button2 1
#define button3 3
#define button4 2

typedef struct
{
  int buttonPressed;

} buttonData;
static buttonData button;

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
  int sliderStateRelais =0;
  int hourNext;
  int minuteNext;
  int sunrise;
  int sunset;
  boolean pumpOn;
  boolean valveOn;
  boolean timeSetByNTP;
  boolean keyboardInput;
  int keyboardHold = 0;
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


typedef struct
{
  char relaisName[10]; //?? 09/08 voor 9 bytes gaat het niet
  boolean stateAtStart;
  boolean state;
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

//char *mainMenu_table[] = {"programma kiezen", "sprinkler kiezen", "relais kiezen", "programma wijzigen", "instellingen"};
char *mainMenu_table[] = {"programma kiezen", "sprinkler kiezen", "relais kiezen", "programma wijzigen","settings"};
char *settingsMenu_table[] = {"tijd wijzigen", "datum wijzigen", "sprinkler settings", "reset options"};
char *sprinklerSettings_table[] = {"Pomp tijd ", "Wacht tijd", "Modus     ", "Debug     ", "Valve info", "Wissel T  "};

char *auto_table[] = {"MANUEEL", "AUTO   "};
char *onoff_table[] = {"UIT", "AAN"};
char *janee_table[] = {"NEE", "JA "};

char *valveSettings_table[] = {"Naam", "werking", "Met Pomp"};
char *sprinklerManueel_table[] = {"Zone", "Tijd"};
char *programmaManueel_table[] = {"Programma"};
char *uurmanueel_table[] = {"Uur", "Min", "Sec"};
char *datummanueel_table[] = {"Jaar", "Maand", "Dag"};
char *dayOfWeek[] = {"Zo", "Ma", "Di", "Wo", "Do", "Vr", "Za"};
int daysInMonth[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

char data_table[9][10] = {"XXXXXXXXZ", "XXXXXXXXZ", "XXXXXXXXZ", "XXXXXXXXZ", "XXXXXXXXZ", "XXXXXXXXZ", "XXXXXXXXZ", "XXXXXXXXX", "XXXXXXXXZ"};
char **sprinklerName_table;
char **relaisName_table;

Button sprinklerZone (0,19+68,118,68+68);
Button relaisZone (120,19+68,120,68+68);
Button b1(0,290,60,30);
Button b2(60,290,60,30);
Button b3(120,290,60,30);
Button b4(180,290,60,30);

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

void IRAM_ATTR timeCheck()
{
  clockData.checkSecond = true;
}

 



hw_timer_t *systemTimer = NULL;


extern const unsigned char
    previewR[120264];  // Referring to external data (Dingdong audio files are
                       // stored inside).
                       // 引用外部数据（dingdong音频文件存储在内）

#define CONFIG_I2S_BCK_PIN     12  //定义I2S相关端口
#define CONFIG_I2S_LRCK_PIN    0
#define CONFIG_I2S_DATA_PIN    2
#define CONFIG_I2S_DATA_IN_PIN 34

#define Speak_I2S_NUMBER I2S_NUM_0  //定义扬声器端口

#define MODE_MIC  0  //定义工作模式
#define MODE_SPK  1
#define DATA_SIZE 1024

bool InitI2SSpeakOrMic(int mode) {  // Init I2S.  初始化I2S
    esp_err_t err = ESP_OK;

    i2s_driver_uninstall(
        Speak_I2S_NUMBER);  // Uninstall the I2S driver.  卸载I2S驱动
    i2s_config_t i2s_config = {
        .mode = (i2s_mode_t)(I2S_MODE_MASTER),  // Set the I2S operating mode.
                                                // 设置I2S工作模式
        .sample_rate = 44100,  // Set the I2S sampling rate.  设置I2S采样率
        .bits_per_sample =
            I2S_BITS_PER_SAMPLE_16BIT,  // Fixed 12-bit stereo MSB.
                                        // 固定为12位立体声MSB
        .channel_format =
            I2S_CHANNEL_FMT_ONLY_RIGHT,  // Set the channel format. 设置频道格式
#if (ESP_IDF_VERSION > ESP_IDF_VERSION_VAL(4, 1, 0))
        .communication_format =
            I2S_COMM_FORMAT_STAND_I2S,  // Set the format of the communication.
#else                                   // 设置通讯格式
        .communication_format = I2S_COMM_FORMAT_I2S,
#endif
        .intr_alloc_flags =
            ESP_INTR_FLAG_LEVEL1,  // Set the interrupt flag.  设置中断的标志
        .dma_buf_count = 2,        // DMA buffer count.  DMA缓冲区计数
        .dma_buf_len   = 128,      // DMA buffer length.  DMA缓冲区长度
    };
    if (mode == MODE_MIC) {
        i2s_config.mode =
            (i2s_mode_t)(I2S_MODE_MASTER | I2S_MODE_RX | I2S_MODE_PDM);
    } else {
        i2s_config.mode     = (i2s_mode_t)(I2S_MODE_MASTER | I2S_MODE_TX);
        i2s_config.use_apll = false;  // I2S clock setup.  I2S时钟设置
        i2s_config.tx_desc_auto_clear =
            true;  // Enables auto-cleanup descriptors for understreams.
                   // 开启欠流自动清除描述符
    }
    // Install and drive I2S.  安装并驱动I2S
    err += i2s_driver_install(Speak_I2S_NUMBER, &i2s_config, 0, NULL);

    i2s_pin_config_t tx_pin_config;

#if (ESP_IDF_VERSION > ESP_IDF_VERSION_VAL(4, 3, 0))
    tx_pin_config.mck_io_num = I2S_PIN_NO_CHANGE;
#endif

    tx_pin_config.bck_io_num =
        CONFIG_I2S_BCK_PIN;  // Link the BCK to the CONFIG_I2S_BCK_PIN pin.
                             // 将BCK链接至CONFIG_I2S_BCK_PIN引脚
    tx_pin_config.ws_io_num    = CONFIG_I2S_LRCK_PIN;     //          ...
    tx_pin_config.data_out_num = CONFIG_I2S_DATA_PIN;     //       ...
    tx_pin_config.data_in_num  = CONFIG_I2S_DATA_IN_PIN;  //      ...
    err +=
        i2s_set_pin(Speak_I2S_NUMBER,
                    &tx_pin_config);  // Set the I2S pin number. 设置I2S引脚编号
    err += i2s_set_clk(
        Speak_I2S_NUMBER, 44100, I2S_BITS_PER_SAMPLE_16BIT,
        I2S_CHANNEL_MONO);  // Set the clock and bitwidth used by I2S Rx and Tx.
                            // 设置I2S RX、Tx使用的时钟和位宽
    return true;
}


void SpeakInit(void) {          // 初始化扬声器
    M5.Axp.SetSpkEnable(true);  //启用扬声器电源
    InitI2SSpeakOrMic(MODE_SPK);
}





void setup()
{
  M5.begin(true, false, true, true); 
  I2Cscan();
  //sprinkler.debugMode = true;
  sprinkler.staat = Wacht;
  sprinkler.keyboardInput = true;
  systemTimer = timerBegin(0, 80, true); // true = count up
  timerAlarmWrite(systemTimer, 1000000, true);
  timerAlarmEnable(systemTimer);
  timerAttachInterrupt(systemTimer, &timeCheck, true);
  SpeakInit();
  loadDataFromFile();
  setupDisplay();
  outlineMainscreen();  
  setupValves();
  showSprinklerSlider();
  disableRelais();
  showRelaySlider();
  //WiFi.begin("57_home", "wonderfulcurtain962");
  setupWifiManager();
  calculateSolarTime();
  setupTemperature();
  sprinkler.keyboardInput = false;
  Serial.print(F("Ready :"));
  Serial.println(F(VERSION));
  startRelaisProgram();
  controleerProgramma(RTCtime.Hours, RTCtime.Minutes, RTCDate.WeekDay);
}

void buttonPressedCheck()
{
  if (sprinkler.keyboardHold==0){
    M5.update();
    if (b1.wasPressed()||b2.wasPressed()) {
      soundsBeep(1000, 100, 1);
      sprinkler.keyboardInput = true;
      //button.buttonPressed = mainMenu();
      button.buttonPressed =mainMenu();
      outlineMainscreen();
      sprinkler.updateRelaisSlider = true;
      sprinkler.updateSprinklerSlider = true;
      sprinkler.keyboardInput = false;
    } else if (b3.wasPressed()||b4.wasPressed()) {
        if (sprinkler.staat == Wacht){
        soundsBeep(1000, 100, 1);
        sprinkler.keyboardInput = true;
        button.buttonPressed = settingsMenu();
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
      button.buttonPressed = sprinklerSelectieTouch();
      outlineMainscreen();
      sprinkler.updateRelaisSlider = true;
      sprinkler.updateSprinklerSlider = true;
      sprinkler.keyboardInput = false;
    } else if (relaisZone.wasPressed()) {
      soundsBeep(1000, 100, 1);
      sprinkler.keyboardInput = true;
      button.buttonPressed = relaisSelectie();
      outlineMainscreen();
      sprinkler.updateRelaisSlider = true;
      sprinkler.updateSprinklerSlider = true;
      sprinkler.keyboardInput = false;
    }  
  }
}



void loop()
{
  buttonPressedCheck();
  if (clockData.checkSecond)
  {
    readTime();
    if ( RTCtime.Minutes != clockData.previousMinute )
    {
      controleerProgramma(RTCtime.Hours, RTCtime.Minutes, RTCDate.WeekDay); //controleerProgramma(now.hour(),now.minute(),now.dayOfTheWeek());
      checkRelaisOnTime(RTCtime.Hours, RTCtime.Minutes);
      checkRelaisTempOnTime();
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
          reconnectWifiManager();
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
    checkRelaisTemp();  
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
