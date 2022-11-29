#include "SystemInfos.h"
#include "../../Configs/Version.h"
#ifdef ARDUINO
#  include "Arduino.h"
#endif
using namespace Page;

SystemInfos::SystemInfos()
{
}

SystemInfos::~SystemInfos()
{

}

void SystemInfos::onCustomAttrConfig()
{

}

void SystemInfos::onViewLoad()
{
    Model.Init();
    View.Create(root);
    AttachEvent(root);
    AttachEvent(View.ui.sport.icon);
    AttachEvent(View.ui.gps.icon);
    AttachEvent(View.ui.imu.icon);
    AttachEvent(View.ui.mag.icon);
    AttachEvent(View.ui.rtc.icon);
    AttachEvent(View.ui.battery.icon);
    AttachEvent(View.ui.storage.icon);
    AttachEvent(View.ui.system.icon);
}

void SystemInfos::onViewDidLoad()
{

}

void SystemInfos::onViewWillAppear()
{
    Model.SetStatusBarStyle(DataProc::STATUS_BAR_STYLE_BLACK);

    timer = lv_timer_create(onTimerUpdate, 1000, this);
    lv_timer_ready(timer);

    View.SetScrollToY(root, -LV_VER_RES, LV_ANIM_OFF);
    lv_obj_set_style_opa(root, LV_OPA_TRANSP, 0);
    lv_obj_fade_in(root, 300, 0);
}

void SystemInfos::onViewDidAppear()
{
    View.onFocus(lv_group_get_default());
}

void SystemInfos::onViewWillDisappear()
{
    lv_group_t* group = lv_group_get_default();
    lv_group_remove_obj(View.ui.system.icon);
    lv_group_remove_obj(View.ui.storage.icon);
    lv_group_remove_obj(View.ui.battery.icon);
    lv_group_remove_obj(View.ui.rtc.icon);
    lv_group_remove_obj(View.ui.imu.icon);
    lv_group_remove_obj(View.ui.mag.icon);
    lv_group_remove_obj(View.ui.gps.icon);
    lv_group_remove_obj(View.ui.sport.icon);
    // lv_obj_fade_out(root, 300, 0);
}

void SystemInfos::onViewDidDisappear()
{


    // lv_group_add_obj(group, ui.system.icon);
    // lv_group_add_obj(group, ui.storage.icon);
    // lv_group_add_obj(group, ui.battery.icon);
    // lv_group_add_obj(group, ui.rtc.icon);
    // lv_group_add_obj(group, ui.imu.icon);
    // lv_group_add_obj(group, ui.mag.icon);
    // lv_group_add_obj(group, ui.gps.icon);
    // lv_group_add_obj(group, ui.sport.icon);
    lv_timer_del(timer);
    lv_obj_fade_out(root, 300, 0);
}

void SystemInfos::onViewDidUnload()
{
    View.Delete();
    Model.Deinit();
}

void SystemInfos::AttachEvent(lv_obj_t* obj)
{
    lv_obj_add_event_cb(obj, onEvent, LV_EVENT_ALL, this);
}

void SystemInfos::Update()
{
    // Serial.println("update");
    char buf[64];
    // Serial.println("sport");

    /* Sport */
    float trip;
    float maxSpd;
    
    Model.GetSportInfo(&trip, buf, sizeof(buf), &maxSpd);
    // View.SetSport(trip, buf, maxSpd);

    // Serial.println("gps");

    /* GPS */
    float lat;
    float lng;
    float alt;
    float course;
    float speed;
    Model.GetGPSInfo(&lat, &lng, &alt, buf, sizeof(buf), &course, &speed);
    // View.SetGPS(lat, lng, alt, buf, course, speed);
    // Serial.println("mag");

    /* MAG */
    float dir;
    int x;
    int y;
    int z;
    Model.GetMAGInfo(&dir, &x, &y, &z);
    View.SetMAG(dir, x, y, z);
    // Serial.println("imu");

    /* IMU */
    int steps;
    Model.GetIMUInfo(&steps, buf, sizeof(buf));
    View.SetIMU(steps, buf);
    // Serial.println("rtc");

    /* RTC */
    Model.GetRTCInfo(buf, sizeof(buf));
    View.SetRTC(buf);
    // Serial.println("power");

    /* Power */
    int usage;
    float voltage;
    Model.GetBatteryInfo(&usage, &voltage, buf, sizeof(buf));
    // View.SetBattery(usage, voltage, buf);
    // Serial.println("srorage");

    /* Storage */
    bool detect;
    const char* type = "-";
    Model.GetStorageInfo(&detect, &type, buf, sizeof(buf));
    View.SetStorage(
        detect ? "OK" : "ERROR",
        buf,
        type,
        VERSION_FILESYSTEM
    );

    /* System */
    DataProc::MakeTimeString(lv_tick_get(), buf, sizeof(buf));
    View.SetSystem(
        VERSION_FIRMWARE_NAME " " VERSION_SOFTWARE,
        VERSION_AUTHOR_NAME,
        VERSION_LVGL,
        buf,
        VERSION_COMPILER,
        VERSION_BUILD_TIME
    );
}

void SystemInfos::onTimerUpdate(lv_timer_t* timer)
{
    SystemInfos* instance = (SystemInfos*)timer->user_data;

    instance->Update();
}

void SystemInfos::onEvent(lv_event_t* event)
{
    SystemInfos* instance = (SystemInfos*)lv_event_get_user_data(event);
    LV_ASSERT_NULL(instance);

    lv_obj_t* obj = lv_event_get_current_target(event);
    lv_event_code_t code = lv_event_get_code(event);

    if (code == LV_EVENT_PRESSED)
    {
        if (lv_obj_has_state(obj, LV_STATE_FOCUSED))
        {
            instance->Manager->Pop();
        }
    }

    if (obj == instance->root)
    {
        if (code == LV_EVENT_LEAVE)
        {
            instance->Manager->Pop();
        }
    }
}
