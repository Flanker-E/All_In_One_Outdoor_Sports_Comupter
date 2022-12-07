#ifndef __ROUTE_SELECT_PRESENTER_H
#define __ROUTE_SELECT_PRESENTER_H

#include "RouteSelectView.h"
#include "RouteSelectModel.h"

namespace Page
{

class RouteSelect : public PageBase
{
public:

public:
    RouteSelect();
    virtual ~RouteSelect();

    virtual void onCustomAttrConfig();
    virtual void onViewLoad();
    virtual void onViewDidLoad();
    virtual void onViewWillAppear();
    virtual void onViewDidAppear();
    virtual void onViewWillDisappear();
    virtual void onViewDidDisappear();
    virtual void onViewDidUnload();

private:
    // void Update();
    void AttachEvent(lv_obj_t* obj);
    static void onTimerUpdate(lv_timer_t* timer);
    static void onEvent(lv_event_t* event);

private:
    RouteSelectView View;
    RouteSelectModel Model;
    lv_timer_t* timer;
};

}

#endif
