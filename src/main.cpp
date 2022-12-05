#include <lvgl.h>
// #include "FS.h"
// #include <demos/lv_demos.h>
// #include <TFT_eSPI.h>
#include "Port/Display.h"
#include "TinyGPSPlus.h"
// #include "Arduino.h"
#include "HAL/HAL.h"

#include <SPI.h>
// #include "epd2in13_V3.h"
// Paint lib
// #include "epdpaint.h"
// #include "imagedata.h"


#define GPS_SERIAL           Serial1
#define CONFIG_GPS_TX_PIN           36
#define CONFIG_GPS_RX_PIN           19
// #define TRANSFER_TO_EINK  digitalWrite(TFT_CS,HIGH);digitalWrite(CS_PIN,LOW);
// #define TRANSFER_TO_LCD digitalWrite(CS_PIN,HIGH);digitalWrite(TFT_CS,LOW);
// #include "App/App.h"
// #include "TouchScreen.h"
/*If you want to use the LVGL examples,
  make sure to install the lv_examples Arduino library
  and uncomment the following line.
#include <lv_examples.h>
*/

// TaskHandle_t handleTaskLvgl;
// void TaskLvglUpdate(void* parameter)
// {
//     ulTaskNotifyTake(pdTRUE, portMAX_DELAY);

//     for (;;)
//     {
//         lv_task_handler();

//         delay(5);
//     }
// }
// #define LV_USE_DEMO_WIDGETS
lv_obj_t * cont;

extern lv_disp_t * disp_lcd;
extern lv_disp_t * disp_eink;

extern lv_obj_t * scr_lcd;
extern lv_obj_t * scr_eink;

extern SemaphoreHandle_t einkUpdateSemaphore;
//lcd information
//GPS,IMU,LCD,E-Ink,SD,BLE
lv_obj_t* Info_GPS;
lv_obj_t* Info_IMU;
lv_obj_t* Info_BLE;
lv_obj_t* Info_LCD;
lv_obj_t* Info_EInk;
lv_obj_t* Info_SD;
//data
lv_obj_t* Data_GPS;
lv_obj_t* Data_IMU;
lv_obj_t* Data_BLE;
lv_obj_t* Data_LCD;
lv_obj_t* Data_EInk;
lv_obj_t* Data_SD;

//eink information
lv_obj_t* Info_North;
lv_obj_t* Info_East;
lv_obj_t* Info_Dest;
lv_obj_t* Info_Batt;
// lv_obj_t* Info_GPS; already in lcd

lv_obj_t* Data_North;
lv_obj_t* Data_East;
lv_obj_t* Data_Dest;
lv_obj_t* Data_Batt;


// lv_style_t red_style;
lv_timer_t* timer;
float trip=0;
int time1=0;

HAL::GPS_Info_t gps_info;
HAL::IMU_Info_t imu_info;

extern volatile bool isLCD;
// eink object
// Epd epd;
// unsigned char image[1536];
// Paint paint(image,96,122);
#define DISP_HOR_RES EPD_WIDTH   //250 x axis of Paint class
#define DISP_VER_RES EPD_HEIGHT//122
#define MY_DISP_HOR_RES DISP_HOR_RES
#define MY_DISP_VER_RES DISP_VER_RES

#define CS_PIN          9

// Epd epdControl;
// Paint einkImg(IMAGE_DATA, EPD_WIDTH, EPD_HEIGHT);
// #define COLORED     0
// #define UNCOLORED   1

