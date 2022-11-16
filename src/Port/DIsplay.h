#ifndef __DISPLAY_H
#define __DISPLAY_H

#include <TFT_eSPI.h>
// #include "App/Configs/Config.h"
#include "lvgl.h"
#include "FS.h"

// #include "TFT_eSPI.h"
typedef TFT_eSPI SCREEN_CLASS;
static TFT_eSPI tft = TFT_eSPI(); /* TFT instance */

#define CS_PIN          9
#define TRANSFER_TO_EINK  digitalWrite(TFT_CS,HIGH);digitalWrite(CS_PIN,LOW);
#define TRANSFER_TO_LCD digitalWrite(CS_PIN,HIGH);digitalWrite(TFT_CS,LOW);

static const uint16_t screenWidth  = 320;
static const uint16_t screenHeight = 240;

static lv_disp_draw_buf_t draw_buf;
// static lv_disp_draw_buf_t draw_buf_eink;
static lv_color_t buf[ screenWidth * 50 ];
// static lv_color_t buf_eink[ screenWidth * 30 ];

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
// void DisplayFault_Init(SCREEN_CLASS* scr);
// void lv_port_disp_init(SCREEN_CLASS* scr);
// void lv_fs_if_init();
// void lv_port_indev_init();

extern TaskHandle_t handleTaskLvgl;

#endif
