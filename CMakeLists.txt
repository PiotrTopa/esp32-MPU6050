set(COMPONENT_SRCS "src/MPU6050.cpp")
set(COMPONENT_ADD_INCLUDEDIRS "include")
set(COMPONENT_REQUIRES
    I2Cdev
)

register_component()