// update the text in certain period using hardware object
void onTimerUpdate(lv_timer_t* timer){
    /*
    "Latitude\n"
    "Longitude\n"
    "Altitude\n"
    "Speed",
    */
//    static bool isLCD=false;
   static int count=0;
   
   Serial.println( "text_update" );
   if(isLCD){
    Serial.println( "is LCD" );
    if(gps_info.isVaild)
          lv_label_set_text_fmt(
              Data_GPS,
              "%s\n"
              "%d\n"
              "%0.6f\n"
              "%0.6f\n"
              "%0.2fm\n"
              "%0.1fkm/h",
              "Yes",
              gps_info.satellites,
              gps_info.latitude,
              gps_info.longitude,
              gps_info.altitude,
              gps_info.speed
              //   trip+=(float)0.1,
              //   time1++
          );
      else{
          lv_label_set_text_fmt(
              Data_GPS,
              "%s\n"
              "%d\n"
              "%0.6f\n"
              "%0.6f\n"
              "%0.2fm\n"
              "%0.1fkm/h",
                "No",
                0,
              (float)0,
              (float)0,
              (float)0,
              (float)0
          );
      }
    
    lv_label_set_text_fmt(
          Data_IMU,
          "%.3f\n%.3f\n%.3f\n",
          imu_info.ax,
          imu_info.ay,
          imu_info.az
          //   trip+=(float)0.1,
          //   time1++
      );
    }
    else{
        Serial.println( "is Eink" );
      lv_label_set_text_fmt(
          Data_North,
          "%dm\n",
          count
      );
      lv_label_set_text_fmt(
          Data_East,
          "%dm\n",
          count
      );
    }
    // count++;
    // if(count==4){
    //     //give semaphore
    //     // Serial.println("give eink update semaphore");
    //     // xSemaphoreGive(einkUpdateSemaphore);
        
    //     count=0;
    //     // Serial.println("update semaphore detected, switch");
    //   if(isLCD){
    //     To_Eink_Port();
    //   // Eink_info_init();
    //   }
    //   else
    //     To_LCD_Port();
    //   isLCD=!isLCD;
    // //   lv_task_handler();
    //   Serial.println("end of switch");
    // }
    Serial.println( "text_updated" );
}

// create item info box using input
// Info: the information text object
// data: the data text object
// name: name of this info box
// infos: the infomation that displayed in information text object
// x and y bias: need to be calculated by user.
void Item_Create(
    lv_obj_t* Info,
    lv_obj_t* &data,
    const char* name,
    // const char* img_src,
    const char* infos,
    int x_bias,
    int y_bias
)
{
  //line
  lv_obj_t* line = lv_line_create(scr_lcd);
  lv_obj_set_size(line, 200, 2);
  lv_obj_set_style_border_color(line,lv_color_hex(0xff931e),0);
  lv_obj_set_style_border_side(line, LV_BORDER_SIDE_BOTTOM, 0);
  lv_obj_set_style_border_width(line, 3, 0);
  lv_obj_align(line, LV_ALIGN_TOP_LEFT, x_bias, y_bias+14);

  //name text 
  lv_obj_t* label = lv_label_create(scr_lcd);
  lv_label_set_text(label, name);
  lv_obj_align(label, LV_ALIGN_TOP_LEFT, x_bias, y_bias);

  // info text
  Info = lv_label_create(scr_lcd);
  lv_label_set_text(Info, infos);
  lv_obj_align(Info, LV_ALIGN_TOP_LEFT, x_bias, y_bias+15);

  //data text
  data = lv_label_create(scr_lcd);
  lv_label_set_text(data, "-");
  lv_obj_align(data, LV_ALIGN_TOP_LEFT, x_bias+120, y_bias+15);

}
void Eink_Item_Create(
    lv_obj_t* Info,
    lv_obj_t* &data,
    // const char* name,
    // const char* img_src,
    const char* infos,
    int x_bias,
    int y_bias
)
{
  // info text
  Info = lv_label_create(scr_eink);
  lv_label_set_text(Info, infos);
  lv_obj_align(Info, LV_ALIGN_TOP_LEFT, x_bias, y_bias);

  //data text
  data = lv_label_create(scr_eink);
  lv_label_set_text(data, "-");
  lv_obj_align(data, LV_ALIGN_TOP_LEFT, x_bias+60, y_bias);

}
void Eink_info_init(void)
{
  Eink_Item_Create(
      Info_North,
      Data_North,
      "North:",
      27,
      53
  );
  Eink_Item_Create(
      Info_East,
      Data_East,
      "East",
      27,
      88
  );
  Eink_Item_Create(
      Info_Dest,
      Data_Dest, 
      "Dest",
      27,
      123
  );

  Eink_Item_Create(
      Info_Batt,
      Data_Batt,
      "Batt:",
      15,
      9
  );
//   Eink_Item_Create(
//       Info_GPS,
//       Data_GPS,
//       "GPS:",
//       173,
//       30
//   );
}

