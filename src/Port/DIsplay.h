#ifndef __DISPLAY_H
#define __DISPLAY_H

#include <TFT_eSPI.h>
// #include "App/Configs/Config.h"
#include "lvgl.h"
#include "FS.h"

// #include "TFT_eSPI.h"
typedef TFT_eSPI SCREEN_CLASS;
extern TFT_eSPI tft; /* TFT instance */
extern lv_indev_t* touch_indev;
extern lv_indev_t* encoder_indev;

#define CS_PIN          9
#define TRANSFER_TO_EINK  digitalWrite(TFT_CS,HIGH);digitalWrite(9,LOW);
#define TRANSFER_TO_LCD digitalWrite(9,HIGH);digitalWrite(TFT_CS,LOW);

static const uint16_t screenWidth  = 320;
static const uint16_t screenHeight = 240;

static lv_disp_draw_buf_t disp_buf;
// static lv_disp_draw_buf_t draw_buf_eink;
// static lv_color_t buf[ screenWidth * 50 ];
// // static lv_color_t buf_eink[ screenWidth * 30 ];

// TFT_eSPI tft = TFT_eSPI(); /* TFT instance */
#define CALIBRATION_FILE "/TouchCalData1"

#define REPEAT_CAL false
// typedef TFT_eSPI SCREEN_CLASS;
void To_LCD_Port();
void To_Eink_Port();
void Port_Init_Eink();
void Port_Init();
void End_spi_transaction();
void startFreeRtos();
void DisplayFault_Init(SCREEN_CLASS* scr);
void lv_port_disp_lcd_init(SCREEN_CLASS* scr);
void lv_port_disp_eink_init();
void lv_fs_if_init();
void lv_port_indev_init();

extern TaskHandle_t handleTaskLvgl;

#endif
