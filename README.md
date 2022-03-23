# MPU6050 sensor driver for ESP32
Sensor driver implementation based on JROWBERG implementation with some minor compatibility modifications. You can find the original code here: https://github.com/jrowberg/i2cdevlib/tree/master/ESP32_ESP-IDF/components/MPU6050

It is great as it allready includes DMP functionality.


It is prepared to be runned as component with dependency on I2Cdev lib (https://github.com/PiotrTopa/esp32-I2Cdev). It is compatibile with PlatformIO esp-idf framework.

# Add components to your project
You need to add this repository along with I2Cdev to your project as ESP-IDF component.
```bash
cd ~/myProjects/myProject
cd components
git clone https://github.com/PiotrTopa/esp32-I2Cdev I2Cdev
git clone https://github.com/PiotrTopa/esp32-MPU6050 MPU6050
```

# Example
Please see code example at `/example/main.c`