void LCD_info_init(){
      Item_Create(
      Info_GPS,
      Data_GPS,
      "GPS",
      "valid\n"
      "satellite\n"
      "Latitude\n"
      "Longitude\n"
      "Altitude\n"
      "Speed",
      20,
      0
  );

  Item_Create(
      Info_IMU,
      Data_IMU,
      "IMU",
      "Step\n"
      "Ax\n"
      "Ay\n"
      "Az",
      20,
      100
  );
  Item_Create(
      Info_BLE,
      Data_BLE,
      "BLE",
      "Connected\n"
      "Speed",
      20,
      200
  );
  Item_Create(
      Info_LCD,
      Data_LCD,
      "LCD",
      "Selected",
      20,
      270
  );
  Item_Create(
      Info_EInk,
      Data_EInk,
      "E-Ink",
      "Selected",
      20,
      325
  );
  Item_Create(
      Info_SD,
      Data_SD,
      "storage",
      "Status\n"
      "Size",
      20,
      380
);
}

void setup()
{
  //  File f = SPIFFS.open(CALIBRATION_FILE, "r");
    Serial.begin( 115200 ); /* prepare for possible serial debug */

    String LVGL_Arduino = "Hello Arduino! ";
    LVGL_Arduino += String('V') + lv_version_major() + "." + lv_version_minor() + "." + lv_version_patch();

    Serial.println( LVGL_Arduino );
    Serial.println( "I am LVGL_Arduino" );

    //GPS init
    GPS_SERIAL.begin(9600, SERIAL_8N1, CONFIG_GPS_TX_PIN, CONFIG_GPS_RX_PIN);
    Serial.print("GPS: TinyGPS++ library v. ");
    Serial.print(TinyGPSPlus::libraryVersion());
    Serial.println(" by Mikal Hart");
    HAL::Power_Init();
    HAL::Encoder_Init();
    //I2C and IMU init
    HAL::I2C_Init(true);
    HAL::IMU_Init();
    // HAL::SD_Init();
    pinMode(TFT_CS,OUTPUT);
    pinMode(CONFIG_SCREEN_BLK_PIN,OUTPUT);
    pinMode(CONFIG_SCREEN_PWR_PIN,OUTPUT);
    pinMode(CONFIG_IMU_GPS_PWR_PIN,OUTPUT);

    digitalWrite(CONFIG_SCREEN_BLK_PIN,HIGH); // low-side nmos
    digitalWrite(CONFIG_SCREEN_PWR_PIN,LOW); // high-side pmos
    digitalWrite(CONFIG_IMU_GPS_PWR_PIN,LOW); // high-side pmos
    // pinMode(9,OUTPUT);
    // digitalWrite(CS_PIN,HIGH);
    // TRANSFER_TO_LCD
    // Port_Init();
    // einkUpdateSemaphore = xSemaphoreCreateBinary();
    // if (einkUpdateSemaphore!=NULL)
    // {
    //     Serial.println("eink update semaphore creation succeed!");
    // }

    lv_init();
    To_LCD_Port();
    
    LCD_info_init();
    // lv_task_handler();
    // delay(3000);
    // // End_spi_transaction();
    // Serial.println("eink set up start");
    // //EINK test
    // // TRANSFER_TO_EINK
    // // Port_Init_Eink();
    // To_Eink_Port();
    // Eink_info_init();
    startFreeRtos();

 
    // create timer that update the data text at certain period
    #if LV_USE_THEME_DEFAULT
    lv_theme_default_init(NULL, lv_palette_main(LV_PALETTE_BLUE), lv_palette_main(LV_PALETTE_RED), LV_THEME_DEFAULT_DARK,
                          LV_FONT_DEFAULT);
    #endif
//data
    timer = lv_timer_create(onTimerUpdate, 2000, nullptr);
    lv_timer_ready(timer);

// #endif
// Update display in parallel thread.
    // xTaskCreate(
    //     TaskLvglUpdate,
    //     "LvglThread",
    //     20000,
    //     nullptr,
    //     configMAX_PRIORITIES - 1,
    //     // NULL);
    //     &handleTaskLvgl);
    // App_Init();
    Serial.println( "notify give" );
    xTaskNotifyGive(handleTaskLvgl);
    Serial.println( "Setup done" );
}

void loop()
{   
    // update data from hardware
    // Serial.println( "GPS_update" );
    HAL::GPS_Update();
    HAL::IMU_Update(&imu_info);
    HAL::GPS_GetInfo(&gps_info);
    HAL::HAL_Update();
    


    // lv_timer_handler(); /* let the GUI do its work */
    // lv_tick_inc(5);
    delay( 20 );
}


  
    /*
  Example animated analogue meters using a ILI9341 TFT LCD screen

  Needs Font 2 (also Font 4 if using large scale label)

  Make sure all the display driver and pin comnenctions are correct by
  editting the User_Setup.h file in the TFT_eSPI library folder.

  #########################################################################
  ###### DON'T FORGET TO UPDATE THE User_Setup.h FILE IN THE LIBRARY ######
  #########################################################################
*/
// #include "FS.h"

