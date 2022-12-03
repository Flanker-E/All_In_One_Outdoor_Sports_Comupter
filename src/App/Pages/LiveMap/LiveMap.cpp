#include "LiveMap.h"
#include "../../Configs/Config.h"
#include "../../Utils/PageManager/PM_Log.h"
#ifndef WIN32
#include "Port/Display.h"
#endif


volatile bool isNormalMode=true;
volatile bool einkNeedUpdate=false;
using namespace Page;

uint16_t LiveMap::mapLevelCurrent = CONFIG_LIVE_MAP_LEVEL_DEFAULT;

LiveMap::LiveMap()
{
    memset(&priv, 0, sizeof(priv));
}

LiveMap::~LiveMap()
{

}

void LiveMap::onCustomAttrConfig()
{
}

void LiveMap::onViewLoad()
{
    const uint32_t tileSize = 256;

    Model.tileConv.SetTileSize(tileSize);
    Model.tileConv.SetViewSize(
        CONFIG_LIVE_MAP_VIEW_WIDTH,
        CONFIG_LIVE_MAP_VIEW_HEIGHT
    );
    Model.tileConv.SetFocusPos(0, 0);

    TileConv::Rect_t rect;
    uint32_t tileNum = Model.tileConv.GetTileContainer(&rect);

    View.Create(root, tileNum);
    lv_slider_set_range(
        View.ui.zoom.slider,
        Model.mapConv.GetLevelMin(),
        Model.mapConv.GetLevelMax()
    );
    View.SetMapTile(tileSize, rect.width / tileSize);

#if CONFIG_LIVE_MAP_DEBUG_ENABLE
    lv_obj_t* contView = lv_obj_create(root);
    lv_obj_center(contView);
    lv_obj_set_size(contView, CONFIG_LIVE_MAP_VIEW_WIDTH, CONFIG_LIVE_MAP_VIEW_HEIGHT);
    lv_obj_set_style_border_color(contView, lv_palette_main(LV_PALETTE_RED), 0);
    lv_obj_set_style_border_width(contView, 1, 0);
#endif

    AttachEvent(root);
    AttachEvent(View.ui.zoom.slider);
    AttachEvent(View.ui.sportInfo.cont);
    

    lv_slider_set_value(View.ui.zoom.slider, mapLevelCurrent, LV_ANIM_OFF);
    Model.mapConv.SetLevel(mapLevelCurrent);
    lv_obj_add_flag(View.ui.map.cont, LV_OBJ_FLAG_HIDDEN);

    /* Point filter */
    Model.pointFilter.SetOffsetThreshold(CONFIG_TRACK_FILTER_OFFSET_THRESHOLD);
    Model.pointFilter.SetOutputPointCallback([](TrackPointFilter * filter, const TrackPointFilter::Point_t* point)
    {
        LiveMap* instance = (LiveMap*)filter->userData;
        instance->TrackLineAppendToEnd((int32_t)point->x, (int32_t)point->y);
    });
    Model.pointFilter.userData = this;

    /* Line filter */
    Model.lineFilter.SetOutputPointCallback(onTrackLineEvent);
    Model.lineFilter.userData = this;
}

void LiveMap::onViewDidLoad()
{

}

void LiveMap::onViewWillAppear()
{
    Model.Init();

    char theme[16];
    Model.GetArrowTheme(theme, sizeof(theme));
    View.SetArrowTheme(theme);

    priv.isTrackAvtive = Model.GetTrackFilterActive();

    Model.SetStatusBarStyle(DataProc::STATUS_BAR_STYLE_BLACK);
    SportInfoUpdate();
    lv_obj_clear_flag(View.ui.loadingLabelInfo, LV_OBJ_FLAG_HIDDEN);
}

