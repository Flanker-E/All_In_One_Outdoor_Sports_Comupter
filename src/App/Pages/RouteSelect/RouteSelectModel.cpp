#include "RouteSelectModel.h"
#include <stdio.h>

using namespace Page;

void RouteSelectModel::Init()
{
    account = new Account("RouteSelectModel", DataProc::Center(), 0, this);

    // account->Subscribe("SportStatus");
    // account->Subscribe("GPS");
    // account->Subscribe("MAG");
    // account->Subscribe("IMU");
    // account->Subscribe("Clock");
    // account->Subscribe("Power");
    // account->Subscribe("Storage");
    account->Subscribe("StatusBar");
}

void RouteSelectModel::Deinit()
{
    if (account)
    {
        delete account;
        account = nullptr;
    }
}


void RouteSelectModel::SetStatusBarStyle(DataProc::StatusBar_Style_t style)
{
    DataProc::StatusBar_Info_t info;
    DATA_PROC_INIT_STRUCT(info);

    info.cmd = DataProc::STATUS_BAR_CMD_SET_STYLE;
    info.param.style = style;

    account->Notify("StatusBar", &info, sizeof(info));
}
