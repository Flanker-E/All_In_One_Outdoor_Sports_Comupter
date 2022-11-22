#include "HAL/HAL.h"
#include "App/Configs/Version.h"

#define DISP_HOR_RES         CONFIG_SCREEN_HOR_RES
#define DISP_VER_RES         CONFIG_SCREEN_VER_RES
#define DISP_BUF_SIZE        CONFIG_SCREEN_BUFFER_SIZE



extern lv_color_t* lv_disp_buf_p;
#if HAVE_PERI==1
void HAL::HAL_Init()
{
    Serial.begin(115200);
    Serial.println(VERSION_FIRMWARE_NAME);
    Serial.println("Version: " VERSION_SOFTWARE);
    Serial.println("Author: " VERSION_AUTHOR_NAME);

    // Move the malloc process to Init() to make sure that the largest heap can be used for this buffer.
    // lv_disp_buf_p = static_cast<lv_color_t*>(malloc(DISP_BUF_SIZE * sizeof(lv_color_t)));
    lv_disp_buf_p = (lv_color_t*)malloc(DISP_BUF_SIZE * sizeof(lv_color_t));
    if (lv_disp_buf_p == nullptr)
        LV_LOG_WARN("lv_port_disp_init malloc failed!\n");

    HAL::BT_Init(); // ToDo: some of the process below will interrupt BLE connection, find it out
    HAL::Power_Init();
    HAL::Backlight_Init();
    HAL::Encoder_Init();
    HAL::Buzz_init();
    HAL::GPS_Init();
    HAL::Audio_Init();
    HAL::SD_Init();
    HAL::I2C_Init(true);
    HAL::IMU_Init();

    HAL::Audio_PlayMusic("Startup");
}

void HAL::HAL_Update()
{
    HAL::Power_Update();
    HAL::Encoder_Update();
    HAL::Audio_Update();
    HAL::IMU_Update();
    HAL::BT_Update();
    __IntervalExecute(HAL::SD_Update(), 500);
}

#else
void HAL::HAL_Init()
{
    Serial.begin(115200);
    Serial.println(VERSION_FIRMWARE_NAME);
    Serial.println("Version: " VERSION_SOFTWARE);
    Serial.println("Author: " VERSION_AUTHOR_NAME);
    Power_Init();
    //open screen backlight
    pinMode(CONFIG_SCREEN_BLK_PIN,OUTPUT);
    digitalWrite(CONFIG_SCREEN_BLK_PIN,HIGH); 
    pinMode(5,OUTPUT);
    digitalWrite(5,LOW); 

    lv_disp_buf_p = (lv_color_t*)malloc(DISP_BUF_SIZE * sizeof(lv_color_t));
    if (lv_disp_buf_p == nullptr)
        LV_LOG_WARN("lv_port_disp_init malloc failed!\n");
    
    Encoder_Init();
    // Serial.println("encoder inited");

    I2C_Init(true); //init and scan
    Touch_Init();
    // Move the malloc process to Init() to make sure that the largest heap can be used for this buffer.
    // lv_disp_buf_p = static_cast<lv_color_t*>(malloc(DISP_BUF_SIZE * sizeof(lv_color_t)));
    
    Buzz_init();
    Audio_Init();
    GPS_Init();
    
    // Backlight_Init();
}

void HAL::HAL_Update()
{
    Power_Update();
    IMU_Update();
    Audio_Update();
    MAG_Update();

    Encoder_Update();
}
#endif

