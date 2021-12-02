#ifndef _QMC5883L_
#define _QMC5883L_

#include <Arduino.h>
#include <Wire.h>

#define QMC8553L_ADDR 0x0D
#define ID_REG_1 0x09
#define ID_REG_2 0x0A
#define STATUS_REG 0x06
#define TEMP_REG 0x07
#define CONTROL_REG_1 0x09
#define CONTROL_REG_2 0x0A
#define SET_RESET_REG 0x0B
#define DATA_REG_START 0x00

#define CONTINOUS_MODE 0x01
#define STANDBY_MODE 0x00
#define IDLE_MODE 0x03

#define SCALE_2G 0x00
#define SCALE_8G 0x01

#define SAMPLE_10HZ 0x00
#define SAMPLE_50HZ 0x01
#define SAMPLE_100HZ 0x02
#define SAMPLE_200HZ 0x03

struct rawDataMag{
    int DataX;
    int DataY;
    int DataZ;
};

struct scaledDataMag{
    float DataX;
    float DataY;
    float DataZ;
};

class QMC5883L{
    public:

        void initMagnetometer();
    
    public: 
        QMC5883L(TwoWire& wire);

        void setMeasureMode(uint8_t mode);
        void setSamplingRate(uint8_t sample);
        void setScale(uint8_t scale);

        rawDataMag readRawData();
        scaledDataMag readScaledData();

    protected:

        void write(uint8_t reg, byte msg);
        uint8_t* read(uint8_t reg,uint8_t len);

    private:
        float mag_scale;
        uint8_t _buffer[16];
        TwoWire* _wire;
};

#endif