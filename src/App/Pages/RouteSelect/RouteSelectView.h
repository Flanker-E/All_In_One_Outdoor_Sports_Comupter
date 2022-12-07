#ifndef __ROUTE_SELECT_VIEW_H
#define __ROUTE_SELECT_VIEW_H

#include "../Page.h"
#include "../../Utils/Routes/Routes.h"
#ifdef WIN32
#include <vector>
#endif
namespace Page
{

class RouteSelectView
{
public:
    void Create(lv_obj_t* root);
    void Delete();

public:
    typedef struct
    {
        lv_obj_t* cont;
        lv_obj_t* icon;
        lv_obj_t* labelInfo;
        lv_obj_t* labelData;
    } item_t;

    struct
    {
        std::vector<lv_obj_t*> icons;
        item_t back;
    } ui;


public:

    void SetScrollToY(lv_obj_t* obj, lv_coord_t y, lv_anim_enable_t en);
    static void onFocus(lv_group_t* e);
    Routes routes;
private:
    struct
    {
        lv_style_t icon;
        lv_style_t focus;
        lv_style_t info;
        lv_style_t data;
    } style;

private:
    void Group_Init();
    void Style_Init();
    void Style_Reset();
    void Item_Create(
        lv_obj_t* par,
        const char* name,
        const char* img_src,
        std::string infos,
        std::string selected
    );
    void Item_Create(
        item_t* item,
        lv_obj_t* par,
        const char* name,
        const char* img_src,
        const char* infos
    );
};

}

#endif // !__VIEW_H
