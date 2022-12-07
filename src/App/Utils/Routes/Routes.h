#ifndef __ROUTES_H
#define __ROUTES_H
#include <string>
// #ifdef WIN32
#include "../../Utils/PageManager/PM_Log.h"
#include <vector>
// #endif
class Routes{
public:
    Routes(){
        // routeChoose="";
        // numRoute=0;
        PM_LOG_DEBUG("create Routes object");
    }
    ~Routes() {}
    static void SetRouteNum(int16_t num)
    {
        numRoute = num;
    }
    static int16_t GetRouteNum()
    {
        return numRoute ;
    }
    static std::string GetRouteChooseName(){
        return routeChoose;
    }
    // choose the route that used to navigate by its name
    static void ChooseRouteByName(std::string chooseName){
        routeChoose="";
        for(auto route:availableRoutes){
            if(routeChoose==route){
                routeChoose = chooseName;
                break;
            }
        }
    }
    // choose the route that used to navigate by its index in vector
    static void ChooseRouteByIdx(int16_t idx){
        PM_LOG_DEBUG("num of route is %d",numRoute);
        if(idx<numRoute&&(routeChoose!=availableRoutes[idx])){
            routeChoose=availableRoutes[idx];
            PM_LOG_DEBUG("route choose is %s",routeChoose);
            }
        else{
            routeChoose="";
            PM_LOG_DEBUG("set route choose to empty");
            }
    }
    static void SetAvailableRoutes(std::vector<std::string> foundRouteName)
    {
        availableRoutes = foundRouteName;
    }
    static std::vector<std::string>* GetRouteNamePtr(){
        return &availableRoutes;
    }

    

private:
    static int16_t numRoute;
    static std::vector<std::string> availableRoutes;

    static std::string routeChoose;
};
#endif