//#include <Arduino.h>
//#include <Wire.h>
//#include <WiFi.h>
//#include <PubSubClient.h>
//#include <ArduinoJson.h>
//#include "../lib/LCD.HPP"
//#include "lcd.h"


/*MQTT连接配置*/
/*-----------------------------------------------------*/
/*
const char* ssid = "min";//ESP32连接的WiFi名称
const char* password = "fn123456";
const char* mqttServer = "7b8797ccfc.iot-mqtts.cn-north-4.myhuaweicloud.com";//华为云MQTT接入地址
const int   mqttPort = 1883;
//以下3个参数可以由HMACSHA256算法生成，为硬件通过MQTT协议接入华为云IoT平台的鉴权依据
const char* clientId = "62fd06773a88483559893db3_1660749706911_0_0_2022092812";
const char* mqttUser = "62fd06773a88483559893db3_1660749706911";
const char* mqttPassword = "e0777b399a2a59d5be926d6dbfa475ea902beddb377946721aaa31d27849af54";
 
WiFiClient espClient; //ESP32WiFi模型定义
PubSubClient client(espClient);

//const char* topic_properties_report = "属性上报topic";
const char* topic_properties_report = "$oc/devices/62fd06773a88483559893db3/sys/properties/report";

//接收到命令后上发的响应topic
char* const topic_Commands_Response = "$oc/devices/62fd06773a88483559893db3/sys/commands/response/request_id=";
//uint8_t sult_val=1;

void Command_Response(char *topic, char *responseName, uint8_t response_Result);


//监听华为云IoT平台下发指令并处理 
void callback(char *topic, byte *payload, unsigned int length)
{
  char *pstr = topic; //指向topic字符串，提取request_id用
 

  Serial.println();
  Serial.println();
  Serial.print("Message arrived [");
  Serial.print(topic);  //将收到消息的topic展示出来
  Serial.print("] ");
  Serial.println();
 
  payload[length] = '\0'; //在收到的内容后面加上字符串结束符
  char strPayload[255] = {0}; 
  strcpy(strPayload, (const char*)payload);
  Serial.println((char *)payload);  //打印出收到的内容
  Serial.println(strPayload);
 
 

  char arr[100];  //存放request_id
  int flag = 0;
  char *p = arr;
  while(*pstr)  //以'='为标志，提取出request_id
  {
    if(flag) *p ++ = *pstr;
    if(*pstr == '=') flag = 1;
    pstr++;
  }
  *p = '\0';  
  Serial.println(arr);
  // strcat(topic_Commands_Response, arr);
  // topic_Commands_Response.concat(arr);
 
 
  //将命令响应topic与resquest_id结合起来
  char topicRes[200] = {0};
  strcat(topicRes, topic_Commands_Response);
  strcat(topicRes, arr);
  Serial.println(topicRes);
 
 
  //payload解析///这是对接收到的平台下发的消息或者命令进行解析
//解析程序同样可以由ArduinoJson库官方网站的ArduinoJson助手生成
  const size_t capacity_Payload_Receive = JSON_OBJECT_SIZE(3) + JSON_OBJECT_SIZE(5) + 150;
  DynamicJsonBuffer jsonBuffer_Payload(capacity_Payload_Receive);
 
  JsonObject& root_Payload = jsonBuffer_Payload.parseObject(strPayload);
 
 
//以下就是根据不同的命令或者消息进行不同的响应，此部分代码请自行修改
 if (root_Payload.success()){ //判断json解析是否成功
    if(!strcmp(root_Payload["command_name"], "user_order"))  //如果收到的内容是“用户下单”
    {
      JsonObject& paras_Payload = root_Payload["paras"];
      const char* paras_address = paras_Payload["address"]; // "88—902"
      const char* paras_user = paras_Payload["user"]; // "wksgogogo"
      const char* paras_number = paras_Payload["number"]; // "3333"
      const char* paras_day = paras_Payload["day"]; // "2022-07-21"
      const char* paras_time = paras_Payload["time"]; // "12:01"
 
      Serial.println("__________JSON Received Parse__________");
      Serial.println(paras_address);
      Serial.println(paras_user);
      Serial.println(paras_number);
      Serial.println(paras_day);
      Serial.println(paras_time);
 /
      Info_UserOrder_Structure OrderInfo;
      strcpy(OrderInfo.userName, paras_user);
      strcpy(OrderInfo.address, paras_address);
      strcpy(OrderInfo.orderNum, paras_number);
      strcpy(OrderInfo.day, paras_day);
      strcpy(OrderInfo.time, paras_time);
 /
      //响应函数会在下文贴出
      Command_Response(topicRes, "user_order", 1);
 
      //OrderInfo_Save(OrderInfo);  //订单信息存储
 
 
      
    }
 
 
    if(!strcmp(root_Payload["command_name"], "open"))   //如果收到的内容是“开锁”
    {
      const char* paras_user = root_Payload["paras"]["user"]; 
      Serial.println("__________JSON Received Parse__________");
      Serial.println(paras_user);
 
      //EOF_ELock_Unlock(paras_user);
 
      Command_Response(topicRes, "open", 1);
    }
  }
}







void MQTT_Init()
{
//WiFi网络连接部分
  WiFi.begin(ssid, password); //开启ESP32的WiFi
  while (WiFi.status() != WL_CONNECTED) { //ESP尝试连接到WiFi网络
    delay(3000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to the WiFi network");
 
 
//MQTT服务器连接部分
  client.setServer(mqttServer, mqttPort); //设置连接到MQTT服务器的参数
 
  client.setKeepAlive (60); //设置心跳时间
 
  while (!client.connected()) { //尝试与MQTT服务器建立连接
    Serial.println("Connecting to MQTT...");
  
    if (client.connect(clientId, mqttUser, mqttPassword )) {
  
      Serial.println("connected");  
  
    } else {
  
      Serial.print("failed with state ");
      Serial.print(client.state());
      delay(6000);
  
    }
  }
 
 //接受平台下发内容的初始化
  client.setCallback(callback); //可以接受任何平台下发的内容
 
}


/*
void str_input( char input)
{
  // String input;

StaticJsonDocument<128> doc;

DeserializationError error = deserializeJson(doc, input);

if (error) {
  Serial.print("deserializeJson() failed: ");
  Serial.println(error.c_str());
  return;
}

const char* sensor = doc["sensor"]; // "gps"
long time = doc["time"]; // 1351824120

float data_0 = doc["data"][0]; // 48.75608
float data_1 = doc["data"][1]; // 2.302038
}


void TASK_Capacity_Report(int capacity)
{
 
//以下部分代码调用了ArduinoJson库将属性上报消息打包为JSON格式
//此部分代码可以通过ArduinoJson库的官方网站直接生成
  StaticJsonBuffer<300> JSONbuffer; //定义静态的JSON缓冲区用于存储JSON消息


  JsonObject& root = JSONbuffer.createObject();
  JsonArray& services = root.createNestedArray("services");
  JsonObject& service_1 = services.createNestedObject();
  JsonObject& properties_1_1 = service_1.createNestedObject("properties");
 
  service_1["service_id"] = "BasicData";
  properties_1_1["luminance"] = capacity;
 
//  root.prettyPrintTo(Serial);//调试用，将JSON打印到串口
 
//以下将生成好的JSON格式消息格式化输出到字符数组中，便于下面通过PubSubClient库发送到服务器
  char JSONmessageBuffer[100];
  root.printTo(JSONmessageBuffer, sizeof(JSONmessageBuffer));
  
  Serial.println("Sending message to MQTT topic..");
  Serial.println(JSONmessageBuffer);
 
 
//以下代码将打包好的JSON数据包通过PubSubClient库发送到服务器 
  if (client.publish(topic_properties_report, JSONmessageBuffer) == true) {
    Serial.println("Success sending message");
  } else {
    Serial.println("Error sending message");
  }
 
//由于本函数是放在loop中不断循环执行，所以添加client.loop()进行保持硬件的活跃度
//避免由于长时间未向服务器发送消息而被服务器踢下线
  client.loop();
  Serial.println("-------------");
  
}




void Command_Response(char *topic, char *responseName, uint8_t response_Result)
{

  const size_t capacity = JSON_OBJECT_SIZE(2) + JSON_OBJECT_SIZE(3);
  DynamicJsonBuffer jsonBuffer(capacity);
 
  JsonObject& root = jsonBuffer.createObject();
  if(response_Result == 1){
    root["result_code"] = 0;
    JsonObject& paras = root.createNestedObject("paras");
    paras["status"] = 200;
    paras["msg"] = "success";
  }
  else if(response_Result == FAIL){
    root["result_code"] = 1;
    JsonObject& paras = root.createNestedObject("paras");
    paras["status"] = 400;
    paras["msg"] = "fail";
  }
  
  if(!strcmp(responseName, "user_order")){
    root["response_name"] = "user_order";
  }
  else if(!strcmp(responseName, "open")){
    root["response_name"] = "open";
  }
  
 
  
 
  root.printTo(Serial); //串口打印出构建好的JSON内容
  Serial.println();
 
 
  char JSONmessageBuffer[300];
  root.printTo(JSONmessageBuffer, sizeof(JSONmessageBuffer)); //将构建的JSON消息复制到char数组中
  
  Serial.println("Sending response to HuaWei Cloud..");
  Serial.println(JSONmessageBuffer);
 
  if (client.publish(topic, JSONmessageBuffer) == true) {
    Serial.println("Success sending response command message");
  } else {
    Serial.println("Error sending response command message");
  }
 
 
  Serial.println("-------------");
}
*/

