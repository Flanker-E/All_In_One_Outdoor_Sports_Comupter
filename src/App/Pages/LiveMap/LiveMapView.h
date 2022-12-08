#ifndef __LIVEMAP_VIEW_H
#define __LIVEMAP_VIEW_H

#include "../Page.h"
#include <vector>
#include <string>
#include "../../Utils/lv_poly_line/lv_poly_line.h"
#include "../../Utils/Routes/Routes.h"

namespace Page
{

class LiveMapView
{
public:
    struct
    {
        lv_obj_t* loadingLabelInfo;
        
        struct{
            lv_obj_t* cont;
            lv_obj_t* info;
        } toEinkLabelInfo;

        lv_style_t styleCont;
        lv_style_t styleLabel;
        lv_style_t styleLine;

        struct
        {
            lv_obj_t* cont;
            lv_obj_t* imgArrow;
            lv_obj_t** imgTiles;
            uint32_t tileNum;            
        } map;

        struct
        {
            lv_obj_t* cont;
            lv_poly_line* lineTrack;
            lv_obj_t* lineActive;
            lv_point_t pointActive[2];
        } track;

        struct
        {
            lv_obj_t* cont;
            lv_poly_line* lineRoute;
            lv_obj_t* lineActive;
            lv_point_t pointActive[2];
            std::vector<std::pair<double,double>> routePoints;
        } route;

        struct
        {
            lv_obj_t* cont;
            lv_obj_t* labelInfo;
            lv_obj_t* slider;
        } zoom;

        struct
        {
            lv_obj_t* cont;
        } move;

        struct
        {
            lv_obj_t* cont;

            lv_obj_t* labelSpeed;
            lv_obj_t* labelTrip;
            lv_obj_t* labelTime;
        } sportInfo;
        lv_obj_t* cont;
    } ui;
    struct{
        struct{
            lv_obj_t* Info_GPS; //already in lcd
            lv_obj_t* Data_GPS;
        }gpsInfo;

        struct{
            lv_obj_t* Info_North; //already in lcd
            lv_obj_t* Data_North;
        }northInfo;

        struct{
            lv_obj_t* Info_East; //already in lcd
            lv_obj_t* Data_East;
        }eastInfo;

        struct{
            lv_obj_t* Info_Dest; //already in lcd
            lv_obj_t* Data_Dest;
        }destInfo;

        struct{
            lv_obj_t* Info_Batt; //already in lcd
            lv_obj_t* Data_Batt;
        }battInfo;
        lv_obj_t* cont;

    } eink_ui;
    
    void Eink_info_init(void);
    void Eink_Update();
    void Create(lv_obj_t* root, uint32_t tileNum);
    void Delete();
    void SetImgArrowStatus(lv_coord_t x, lv_coord_t y, float angle)
    {
        lv_obj_t* img = ui.map.imgArrow;
        lv_obj_set_pos(img, x, y);
        lv_img_set_angle(img, int16_t(angle * 10));
    }
    void SetMapTile(uint32_t tileSize, uint32_t widthCnt);
    void SetMapTileSrc(uint32_t index, const char* src);
    void SetArrowTheme(const char* theme);
    void SetLineActivePoint(lv_coord_t x, lv_coord_t y);

private:
    void Style_Create();
    void Eink_Item_Create(
    lv_obj_t* Info,
    lv_obj_t* &data,
    const char* infos,
    int x_bias,
    int y_bias,
    bool is_large_font = true
    );
    void Map_Create(lv_obj_t* par, uint32_t tileNum);
    void ZoomCtrl_Create(lv_obj_t* par);
    void SportInfo_Create(lv_obj_t* par);
    lv_obj_t* ImgLabel_Create(lv_obj_t* par, const void* img_src, lv_coord_t x_ofs, lv_coord_t y_ofs);
    void Track_Create(lv_obj_t* par);
    void Route_Create(lv_obj_t* par);
    // static StorageService storageService;
};

}

#endif // !__VIEW_H
