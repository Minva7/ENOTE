#include <Arduino.h>
#include <lvgl.h>
//#include "test.h"
#include "string.h"
#include "../lib/user_gui.h"
#include <TFT_eSPI.h>
//2025 LVGL8   test
//If you want to use the LVGL examples,
 // make sure to install the lv_examples Arduino library
 // and uncomment the following line.
//#include <lv_examples.h>

//#include "../lib/ex_flash.h"
#include <SPI.h>
#include <EEPROM.h>
#include "../lib/ex_flash.h"

#include <WiFi.h>
#include <WebServer.h>
#include <ESPmDNS.h>

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

// 云服务
uint8_t txValue = 0;                         //后面需要发送的值

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
<body>enote内容修改 (‘*x’ 修改内容）（‘-x’ 删除内容）（‘clear_all’ 清除全部） \
<form>\
<input type=\"text\" placeholder=“请输入修改的内容” id=\"aa\">\
<br>\
<input type=\"button\" value=\"修改\" onclick=\"wifi()\">\
</form>\
</body>\
</html>";

 


unsigned char temp_test = 0xaa;
char test_write_str[50] = "#test write eerrom to gui ok ! \r\n";
char test_read_str[50] = "star write eerrom ";
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
  
  //Serial.println();
  //Serial.print("RX_str: ");
  //Serial.println(str);
  if (strcmp(str, "clear_all") == 0)
        clear_all();
  else if (str[0] == '-')
  {
    strcat(str,  "\r");
    str[48] = 0x0d;
        if ((str[1] >= 0x31) && (str[1] <= 0x37))
        {
            char clear_str[50] = "               /r";
            Serial.println("inter the clear tip");
            Serial.printf("%d\r\n", (str[1] - 0x31) * 50);
            user_write_eeprom(150, clear_str, 49);
           
        }
  }
  else
  {
    strcat(str,  "\r");
    str[48] = 0x0d;
    modify_display(str);
  }
        
  user_start_gui();
  server.send(200, "text/html", "seccuss!");

}

#include <Wire.h>
void setup()
{
    Serial.begin( 115200 ); 

// lcd BL pwm
  ledcSetup(0, 30000, 8);
  ledcAttachPin(21, 0); 
  ledcWrite(0, 60);
 //   pinMode(4,OUTPUT);
 // pinMode(21,OUTPUT);
   // digitalWrite(21,LOW);


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
    server.handleClient();

    delay( 1 );
    
}   

