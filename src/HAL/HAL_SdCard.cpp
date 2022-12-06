#include "HAL/HAL.h"
#include "SPI.h"
#include "SD.h"
// #if HAVE_PERI==1
static uint64_t cardSize = 0;
const char* SD_type = "UNKNOWN";
static HAL::SD_CallbackFunction_t SD_EventCallback = nullptr;

bool HAL::SD_Init()
{
    pinMode(CONFIG_SD_DET_PIN, INPUT);

    SPIClass* sd_spi = new SPIClass(HSPI); // another SPI
    if (!SD.begin(15, *sd_spi, 80000000)) // SD-Card SS pin is 15
    {
        Serial.println("Card Mount Failed");
        return false;
    }
    uint8_t cardType = SD.cardType();

    if (cardType == CARD_NONE)
    {
        Serial.println("No SD card attached");
        return false;
    }

    Serial.print("SD Card Type: ");
    if (cardType == CARD_MMC)
    {
        Serial.println("MMC");
        SD_type = "MMC";
    } else if (cardType == CARD_SD)
    {
        Serial.println("SDSC");
        SD_type = "SDSC";
    } else if (cardType == CARD_SDHC)
    {
        Serial.println("SDHC");
        SD_type = "SDHC";
    } else
    {
        Serial.println("UNKNOWN");
    }

    cardSize = SD.cardSize() / (1024 * 1024);
    Serial.printf("SD Card Size: %lluMB\n", cardSize);

    return true;
}

bool HAL::SD_GetReady()
{
    return !digitalRead(CONFIG_SD_DET_PIN);
}

float HAL::SD_GetCardSizeMB()
{
    return cardSize;
}

static void SD_Check(bool isInsert)
{

}

void HAL::SD_SetEventCallback(SD_CallbackFunction_t callback)
{
    // SD_EventCallback = callback;
}

void HAL::SD_Update()
{

}

const char* HAL::SD_GetTypeName()
{
    // uint8_t cardType = SD.cardType();

    // if (cardType == CARD_MMC)
    // {
    //     // Serial.println("MMC");
    //     SD_type = "MMC";
    // } else if (cardType == CARD_SD)
    // {
    //     // Serial.println("SDSC");
    //     SD_type = "SDSC";
    // } else if (cardType == CARD_SDHC)
    // {
    //     // Serial.println("SDHC");
    //     SD_type = "SDHC";
    // } else
    // {
    //     // Serial.println("UNKNOWN");
    //     SD_type = "UNKNOWN";
    // }

    return SD_type;
}
// #else

// bool HAL::SD_Init()
// {
//     return true;
// }

// bool HAL::SD_GetReady()
// {
//     return true;
// }

// float HAL::SD_GetCardSizeMB()
// {
//     return 32 * 1024;
// }

// const char* HAL::SD_GetTypeName()
// {
//     return "SDHC";
// }

// static void SD_Check(bool isInsert)
// {
   
// }

// void HAL::SD_SetEventCallback(SD_CallbackFunction_t callback)
// {
    
// }

// void HAL::SD_Update()
// {
    
// }
// #endif