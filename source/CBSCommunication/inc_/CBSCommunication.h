#ifndef CBSCOMMUNICATION_INC__CBSCOMMUNICATION_H_
#define CBSCOMMUNICATION_INC__CBSCOMMUNICATION_H_

#include "common_types.h"
#include "peripherals.h"
#include "fsl_gpio.h"

#include "ArduCAM.h"

void write_reg(uint8_t addr, uint8_t data);
uint8_t read_reg(uint8_t addr);
uint8_t read_fifo();
bool send_data(uint8_t data);

bool rdSensorReg16_8(uint8_t addr, uint16_t regID, uint8_t* regDat);
bool wrSensorReg16_8(uint8_t addr, uint16_t regID, uint8_t regDat);
bool wrSensorRegs16_8(uint8_t addr, const struct sensor_reg reglist[]);

void delay(uint32_t ms);

void CS_HIGH();
void CS_LOW();

#endif /* CBSCOMMUNICATION_INC__CBSCOMMUNICATION_H_ */
