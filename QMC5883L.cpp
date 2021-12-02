#include "QMC5883L.h"
#include <Arduino.h>

QMC5883L::QMC5883L(TwoWire& wire){
    _wire = & wire;
    mag_scale = 1.0;
}

void QMC5883L::setMeasureMode(uint8_t mode){
    // read accual value of regiter
    uint8_t* buff;
    write(SET_RESET_REG,0x01);
    buff = read(CONTROL_REG_1,1);
    //reset field for measurement mode
    *buff &= 0xFC;
    //write measurement mode
    *buff |= mode;
    write(CONTROL_REG_1,*buff);
}

void QMC5883L::setSamplingRate(uint8_t sample){
    // read accual value of regiter
    uint8_t* buff;
    buff = read(CONTROL_REG_1,1);
    //reset field for sampling rate
    *buff &= 0xF3;
    //write measurement mode
    *buff |= (sample << 2);
    write(CONTROL_REG_1,*buff);
}

void QMC5883L::setScale(uint8_t scale){ // Scaled value in G
    // read accual value of regiter
    uint8_t* buff;
    buff = read(CONTROL_REG_1,1);
    //reset field for scale
    *buff &= 0xCF;
    //write measurement mode
    *buff |= (scale << 4);
    write(CONTROL_REG_1,*buff);
    switch(scale){ 
        case SCALE_2G:
            mag_scale = 1/1200; 
            break;

        case SCALE_8G:
            mag_scale = 1/3000;
            break;
    }

}

rawDataMag QMC5883L::readRawData(){
    rawDataMag raw = rawDataMag();
    uint8_t* buff = read(DATA_REG_START,6);

    raw.DataX = (buff[1] << 8) | buff[0];
    raw.DataY = (buff[3] << 8) | buff[2];
    raw.DataZ = (buff[5] << 8) | buff[4];

    return raw;
}

scaledDataMag QMC5883L::readScaledData(){
    rawDataMag raw = readRawData();
    scaledDataMag scaled = scaledDataMag();

    scaled.DataX = mag_scale*raw.DataX;
    scaled.DataY = mag_scale*raw.DataX;
    scaled.DataZ = mag_scale*raw.DataX;
    return scaled;
}


void QMC5883L::write(uint8_t reg, byte data){
    _wire->beginTransmission(QMC8553L_ADDR);
    _wire->write(reg);
    _wire->write(data);
    _wire->endTransmission();
}


uint8_t* QMC5883L::read(uint8_t reg,uint8_t len){
    _wire->beginTransmission(QMC8553L_ADDR);
    _wire->write(reg);
    _wire->endTransmission();

    _wire->requestFrom(QMC8553L_ADDR,len);

    if(_wire->available() == len){
        for(uint8_t i = 0; i < len && i < sizeof(_buffer); i++)
        _buffer[i] = _wire->read();
    }

    return _buffer;
}