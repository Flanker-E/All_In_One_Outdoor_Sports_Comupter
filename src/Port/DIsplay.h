#ifndef __DISPLAY_H
#define __DISPLAY_H

#include <TFT_eSPI.h>
// #include "App/Configs/Config.h"
#include "lvgl.h"
#include "FS.h"

// #include "TFT_eSPI.h"
typedef TFT_eSPI SCREEN_CLASS;
static TFT_eSPI tft = TFT_eSPI(); /* TFT instance */

static const uint16_t screenWidth  = 320;
static const uint16_t screenHeight = 240;

static lv_disp_draw_buf_t draw_buf;
static lv_color_t buf[ screenWidth * 40 ];

// TFT_eSPI tft = TFT_eSPI(); /* TFT instance */
#define CALIBRATION_FILE "/TouchCalData1"

#define REPEAT_CAL false
// typedef TFT_eSPI SCREEN_CLASS;

void Port_Init();
// void DisplayFault_Init(SCREEN_CLASS* scr);
// void lv_port_disp_init(SCREEN_CLASS* scr);
// void lv_fs_if_init();
// void lv_port_indev_init();

extern TaskHandle_t handleTaskLvgl;

#endif