void LiveMap::onViewDidAppear()
{
    priv.timer = lv_timer_create([](lv_timer_t* timer)
    {
        LiveMap* instance = (LiveMap*)timer->user_data;
        instance->Update();
    },
    100,
    this);
    priv.lastMapUpdateTime = 0;
    // used to determine long press and double click. long press is disabled by LVGL when scroll is used to edit.
    priv.lastClickededTime = 0;
    priv.lastPressedTime = 0;
    priv.clickedBefore = false;

    lv_obj_clear_flag(View.ui.map.cont, LV_OBJ_FLAG_HIDDEN);
    lv_obj_add_flag(View.ui.loadingLabelInfo, LV_OBJ_FLAG_HIDDEN);

    priv.lastTileContOriPoint.x = 0;
    priv.lastTileContOriPoint.y = 0;

    priv.isTrackAvtive = Model.GetTrackFilterActive();
    if (!priv.isTrackAvtive)
    {
        Model.pointFilter.SetOutputPointCallback(nullptr);
    }

    lv_group_t* group = lv_group_get_default();
    lv_group_add_obj(group, View.ui.zoom.slider);
    lv_group_set_editing(group, View.ui.zoom.slider);
}

void LiveMap::onViewWillDisappear()
{
    lv_timer_del(priv.timer);
    lv_obj_add_flag(View.ui.map.cont, LV_OBJ_FLAG_HIDDEN);
    lv_obj_fade_out(root, 250, 250);
}

void LiveMap::onViewDidDisappear()
{
    Model.Deinit();
}

void LiveMap::onViewDidUnload()
{
    View.Delete();
}

void LiveMap::AttachEvent(lv_obj_t* obj)
{
    lv_obj_add_event_cb(obj, onEvent, LV_EVENT_ALL, this);
}

// Will be called based on timer (100ms). Update info every 1s.
void LiveMap::Update()
{
    
    if (lv_tick_elaps(priv.lastMapUpdateTime) >= CONFIG_GPS_REFR_PERIOD)
    {
        // static bool lcdSwitchInfo=false;
        static bool lastIsNormalMode=true;
        if (isNormalMode){
            PM_LOG_INFO("normal mode");
            // if(lcdSwitchInfo==true){
            // }
            if(isNormalMode!=lastIsNormalMode){
                // came from low power mode
                // give lcd power supply and init LCD
                #ifndef WIN32
                LCD_Power_On();
                To_LCD_Port();
                #endif
                // hide cover
                lv_obj_add_flag(View.ui.toEinkLabelInfo.info, LV_OBJ_FLAG_HIDDEN);
                lv_obj_add_flag(View.ui.toEinkLabelInfo.cont, LV_OBJ_FLAG_HIDDEN);
                PM_LOG_INFO("switch back to LCD, turn on its power");
            }
            
        }
        else{
            static bool einkInfoInited=false;
            PM_LOG_INFO("low power mode");
            if(isNormalMode!=lastIsNormalMode){
                // show cover
                lv_obj_clear_flag(View.ui.toEinkLabelInfo.info, LV_OBJ_FLAG_HIDDEN);
                lv_obj_clear_flag(View.ui.toEinkLabelInfo.cont, LV_OBJ_FLAG_HIDDEN);
                PM_LOG_INFO("switch back to LCD, turn off its power");
            }
            else{
                #ifndef WIN32
                LCD_Power_Off();
                #endif
                if(!einkInfoInited){
                    PM_LOG_INFO("eink init");
                    #ifndef WIN32
                    
                    To_Eink_Port();
                    #endif
                    View.Eink_info_init();
                    AttachEvent(View.eink_ui.cont);
                    #ifndef WIN32
                    To_LCD_Port();
                    #endif
                    einkInfoInited=true;
                }
                if(einkNeedUpdate){
                    PM_LOG_INFO("update eink");
                    #ifndef WIN32
                    To_Eink_Port();
                    #endif            
                    View.Eink_Update();
                    #ifndef WIN32
                    To_LCD_Port();
                    #endif
                    einkNeedUpdate=false;
                }
            }
            // if(lcdSwitchInfo==false){
            //     //show swith indication
            //     lv_obj_clear_flag(View.ui.toEinkLabelInfo.info, LV_OBJ_FLAG_HIDDEN);
            //     lv_obj_clear_flag(View.ui.toEinkLabelInfo.cont, LV_OBJ_FLAG_HIDDEN);
            //     lcdSwitchInfo=true;
            // }
        }
        lastIsNormalMode=isNormalMode;
        CheckPosition();
        SportInfoUpdate();
        priv.lastMapUpdateTime = lv_tick_get();
    }
    else if (lv_tick_elaps(priv.lastContShowTime) >= 3000)
    {
        lv_obj_add_state(View.ui.zoom.cont, LV_STATE_USER_1);
    }
}

