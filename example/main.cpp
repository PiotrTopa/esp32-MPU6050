#include <stdio.h>
#include <stdlib.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_err.h"
#include "esp_log.h"
#include "esp_system.h"

#include "MPU6050.h"
#include "MPU6050_6Axis_MotionApps20.h"
#include "I2Cdev.h"

#define PIN_I2C_SDA GPIO_NUM_32
#define PIN_I2C_SCL GPIO_NUM_33

I2Cdev i2c = I2Cdev(I2C_NUM_0);
MPU6050 mpu;

extern "C" void update_mpu(void *pvParameters)
{
	float ypr[3];
	uint16_t packetSize = 42; // expected DMP packet size (default is 42 bytes)
	uint16_t fifoCount;		  // count of all bytes currently in FIFO
	uint8_t fifoBuffer[64];	  // FIFO storage buffer
	uint8_t mpuIntStatus;	  // holds actual interrupt status byte from MPU
    Quaternion quaternion; // [w, x, y, z]         quaternion container
    VectorFloat gravity;   // [x, y, z]            gravity vector

	mpu.CalibrateGyro();
	mpu.CalibrateAccel();
	mpu.dmpInitialize();

	// mpu.setXGyroOffset(CONFIG_X_GYRO_OFFSET);
	// mpu.setYGyroOffset(CONFIG_Y_GYRO_OFFSET);
	// mpu.setZGyroOffset(CONFIG_Z_GYRO_OFFSET);
	// mpu.setZAccelOffset(CONFIG_Z_ACCEL_OFFSET);
	mpu.setDMPEnabled(true);

	while (true)
	{
		mpuIntStatus = mpu.getIntStatus();
		// get current FIFO count
		fifoCount = mpu.getFIFOCount();

		if ((mpuIntStatus & 0x10) || fifoCount == 1024)
		{
			// reset so we can continue cleanly
			mpu.resetFIFO();
		}
		else if (mpuIntStatus & 0x02)
		{
			// wait for correct available data length, should be a VERY short wait
			while (fifoCount < packetSize)
			{
				fifoCount = mpu.getFIFOCount();
			}

			// read a packet from FIFO
			mpu.getFIFOBytes(fifoBuffer, packetSize);

			// read and calculate data
			mpu.dmpGetQuaternion(&quaternion, fifoBuffer);
			mpu.dmpGetGravity(&gravity, &quaternion);
			mpu.dmpGetYawPitchRoll(ypr, &quaternion, &gravity);

			printf("Yaw: %3.2f Pitch: %3.2f Roll: %3.2f\n", ypr[0] * 180/M_PI, ypr[1] * 180/M_PI, ypr[2] * 180/M_PI);
		}
	}
}

void start(void)
{

	i2c.initialize(PIN_I2C_SDA, PIN_I2C_SCL, 400000);
	mpu.initialize(&i2c, MPU6050_DEFAULT_ADDRESS);

	xTaskCreate(update_mpu, "update_mpu", 2048, NULL, 2, NULL);
}

extern "C" void app_main(void)
{
	start();
}