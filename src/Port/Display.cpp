#include "Display.h"

#include "epd2in13_V3.h"
#include "epdpaint.h"
#include "imagedata.h"

#define DISP_VER_RES EPD_WIDTH   //122 x axis of Paint class
#define DISP_HOR_RES EPD_HEIGHT  //250
#define MY_DISP_HOR_RES DISP_HOR_RES
#define MY_DISP_VER_RES DISP_VER_RES
// #include "FS.h"
lv_disp_t * disp_lcd;
lv_disp_t * disp_eink;

lv_obj_t * scr_lcd;
lv_obj_t * scr_eink;

Epd epdControl;
Paint einkImg(IMAGE_DATA, EPD_WIDTH, EPD_HEIGHT);
TaskHandle_t handleTaskLvgl;
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

/* Display flushing */
void my_disp_flush( lv_disp_drv_t *disp, const lv_area_t *area, lv_color_t *color_p )
{
    uint32_t w = ( area->x2 - area->x1 + 1 );
    uint32_t h = ( area->y2 - area->y1 + 1 );

    tft.startWrite();
    tft.setAddrWindow( area->x1, area->y1, w, h );
    tft.pushColors( ( uint16_t * )&color_p->full, w * h, true );
    tft.endWrite();

    lv_disp_flush_ready( disp );
    Serial.printf("LCD: y1 %d,y2 %d,x1 %d,x2 %d\r\n",area->y1,area->y2,area->x1,area->x2);

    // Serial.println("my_disp_flush: all pixel updated");
}

/*Read the touchpad*/
void my_touchpad_read( lv_indev_drv_t * indev_driver, lv_indev_data_t * data )
{
    uint16_t touchX, touchY;

    bool touched = tft.getTouch( &touchX, &touchY);

    if( !touched )
    {
        data->state = LV_INDEV_STATE_REL;
    }
    else
    {
        data->state = LV_INDEV_STATE_PR;

        /*Set the coordinates*/
        data->point.x = touchX;
        data->point.y = touchY;

    }
    
}
void set_pixel(int32_t x, int32_t y, lv_color_t color_p)
{
  int colorToDraw;
  bool setToBlack = ((color_p.ch.red >= 20) && 
                    (color_p.ch.green >= 30) && 
                    (color_p.ch.blue >= 20) );
  colorToDraw = int(setToBlack);
  einkImg.DrawPixel(x, y, colorToDraw);
}
void my_disp_flush_eink(lv_disp_drv_t *disp, const lv_area_t *area, lv_color_t *color_p)
{
  int32_t x, y;
  /*It's a very slow but simple implementation.
   *`set_pixel` needs to be written by you to a set pixel on the screen*/
  for (y = area->y1; y <= area->y2; y++)
  {
    for (x = area->x1; x <= area->x2; x++)
    {
      set_pixel(x, y, *color_p);
      
      color_p++;
    }
  }
  if(area->y2>=120)
    epdControl.Display(einkImg.GetImage());
  Serial.printf("y1 %d,y2 %d,x1 %d,x2 %d\r\n",area->y1,area->y2,area->x1,area->x2);
  // Serial.println("my_disp_flush: all pixel updated");

  lv_disp_flush_ready(disp); /* Indicate you are ready with the flushing*/
}


void Port_Init_Eink(){
  epdControl.Init(FULL);
  // lv_init();

    // lv_disp_draw_buf_init( &draw_buf_eink, buf_eink, NULL, screenWidth * 30 );

  lv_disp_draw_buf_init(&draw_buf_eink, buf_eink, NULL, screenWidth * 30); //Initialize the display buffer.
  static lv_disp_drv_t disp_drv;                                                  //Descriptor of a display driver
  lv_disp_drv_init(&disp_drv);                                                    //Basic initialization
  disp_drv.flush_cb = my_disp_flush_eink;                                              //Set your driver function
  disp_drv.draw_buf = &draw_buf_eink;                                                  //Assign the buffer to the display
  disp_drv.hor_res = MY_DISP_HOR_RES;                                             //Set the horizontal resolution of the display
  disp_drv.ver_res = MY_DISP_VER_RES;                                             //Set the vertical resolution of the display
  // disp_drv.sw_rotate=1;
  // disp_drv.rotated = LV_DISP_ROT_90;                                          //Set the rotation the display to 90
  disp_eink=lv_disp_drv_register(&disp_drv);  
  lv_disp_set_default(disp_eink); 
  scr_eink = lv_scr_act();                                             //Finally register the driver
  // spi.endTransaction();
  // lv_example_get_started_1();
  
  Serial.println("end of set up");
      
}
void End_spi_transaction(){
  spi.endTransaction();
}
void Port_Init(){
    static bool inited=false;
    Serial.println("test");
    tft.begin();          /* TFT init */
    tft.setRotation( 2 ); /* Landscape orientation, flipped */
    tft.fillScreen(TFT_BLACK);
    if(!inited){
    lv_init();
    
    
    Serial.println("test");
    // }
    lv_disp_draw_buf_init( &draw_buf, buf, NULL, screenWidth * 50 );
    Serial.println("test");
    

    // lv_init();
    // lv_port_disp_init(&tft);

    /*Set the touchscreen calibration data,
     the actual data for your display can be aquired using
     the Generic -> Touch_calibrate example from the TFT_eSPI library*/
    touch_calibrate();
    uint16_t calData[5] = { 275, 3620, 264, 3532, 0x02 };
    tft.setTouch( calData );

    // lv_disp_draw_buf_init( &draw_buf, buf, NULL, screenWidth * 50 );

    /*Initialize the display*/
    static lv_disp_drv_t disp_drv;
    lv_disp_drv_init( &disp_drv );
    Serial.println("test");
    /*Change the following line to your display resolution*/
    disp_drv.hor_res = screenHeight;
    disp_drv.ver_res = screenWidth ;
    disp_drv.flush_cb = my_disp_flush;
    disp_drv.draw_buf = &draw_buf;
    disp_lcd=lv_disp_drv_register( &disp_drv );
    Serial.println("test");

    // lv_port_indev_init();
    
    /*Initialize the (dummy) input device driver*/
    static lv_indev_drv_t indev_drv;
    lv_indev_drv_init( &indev_drv );
    indev_drv.type = LV_INDEV_TYPE_POINTER;
    indev_drv.read_cb = my_touchpad_read;
    lv_indev_drv_register( &indev_drv );
    Serial.println("test");
        scr_lcd = lv_scr_act();
    }
    
    inited=true;
    lv_disp_set_default(disp_lcd);
    Serial.println("test");
    // lv_fs_if_init();
    

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
}