// #include <SPI.h>
// #include <TFT_eSPI.h>      // Hardware-specific library

// TFT_eSPI tft = TFT_eSPI(); // Invoke custom library

// // This is the file name used to store the calibration data
// // You can change this to create new calibration files.
// // The SPIFFS file name must start with "/".
// #define CALIBRATION_FILE "/TouchCalData1"

// // Set REPEAT_CAL to true instead of false to run calibration
// // again, otherwise it will only be done once.
// // Repeat calibration if you change the screen rotation.
// #define REPEAT_CAL false

// // Keypad start position, key sizes and spacing
// #define KEY_X 40 // Centre of key
// #define KEY_Y 96
// #define KEY_W 62 // Width and height
// #define KEY_H 30
// #define KEY_SPACING_X 18 // X and Y gap
// #define KEY_SPACING_Y 20
// #define KEY_TEXTSIZE 1   // Font size multiplier

// // Using two fonts since numbers are nice when bold
// #define LABEL1_FONT &FreeSansOblique12pt7b // Key label font 1
// #define LABEL2_FONT &FreeSansBold12pt7b    // Key label font 2

// // Numeric display box size and location
// #define DISP_X 1
// #define DISP_Y 10
// #define DISP_W 238
// #define DISP_H 50
// #define DISP_TSIZE 3
// #define DISP_TCOLOR TFT_CYAN

// // Number length, buffer for storing it and character index
// #define NUM_LEN 12
// char numberBuffer[NUM_LEN + 1] = "";
// uint8_t numberIndex = 0;

// // We have a status line for messages
// #define STATUS_X 120 // Centred on this
// #define STATUS_Y 65

// // Create 15 keys for the keypad
// char keyLabel[15][5] = {"New", "Del", "Send", "1", "2", "3", "4", "5", "6", "7", "8", "9", ".", "0", "#" };
// uint16_t keyColor[15] = {TFT_RED, TFT_DARKGREY, TFT_DARKGREEN,
//                          TFT_BLUE, TFT_BLUE, TFT_BLUE,
//                          TFT_BLUE, TFT_BLUE, TFT_BLUE,
//                          TFT_BLUE, TFT_BLUE, TFT_BLUE,
//                          TFT_BLUE, TFT_BLUE, TFT_BLUE
//                         };

// // Invoke the TFT_eSPI button class and create all the button objects
// TFT_eSPI_Button key[15];

// //------------------------------------------------------------------------------------------
// //------------------------------------------------------------------------------------------

// void drawKeypad()
// {
//   // Draw the keys
//   for (uint8_t row = 0; row < 5; row++) {
//     for (uint8_t col = 0; col < 3; col++) {
//       uint8_t b = col + row * 3;

//       if (b < 3) tft.setFreeFont(LABEL1_FONT);
//       else tft.setFreeFont(LABEL2_FONT);

//       key[b].initButton(&tft, KEY_X + col * (KEY_W + KEY_SPACING_X),
//                         KEY_Y + row * (KEY_H + KEY_SPACING_Y), // x, y, w, h, outline, fill, text
//                         KEY_W, KEY_H, TFT_WHITE, keyColor[b], TFT_WHITE,
//                         keyLabel[b], KEY_TEXTSIZE);
//       key[b].drawButton();
//     }
//   }
// }

// //------------------------------------------------------------------------------------------

// void touch_calibrate()
// {
//   uint16_t calData[5];
//   uint8_t calDataOK = 0;

//   // check file system exists
//   if (!SPIFFS.begin()) {
//     Serial.println("Formating file system");
//     SPIFFS.format();
//     SPIFFS.begin();
//   }

//   // check if calibration file exists and size is correct
//   if (SPIFFS.exists(CALIBRATION_FILE)) {
//     if (REPEAT_CAL)
//     {
//       // Delete if we want to re-calibrate
//       SPIFFS.remove(CALIBRATION_FILE);
//     }
//     else
//     {
//       File f = SPIFFS.open(CALIBRATION_FILE, "r");
//       if (f) {
//         if (f.readBytes((char *)calData, 14) == 14)
//           calDataOK = 1;
//         f.close();
//       }
//     }
//   }