/*
void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  //MQTT_Init();
  digitalWrite(21,HIGH);
  digitalWrite(2,HIGH);
  //LCD_Init();
 // LCD_DisplayOn();
 // test1();
}

void loop() {
  // put your main code here, to run repeatedly:
 // digitalWrite(18,HIGH);
 // delay(1000);
 // digitalWrite(18,LOW);
 LCD_Clear(WHITE);//清全屏白色
  delay(3000);
  LCD_Clear(BLUE);//清全屏白色
  delay(3000);
  // Serial.println("working");
   //TASK_Capacity_Report(2);
}
*/

/*
TFT_eSPI tft = TFT_eSPI();       // Invoke custom library

//#define GRIDX 80
//#define GRIDY 60
//#define CELLXY 4

#define GRIDX 160
#define GRIDY 106
#define CELLXY 3

#define GEN_DELAY 0

//Current grid
uint8_t grid[GRIDX][GRIDY];

//The new grid for the next generation
uint8_t newgrid[GRIDX][GRIDY];

//Number of generations
#define NUMGEN 600

uint16_t genCount = 0;
// Check the Moore neighbourhood
int getNumberOfNeighbors(int x, int y) {
  return grid[x - 1][y] + grid[x - 1][y - 1] + grid[x][y - 1] + grid[x + 1][y - 1] + grid[x + 1][y] + grid[x + 1][y + 1] + grid[x][y + 1] + grid[x - 1][y + 1];
}
//Compute the CA. Basically everything related to CA starts here
void computeCA() {
  for (int16_t x = 1; x < GRIDX; x++) {
    for (int16_t y = 1; y < GRIDY; y++) {
      int neighbors = getNumberOfNeighbors(x, y);
      if (grid[x][y] == 1 && (neighbors == 2 || neighbors == 3 ))
      {
        newgrid[x][y] = 1;
      }
      else if (grid[x][y] == 1)  newgrid[x][y] = 0;
      if (grid[x][y] == 0 && (neighbors == 3))
      {
        newgrid[x][y] = 1;
      }
      else if (grid[x][y] == 0) newgrid[x][y] = 0;
    }
  }
}
//Initialise Grid
void initGrid(void) {
  for (int16_t x = 0; x < GRIDX; x++) {
    for (int16_t y = 0; y < GRIDY; y++) {
      newgrid[x][y] = 0;

      if (x == 0 || x == GRIDX - 1 || y == 0 || y == GRIDY - 1) {
        grid[x][y] = 0;
      }
      else {
        if (random(3) == 1)
          grid[x][y] = 1;
        else
          grid[x][y] = 0;
      }

    }
  }
}

//Draws the grid on the display
void drawGrid(void) {

  uint16_t color = TFT_WHITE;
  for (int16_t x = 1; x < GRIDX - 1; x++) {
    for (int16_t y = 1; y < GRIDY - 1; y++) {
      if ((grid[x][y]) != (newgrid[x][y])) {
        if (newgrid[x][y] == 1) color = 0xFFFF; //random(0xFFFF);
        else color = 0;
        tft.fillRect(CELLXY * x, CELLXY * y, CELLXY, CELLXY, color);
      }
    }
  }
}

void setup()   {

  //Set up the display
  tft.init();
  tft.setRotation(7);
  tft.fillScreen(TFT_BLACK);
  tft.setTextSize(1);
  tft.setTextColor(TFT_WHITE);
  tft.setCursor(0, 0);

  tft.fillScreen(TFT_BLACK);
  tft.setTextSize(2);
  tft.setTextColor(TFT_WHITE);
  tft.setCursor(40, 5);
  tft.println(F("Today:1. test epad one time"));
  tft.setCursor(40, 40);
  tft.println(F("Today:2. play the game"));
  tft.setCursor(40, 70);
  tft.println(F("Today:1. test epad one time"));
  tft.setCursor(40, 100);
  tft.println(F("Today:2. play the game"));
  tft.setCursor(40, 130);
  tft.println(F("Today:1. test epad one time"));
  tft.setCursor(40, 160);
  tft.println(F("Today:2. play the game"));

}

void loop() {

  //Display a simple splash screen
  

  while (1)
  {
    tft.fillScreen(TFT_BLACK);
    tft.fillScreen(TFT_WHITE);
  }

}

/*
TFT_eSPI tft = TFT_eSPI();       // Invoke custom library
// Create two sprites for a DMA toggle buffer
TFT_eSprite spr[2] = {TFT_eSprite(&tft), TFT_eSprite(&tft) };

// Toggle buffer selection
bool sprSel = 0;

void setup(void) {
  tft.init();
 // tft.initDMA();
  tft.setRotation(1);

}

void loop() {

  tft.fillScreen(TFT_BLACK);

  // Draw some random circles
  for (int i = 0; i < 40; i++)
  {
    int rx = random(60);
    int ry = random(60);
    int x = rx + random(480 - rx - rx);
    int y = ry + random(320 - ry - ry);
    tft.fillEllipse(x, y, rx, ry, random(0xFFFF));
  }

  delay(2000);
  tft.fillScreen(TFT_BLACK);

  for (int i = 0; i < 40; i++)
  {
    int rx = random(60);
    int ry = random(60);
    int x = rx + random(480 - rx - rx);
    int y = ry + random(320 - ry - ry);
    tft.drawEllipse(x, y, rx, ry, random(0xFFFF));
  }

  delay(2000);
}
*/

