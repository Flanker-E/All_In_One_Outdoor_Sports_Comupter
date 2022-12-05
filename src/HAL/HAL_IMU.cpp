#include "HAL/HAL.h"
// #include "MPU9250.h"
// #include "App/Accounts/Account_Master.h"
#include "MPU6050.h"
#include "App/Common/DataProc/DataProc.h"

// static MPU9250 mpu;
static MPU6050 mpu;
int16_t ax, ay, az;
int16_t gx, gy, gz;
int16_t roll, yaw, pitch;

static HAL::CommitFunc_t CommitFunc = nullptr;
static void* UserData = nullptr;

// bool HAL::IMU_Init()
// {
//     mpu.initialize();
//     if (!mpu.testConnection())
//     {
//         Serial.println("MPU connection failed.");
//         return true;
//     }
//     else
//         return false;
// }
bool HAL::IMU_Init()
{
    Serial.print("IMU: init...");

    // bool success = imu.Init();
    bool success = false;

    Serial.println(success ? "success" : "failed");

    return success;
}

// void HAL::IMU_Update()
// {
//     // memset(info, 0, sizeof(GPS_Info_t));
//     IMU_Info_t imuInfo;
// //    mpu.update();
//     mpu.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);
//     mpu.getRotation(&roll,&yaw,&pitch);
//     imuInfo.ax = ax;
//     imuInfo.ay = ay;
//     imuInfo.az = az;
//     imuInfo.gx = gx;
//     imuInfo.gy = gy;
//     imuInfo.gz = gz;
//     imuInfo.mx = 0; //6050无
//     imuInfo.my = 0;
//     imuInfo.mz = 0;
//     imuInfo.roll = roll;
//     imuInfo.yaw = yaw;
//     imuInfo.pitch = pitch;
//     Serial.println("MPU update.");

//     // AccountSystem::IMU_Commit(&imuInfo);
//     if(CommitFunc)
//     {
//         CommitFunc(&imuInfo, UserData);
//     }
// }

void HAL::IMU_SetCommitCallback(CommitFunc_t func, void* userData)
{
    CommitFunc = func;
    UserData = userData;
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


void HAL::IMU_Update()
{
    IMU_Info_t imuInfo;
    // imu.GetMotion6(
    //     &imuInfo.ax, &imuInfo.ay, &imuInfo.az,
    //     &imuInfo.gx, &imuInfo.gy, &imuInfo.gz
    // );

    imuInfo.ax = 0;
    imuInfo.ay = 0;
    imuInfo.az = 0;
    imuInfo.gx = 0;
    imuInfo.gy = 0;
    imuInfo.gz = 0;

//    Serial.printf(
//        "ax = %d, ay = %d, az = %d, gx = %d, gy = %d, gz = %d\r\n",
//        imuInfo.ax, imuInfo.ay, imuInfo.az, imuInfo.gx, imuInfo.gy, imuInfo.gz
//    );

    // imuInfo.steps = imu.GetCurrentStep();
    imuInfo.steps = 0;

    if(CommitFunc)
    {
        CommitFunc(&imuInfo, UserData);
    }
}

#else

void HAL::IMU_Update()
{
    static int16_t steps;
    steps++;
    if (steps > 9999)
    {
        steps = 0;
    }

    IMU_Info_t imu;
    imu.steps = steps;

    imu.ax = rand() % 1000 - 500;
    imu.ay = rand() % 1000 - 500;
    imu.az = rand() % 1000 - 500;
    imu.gx = rand() % 1000 - 500;
    imu.gy = rand() % 1000 - 500;
    imu.gz = rand() % 1000 - 500;

    if (CommitFunc)
    {
        CommitFunc(&imu, UserData);
    }
}
#endif