#ifndef __ROUTE_SELECT_MODEL_H
#define __ROUTE_SELECT_MODEL_H

#include "../../Common/DataProc/DataProc.h"

namespace Page
{

class RouteSelectModel
{
public:
    void Init();
    void Deinit();


    void SetStatusBarStyle(DataProc::StatusBar_Style_t style);

private:
    Account* account;

private:

};

}

#endif
