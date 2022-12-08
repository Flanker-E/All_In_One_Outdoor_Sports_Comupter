#include "MapSettings.h"
#include "../../Utils/PageManager/PM_Log.h"
#ifdef ARDUINO
#  include "Arduino.h"
#endif
using namespace Page;

MapSettings::MapSettings()
: lastFocus(nullptr){}
MapSettings::~MapSettings(){}

void MapSettings::onCustomAttrConfig(){}

void MapSettings::onViewLoad(){
    Model.Init();
    View.Create(root);
    AttachEvent(root);
    AttachEvent(View.ui.liveMap.icon);
    // AttachEvent(View.ui.ble.icon);
    AttachEvent(View.ui.availableRoute.icon);
    AttachEvent(View.ui.back.icon);
    
}

void MapSettings::onViewDidLoad(){}

void MapSettings::onViewWillAppear(){
    //Serial.println("onviewwillappear");
    lv_indev_wait_release(lv_indev_get_act());
    // View.Style_Init();
    View.Group_Init();
    
    Model.SetStatusBarStyle(DataProc::STATUS_BAR_STYLE_BLACK);
    //Serial.println("onviewwillappear");
    // timer = lv_timer_create(onTimerUpdate, 1000, this);
    // lv_timer_ready(timer);

    if (lastFocus!=nullptr)
    {
        lv_group_focus_obj(lastFocus);
    }
    else
    {
        lv_group_focus_obj(View.ui.liveMap.icon);
    }

    View.SetScrollToY(root, -LV_VER_RES, LV_ANIM_OFF);
    //Serial.println("onviewwillappear");
    lv_obj_set_style_opa(root, LV_OPA_TRANSP, 0);
    //Serial.println("onviewwillappear");
    lv_obj_fade_in(root, 300, 0);

}

void MapSettings::onViewDidAppear(){
    lv_group_t* group = lv_group_get_default();
    View.onFocus(group);
    // PM_LOG_DEBUG("group pointer addr %p",group);
    // if(group->focus_cb)
    //     PM_LOG_DEBUG("group focus cb");
    // else
    //     PM_LOG_DEBUG("group no focus cb");
    
}

void MapSettings::onViewWillDisappear(){
    
    lv_group_t* group = lv_group_get_default();
    lastFocus = lv_group_get_focused(group);
    // lv_group_remove_all_objs(group);
    lv_group_remove_obj(View.ui.liveMap.icon);
    lv_group_remove_obj(View.ui.availableRoute.icon);
    // lv_group_remove_obj(View.ui.wifi.icon);
    lv_group_remove_obj(View.ui.back.icon);
    // group->focus_cb
}

void MapSettings::onViewDidDisappear(){
    lv_obj_fade_out(root, 300, 0);
}

void MapSettings::onViewDidUnload(){
    View.Delete();
    Model.Deinit();
    lastFocus=nullptr;
}

void MapSettings::onTimer(lv_timer_t* timer){}
void MapSettings::Update(){}

void MapSettings::onBtnClicked(lv_obj_t* btn)
{
    if (btn == View.ui.liveMap.button)
    {
        Manager->Push("Pages/LiveMap");
        PM_LOG_DEBUG_LN("LiveMap");
        
    }
    else if (btn == View.ui.back.button)
    {
        Manager->Pop();
        PM_LOG_DEBUG_LN("go back");
    }
}

void MapSettings::AttachEvent(lv_obj_t* obj){
    lv_obj_add_event_cb(obj, onEvent, LV_EVENT_ALL, this);
}

void MapSettings::onTimerUpdate(lv_timer_t* timer){}

void MapSettings::onEvent(lv_event_t* event){
    MapSettings* instance = (MapSettings*)lv_event_get_user_data(event);
    LV_ASSERT_NULL(instance);

    lv_obj_t* obj = lv_event_get_current_target(event);
    lv_event_code_t code = lv_event_get_code(event);
    if (code ==LV_EVENT_LONG_PRESSED){
        instance->Manager->Pop();
    }
    if (obj == instance->View.ui.liveMap.icon)
    {
        if (code == LV_EVENT_SHORT_CLICKED)
        {
            // instance->Manager->Pop();
            instance->Manager->Push("Pages/LiveMap");
            PM_LOG_DEBUG_LN("LiveMap");
        }
    }
    else if (obj == instance->View.ui.availableRoute.icon){
        if (code == LV_EVENT_SHORT_CLICKED)
        {
            // instance->Manager->Pop();
            instance->Manager->Push("Pages/RouteSelect");
            PM_LOG_INFO("RouteSelect");
        }
    }
    else if (obj == instance->View.ui.back.icon)
    {
        if (code == LV_EVENT_SHORT_CLICKED)
        {
            instance->Manager->Pop();
            // instance->Manager->Push("Pages/SystemInfos");
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