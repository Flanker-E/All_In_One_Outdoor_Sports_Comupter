#ifndef __MAPSETTINGS_MODEL_H
#define __MAPSETTINGS_MODEL_H

#include "../../Common/DataProc/DataProc.h"
#include "HAL/HAL.h"

namespace Page
{

class MapSettingsModel
{
public:
    void Init();
    void Deinit();

    // void PlayMusic(const char* music);
    // void SetEncoderEnable(bool en)
    // {
    //     HAL::Encoder_SetEnable(en);
    // }
    // void SetStatusBarAppear(bool en);
    void SetStatusBarStyle(DataProc::StatusBar_Style_t style);

private:
    Account* account;
};

}

#endif