void LiveMap::UpdateDelay(uint32_t ms)
{
    priv.lastMapUpdateTime = lv_tick_get() - 1000 + ms;
}

void LiveMap::SportInfoUpdate()
{
    lv_label_set_text_fmt(
        View.ui.sportInfo.labelSpeed,
        "%02d",
        (int)Model.sportStatusInfo.speedKph
    );

    lv_label_set_text_fmt(
        View.ui.sportInfo.labelTrip,
        "%0.1f km",
        Model.sportStatusInfo.singleDistance / 1000
    );

    char buf[16];
    lv_label_set_text(
        View.ui.sportInfo.labelTime,
        DataProc::MakeTimeString(Model.sportStatusInfo.singleTime, buf, sizeof(buf))
    );
}

void LiveMap::CheckPosition()
{
    bool refreshMap = false;

    HAL::GPS_Info_t gpsInfo;
    Model.GetGPS_Info(&gpsInfo);
    // check tile level
    mapLevelCurrent = lv_slider_get_value(View.ui.zoom.slider);
    if (mapLevelCurrent != Model.mapConv.GetLevel())
    {
        refreshMap = true;
        Model.mapConv.SetLevel(mapLevelCurrent);
    }
    // get current map pixel pos and focus on it
    int32_t mapX, mapY;
    Model.mapConv.ConvertMapCoordinate(
        gpsInfo.longitude, gpsInfo.latitude,
        &mapX, &mapY
    );
    Model.tileConv.SetFocusPos(mapX, mapY);

    if (GetIsMapTileContChanged())
    {
        refreshMap = true;
    }
    // refresh
    if (refreshMap)
    {
        TileConv::Rect_t rect;
        Model.tileConv.GetTileContainer(&rect);

        Area_t area =
        {
            .x0 = rect.x,
            .y0 = rect.y,
            .x1 = rect.x + rect.width - 1,
            .y1 = rect.y + rect.height - 1
        };

        onMapTileContRefresh(&area, mapX, mapY);
    }
    // update arrow, track and map pos.
    MapTileContUpdate(mapX, mapY, gpsInfo.course);

    if (priv.isTrackAvtive)
    {
        Model.pointFilter.PushPoint(mapX, mapY);
    }
}

void LiveMap::onMapTileContRefresh(const Area_t* area, int32_t x, int32_t y)
{
    LV_LOG_INFO(
        "area: (%d, %d) [%dx%d]",
        area->x0, area->y0,
        area->x1 - area->x0 + 1,
        area->y1 - area->y0 + 1
    );

    MapTileContReload();

    if (priv.isTrackAvtive)
    {
        TrackLineReload(area, x, y);
    }
}

void LiveMap::MapTileContUpdate(int32_t mapX, int32_t mapY, float course)
{
    TileConv::Point_t offset;
    TileConv::Point_t curPoint = { mapX, mapY };
    Model.tileConv.GetOffset(&offset, &curPoint);

    /* arrow */
    lv_obj_t* img = View.ui.map.imgArrow;
    Model.tileConv.GetFocusOffset(&offset);
    lv_coord_t x = offset.x - lv_obj_get_width(img) / 2;
    lv_coord_t y = offset.y - lv_obj_get_height(img) / 2;
    View.SetImgArrowStatus(x, y, course);

    /* active line */
    if (priv.isTrackAvtive)
    {
        View.SetLineActivePoint((lv_coord_t)offset.x, (lv_coord_t)offset.y);
    }

    /* map cont */
    Model.tileConv.GetTileContainerOffset(&offset);

    lv_coord_t baseX = (LV_HOR_RES - CONFIG_LIVE_MAP_VIEW_WIDTH) / 2;
    lv_coord_t baseY = (LV_VER_RES - CONFIG_LIVE_MAP_VIEW_HEIGHT) / 2;
    lv_obj_set_pos(View.ui.map.cont, baseX - offset.x, baseY - offset.y);
}