/*

#include <SPI.h>
#include <TFT_eSPI.h>
TFT_eSPI tft = TFT_eSPI();

//====================================================================

void setup(void) {
  Serial.begin(115200);
  Serial.println("\n\nStarting...");

  tft.init();

  tft.fillScreen(TFT_BLACK);
  tft.setTextSize(2);
  tft.setTextColor(TFT_WHITE);
  tft.setCursor(40, 5);
  tft.println(F("Today:1. test epad one time"));
}

//====================================================================

void loop() {

  uint16_t x, y;

  tft.getTouchRaw(&x, &y);
  x=x/10;
  x=x*0.84;
  y=y/10;
  y=y*1.26;
  y=y-20;
  y=480-y;

  x=x-5;
  Serial.printf("x: %i     ", x);

  Serial.printf("y: %i     ", y);

  Serial.printf("z: %i \n", tft.getTouchRawZ());
  tft.drawSpot(x,y,3.0,TFT_WHITE,TFT_BLACK);
  delay(250);

}

//====================================================================
*/

/*
#include <SPI.h>
#include <TFT_eSPI.h>      // Hardware-specific library

TFT_eSPI tft = TFT_eSPI(); // Invoke custom library

//------------------------------------------------------------------------------------------

// Code to run a screen calibration, not needed when calibration values set in setup()
void touch_calibrate()
{
  uint16_t calData[5];
  uint8_t calDataOK = 0;

  // Calibrate
  tft.fillScreen(TFT_BLACK);
  tft.setCursor(20, 0);
  tft.setTextFont(2);
  tft.setTextSize(1);
  tft.setTextColor(TFT_WHITE, TFT_BLACK);

  tft.println("Touch corners as indicated");

  tft.setTextFont(1);
  tft.println();

  tft.calibrateTouch(calData, TFT_MAGENTA, TFT_BLACK, 15);

  Serial.println(); Serial.println();
  Serial.println("// Use this calibration code in setup():");
  Serial.print("  uint16_t calData[5] = ");
  Serial.print("{ ");

  for (uint8_t i = 0; i < 5; i++)
  {
    Serial.print(calData[i]);
    if (i < 4) Serial.print(", ");
  }

  Serial.println(" };");
  Serial.print("  tft.setTouch(calData);");
  Serial.println(); Serial.println();

  tft.fillScreen(TFT_BLACK);
  
  tft.setTextColor(TFT_GREEN, TFT_BLACK);
  tft.println("Calibration complete!");
  tft.println("Calibration code sent to Serial port.");

  delay(4000);
}

void setup() {
  // Use serial port
  Serial.begin(115200);

  // Initialise the TFT screen
  tft.init();

  // Set the rotation to the orientation you wish to use in your project before calibration
  // (the touch coordinates returned then correspond to that rotation only)
  tft.setRotation(1);

  // Calibrate the touch screen and retrieve the scaling factors
 // touch_calibrate();


  // Replace above line with the code sent to Serial Monitor
  // once calibration is complete, e.g.:
  //uint16_t calData[5] = { 286, 3534, 283, 3600, 6 };  // demo
  uint16_t calData[5] = { 225, 3684, 261, 3501, 7 };    //test better
  tft.setTouch(calData);


  // Clear the screen
  tft.fillScreen(TFT_BLACK);
  tft.drawCentreString("Touch screen to test!",tft.width()/2, tft.height()/2, 2);
}

//------------------------------------------------------------------------------------------

void loop(void) {
  uint16_t x = 0, y = 0; // To store the touch coordinates

  // Pressed will be set true is there is a valid touch on the screen
  bool pressed = tft.getTouch(&x, &y);

  Serial.print("x,y = ");
  Serial.print(x);
  Serial.print(",");
  Serial.println(y);
  delay(200);

  // Draw a white spot at the detected coordinates
  if (pressed) {
    tft.fillCircle(x, y, 2, TFT_WHITE);
    //Serial.print("x,y = ");
    //Serial.print(x);
    //Serial.print(",");
    //Serial.println(y);
  }
}

//------------------------------------------------------------------------------------------
*/


