#include "DialplateView.h"
#include "../../Configs/Config.h"
#include <stdarg.h>
#include <stdio.h>

#define ARRAY_SIZE(arr) (sizeof(arr)/sizeof(arr[0]))

using namespace Page;

void DialplateView::Create(lv_obj_t* root)
{
    lv_obj_remove_style_all(root);
    lv_obj_set_size(root, LV_HOR_RES, LV_VER_RES);

    BottomInfo_Create(root);
    TopInfo_Create(root);
    BtnCont_Create(root);

    ui.anim_timeline = lv_anim_timeline_create();

#define ANIM_DEF(start_time, obj, attr, start, end) \
    {start_time, obj, LV_ANIM_EXEC(attr), start, end, 500, lv_anim_path_ease_out, true}

#define ANIM_OPA_DEF(start_time, obj) \
    ANIM_DEF(start_time, obj, opa_scale, LV_OPA_TRANSP, LV_OPA_COVER)

    lv_coord_t y_tar_top = lv_obj_get_y(ui.topInfo.cont);
    lv_coord_t y_tar_bottom = lv_obj_get_y(ui.bottomInfo.cont);
    lv_coord_t h_tar_btn = lv_obj_get_height(ui.btnCont.btnRec);

    lv_anim_timeline_wrapper_t wrapper[] =
    {
        ANIM_DEF(0, ui.topInfo.cont, y, -lv_obj_get_height(ui.topInfo.cont), y_tar_top),

        ANIM_DEF(200, ui.bottomInfo.cont, y, -lv_obj_get_height(ui.bottomInfo.cont), y_tar_bottom),
        ANIM_OPA_DEF(200, ui.bottomInfo.cont),

        ANIM_DEF(500, ui.btnCont.btnMap, height, 0, h_tar_btn),
        ANIM_DEF(600, ui.btnCont.btnRec, height, 0, h_tar_btn),
        ANIM_DEF(700, ui.btnCont.btnMenu, height, 0, h_tar_btn),
        LV_ANIM_TIMELINE_WRAPPER_END
    };
    lv_anim_timeline_add_wrapper(ui.anim_timeline, wrapper);
}

void DialplateView::Delete()
{
    if(ui.anim_timeline)
    {
        lv_anim_timeline_del(ui.anim_timeline);
        ui.anim_timeline = nullptr;
    }
}

void DialplateView::TopInfo_Create(lv_obj_t* par)
{
    lv_obj_t* cont = lv_obj_create(par);
    lv_obj_remove_style_all(cont);
    lv_obj_set_size(cont, 254, 254);// LV_HOR_RES, LV_HOR_RES);// 7.0 / 12 * LV_VER_RES);

    lv_obj_set_style_bg_opa(cont, LV_OPA_COVER, 0);
    lv_obj_set_style_bg_color(cont, COLOR_DIM_TEXT, 0);

    lv_obj_set_style_radius(cont, 254/2, 0);
    lv_obj_align(cont, LV_ALIGN_TOP_MID, 0, -254 / 2 + 25);
    //lv_obj_set_y(cont, -254/2+25);
    ui.topInfo.cont = cont;

    //subcont form ring
    lv_obj_t* subcont = lv_obj_create(cont);
    lv_obj_remove_style_all(subcont);
    lv_obj_set_size(subcont, 180, 180);// LV_HOR_RES, LV_HOR_RES);// 7.0 / 12 * LV_VER_RES);

    lv_obj_set_style_bg_opa(subcont, LV_OPA_COVER, 0);
    lv_obj_set_style_bg_color(subcont, COLOR_BACKGROUND, 0);

    lv_obj_set_style_radius(subcont, 180 / 2, 0);
    lv_obj_align(subcont, LV_ALIGN_CENTER, 0, 0);
    //lv_obj_set_y(subcont, -254/2+25);
    ui.topInfo.subcont = subcont;

    lv_obj_t* label = lv_label_create(cont);
    lv_obj_set_style_text_font(label, ResourcePool::GetFont("barlow_conExBoIt_85"), 0);
    //lv_obj_set_style_text_font(label, &lv_font_montserrat_48, 0);
    lv_obj_set_style_text_color(label, COLOR_TEXT, 0);
    lv_label_set_text(label, "00");
    lv_obj_align(label, LV_ALIGN_CENTER, 0, 40);
    ui.topInfo.labelSpeed = label;

    label = lv_label_create(cont);
    lv_obj_set_style_text_font(label, ResourcePool::GetFont("barlow_conExBoIt_30"), 0);
    lv_obj_set_style_text_color(label, COLOR_TEXT, 0);
    lv_label_set_text(label, "KM/H");
    lv_obj_align_to(label, ui.topInfo.labelSpeed, LV_ALIGN_OUT_BOTTOM_MID, 0, 5);
    ui.topInfo.labelUint = label;
}

