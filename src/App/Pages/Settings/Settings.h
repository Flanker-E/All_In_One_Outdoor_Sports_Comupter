#ifndef __SETTINGS_PRESENTER_H
#define __SETTINGS_PRESENTER_H

#include "SettingsView.h"
#include "SettingsModel.h"

namespace Page
{

class Settings : public PageBase
{
public:

public:
    Settings();
    virtual ~Settings();

    virtual void onCustomAttrConfig();
    virtual void onViewLoad();
    virtual void onViewDidLoad();
    virtual void onViewWillAppear();
    virtual void onViewDidAppear();
    virtual void onViewWillDisappear();
    virtual void onViewDidDisappear();
    virtual void onViewDidUnload();

private:
    static void onTimer(lv_timer_t* timer);
    void Update();
    void AttachEvent(lv_obj_t* obj);
    static void onTimerUpdate(lv_timer_t* timer);
    static void onEvent(lv_event_t* event);
    void onBtnClicked(lv_obj_t* btn);

private:
    SettingsView View;
    SettingsModel Model;
    lv_timer_t* timer;
    lv_obj_t* lastFocus;
};

}

#endif