/*
    Video: https://www.youtube.com/watch?v=oCMOYS71NIU
    Based on Neil Kolban example for IDF: https://github.com/nkolban/esp32-snippets/blob/master/cpp_utils/tests/BLE%20Tests/SampleNotify.cpp
    Ported to Arduino ESP32 by Evandro Copercini

   Create a BLE server that, once we receive a connection, will send periodic notifications.
   创建一个BLE服务器，一旦我们收到连接，将会周期性发送通知

   T使用步骤：
   1. 创建一个 BLE Server
   2. 创建一个 BLE Service
   3. 创建一个 BLE Characteristic
   4. 创建一个 BLE Descriptor
   5. 开始服务
   6. 开始广播
*/




#include <Arduino.h>


//#include <lv_demos.h>
#include <lvgl.h>
//#include "test.h"
#include "string.h"
#include "../lib/user_gui.h"
#include <TFT_eSPI.h>

//If you want to use the LVGL examples,
 // make sure to install the lv_examples Arduino library
 // and uncomment the following line.
//#include <lv_examples.h>

//#include "../lib/ex_flash.h"
#include <SPI.h>
#include <EEPROM.h>
#include "../lib/ex_flash.h"

#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
#include <BLE2902.h>

#include <WiFi.h>
#include <WebServer.h>
#include <ESPmDNS.h>
//#include "common.h"

