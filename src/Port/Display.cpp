#include "Display.h"
// #include "FS.h"
TFT_eSPI tft = TFT_eSPI();
TaskHandle_t handleTaskLvgl;
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
/* Display flushing */
// void my_disp_flush( lv_disp_drv_t *disp, const lv_area_t *area, lv_color_t *color_p )
// {
//     uint32_t w = ( area->x2 - area->x1 + 1 );
//     uint32_t h = ( area->y2 - area->y1 + 1 );

//     tft.startWrite();
//     tft.setAddrWindow( area->x1, area->y1, w, h );
//     tft.pushColors( ( uint16_t * )&color_p->full, w * h, true );
//     tft.endWrite();

//     lv_disp_flush_ready( disp );
// }

/*Read the touchpad*/
// static void my_touchpad_read( lv_indev_drv_t * indev_driver, lv_indev_data_t * data )
// {
//     uint16_t touchX, touchY;

//     bool touched = tft.getTouch( &touchX, &touchY);

//     if( !touched )
//     {
//         data->state = LV_INDEV_STATE_REL;
//     }
//     else
//     {
//         data->state = LV_INDEV_STATE_PR;

//         /*Set the coordinates*/
//         data->point.x = touchX;
//         data->point.y = touchY;

//     }
// }
// lv_indev_t* touch_indev;

void Port_Init(){


    tft.begin();          /* TFT init */
    tft.setRotation( 3 ); /* Landscape orientation, flipped */
    tft.fillScreen(TFT_BLACK);

    lv_init();

     /*Set the touchscreen calibration data,
     the actual data for your display can be aquired using
     the Generic -> Touch_calibrate example from the TFT_eSPI library*/
    #ifdef TOUCH_CS
    touch_calibrate();
    Serial.println( "calib" );

    uint16_t calData[5] = { 275, 3620, 264, 3532, 0x02 };//1:0x01,tftrotate2:0x02,3:0x07
    tft.setTouch( calData );
    #endif


    lv_port_disp_init(&tft);
   
    // lv_disp_draw_buf_init( &draw_buf, buf, NULL, screenWidth * 80 );

    // /*Initialize the display*/
    // static lv_disp_drv_t disp_drv;
    // lv_disp_drv_init( &disp_drv );
    // /*Change the following line to your display resolution*/
    // disp_drv.hor_res = screenWidth;
    // disp_drv.ver_res = screenHeight;
    // disp_drv.flush_cb = my_disp_flush;
    // disp_drv.draw_buf = &draw_buf;
    // lv_disp_drv_register( &disp_drv );

    //  Serial.println( "buff" );
    // #ifdef TOUCH_CS
    lv_port_indev_init();
    // #endif
    
    /*Initialize the (dummy) input device driver*/
    // static lv_indev_drv_t indev_drv;
    // lv_indev_drv_init( &indev_drv );
    // indev_drv.type = LV_INDEV_TYPE_POINTER;
    // indev_drv.read_cb = my_touchpad_read;
    // touch_indev = lv_indev_drv_register( &indev_drv );
    // Serial.println( "indev" );
    // lv_group_t* group = lv_group_create();
    // lv_indev_set_group(touch_indev, group);
    // lv_group_set_default(group);
    // Serial.printf( "displaytest:%d\n",test );
    lv_fs_if_init();

    xTaskCreate(
        TaskLvglUpdate,
        "LvglThread",
        20000,
        nullptr,
        configMAX_PRIORITIES - 1,
        // NULL);
        &handleTaskLvgl);
    
}