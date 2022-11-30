#include "Display.h"
#include "epd1in54_V2.h"

lv_disp_t * disp_lcd;
lv_disp_t * disp_eink;

lv_obj_t * scr_lcd;
lv_obj_t * scr_eink;
volatile bool spi_started=false;
volatile bool isLCD=false;

TFT_eSPI tft = TFT_eSPI();
TaskHandle_t handleTaskLvgl;

extern Epd epdControl;
extern SPIClass spi;

void TaskLvglUpdate(void* parameter)
{
    ulTaskNotifyTake(pdTRUE, portMAX_DELAY);

    for (;;)
    {
        lv_task_handler();

        delay(5);
    }
}
#ifdef TOUCH_CS
void touch_calibrate()
{
  uint16_t calData[5];
  uint8_t calDataOK = 0;

  // check file system exists
  if (!SPIFFS.begin()) {
    Serial.println("Formating file system");
    SPIFFS.format();
    SPIFFS.begin();
  }

  // check if calibration file exists and size is correct
  if (SPIFFS.exists(CALIBRATION_FILE)) {
    if (REPEAT_CAL)
    {
      // Delete if we want to re-calibrate
      SPIFFS.remove(CALIBRATION_FILE);
    }
    else
    {
      fs::File f = SPIFFS.open(CALIBRATION_FILE, "r");
      if (f) {
        if (f.readBytes((char *)calData, 14) == 14)
          calDataOK = 1;
        f.close();
      }
    }
  }

  if (calDataOK && !REPEAT_CAL) {
    // calibration data valid
    tft.setTouch(calData);
    Serial.printf("Calibration data:");
    for(auto num:calData){
      Serial.printf("%d,",num);
    }
    Serial.printf("\n");
    Serial.println("Calibration data valid!");
  } else {
    // data not valid so recalibrate
    tft.fillScreen(TFT_BLACK);
    tft.setCursor(20, 0);
    tft.setTextFont(2);
    tft.setTextSize(1);
    tft.setTextColor(TFT_WHITE, TFT_BLACK);

    tft.println("Touch corners as indicated");
    Serial.println("Touch corners as indicated");
    tft.setTextFont(1);
    tft.println();

    if (REPEAT_CAL) {
      tft.setTextColor(TFT_RED, TFT_BLACK);
      tft.println("Set REPEAT_CAL to false to stop this running again!");
    }

    tft.calibrateTouch(calData, TFT_MAGENTA, TFT_BLACK, 15);

    tft.setTextColor(TFT_GREEN, TFT_BLACK);
    tft.println("Calibration complete!");
    Serial.println("Calibration complete!");

    // store data
    fs::File f = SPIFFS.open(CALIBRATION_FILE, "w");
    if (f) {
      f.write((const unsigned char *)calData, 14);
      f.close();
    }
  }
}
#endif

void End_spi_transaction(){
  spi.endTransaction();
}

void To_LCD_Port(){
  if(spi_started){
    End_spi_transaction();
    Serial.println("spi trans end");
    }
  TRANSFER_TO_LCD
  Port_Init();
  lv_disp_set_default(disp_lcd);
  Serial.println("switched to LCD"); 
}
void To_Eink_Port(){
  if(spi_started){
    End_spi_transaction();
    Serial.println("spi trans end");
    }
  TRANSFER_TO_EINK
  Port_Init_Eink();
  lv_disp_set_default(disp_eink);
  Serial.println("switched to Eink"); 
}

void Port_Init(){
    static bool inited=false;
    Serial.println("start of lcd init");

    tft.begin();          /* TFT init */
    spi_started=true;
    tft.setRotation( 3 ); /* Landscape orientation, flipped */
    tft.fillScreen(TFT_BLACK);

    if(!inited){
    lv_init();
    Serial.println("start of lcd lvgl driver init");
     /*Set the touchscreen calibration data,
     the actual data for your display can be aquired using
     the Generic -> Touch_calibrate example from the TFT_eSPI library*/
    #ifdef TOUCH_CS
    touch_calibrate();
    Serial.println( "calib" );

    uint16_t calData[5] = { 275, 3620, 264, 3532, 0x02 };//1:0x01,tftrotate2:0x02,3:0x07
    tft.setTouch( calData );
    #endif

    lv_port_disp_lcd_init(&tft);
   
    lv_port_indev_init();

    scr_lcd = lv_scr_act();

    lv_fs_if_init();
    // scr_lcd = lv_scr_act();
    Serial.println("end of lcd lvgl driver init");
    startFreeRtos();
    }
    
    inited=true;
    // lv_disp_set_default(disp_lcd);
    Serial.println("end of lcd init");

    // xTaskCreate(
    //     TaskLvglUpdate,
    //     "LvglThread",
    //     20000,
    //     nullptr,
    //     configMAX_PRIORITIES - 1,
    //     // NULL);
    //     &handleTaskLvgl);
    
}

void Port_Init_Eink(){
  static bool inited=false;
  Serial.println("begin of eink init");
  epdControl.LDirInit();
  if(!inited){
  Serial.println("begin of eink lvgl driver init");

  lv_port_disp_eink_init();
  
  scr_eink = lv_scr_act();                                             //Finally register the driver
  Serial.println("end of eink lvgl driver init");
  }
  inited=true;
  // spi.endTransaction();
  Serial.println("end of eink init");
      
}

void startFreeRtos(){
  xTaskCreate(
      TaskLvglUpdate,
      "LvglThread",
      20000,
      nullptr,
      configMAX_PRIORITIES - 1,
      // NULL);
      &handleTaskLvgl);
  // xTaskCreate(
  //     TaskEinkUpdate,
  //     "Update eink when detected semaphore",
  //     10000,
  //     nullptr,
  //     configMAX_PRIORITIES - 2,
  //     NULL);
}