//#include <WiFi.h>
////#include <SPIFFS.h>
//#include "ESPAsyncWebServer.h"

#include "../lib/touch40.h"
#include "../lib/FT6336.h"
#define LCD_40 1
//#define LCD_35

// Code to run a screen calibration, not needed when calibration values set in setup()
void touch_calibrate()
{
  uint16_t calData[5];
  uint8_t calDataOK = 0;
TFT_eSPI tft = TFT_eSPI(480,320); 
  // Calibrate
  tft.fillScreen(TFT_BLACK);
  tft.setCursor(20, 0);
  tft.setTextFont(2);
  tft.setTextSize(1);
  tft.setTextColor(TFT_WHITE, TFT_BLACK);

  tft.println("Touch corners as indicated");

  tft.setTextFont(1);
  tft.println();

  //tft.calibrateTouch(calData, TFT_MAGENTA, TFT_BLACK, 15);

  Serial.println(); Serial.println();
  Serial.println("// Use this calibration code in setup():");
  Serial.print("  uint16_t calData[5] = ");
  Serial.print("{ ");

  for (uint8_t i = 0; i < 5; i++)
  {
    Serial.print(calData[i]);
    if (i < 4) Serial.print(", ");
  }

  Serial.println(" };");
  Serial.print("  tft.setTouch(calData);");
  Serial.println(); Serial.println();

  tft.fillScreen(TFT_BLACK);
  
  tft.setTextColor(TFT_GREEN, TFT_BLACK);
  tft.println("Calibration complete!");
  tft.println("Calibration code sent to Serial port.");

  delay(4000);
}

uint8_t txValue = 0;                         //后面需要发送的值
BLEServer *pServer = NULL;                   //BLEServer指针 pServer
BLECharacteristic *pTxCharacteristic = NULL; //BLECharacteristic指针 pTxCharacteristic
bool deviceConnected = false;                //本次连接状态
bool oldDeviceConnected = false;             //上次连接状态d
// See the following for generating UUIDs: https://www.uuidgenerator.net/
#define SERVICE_UUID "12a59900-17cc-11ec-9621-0242ac130002" // UART service UUID
#define CHARACTERISTIC_UUID_RX "12a59e0a-17cc-11ec-9621-0242ac130002"
#define CHARACTERISTIC_UUID_TX "12a5a148-17cc-11ec-9621-0242ac130002"

void modify_display(char *content)
{
  unsigned char cb_num = 0;
  char temp[50] = "'/0'";
  if(*content == '*')
  {
    content++;
    cb_num = (*content)  - 0x30;
    content--;
    if((cb_num > 0) && (cb_num < 7))
    {
      user_write_eeprom((cb_num*50)-50, content, 49);
      get_str_to_gui(temp, (cb_num*50)-50);
      user_gui_cb(cb_num, temp);
    }
  }
}

class MyServerCallbacks : public BLEServerCallbacks
{
    void onConnect(BLEServer *pServer)
    {
        deviceConnected = true;
    };

    void onDisconnect(BLEServer *pServer)
    {
        deviceConnected = false;
    }
};

class MyCallbacks : public BLECharacteristicCallbacks
{
    void onWrite(BLECharacteristic *pCharacteristic)
    {
        char str[50] ;
        std::string rxValue = pCharacteristic->getValue(); //接收信息

        if (rxValue.length() > 0)
        { //向串口输出收到的值
            Serial.print("RX: ");
            for (int i = 0; i < rxValue.length(); i++)
            {
              Serial.print(rxValue[i]);
              str[i] = rxValue[i];

            }  
            strcat(str,  "\r");
            str[48] = 0x0d;
            Serial.println();
            Serial.print("RX_str: ");
            Serial.println(str);
            if(strcmp(str, "clear_all") == 0)
              clear_all();
            else if(str[0] == '-')
            {
              if( (str[1] >= 0x31) && (str[1] <= 0x37) )
              {
                char clear_str[50] = "               /r";
                Serial.println("inter the clear tip");
                Serial.printf("%d\r\n", (str[1] - 0x31)*50);
                user_write_eeprom((str[1] - 0x31)*50, clear_str, 49);
              }
                         
            }
            else
              modify_display(str);
            
        }
        
    }
};

void BLE_user_init(void)
{
  BLEDevice::init("E_NOTE");//在这里面是ble的名称

    // 创建一个 BLE 服务
    pServer = BLEDevice::createServer();
    pServer->setCallbacks(new MyServerCallbacks()); //设置回调
    BLEService *pService = pServer->createService(SERVICE_UUID);

    // 创建一个 BLE 特征
    pTxCharacteristic = pService->createCharacteristic(CHARACTERISTIC_UUID_TX, BLECharacteristic::PROPERTY_NOTIFY);
    pTxCharacteristic->addDescriptor(new BLE2902());
    BLECharacteristic *pRxCharacteristic = pService->createCharacteristic(CHARACTERISTIC_UUID_RX, BLECharacteristic::PROPERTY_WRITE);
    pRxCharacteristic->setCallbacks(new MyCallbacks()); //设置回调

    pService->start();                  // 开始服务
    pServer->getAdvertising()->start(); // 开始广播
    Serial.println(" 等待一个客户端连接，且发送通知... ");
}