//   if (calDataOK && !REPEAT_CAL) {
//     // calibration data valid
//     tft.setTouch(calData);
//   } else {
//     // data not valid so recalibrate
//     tft.fillScreen(TFT_BLACK);
//     tft.setCursor(20, 0);
//     tft.setTextFont(2);
//     tft.setTextSize(1);
//     tft.setTextColor(TFT_WHITE, TFT_BLACK);

//     tft.println("Touch corners as indicated");

//     tft.setTextFont(1);
//     tft.println();

//     if (REPEAT_CAL) {
//       tft.setTextColor(TFT_RED, TFT_BLACK);
//       tft.println("Set REPEAT_CAL to false to stop this running again!");
//     }

//     tft.calibrateTouch(calData, TFT_MAGENTA, TFT_BLACK, 15);

//     tft.setTextColor(TFT_GREEN, TFT_BLACK);
//     tft.println("Calibration complete!");

//     // store data
//     File f = SPIFFS.open(CALIBRATION_FILE, "w");
//     if (f) {
//       f.write((const unsigned char *)calData, 14);
//       f.close();
//     }
//   }
// }

// //------------------------------------------------------------------------------------------

// // Print something in the mini status bar
// void status(const char *msg) {
//   tft.setTextPadding(240);
//   //tft.setCursor(STATUS_X, STATUS_Y);
//   tft.setTextColor(TFT_WHITE, TFT_DARKGREY);
//   tft.setTextFont(0);
//   tft.setTextDatum(TC_DATUM);
//   tft.setTextSize(1);
//   tft.drawString(msg, STATUS_X, STATUS_Y);
// }

// void setup() {
//   // Use serial port
//   Serial.begin(115200);

//   // Initialise the TFT screen
//   tft.init();

//   // Set the rotation before we calibrate
//   tft.setRotation(0);

//   // Calibrate the touch screen and retrieve the scaling factors
//   touch_calibrate();

//   // Clear the screen
//   tft.fillScreen(TFT_BLACK);

//   // Draw keypad background
//   tft.fillRect(0, 0, 240, 320, TFT_DARKGREY);

//   // Draw number display area and frame
//   tft.fillRect(DISP_X, DISP_Y, DISP_W, DISP_H, TFT_BLACK);
//   tft.drawRect(DISP_X, DISP_Y, DISP_W, DISP_H, TFT_WHITE);

//   // Draw keypad
//   drawKeypad();
// }

// //------------------------------------------------------------------------------------------

// void loop(void) {
//   uint16_t t_x = 0, t_y = 0; // To store the touch coordinates

//   // Pressed will be set true is there is a valid touch on the screen
//   bool pressed = tft.getTouch(&t_x, &t_y);

//   // / Check if any key coordinate boxes contain the touch coordinates
//   for (uint8_t b = 0; b < 15; b++) {
//     if (pressed && key[b].contains(t_x, t_y)) {
//       key[b].press(true);  // tell the button it is pressed
//     } else {
//       key[b].press(false);  // tell the button it is NOT pressed
//     }
//   }

//   // Check if any key has changed state
//   for (uint8_t b = 0; b < 15; b++) {

//     if (b < 3) tft.setFreeFont(LABEL1_FONT);
//     else tft.setFreeFont(LABEL2_FONT);

//     if (key[b].justReleased()) key[b].drawButton();     // draw normal

//     if (key[b].justPressed()) {
//       key[b].drawButton(true);  // draw invert

//       // if a numberpad button, append the relevant # to the numberBuffer
//       if (b >= 3) {
//         if (numberIndex < NUM_LEN) {
//           numberBuffer[numberIndex] = keyLabel[b][0];
//           numberIndex++;
//           numberBuffer[numberIndex] = 0; // zero terminate
//         }
//         status(""); // Clear the old status
//       }

//       // Del button, so delete last char
//       if (b == 1) {
//         numberBuffer[numberIndex] = 0;
//         if (numberIndex > 0) {
//           numberIndex--;
//           numberBuffer[numberIndex] = 0;//' ';
//         }
//         status(""); // Clear the old status
//       }

//       if (b == 2) {
//         status("Sent value to serial port");
//         Serial.println(numberBuffer);
//       }
//       // we dont really check that the text field makes sense
//       // just try to call
//       if (b == 0) {
//         status("Value cleared");
//         numberIndex = 0; // Reset index to 0
//         numberBuffer[numberIndex] = 0; // Place null in buffer
//       }