void LiveMap::MapTileContReload()
{
    /* tile src */
    for (uint32_t i = 0; i < View.ui.map.tileNum; i++)
    {
        TileConv::Point_t pos;
        Model.tileConv.GetTilePos(i, &pos);

        char path[64];
        Model.mapConv.ConvertMapPath(pos.x, pos.y, path, sizeof(path));

        View.SetMapTileSrc(i, path);
    }
}

bool LiveMap::GetIsMapTileContChanged()
{
    TileConv::Point_t pos;
    Model.tileConv.GetTilePos(0, &pos);

    bool ret = (pos.x != priv.lastTileContOriPoint.x || pos.y != priv.lastTileContOriPoint.y);

    priv.lastTileContOriPoint = pos;

    return ret;
}

void LiveMap::TrackLineReload(const Area_t* area, int32_t x, int32_t y)
{
    Model.lineFilter.SetClipArea(area);
    Model.lineFilter.Reset();
    Model.TrackReload([](TrackPointFilter * filter, const TrackPointFilter::Point_t* point)
    {
        LiveMap* instance = (LiveMap*)filter->userData;
        instance->Model.lineFilter.PushPoint((int32_t)point->x, (int32_t)point->y);
    }, this);
    Model.lineFilter.PushPoint(x, y);
    Model.lineFilter.PushEnd();
}

void LiveMap::TrackLineAppend(int32_t x, int32_t y)
{
    TileConv::Point_t offset;
    TileConv::Point_t curPoint = { x, y };
    Model.tileConv.GetOffset(&offset, &curPoint);
    View.ui.track.lineTrack->append((lv_coord_t)offset.x, (lv_coord_t)offset.y);
}

void LiveMap::TrackLineAppendToEnd(int32_t x, int32_t y)
{
    TileConv::Point_t offset;
    TileConv::Point_t curPoint = { x, y };
    Model.tileConv.GetOffset(&offset, &curPoint);
    View.ui.track.lineTrack->append_to_end((lv_coord_t)offset.x, (lv_coord_t)offset.y);
}

void LiveMap::onTrackLineEvent(TrackLineFilter* filter, TrackLineFilter::Event_t* event)
{
    LiveMap* instance = (LiveMap*)filter->userData;
    lv_poly_line* lineTrack = instance->View.ui.track.lineTrack;

    switch (event->code)
    {
    case TrackLineFilter::EVENT_START_LINE:
        lineTrack->start();
        instance->TrackLineAppend(event->point->x, event->point->y);
        break;
    case TrackLineFilter::EVENT_APPEND_POINT:
        instance->TrackLineAppend(event->point->x, event->point->y);
        break;
    case TrackLineFilter::EVENT_END_LINE:
        if (event->point != nullptr)
        {
            instance->TrackLineAppend(event->point->x, event->point->y);
        }
        lineTrack->stop();
        break;
    case TrackLineFilter::EVENT_RESET:
        lineTrack->reset();
        break;
    default:
        break;
    }
}