//If you want to use the LVGL examples,
 // make sure to install the lv_examples Arduino library
 // and uncomment the following line.
//#include <lv_examples.h>


//Change to your screen resolution
static const unsigned int screenWidth  = 480;
static const unsigned int screenHeight = 320;
TFT_eSPI tft = TFT_eSPI(screenWidth, screenHeight); 
static lv_disp_draw_buf_t draw_buf;
static lv_color_t buf[ screenWidth * 10 ];



#if 1 //LV_USE_LOG != 0
// Serial debugging 
void my_print(const char * buf)
{
    Serial.printf(buf);
    Serial.flush();
}
#endif

//Display flushing 
void my_disp_flush( lv_disp_drv_t *disp, const lv_area_t *area, lv_color_t *color_p )
{
    uint32_t w = ( area->x2 - area->x1 + 1 );
    uint32_t h = ( area->y2 - area->y1 + 1 );


   // tft.setSwapBytes(true);
  //  tft.pushImageDMA(area->x1, area->y1, w, h,(uint16_t *)&color_p->full);

    tft.startWrite();
    tft.setAddrWindow( area->x1, area->y1, w, h );
    tft.pushColors( ( uint16_t * )&color_p->full, w * h, true );
    tft.endWrite();

    lv_disp_flush_ready( disp );
}


#ifdef LCD_35

//Read the touchpad
void my_touchpad_read( lv_indev_drv_t * indev_driver, lv_indev_data_t * data )
{
    uint16_t touchX, touchY;

    bool touched = tft.getTouch( &touchX, &touchY, 600 );

    if( !touched )
    {
        data->state = LV_INDEV_STATE_REL;
    }
    else
    {
        data->state = LV_INDEV_STATE_PR;

        //Set the coordinates
        data->point.x = touchX;
        data->point.y = touchY;

        Serial.print( "Data x " );
        Serial.println( touchX );

        Serial.print( "Data y " );
        Serial.println( touchY );
    }
}
#else

#define I2C_SDA 32
#define I2C_SCL 25
#define RST_N_PIN 33
#define INT_N_PIN 39
FT6336U ft6336u(I2C_SDA, I2C_SCL, RST_N_PIN, INT_N_PIN); 
void my_touchpad_read(lv_indev_drv_t *indev_driver, lv_indev_data_t *data)
{
  if(digitalRead(INT_N_PIN) == 0) {
      if(ft6336u.read_td_status() == 1 )
      {
        Serial.print("FT6336U TD Status: "); 
        Serial.println(ft6336u.read_td_status());  
        Serial.print("FT6336U Touch Event/ID 1: ("); 
        Serial.print(ft6336u.read_touch1_event()); Serial.print(" / "); Serial.print(ft6336u.read_touch1_id()); Serial.println(")"); 
        Serial.print("FT6336U Touch Position 1: ("); 
        Serial.print(ft6336u.read_touch1_x()); Serial.print(" , "); Serial.print(ft6336u.read_touch1_y()); Serial.println(")"); 
        Serial.print("FT6336U Touch Weight/MISC 1: ("); 
        Serial.print(ft6336u.read_touch1_weight()); Serial.print(" / "); Serial.print(ft6336u.read_touch1_misc()); Serial.println(")"); 
        Serial.print("FT6336U Touch Event/ID 2: ("); 
        Serial.print(ft6336u.read_touch2_event()); Serial.print(" / "); Serial.print(ft6336u.read_touch2_id()); Serial.println(")"); 
        Serial.print("FT6336U Touch Position 2: ("); 
        Serial.print(ft6336u.read_touch2_x()); Serial.print(" , "); Serial.print(ft6336u.read_touch2_y()); Serial.println(")"); 
        Serial.print("FT6336U Touch Weight/MISC 2: ("); 
        Serial.print(ft6336u.read_touch2_weight()); Serial.print(" / "); Serial.print(ft6336u.read_touch2_misc()); Serial.println(")"); 
        data->state = LV_INDEV_STATE_PR;
        data->point.x = TouchHeight - ft6336u.read_touch1_y();
        data->point.y = ft6336u.read_touch1_x();
      }
        
    }
    else{
      data->state = LV_INDEV_STATE_REL;
    }
  
}
#endif // DEBUG

void LVGL_user_init(void)
{
  //uint16_t calData[5] = { 275, 3620, 264, 3532, 1 };
    //uint16_t calData[5] = { 225, 3684, 261, 3501, 7 };
#ifdef LCD_35
    uint16_t calData[5] = { 18, 1, 1, 1, 0 }; // 4.0
    tft.setTouch( calData );
#else
//touch40_init(tft.width(), tft.height(),tft.getRotation());
#endif // DEBUG


    lv_disp_draw_buf_init( &draw_buf, buf, NULL, screenWidth * 10 );

    //Initialize the display
    static lv_disp_drv_t disp_drv;
    lv_disp_drv_init( &disp_drv );
    //Change the following line to your display resolution
    disp_drv.hor_res = screenWidth;
    disp_drv.ver_res = screenHeight;
    disp_drv.flush_cb = my_disp_flush;
    disp_drv.draw_buf = &draw_buf;
    lv_disp_drv_register( &disp_drv );

    //Initialize the (dummy) input device driver
    static lv_indev_drv_t indev_drv;
    lv_indev_drv_init( &indev_drv );
    indev_drv.type = LV_INDEV_TYPE_POINTER;
    indev_drv.read_cb = my_touchpad_read;
    lv_indev_drv_register( &indev_drv );

}


