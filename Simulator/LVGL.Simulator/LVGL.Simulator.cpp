/*
 * PROJECT:   LVGL ported to Windows Desktop
 * FILE:      LVGL.Windows.Desktop.cpp
 * PURPOSE:   Implementation for LVGL ported to Windows Desktop
 *
 * LICENSE:   The MIT License
 *
 * DEVELOPER: Mouri_Naruto (Mouri_Naruto AT Outlook.com)
 */

#include <Windows.h>
#include <stdio.h>
#include "resource.h"
#include "App.h"
#include "Common/HAL/HAL.h"

#if _MSC_VER >= 1200
 // Disable compilation warnings.
#pragma warning(push)
// nonstandard extension used : bit field types other than int
#pragma warning(disable:4214)
// 'conversion' conversion from 'type1' to 'type2', possible loss of data
#pragma warning(disable:4244)
#endif

#include "lvgl/lvgl.h"
#include "lvgl/examples/lv_examples.h"
#include "lv_drivers/win32drv/win32drv.h"
#include "lv_fs_if/lv_fs_if.h"

#if _MSC_VER >= 1200
// Restore compilation warnings.
#pragma warning(pop)
#endif
lv_obj_t* scr_lcd = nullptr;
lv_obj_t* scr_eink = nullptr;

lv_disp_t* disp_lcd = nullptr;
lv_disp_t* disp_eink = nullptr;

#define SCREEN_HOR_RES  240
#define SCREEN_VER_RES  320

#define EINK_HOR_RES  200
#define EINK_VER_RES  200

int main()
{
    lv_init();
    printf("lvinit\r\n");
    lv_fs_if_init();
    printf("lvfs\r\n");
    if ((disp_lcd = lv_win32_2_init(
        GetModuleHandleW(NULL),
        SW_SHOW,
        SCREEN_HOR_RES,
        SCREEN_VER_RES,
        LoadIconW(GetModuleHandleW(NULL), MAKEINTRESOURCE(IDI_LVGL))))==nullptr)
    {
        return -1;
    }

    scr_lcd = lv_scr_act();

    if ((disp_eink = lv_win32_init(
        GetModuleHandleW(NULL),
        SW_SHOW,
        EINK_HOR_RES,
        EINK_VER_RES,
        LoadIconW(GetModuleHandleW(NULL), MAKEINTRESOURCE(IDI_LVGL))))==nullptr)
    {
        return -1;
    }
    lv_disp_set_default(disp_eink);
    scr_eink= lv_scr_act();
    printf("eink\r\n");
    
    lv_disp_set_default(disp_lcd);
    printf("lcd\r\n");
    lv_win32_add_all_input_devices_to_group(NULL);
    printf("hal\r\n");
    HAL::HAL_Init();  
    printf("app\r\n");
    App_Init();
    printf("loop\r\n");
    while (!lv_win32_quit_signal)
    {
        lv_timer_handler();
        HAL::HAL_Update();
        Sleep(1);
    }

    App_UnInit();

    return 0;
}
