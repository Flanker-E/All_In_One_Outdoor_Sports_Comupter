#ifndef __ROUTES_H
#define __ROUTES_H
#include <string>
// #ifdef WIN32
#include <vector>
// #endif
class Routes{
public:
    Routes(){
        routeChoose="";
        numRoute=0;
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

    static void ChooseRoute(std::string chooseName){
        for(auto route:routeName){
            if(routeChoose==route){
                routeChoose = chooseName;
                break;
            }
        }
    }
    
    static void ChooseRouteNameByIdx(int16_t idx){
        if(idx<numRoute)
            routeChoose=routeName[idx-1];
        else
            routeChoose="";
    }
    static void SetRouteName(std::vector<std::string> foundRouteName)
    {
        routeName = foundRouteName;
    }
    static std::vector<std::string>* GetRouteNamePtr(){
        return &routeName;
    }

    

private:
    static int16_t numRoute;
    static std::vector<std::string> routeName;

    static std::string routeChoose;
};
#endif