/*
  ESP32 Web Server - STA Mode
  modified on 25 MAy 2019
  by Mohammadreza Akbari @ Electropeak
  https://electropeak.com/learn
*/
 


 //	const char* ssid     = "Tenda_6802F0";
//	const char* password = "ming6617.";
// SSID & Password
const char* ssid = "Tenda_6802F0";
const char* password = "ming6617.";

//const char* ssid = "MIN";
//const char* password = "ming6617.";
WebServer server(80);  // Object of WebServer(HTTP port, 80 is defult)
 

 // HTML & CSS contents which display on web server
String HTML = "<!DOCTYPE html>\
<html>\
<head>\
<meta charset=\"utf-8\">\
<meta http-equiv=\"Content-Type\" content=\"text/html;\
<meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\" <meta http-equiv=\"X-UA-Compatible\" content=\"ie=edge\">\
<title>e_note内容修改中心</title>\
<script type=\"text/javascript\">\
function wifi(){\
var ssid = aa.value;\
var xmlhttp=new XMLHttpRequest();\
xmlhttp.open(\"GET\",\"/set?ssid=\"+ssid,true);\
xmlhttp.send();\
xmlhttp.onload = function(e){\
alert(this.responseText);}\
}\
</script>\
</head>\
<body>hi, enote内容修改\
<form>\
<input type=\"text\" placeholder=“请输入修改的内容” id=\"aa\">\
<br>\
<input type=\"button\" value=\"修改\" onclick=\"wifi()\">\
</form>\
</body>\
</html>";

 


unsigned char temp_test = 0xaa;
char test_write_str[50] = "*test write eerrom to gui ok ! \r\n";
char test_read_str[50] = "star write eerrom ";
/*
void start_gui(void)
{
  get_str_to_gui(test_read_str, 0);

  get_str_to_gui(test_read_str, 50);

  get_str_to_gui(test_read_str, 100);

  get_str_to_gui(test_read_str, 150);

  get_str_to_gui(test_read_str, 200);

  get_str_to_gui(test_read_str, 250);

  get_str_to_gui(test_read_str, 300);
}
*/
void user_start_gui(void)
{
  char temp_str[50] = {0};
  unsigned char cb_amount = 0;

    get_str_to_gui(temp_str, 0);
    if(temp_str[0] =='1' )
      cb_amount++;
  
    get_str_to_gui(temp_str, 50);
    if(temp_str[0] =='2' )
      cb_amount++;
      
    get_str_to_gui(temp_str, 100);
    if(temp_str[0] =='3' )
      cb_amount++;
      
    get_str_to_gui(temp_str, 150);
    if(temp_str[0] =='4' )
      cb_amount++;
      
    get_str_to_gui(temp_str, 200);
    if(temp_str[0] =='5' )
      cb_amount++;
      
    get_str_to_gui(temp_str, 250);
    if(temp_str[0] =='6' )
      cb_amount++;
    
    user_gui(cb_amount);
/*
    switch(cb_amount)
    {
      case 1 :get_str_to_gui(temp_str, 0);
              user_gui_cb(1, temp_str); 
              break;
      case 2 :get_str_to_gui(temp_str, 50);
              user_gui_cb(2, temp_str);
              break;
      case 3 :get_str_to_gui(temp_str, 100);
              user_gui_cb(3, temp_str); 
              break;
      case 4 :get_str_to_gui(temp_str, 150);
              user_gui_cb(4, temp_str);  
              break;
      case 5 :get_str_to_gui(temp_str, 200);
              user_gui_cb(5, temp_str); 
              break; 
      case 6 :get_str_to_gui(temp_str, 250);
              user_gui_cb(6, temp_str);                          
    }*/
      
    for(char i=1; i <= cb_amount ;i++)
    {
      get_str_to_gui(temp_str, (i-1)*50);
      user_gui_cb(i, temp_str); 
    }
}
// Handle root url (/)
void handle_root() {
  server.send(200, "text/html", HTML);
}


void handle_get()
{
  String texts = server.arg("ssid");
  char str[51];
for (int i = 0; texts[i] != '\0'; i++)
{
  str[i] = texts[i];
} 
  Serial.println("received: "+ texts);
  strcat(str,  "\r");
  str[48] = 0x0d;
  Serial.println();
  Serial.print("RX_str: ");
  Serial.println(str);
  if (strcmp(str, "clear_all") == 0)
        clear_all();
  else if (str[0] == '-')
  {
        if ((str[1] >= 0x31) && (str[1] <= 0x37))
        {
            char clear_str[50] = "               /r";
            Serial.println("inter the clear tip");
            Serial.printf("%d\r\n", (str[1] - 0x31) * 50);
            user_write_eeprom(150, clear_str, 49);
           
        }
  }
  else
        modify_display(str);
  user_start_gui();
  server.send(200, "text/html", "seccuss!");

}

