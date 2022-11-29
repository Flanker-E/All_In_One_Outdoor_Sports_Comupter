#ifndef __SETTINGS_VIEW_H
#define __SETTINGS_VIEW_H

#include "../Page.h"

namespace Page
{

class SettingsView
{
public:
    void Create(lv_obj_t* root);
    void Delete();
    void SetScrollToY(lv_obj_t* obj, lv_coord_t y, lv_anim_enable_t en);
    static void onFocus(lv_group_t* g);
    void Group_Init();
    void Style_Init();

public:
    // struct
    // {
    //     lv_obj_t* cont;
    //     // lv_obj_t* labelLogo;
    //     struct
    //     {
    //         lv_obj_t* cont;
    //         lv_obj_t* btnInfos;
    //         // lv_obj_t* btnRec;
    //         lv_obj_t* btnBack;
    //     } btnCont;

    //     lv_anim_timeline_t* anim_timeline;
    // } ui;

    typedef struct
    {
        lv_obj_t* cont;
        lv_obj_t* icon;
        lv_obj_t* labelInfo;
        lv_obj_t* button;
    } item_t;

    struct
    {
        item_t info;
        item_t back;
        item_t ble;
        item_t wifi;
        // item_t mag;
        // item_t imu;
        // item_t rtc;
        // item_t battery;
        // item_t storage;
        // item_t system;
    } ui;

private:
    struct
    {
        lv_style_t icon;
        lv_style_t focus;
        lv_style_t info;
        lv_style_t data;
    } style;

private:
    
    
    void Style_Reset();
    void Item_Create(
        item_t* item,
        lv_obj_t* par,
        const char* name,
        const char* img_src,
        const char* infos
    );
    // void BtnCont_Create(lv_obj_t* par);
    // lv_obj_t* Btn_Create(lv_obj_t* par, const void* img_src, lv_coord_t x_ofs);
};

}

#endif // !__VIEW_H