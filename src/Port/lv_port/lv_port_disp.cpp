#include "Port/Display.h"
#include "epd1in54_V2.h"
#include "epdpaint.h"
#include "imagedata.h"

#define DISP_HOR_RES         240
#define DISP_VER_RES         320
#define EINK_DISP_HOR_RES EPD_HEIGHT
#define EINK_DISP_VER_RES EPD_WIDTH
#define DISP_BUF_SIZE        screenWidth * 50

//static lv_color_t lv_full_disp_buf[DISP_BUF_SIZE];
lv_color_t* lv_disp_buf_p;

// static lv_disp_draw_buf_t disp_buf;
// static lv_disp_drv_t disp_drv;

extern lv_disp_t * disp_lcd;
extern lv_disp_t * disp_eink;

Epd epdControl;
Paint einkImg(IMAGE_DATA, EPD_WIDTH, EPD_HEIGHT);


void my_print(lv_log_level_t level, const char* file, uint32_t line, const char* fun, const char* dsc)
{
    Serial.printf("%s@%d %s->%s\r\n", file, line, fun, dsc);
    Serial.flush();
}

/**
  * @brief  ��Ļˢ�»ص�����
  * @param  disp:��Ļ������ַ
  * @param  area:ˢ������
  * @param  color_p:ˢ�»�������ַ
  * @retval ��
  */
static void disp_flush_cb(lv_disp_drv_t* disp, const lv_area_t* area, lv_color_t* color_p)
{
    SCREEN_CLASS* screen = (SCREEN_CLASS*) disp->user_data;

    int32_t w = (area->x2 - area->x1 + 1);
    int32_t h = (area->y2 - area->y1 + 1);

    screen->startWrite();
    screen->setAddrWindow(area->x1, area->y1, w, h);
    screen->pushColors((uint16_t*) (&color_p->full), w * h, true);
    screen->endWrite();

    lv_disp_flush_ready(disp);
    Serial.printf("LCD: y1 %d,y2 %d,x1 %d,x2 %d\r\n",area->y1,area->y2,area->x1,area->x2);

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
  // if(area->y2>=120)
    epdControl.Display(einkImg.GetImage());
  Serial.printf("y1 %d,y2 %d,x1 %d,x2 %d\r\n",area->y1,area->y2,area->x1,area->x2);
  // Serial.println("my_disp_flush: all pixel updated");

  lv_disp_flush_ready(disp); /* Indicate you are ready with the flushing*/
}

static void disp_wait_cb(lv_disp_drv_t* disp_drv)
{
//    __wfi();
}

/**
  * @brief  ��Ļ��ʼ��
  * @param  ��
  * @retval ��
  */
void lv_port_disp_lcd_init(SCREEN_CLASS* scr)
{
  // lv_log_register_print_cb(
  //     reinterpret_cast<lv_log_print_g_cb_t>(my_print)); /* register print function for debugging */

  /* Move the malloc process to Init() to make sure that the largest heap can be used for this buffer.
    *
  lv_disp_buf_p = static_cast<lv_color_t*>(malloc(DISP_BUF_SIZE * sizeof(lv_color_t)));
  if (lv_disp_buf_p == nullptr)
      LV_LOG_WARN("lv_port_disp_init malloc failed!\n");
  */
  static lv_disp_drv_t disp_drv;
  lv_disp_draw_buf_init(&disp_buf, lv_disp_buf_p, nullptr, DISP_BUF_SIZE);

  /*Initialize the display*/
  lv_disp_drv_init(&disp_drv);
  disp_drv.hor_res = DISP_HOR_RES;
  disp_drv.ver_res = DISP_VER_RES; 
  disp_drv.flush_cb = disp_flush_cb;
  disp_drv.wait_cb = disp_wait_cb;
  disp_drv.draw_buf = &disp_buf;
  disp_drv.user_data = scr;
  lv_disp_drv_register(&disp_drv);

  disp_lcd=lv_disp_drv_register( &disp_drv );
  lv_disp_set_default(disp_lcd);
}

void lv_port_disp_eink_init(){
  static lv_disp_drv_t disp_drv; 
  lv_disp_draw_buf_init( &disp_buf, buf, NULL, screenWidth * 50 );
                                                 //Descriptor of a display driver
  lv_disp_drv_init(&disp_drv);                                                    //Basic initialization
  disp_drv.flush_cb = my_disp_flush_eink;                                              //Set your driver function
  // disp_drv.draw_buf = &draw_buf_eink;                                                  //Assign the buffer to the display
  disp_drv.draw_buf = &disp_buf; 
  disp_drv.hor_res = EINK_DISP_HOR_RES;                                             //Set the horizontal resolution of the display
  disp_drv.ver_res = EINK_DISP_VER_RES;                                             //Set the vertical resolution of the display
  // disp_drv.sw_rotate=1;
  // disp_drv.rotated = LV_DISP_ROT_90;                                          //Set the rotation the display to 90
  disp_eink=lv_disp_drv_register(&disp_drv);  
  lv_disp_set_default(disp_eink); 
}