void DialplateView::BottomInfo_Create(lv_obj_t* par)
{
    lv_obj_t* cont = lv_obj_create(par);
    lv_obj_remove_style_all(cont);
    lv_obj_set_style_bg_color(cont, COLOR_BACKGROUND, 0);
    lv_obj_set_size(cont, LV_HOR_RES, 2.9/8.0*LV_VER_RES);
    lv_obj_align(cont, LV_ALIGN_TOP_MID, 0, 5.7 / 12 * LV_VER_RES);

    lv_obj_set_flex_flow(cont, LV_FLEX_FLOW_ROW_WRAP);

    lv_obj_set_flex_align(
        cont,
        LV_FLEX_ALIGN_SPACE_EVENLY,
        LV_FLEX_ALIGN_CENTER,
        LV_FLEX_ALIGN_CENTER
    );

    ui.bottomInfo.cont = cont;

    const char* unitText[4] =
    {
        "AVG",
        "Time",
        "Trip",
        "Calorie"
    };

    for (int i = 0; i < ARRAY_SIZE(ui.bottomInfo.labelInfoGrp); i++)
    {
        SubInfoGrp_Create(
            cont,
            &(ui.bottomInfo.labelInfoGrp[i]),
            unitText[i]
        );
    }
}

void DialplateView::SubInfoGrp_Create(lv_obj_t* par, SubInfo_t* info, const char* unitText)
{
    lv_obj_t* cont = lv_obj_create(par);
    lv_obj_remove_style_all(cont);
    lv_obj_set_size(cont, 93, 50);

    lv_obj_set_flex_flow(cont, LV_FLEX_FLOW_COLUMN);
    lv_obj_set_flex_align(
        cont,
        LV_FLEX_ALIGN_SPACE_AROUND,
        LV_FLEX_ALIGN_CENTER,
        LV_FLEX_ALIGN_CENTER
    );

    lv_obj_t* label = lv_label_create(cont);
    lv_obj_set_style_text_font(label, ResourcePool::GetFont("barlow_conExBoIt_23"), 0);
    lv_obj_set_style_text_color(label, COLOR_TEXT, 0);
    info->lableValue = label;

    label = lv_label_create(cont);
    lv_obj_set_style_text_font(label, ResourcePool::GetFont("barlow_conExBoIt_17"), 0);
    lv_obj_set_style_text_color(label, COLOR_DIM_TEXT, 0);
    lv_label_set_text(label, unitText);
    info->lableUnit = label;

    info->cont = cont;
}

void DialplateView::BtnCont_Create(lv_obj_t* par)
{
    lv_obj_t* cont = lv_obj_create(par);
    lv_obj_remove_style_all(cont);
    lv_obj_set_size(cont, LV_HOR_RES, 40);
    lv_obj_align_to(cont, ui.bottomInfo.cont, LV_ALIGN_OUT_BOTTOM_MID, 0, 0);

    /*lv_obj_set_flex_flow(cont, LV_FLEX_FLOW_ROW);
    lv_obj_set_flex_place(
        cont,
        LV_FLEX_PLACE_SPACE_AROUND,
        LV_FLEX_PLACE_CENTER,
        LV_FLEX_PLACE_CENTER
    );*/

    ui.btnCont.cont = cont;

    ui.btnCont.btnMap = Btn_Create(cont, ResourcePool::GetImage("locate"), -80);
    ui.btnCont.btnRec = Btn_Create(cont, ResourcePool::GetImage("start"), 0);
    ui.btnCont.btnMenu = Btn_Create(cont, ResourcePool::GetImage("menu"), 80);
}

lv_obj_t* DialplateView::Btn_Create(lv_obj_t* par, const void* img_src, lv_coord_t x_ofs)
{
    lv_obj_t* obj = lv_obj_create(par);
    lv_obj_remove_style_all(obj);
    lv_obj_set_size(obj, 40, 31);
    lv_obj_clear_flag(obj, LV_OBJ_FLAG_SCROLLABLE);

    lv_obj_align(obj, LV_ALIGN_CENTER, x_ofs, 0);
    lv_obj_set_style_bg_img_src(obj, img_src, 0);

    lv_obj_set_style_bg_opa(obj, LV_OPA_COVER, 0);
    lv_obj_set_style_width(obj, 45, LV_STATE_PRESSED);
    lv_obj_set_style_height(obj, 25, LV_STATE_PRESSED);
    lv_obj_set_style_bg_color(obj, COLOR_UNFOCUS, 0);
    lv_obj_set_style_bg_color(obj, COLOR_PRESSED, LV_STATE_PRESSED);
    lv_obj_set_style_bg_color(obj, COLOR_FOCUS, LV_STATE_FOCUSED);
    lv_obj_set_style_radius(obj, 9, 0);

    static lv_style_transition_dsc_t tran;
    static const lv_style_prop_t prop[] = { LV_STYLE_WIDTH, LV_STYLE_HEIGHT, LV_STYLE_PROP_INV};
    lv_style_transition_dsc_init(
        &tran,
        prop,
        lv_anim_path_ease_out,
        200,
        0,
        nullptr
    );
    lv_obj_set_style_transition(obj, &tran, LV_STATE_PRESSED);
    lv_obj_set_style_transition(obj, &tran, LV_STATE_FOCUSED);

    lv_obj_update_layout(obj);

    return obj;
}

void DialplateView::AppearAnimStart(bool reverse)
{
    lv_anim_timeline_set_reverse(ui.anim_timeline, reverse);
    lv_anim_timeline_start(ui.anim_timeline);
}
