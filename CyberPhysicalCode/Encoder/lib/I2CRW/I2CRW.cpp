#include "I2CRW.h"

/**
 *  Constructor of I2CRW class
 *  @param device_address   address of the device
 *  @param sda  pin name of the I2C data bus
 *  @param scl  pin name of the I2C clock
 */
I2CRW::I2CRW(int device_address, PinName sda, PinName scl) : I2C(sda, scl) {
    this->device_address = device_address;
}

/**
 *  Read the content of a register
 *  @param  reg_addr    address of the register
 *  @param  reg_data    contains the read data
 *  @param  cnt     number of bytes to read
 *  @return     0 success, non-0 unsuccess
 */
int I2CRW::read_reg(int reg_addr, int *reg_data, int cnt) {
    int res = this->write(device_address<<1,(char*)&reg_addr,1);
    res += this->read(device_address<<1,(char*)reg_data,cnt);
    return res;
}

/**
 *  Read the content of a 8 bit register
 *  @param  reg_addr    address of the register
 *  @param  reg_data    contains the read data
 *  @return     0 success, non-0 unsuccess
 */
int I2CRW::read_reg8(int reg_addr, int *reg_data) {
    return I2CRW::read_reg(reg_addr,reg_data,1);
}

/**
 *  Write the content of a 8 bit register
 *  @param  reg_addr    address of the register
 *  @param  reg_data    contains the data to write
 *  @return     0 success, non-0 unsuccess
 */
int I2CRW::write_reg8(int reg_addr, int reg_data) {
    char cmd[2];
    cmd[0] = (char)reg_addr;
    cmd[1] = (char)reg_data;
    return this->write(device_address<<1,cmd,2);
}