void LiveMap::onEvent(lv_event_t* event)
{
    LiveMap* instance = (LiveMap*)lv_event_get_user_data(event);
    LV_ASSERT_NULL(instance);

    lv_obj_t* obj = lv_event_get_current_target(event);
    lv_event_code_t code = lv_event_get_code(event);

    #ifdef WIN32
    //determine whether click is stale
    if (lv_tick_elaps(instance->priv.lastClickededTime) >= CONFIG_MAP_DOUBLE_CLICKED_DELAY){

        instance->priv.clickedBefore=false;
        }
    #endif
    // PM_LOG_INFO("code is %d",code);
    // if(obj == instance->View.eink_ui.cont){
    //     PM_LOG_INFO("eink ui%d",code);
    // }
    // else if(obj == instance->View.ui.cont){
    //     PM_LOG_INFO("ui%d",code);
    // }
    // else if(obj == instance->View.ui.zoom.slider){
    //     PM_LOG_INFO("slider%d",code);
    // }
    // else if(obj == instance->View.ui.sportInfo.cont){
    //     PM_LOG_INFO("map info%d",code);
    // }
    // else if(obj == instance->View.ui.map.cont){
    //     PM_LOG_INFO("map cont%d",code);
    // }

    if (code == LV_EVENT_LEAVE)
    {
        instance->Manager->Pop();
        return;
    }
    // else if (code == LV_EVENT_PRESSED)
    // {
    //     PM_LOG_INFO("outside pressed");
    //     instance->priv.lastPressedTime=lv_tick_get();

    // }
    else if(code ==LV_EVENT_LONG_PRESSED_REPEAT){
        // PM_LOG_INFO("outside released");
        // if (lv_tick_elaps(instance->priv.lastPressedTime) >= CONFIG_MAP_LONG_PRESSED_TIME){
            PM_LOG_INFO("outside long pressed");
            if(isNormalMode)
                instance->Manager->Pop();
            else{
                PM_LOG_INFO("event give update");
                einkNeedUpdate=true;
            }
        // }
    }
    #ifdef WIN32
    else if(code == LV_EVENT_SHORT_CLICKED){
        PM_LOG_INFO("outside short clicked");
        // determine whether double clicked first
        if(instance->priv.clickedBefore==true){
            PM_LOG_INFO("outside double clicked");
            isNormalMode=!isNormalMode;
            if(!isNormalMode){
                lv_obj_clear_flag(instance->View.ui.toEinkLabelInfo.info, LV_OBJ_FLAG_HIDDEN);
                lv_obj_clear_flag(instance->View.ui.toEinkLabelInfo.cont, LV_OBJ_FLAG_HIDDEN);
            }
            else{
                lv_obj_add_flag(instance->View.ui.toEinkLabelInfo.info, LV_OBJ_FLAG_HIDDEN);
                lv_obj_add_flag(instance->View.ui.toEinkLabelInfo.cont, LV_OBJ_FLAG_HIDDEN);
            }
            einkNeedUpdate=true;
        }
        instance->priv.clickedBefore=true;
        instance->priv.lastClickededTime=lv_tick_get();
    }
    #endif

    if (obj == instance->View.ui.zoom.slider)
    {
        if (code == LV_EVENT_VALUE_CHANGED)
        {
            PM_LOG_INFO("slider value changed");
            int32_t level = lv_slider_get_value(obj);
            int32_t levelMax = instance->Model.mapConv.GetLevelMax();
            lv_label_set_text_fmt(instance->View.ui.zoom.labelInfo, "%d/%d", level, levelMax);

            lv_obj_clear_state(instance->View.ui.zoom.cont, LV_STATE_USER_1);
            instance->priv.lastContShowTime = lv_tick_get();
            instance->UpdateDelay(200);
        }
        // else if (code == LV_EVENT_SHORT_CLICKED)
        // {
        //     PM_LOG_INFO("slider short clicked");
        //     if(isNormalMode)
        //         instance->Manager->Pop();
        //     else{
        //         PM_LOG_INFO("event give update");
        //         einkNeedUpdate=true;
        //     }
        // }
        // else if(code == LV_EVENT_LONG_PRESSED){
        //     PM_LOG_INFO("slider long_pressed");
        //     isNormalMode=!isNormalMode;
        //     einkNeedUpdate=true;
        // }
    }

    // if (obj == instance->View.ui.sportInfo.cont ||obj == instance->View.ui.cont)
    // {
    //     if (code == LV_EVENT_SHORT_CLICKED)
    //     {
    //         PM_LOG_INFO("info short clicked");
    //         if(isNormalMode)
    //             instance->Manager->Pop();
    //         else{
    //             PM_LOG_INFO("event give update");
    //             einkNeedUpdate=true;
    //         }
    //     }
    //     else if(code == LV_EVENT_LONG_PRESSED){
    //         PM_LOG_INFO("info long_pressed");
    //         isNormalMode=!isNormalMode;
    //         einkNeedUpdate=true;
    //     }
    // }
    //LV_EVENT_SHORT_CLICKED
    //LV_EVENT_LONG_PRESSED_REPEAT
}
