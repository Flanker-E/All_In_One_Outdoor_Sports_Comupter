#include "HAL.h"

void HAL::SW_Init(){
    pinMode(CONFIG_SCREEN_BLK_PIN,OUTPUT);
    pinMode(CONFIG_SCREEN_PWR_PIN,OUTPUT);
    pinMode(CONFIG_IMU_GPS_PWR_PIN,OUTPUT);

    digitalWrite(CONFIG_SCREEN_BLK_PIN,HIGH); // low-side nmos
    digitalWrite(CONFIG_SCREEN_PWR_PIN,LOW); // high-side pmos
    digitalWrite(CONFIG_IMU_GPS_PWR_PIN,LOW); // high-side pmos
}
void HAL::LCD_On(){
    digitalWrite(CONFIG_SCREEN_PWR_PIN,LOW);
}
void HAL::LCD_Off(){
    digitalWrite(CONFIG_SCREEN_PWR_PIN,HIGH);
}
void HAL::IMU_GPS_On(){
    digitalWrite(CONFIG_IMU_GPS_PWR_PIN,LOW);
}
void HAL::IMU_GPS_Off(){
    digitalWrite(CONFIG_IMU_GPS_PWR_PIN,HIGH);
}