#include <Wire.h>
void setup()
{
    Serial.begin( 115200 ); 

//    pinMode(22,OUTPUT);
 //   pinMode(4,OUTPUT);
//    digitalWrite(22,HIGH);
 //   digitalWrite(4,HIGH);
    EEPROM.begin(4096);    //申请空间，传入参数为size，为需要读写的数据字节最大地址+1，取值1~4096；
    ft6336u.begin(); 
    //ft6336u.setRotation(Rotation_2);
    Serial.print("FT6336U Firmware Version: "); 
    Serial.println(ft6336u.read_firmware_id());  
    Serial.print("FT6336U Device Mode: "); 
    Serial.println(ft6336u.read_device_mode()); 
    //BLE_user_init();

    static lv_disp_draw_buf_t draw_buf;
  //  static lv_color_t buf_2_1[screenWidth * 30]; //A buffer for 10 rows
  //  static lv_color_t buf_2_2[screenWidth * 30]; //An other buffer for 10

  //  String LVGL_Arduino = "Hello Arduino! ";
  //  LVGL_Arduino += String('V') + lv_version_major() + "." + lv_version_minor() + "." + lv_version_patch();


    lv_init();

    tft.begin();          
    tft.setRotation( 3 ); // Landscape orientation, flipped 
    
    LVGL_user_init();
    user_start_gui();

    Serial.println( "Setup done" );

    Serial.println("Try Connecting to ");
  Serial.println(ssid);
 
  // Connect to your wi-fi modem
  WiFi.begin(ssid, password);
 
  // Check wi-fi is connected to wi-fi network
  while (WiFi.status() != WL_CONNECTED) {
  delay(1000);
  Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected successfully");
  Serial.print("Got IP: ");
  Serial.println(WiFi.localIP());  //Show ESP32 IP on serial

  if (!MDNS.begin("menote")) { //自定义域名
    Serial.println("Error setting up MDNS responder!");
  }
  MDNS.addService("http", "tcp", 80); //启用DNS服务
 
  server.on("/", handle_root);
  server.on("/set", HTTP_GET, handle_get);
  server.begin();

  Serial.println("HTTP server started");
  delay(100); 
}

void loop()
{
  lv_timer_handler(); // let the GUI do its work 
     // deviceConnected 已连接
   // server.handleClient();

     /*
    if (deviceConnected)
    {
        pTxCharacteristic->setValue(&txValue, 1); // 设置要发送的值为1
        pTxCharacteristic->notify();              // 广播
        txValue++;                                // 指针数值自加1
        delay(200);                              // 如果有太多包要发送，蓝牙会堵塞
    }

    // disconnecting  断开连接
   
    if (!deviceConnected && oldDeviceConnected)
    {
        delay(100);                  // 留时间给蓝牙缓冲
        pServer->startAdvertising(); // 重新广播
        Serial.println(" 开始广播 ");
        oldDeviceConnected = deviceConnected;
    }


    // connecting  正在连接
    if (deviceConnected && !oldDeviceConnected)
    {
        // do stuff here on connecting
        oldDeviceConnected = deviceConnected;
    }
    */
    delay( 1 );
    
}   





/*


void setup() {
    Serial.begin(115200); 

     
}

void loop() {
    if(digitalRead(INT_N_PIN) == 0) {
      if(ft6336u.read_td_status() == 1 )
      {
        Serial.print("FT6336U TD Status: "); 
        Serial.println(ft6336u.read_td_status());  
        Serial.print("FT6336U Touch Event/ID 1: ("); 
        Serial.print(ft6336u.read_touch1_event()); Serial.print(" / "); Serial.print(ft6336u.read_touch1_id()); Serial.println(")"); 
        Serial.print("FT6336U Touch Position 1: ("); 
        Serial.print(ft6336u.read_touch1_x()); Serial.print(" , "); Serial.print(ft6336u.read_touch1_y()); Serial.println(")"); 
        Serial.print("FT6336U Touch Weight/MISC 1: ("); 
        Serial.print(ft6336u.read_touch1_weight()); Serial.print(" / "); Serial.print(ft6336u.read_touch1_misc()); Serial.println(")"); 
        Serial.print("FT6336U Touch Event/ID 2: ("); 
        Serial.print(ft6336u.read_touch2_event()); Serial.print(" / "); Serial.print(ft6336u.read_touch2_id()); Serial.println(")"); 
        Serial.print("FT6336U Touch Position 2: ("); 
        Serial.print(ft6336u.read_touch2_x()); Serial.print(" , "); Serial.print(ft6336u.read_touch2_y()); Serial.println(")"); 
        Serial.print("FT6336U Touch Weight/MISC 2: ("); 
        Serial.print(ft6336u.read_touch2_weight()); Serial.print(" / "); Serial.print(ft6336u.read_touch2_misc()); Serial.println(")"); 
      }
        
    }
delay(1);
}
*/