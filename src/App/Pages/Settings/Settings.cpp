#include "Settings.h"
#include "../../Utils/PageManager/PM_Log.h"
#ifdef ARDUINO
#  include "Arduino.h"
#endif
using namespace Page;

Settings::Settings(){}
Settings::~Settings(){}

void Settings::onCustomAttrConfig(){}

void Settings::onViewLoad(){
    Model.Init();
    View.Create(root);
    AttachEvent(root);
    AttachEvent(View.ui.info.icon);
    AttachEvent(View.ui.back.icon);
     //AttachEvent(View.ui.imu.icon);
    // AttachEvent(View.ui.mag.icon);
    // AttachEvent(View.ui.rtc.icon);
    // AttachEvent(View.ui.battery.icon);
    // AttachEvent(View.ui.storage.icon);
    // AttachEvent(View.ui.system.icon);
}

void Settings::onViewDidLoad(){}

void Settings::onViewWillAppear(){
    //Serial.println("onviewwillappear");
    lv_indev_wait_release(lv_indev_get_act());
    View.Group_Init();
    Model.SetStatusBarStyle(DataProc::STATUS_BAR_STYLE_BLACK);
    //Serial.println("onviewwillappear");
    // timer = lv_timer_create(onTimerUpdate, 1000, this);
    // lv_timer_ready(timer);

    View.SetScrollToY(root, -LV_VER_RES, LV_ANIM_OFF);
    //Serial.println("onviewwillappear");
    lv_obj_set_style_opa(root, LV_OPA_TRANSP, 0);
    //Serial.println("onviewwillappear");
    lv_obj_fade_in(root, 300, 0);
    //Serial.println("onviewwillappear");
    //Serial.println("onviewwillappear");
    // lv_indev_wait_release(lv_indev_get_act());
    // lv_group_t* group = lv_group_get_default();
    // lv_group_set_wrap(group, false);
    // lv_group_add_obj(group, View.ui.btnCont.btnInfos);
    // lv_group_add_obj(group, View.ui.btnCont.btnBack);
    // //Serial.println("btnRec");
    // // lv_group_add_obj(group, View.ui.btnCont.btnMenu);
    // //Serial.println("btnMenu");

    // if (lastFocus)
    // {
    //     lv_group_focus_obj(lastFocus);
    // }
    // else
    // {
    //     lv_group_focus_obj(View.ui.btnCont.btnInfos);
    // }
    // //Serial.println("group");
    // // Model.SetStatusBarStyle(DataProc::STATUS_BAR_STYLE_TRANSP);
    // Model.SetStatusBarStyle(DataProc::STATUS_BAR_STYLE_BLACK);
    // //Serial.println("to update");
    // Update();

    // View.SetScrollToY(root, -LV_VER_RES, LV_ANIM_OFF);
    // lv_obj_set_style_opa(root, LV_OPA_TRANSP, 0);
    // lv_obj_fade_in(root, 300, 0);
    // View.AppearAnimStart();
}

void Settings::onViewDidAppear(){
    View.onFocus(lv_group_get_default());
}

void Settings::onViewWillDisappear(){
    //lv_obj_fade_out(root, 300, 0);
    lv_group_t* group = lv_group_get_default();
    lv_group_remove_all_objs(group);
}

void Settings::onViewDidDisappear(){}

void Settings::onViewDidUnload(){
    View.Delete();
    Model.Deinit();
}

void Settings::onTimer(lv_timer_t* timer){}
void Settings::Update(){}

void Settings::onBtnClicked(lv_obj_t* btn)
{
    if (btn == View.ui.info.button)
    {
        Manager->Push("Pages/SystemInfos");
        PM_LOG_DEBUG_LN("sysinfo");
        
    }
    else if (btn == View.ui.back.button)
    {
        Manager->Pop();
        PM_LOG_DEBUG_LN("go back");
    }
}

void Settings::AttachEvent(lv_obj_t* obj){
    lv_obj_add_event_cb(obj, onEvent, LV_EVENT_ALL, this);
}

void Settings::onTimerUpdate(lv_timer_t* timer){}

void Settings::onEvent(lv_event_t* event){
    Settings* instance = (Settings*)lv_event_get_user_data(event);
    LV_ASSERT_NULL(instance);

    lv_obj_t* obj = lv_event_get_current_target(event);
    lv_event_code_t code = lv_event_get_code(event);
    //if (code == LV_EVENT_SHORT_CLICKED)
    //{
    //    PM_LOG_DEBUG_LN("shortclick");
    //    instance->onBtnClicked(obj);
    //}
    // if()
    if (obj == instance->View.ui.info.icon)
    {
        if (code == LV_EVENT_SHORT_CLICKED)
        {
            // instance->Manager->Pop();
            instance->Manager->Push("Pages/SystemInfos");
            PM_LOG_DEBUG_LN("sysinfo");
        }
    }
    //if (obj == instance->View.ui.info.icon)
    //    PM_LOG_DEBUG_LN("is info icon");
    //if (obj == instance->View.ui.info.labelInfo)
    //    PM_LOG_DEBUG_LN("is info");
    //if (obj == instance->View.ui.info.button)
    //    PM_LOG_DEBUG_LN("is button");
    //if (obj == instance->View.ui.back.icon)
    //    PM_LOG_DEBUG_LN("is back icon");
    if (obj == instance->View.ui.back.icon)
    {
        if (code == LV_EVENT_SHORT_CLICKED)
        {
            instance->Manager->Pop();
            // instance->Manager->Push("Pages/SystemInfos");
            PM_LOG_DEBUG_LN("go back");
            PM_LOG_INFO("go back");
        }
    }
    // if (code == LV_EVENT_PRESSED)
    // {
    //     if (lv_obj_has_state(obj, LV_STATE_FOCUSED))
    //     {
    //         instance->Manager->Pop();
    //     }
    // }

    if (obj == instance->root)
    {
        // PM_LOG_DEBUG_LN("is root");
        if (code == LV_EVENT_LEAVE)
        {
            instance->Manager->Pop();
        }
    }
}