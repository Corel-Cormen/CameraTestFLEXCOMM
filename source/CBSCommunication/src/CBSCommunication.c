#include "CBSCommunication.h"
#include "CameraConfig.h"

#include "fsl_debug_console.h"

static i2c_master_transfer_t i2c_tr = { 0 };
static spi_transfer_t spi_tr = { 0 };

static status_t send_i2c(uint8_t addr, uint8_t* data, uint8_t data_len)
{
	bool sendStatus = false;

	i2c_tr.data = data;
	i2c_tr.dataSize = data_len;
	i2c_tr.direction = kI2C_Write;
	i2c_tr.slaveAddress = addr;
	i2c_tr.flags = kI2C_TransferDefaultFlag;

	if(I2C_MasterTransferBlocking(FLEXCOMM4_PERIPHERAL, &i2c_tr) == kStatus_Success)
	{
		sendStatus = true;
	}
	return sendStatus;
}

static status_t read_i2c(uint8_t addr, uint8_t* data, uint8_t data_len)
{
	bool readStatus = false;

	i2c_tr.data = data;
	i2c_tr.dataSize = data_len;
	i2c_tr.direction = kI2C_Read;
	i2c_tr.slaveAddress = addr;
	i2c_tr.flags = kI2C_TransferDefaultFlag;

	if(I2C_MasterTransferBlocking(FLEXCOMM4_PERIPHERAL, &i2c_tr) == kStatus_Success)
	{
		readStatus = true;
	}
	return readStatus;
}

static status_t send_and_read_spi(uint8_t* tx_data, uint8_t tx_len, uint8_t* rx_data)
{
	bool sendReadStatus = false;

	spi_tr.dataSize = tx_len;
	spi_tr.txData = tx_data;
	spi_tr.rxData = rx_data;
	spi_tr.configFlags = kSPI_FrameAssert;

	if(SPI_MasterTransferBlocking(FLEXCOMM8_PERIPHERAL, &spi_tr) == kStatus_Success)
	{
		sendReadStatus = true;
	}
	return sendReadStatus;
}

static status_t send_spi(uint8_t* data, uint8_t data_len)
{
	return send_and_read_spi(data, data_len, NULL);
}

static bool bus_write(int address, int value)
{
	CS_LOW();

	uint8_t send_data[] = { address, value };
	bool status = send_spi(send_data, ARRAY_SIZE(send_data));

	CS_HIGH();
	return status;
}

static uint8_t bus_read(int address)
{
	CS_LOW();

	uint8_t send_data[] = { address, 0x00 };
	uint8_t read_data[2] = { 0 };
	send_and_read_spi(send_data, 2, read_data);
	uint8_t value = read_data[1];

	CS_HIGH();
	return value;
}

void write_reg(uint8_t addr, uint8_t data)
{
	(void) bus_write(addr | 0x80, data);
}

uint8_t read_reg(uint8_t addr)
{
	return bus_read(addr & 0x7F);
}

uint8_t read_fifo()
{
	return bus_read(SINGLE_FIFO_READ);
}

bool send_data(uint8_t data)
{
	uint8_t send_data[] = { data };
	return send_spi(send_data, ARRAY_SIZE(send_data));
}

bool rdSensorReg16_8(uint8_t addr, uint16_t regID, uint8_t* regDat)
{
	bool returnStatus = false;
	uint8_t send_data[] = {(regID >> 8), (regID & 0x00FF)};
	uint8_t read_data[1] = { 0 };

	bool sendStatus = send_i2c(addr, send_data, ARRAY_SIZE(send_data));
	bool readStatus = read_i2c(addr, read_data, ARRAY_SIZE(read_data));

	if((true == sendStatus) && (true == readStatus))
	{
		*regDat = read_data[0];
		returnStatus = true;
	}

	delay(1);

	return returnStatus;
}

bool wrSensorReg16_8(uint8_t addr, uint16_t regID, uint8_t regDat)
{
	bool returnStatus = false;
	uint8_t send_data[] = { (regID >> 8), (regID & 0x00FF), (regDat & 0x00FF) };

	bool sendStatus = send_i2c(addr, send_data, ARRAY_SIZE(send_data));

	if(true == sendStatus)
	{
		returnStatus = true;
	}

	delay(1);

	return returnStatus;
}

bool wrSensorRegs16_8(uint8_t addr, const struct sensor_reg reglist[])
{
	bool returnStatus = true;
	bool writeStatus = false;
	unsigned int regAddr = 0;
	unsigned char regVal = 0;
	const struct sensor_reg *regElement = reglist;

	while((regAddr != 0xffff) | (regVal != 0xff))
	{
		regAddr = regElement->reg;
		regVal = regElement->val;

		writeStatus = wrSensorReg16_8(addr, regAddr, regVal);
		if(false == writeStatus)
		{
			returnStatus = false;
		}

		regElement++;
	}

	delay(1);

	return returnStatus;
}

void CS_HIGH()
{
	GPIO_PinWrite(GPIO, 1, 1, 1);	/* HARDWARE SSEL */
}

void CS_LOW()
{
	GPIO_PinWrite(GPIO, 1, 1, 0);	/* HARDWARE SSEL */
}

void delay(uint32_t ms)
{
	SDK_DelayAtLeastUs(ms*1000, SDK_DEVICE_MAXIMUM_CPU_CLOCK_FREQUENCY);
}
