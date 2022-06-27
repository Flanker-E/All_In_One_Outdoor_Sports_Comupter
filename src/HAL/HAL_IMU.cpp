#include "HAL/HAL.h"
#include "MPU9250.h"
#include "App/Accounts/Account_Master.h"

static MPU9250 mpu;

void HAL::IMU_Init()
{
    if (!mpu.setup(0x68))
    {
        Serial.println("MPU connection failed.");
    }
}
#if HAVE_PERI==1
void HAL::IMU_Update()
{
    IMU_Info_t imuInfo;
    mpu.update();

    imuInfo.ax = mpu.getAccX();
    imuInfo.ay = mpu.getAccY();
    imuInfo.az = mpu.getAccZ();
    imuInfo.gx = mpu.getGyroX();
    imuInfo.gy = mpu.getGyroY();
    imuInfo.gz = mpu.getGyroZ();
    imuInfo.mx = mpu.getMagX();
    imuInfo.my = mpu.getMagY();
    imuInfo.mz = mpu.getMagZ();
    imuInfo.roll = mpu.getRoll();
    imuInfo.yaw = mpu.getYaw();
    imuInfo.pitch = mpu.getPitch();

    AccountSystem::IMU_Commit(&imuInfo);
}
#else
void HAL::IMU_Update()
{
    IMU_Info_t imu;
    imu.ax = rand() % 1000 - 500;
    imu.ay = rand() % 1000 - 500;
    imu.az = rand() % 1000 - 500;
    imu.gx = rand() % 1000 - 500;
    imu.gy = rand() % 1000 - 500;
    imu.gz = rand() % 1000 - 500;
    imu.roll = rand() % 1000 - 500;
    imu.yaw = rand() % 1000 - 500;
    imu.pitch = rand() % 1000 - 500;
    AccountSystem::IMU_Commit(&imu);
}
#endif