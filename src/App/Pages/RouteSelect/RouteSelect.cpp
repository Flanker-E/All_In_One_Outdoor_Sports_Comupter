#include "RouteSelect.h"
#include "../../Configs/Version.h"
#include "../../Utils/PageManager/PM_Log.h"
#ifdef ARDUINO
#  include "Arduino.h"
#endif
using namespace Page;

RouteSelect::RouteSelect()
{
}

RouteSelect::~RouteSelect()
{

}

void RouteSelect::onCustomAttrConfig()
{

}

void RouteSelect::onViewLoad()
{
    Model.Init();
    View.Create(root);
    AttachEvent(root);
    for(auto icon:View.ui.icons){
        AttachEvent(icon);
    }
    AttachEvent(View.ui.back.icon);
}

void RouteSelect::onViewDidLoad()
{

}

void RouteSelect::onViewWillAppear()
{
    Model.SetStatusBarStyle(DataProc::STATUS_BAR_STYLE_BLACK);

    // timer = lv_timer_create(onTimerUpdate, 1000, this);
    // lv_timer_ready(timer);

    View.SetScrollToY(root, -LV_VER_RES, LV_ANIM_OFF);
    lv_obj_set_style_opa(root, LV_OPA_TRANSP, 0);
    lv_obj_fade_in(root, 300, 0);
}

void RouteSelect::onViewDidAppear()
{
    View.onFocus(lv_group_get_default());
}

void RouteSelect::onViewWillDisappear()
{
    lv_group_t* group = lv_group_get_default();
    for(auto icon:View.ui.icons){
        lv_group_remove_obj(icon);
    }
    // AttachEvent(View.ui.back.icon);
    lv_group_remove_obj(View.ui.back.icon);
    // lv_obj_fade_out(root, 300, 0);
}

void RouteSelect::onViewDidDisappear()
{


    // lv_group_add_obj(group, ui.system.icon);
    // lv_group_add_obj(group, ui.storage.icon);
    // lv_group_add_obj(group, ui.battery.icon);
    // lv_group_add_obj(group, ui.rtc.icon);
    // lv_group_add_obj(group, ui.imu.icon);
    // lv_group_add_obj(group, ui.mag.icon);
    // lv_group_add_obj(group, ui.gps.icon);
    // lv_group_add_obj(group, ui.sport.icon);
    // lv_timer_del(timer);
    lv_obj_fade_out(root, 300, 0);
}

void RouteSelect::onViewDidUnload()
{
    View.Delete();
    Model.Deinit();
}

void RouteSelect::AttachEvent(lv_obj_t* obj)
{
    lv_obj_add_event_cb(obj, onEvent, LV_EVENT_ALL, this);
}




void RouteSelect::onEvent(lv_event_t* event)
{
    RouteSelect* instance = (RouteSelect*)lv_event_get_user_data(event);
    LV_ASSERT_NULL(instance);

    lv_obj_t* obj = lv_event_get_current_target(event);
    lv_event_code_t code = lv_event_get_code(event);

    if (code == LV_EVENT_SHORT_CLICKED)
    {
        if(obj == instance->View.ui.back.icon){
            instance->Manager->Pop();
            PM_LOG_INFO("go back");
        }
        else{
            for(int i=0;i<instance->View.ui.icons.size();i++){
                if(obj == instance->View.ui.icons[i]){
                    instance->View.routes.ChooseRouteByIdx(i);
                    PM_LOG_INFO("choose num %d route",i);
                    instance->Manager->Pop();
                }
            }
        }
        // if (lv_obj_has_state(obj, LV_STATE_FOCUSED))
        // {
        //     instance->Manager->Pop();
        // }
    }

    if (obj == instance->root)
    {
        if (code == LV_EVENT_LEAVE)
        {
            instance->Manager->Pop();
        }
    }
}
