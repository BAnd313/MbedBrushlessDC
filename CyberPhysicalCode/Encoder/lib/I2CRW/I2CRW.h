#include "mbed.h"

#ifndef I2CRW_H
#define I2CRW_H

class I2CRW : public I2C {
    private:
        int device_address;
    public:
        I2CRW(int device_address, PinName sda, PinName scl);
        int read_reg(int reg_addr, int *reg_data, int cnt);
        int read_reg8(int reg_addr, int *reg_data);
        int write_reg8(int reg_addr, int reg_data);
};

#endif