//       // Update the number display field
//       tft.setTextDatum(TL_DATUM);        // Use top left corner as text coord datum
//       tft.setFreeFont(&FreeSans18pt7b);  // Choose a nicefont that fits box
//       tft.setTextColor(DISP_TCOLOR);     // Set the font colour

//       // Draw the string, the value returned is the width in pixels
//       int xwidth = tft.drawString(numberBuffer, DISP_X + 4, DISP_Y + 12);

//       // Now cover up the rest of the line up by drawing a black rectangle.  No flicker this way
//       // but it will not work with italic or oblique fonts due to character overlap.
//       tft.fillRect(DISP_X + 4 + xwidth, DISP_Y + 1, DISP_W - xwidth - 5, DISP_H - 2, TFT_BLACK);

//       delay(10); // UI debouncing
//     }
//   }
// }


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
// #include <Arduino.h>
// #include <BLEDevice.h>
// #include <BLEServer.h>
// #include <BLEUtils.h>
// #include <BLE2902.h>
// // #include "common.h"

// uint8_t txValue = 0;
// BLEServer *pServer = NULL;                   //BLEServer指针 pServer
// BLECharacteristic *pTxCharacteristic = NULL; //BLECharacteristic指针 pTxCharacteristic
// bool deviceConnected = false;                //本次连接状态
// bool oldDeviceConnected = false;             //上次连接状态

// // See the following for generating UUIDs: https://www.uuidgenerator.net/
// #define SERVICE_UUID "3edbfa93-d64b-43c0-9d3c-e7388d43cd2f" // UART service UUID
// #define CHARACTERISTIC_UUID_RX "b42546cf-eac9-4b06-9377-17d71468e337"
// #define CHARACTERISTIC_UUID_TX "89b8d897-1faa-4c1d-9237-2aaf3584484c"

// class MyServerCallbacks : public BLEServerCallbacks
// {
//     void onConnect(BLEServer *pServer)
//     {
//         deviceConnected = true;
//     };

//     void onDisconnect(BLEServer *pServer)
//     {
//         deviceConnected = false;
//     }
// };

// class MyCallbacks : public BLECharacteristicCallbacks
// {
//     void onWrite(BLECharacteristic *pCharacteristic)
//     {
//         std::string rxValue = pCharacteristic->getValue(); //接收信息

//         if (rxValue.length() > 0)
//         { //向串口输出收到的值
//             Serial.print("RX: ");
//             for (int i = 0; i < rxValue.length(); i++)
//                 Serial.print(rxValue[i]);
//             Serial.println();
//         }
//     }
// };

// void setup()
// {
//     Serial.begin(115200);

//     // 创建一个 BLE 设备
//     BLEDevice::init("UART_BLE");

//     // 创建一个 BLE 服务
//     pServer = BLEDevice::createServer();
//     pServer->setCallbacks(new MyServerCallbacks()); //设置回调
//     BLEService *pService = pServer->createService(SERVICE_UUID);

//     // 创建一个 BLE 特征
//     pTxCharacteristic = pService->createCharacteristic(CHARACTERISTIC_UUID_TX, BLECharacteristic::PROPERTY_NOTIFY);
//     pTxCharacteristic->addDescriptor(new BLE2902());
//     BLECharacteristic *pRxCharacteristic = pService->createCharacteristic(CHARACTERISTIC_UUID_RX, BLECharacteristic::PROPERTY_WRITE);
//     pRxCharacteristic->setCallbacks(new MyCallbacks()); //设置回调

//     pService->start();                  // 开始服务
//     pServer->getAdvertising()->start(); // 开始广播
//     Serial.println(" waiting for connection... ");
// }

// void loop()
// {
//     // deviceConnected 已连接
//     if (deviceConnected)
//     {
//         pTxCharacteristic->setValue(&txValue, 1); // 设置要发送的值为1
//         pTxCharacteristic->notify();              // 广播
//         txValue++;                                // 指针地址自加1
//         delay(2000);                              // 如果有太多包要发送，蓝牙会堵塞
//     }

//     // disconnecting  断开连接
//     if (!deviceConnected && oldDeviceConnected)
//     {
//         delay(500);                  // 留时间给蓝牙缓冲
//         pServer->startAdvertising(); // 重新广播
//         Serial.println(" begin broadcast ");
//         oldDeviceConnected = deviceConnected;
//     }

//     // connecting  正在连接
//     if (deviceConnected && !oldDeviceConnected)
//     {
//         // do stuff here on connecting
//         oldDeviceConnected = deviceConnected;
//     }
// }