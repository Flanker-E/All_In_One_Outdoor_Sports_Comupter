#include "HAL/HAL.h"
#include "MPU6050.h"
// #include "App/Accounts/Account_Master.h"

static MPU6050 mpu;
int16_t ax, ay, az;
int16_t gx, gy, gz;
int16_t roll, yaw, pitch;

void HAL::IMU_Init()
{
    mpu.initialize();
    if (mpu.testConnection())
    {
        Serial.println("MPU connection failed.");
    }
}

void HAL::IMU_Update(IMU_Info_t* info)
{
    memset(info, 0, sizeof(GPS_Info_t));
    // IMU_Info_t imuInfo;
   // mpu.update();
    mpu.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);
    mpu.getRotation(&roll,&yaw,&pitch);
    info->ax = ax;
    info->ay = ay;
    info->az = az;
    info->gx = gx;
    info->gy = gy;
    info->gz = gz;
    info->mx = 0; //6050无
    info->my = 0;
    info->mz = 0;
    info->roll = roll;
    info->yaw = yaw;
    info->pitch = pitch;

    // AccountSystem::IMU_Commit(&imuInfo);
}
