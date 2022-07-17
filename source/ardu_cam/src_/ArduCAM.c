#include "ArduCAM.h"
#include "CameraConfig.h"
#include "CameraTypes.h"
#include "CBSCommunication.h"

#if (defined(OV5642_MINI_5MP_PLUS))
	#include "ov5642_regs.h"
#endif

#include "fsl_debug_console.h"

static Error_Code_T resetCPLD(uint8_t sensorModel)
{
	Error_Code_T result = SUCCESS;

	switch(sensorModel)
	{
	case OV5642:
		write_reg(0x07, 0x80);
		delay(100);
		write_reg(0x07, 0x00);
		delay(100);
		break;
	default:
		result = ERROR;
	}

	return result;
}

static uint8_t get_bit(uint8_t addr, uint8_t bit)
{
	uint8_t val = read_reg(addr);
	val = val & bit;
	return val;
}

void set_burst_fifo()
{
	(void) send_data(BURST_FIFO_READ);
}

Error_Code_T initModel(ArduCAM* arduCam, uint8_t model)
{
	Error_Code_T result = SUCCESS;

	if(arduCam == NULL)
	{
		result = NULLPTR;
	}
	else
	{
		switch(model)
		{
		case OV5642_MINI_5MP_PLUS:
			arduCam->sensor_model = OV5642;
			arduCam->sensor_addr = 0x3C;
			arduCam->m_fmt = UNDEFINED_FORMAT;
			break;
		default:
			result = ERROR;
		}

		result = resetCPLD(arduCam->sensor_model);
	}

	return result;
}

Error_Code_T init(ArduCAM* arduCam)
{
	Error_Code_T result = SUCCESS;

	if(arduCam == NULL)
	{
		result = NULLPTR;
	}
	else
	{
		switch(arduCam->sensor_model)
		{
		case OV5642:
			if(arduCam->m_fmt != UNDEFINED_FORMAT)
			{
				(void) wrSensorReg16_8(arduCam->sensor_addr, 0x3008, 0x80);
				if(arduCam->m_fmt == RAW)
				{
					(void) wrSensorRegs16_8(arduCam->sensor_addr, OV5642_1280x960_RAW);
					(void) wrSensorRegs16_8(arduCam->sensor_addr, OV5642_640x480_RAW);
				}
				else
				{
					(void) wrSensorRegs16_8(arduCam->sensor_addr, OV5642_QVGA_Preview);
					delay(100);
				}

				if(arduCam->m_fmt == JPEG)
				{
					delay(100);
					(void) wrSensorRegs16_8(arduCam->sensor_addr, OV5642_JPEG_Capture_QSXGA);
					(void) wrSensorRegs16_8(arduCam->sensor_addr, ov5642_320x240);
					delay(100);
					(void) wrSensorReg16_8(arduCam->sensor_addr, 0x3818, 0xa8);
					(void) wrSensorReg16_8(arduCam->sensor_addr, 0x3621, 0x10);
					(void) wrSensorReg16_8(arduCam->sensor_addr, 0x3801, 0xb0);
					(void) wrSensorReg16_8(arduCam->sensor_addr, 0x4407, 0x04);
					(void) wrSensorReg16_8(arduCam->sensor_addr, 0x5888, 0x00);
					(void) wrSensorReg16_8(arduCam->sensor_addr, 0x5000, 0xFF);
				}
				else
				{
					uint8_t reg_val;
					(void) wrSensorReg16_8(arduCam->sensor_addr, 0x4740, 0x21);
					(void) wrSensorReg16_8(arduCam->sensor_addr, 0x501e, 0x2a);
					(void) wrSensorReg16_8(arduCam->sensor_addr, 0x5002, 0xf8);
					(void) wrSensorReg16_8(arduCam->sensor_addr, 0x501f, 0x01);
					(void) wrSensorReg16_8(arduCam->sensor_addr, 0x4300, 0x61);
					(void) rdSensorReg16_8(arduCam->sensor_addr, 0x3818, &reg_val);
					(void) wrSensorReg16_8(arduCam->sensor_addr, 0x3818, (reg_val | 0x60) & 0xff);
					(void) rdSensorReg16_8(arduCam->sensor_addr, 0x3621, &reg_val);
					(void) wrSensorReg16_8(arduCam->sensor_addr, 0x3621, reg_val & 0xdf);
				}
			}
			else
			{
				result = ERROR;
			}
			break;
		default:
			result = ERROR;
		}
	}

	return result;
}

void makePhotoCapture()
{
	write_reg(ARDUCHIP_FIFO, FIFO_START_MASK);
	while(!get_bit(ARDUCHIP_TRIG, CAP_DONE_MASK));
}

uint32_t getlenghtJPEG()
{
	uint32_t len1 = read_reg(FIFO_SIZE1);
	uint32_t len2 = read_reg(FIFO_SIZE2);
	uint32_t len3 = read_reg(FIFO_SIZE3) & 0x7f;
	uint32_t length = ((len3 << 16) | (len2 << 8) | len1) & 0x07fffff;
	return length;
}

#include "fsl_debug_console.h"

Error_Code_T getImage(uint8_t *image, uint32_t size)
{
	Error_Code_T result = ERROR;

	if(image == NULL)
	{
		result = NULLPTR;
	}
	else
	{
		uint32_t length = getlenghtJPEG();
		delay(1);
		if(length < size)
		{
			CS_LOW();
			set_burst_fifo();
			for(uint32_t counterArrImg = 0; counterArrImg < length; counterArrImg++)
			{
				image[counterArrImg] = read_fifo();
			}
			CS_HIGH();

			if((image[0] == 0xFF) && (image[1] == 0xD8) && image[2] == 0xFF)
			{
				result = SUCCESS;
			}
		}
		else
		{
			result = ERROR;
		}
	}

	return result;
}

Error_Code_T testCameraSPI()
{
	Error_Code_T result = ERROR;

	write_reg(ARDUCHIP_TEST1, 0x55);
	uint8_t testValue = read_reg(ARDUCHIP_TEST1);
	if(testValue == 0x55)
	{
		result = SUCCESS;
	}

	return result;
}

Error_Code_T checkTypeCameraModule(ArduCAM* arduCam)
{
	Error_Code_T result = ERROR;

	if(arduCam == NULL)
	{
		result = NULLPTR;
	}
	else
	{
		uint8_t vid = 0;
		uint8_t pid = 0;

		switch(arduCam->sensor_model)
		{
		case OV5642:
			(void) rdSensorReg16_8(arduCam->sensor_addr, OV5642_CHIPID_HIGH, &vid);
			(void) rdSensorReg16_8(arduCam->sensor_addr, OV5642_CHIPID_LOW, &pid);
			if((vid == 0x56) && (pid == 0x42))
			{
				result = SUCCESS;
			}
			break;
		default:
			result = ERROR;
		}
	}

	return result;
}

Error_Code_T setCameraFormatImage(ArduCAM* arduCam, uint8_t fmt)
{
	Error_Code_T result = SUCCESS;

	if(arduCam == NULL)
	{
		result = NULLPTR;
	}
	else
	{
		switch(fmt)
		{
		case BMP:
			arduCam->m_fmt = BMP;
			break;
		case RAW:
			arduCam->m_fmt = RAW;
			break;
		case JPEG:
			arduCam->m_fmt = JPEG;
			break;
		default:
			arduCam->m_fmt = UNDEFINED_FORMAT;
			result = ERROR;
		}
	}

	return result;
}

Error_Code_T setJPEGsize(ArduCAM* arduCam, uint8_t size)
{
	Error_Code_T result = SUCCESS;

	if(arduCam == NULL)
	{
		result = NULLPTR;
	}
	else
	{
		if(arduCam->sensor_model == OV5642)
		{
			switch(size)
			{
			case OV5642_320x240:
				(void) wrSensorRegs16_8(arduCam->sensor_addr, ov5642_320x240);
				break;
			case OV5642_640x480:
				(void) wrSensorRegs16_8(arduCam->sensor_addr, ov5642_640x480);
				break;
			case OV5642_1024x768:
				(void) wrSensorRegs16_8(arduCam->sensor_addr, ov5642_1024x768);
				break;
			case OV5642_1280x960:
				(void) wrSensorRegs16_8(arduCam->sensor_addr, ov5642_1280x960);
				break;
			case OV5642_1600x1200:
				(void) wrSensorRegs16_8(arduCam->sensor_addr, ov5642_1600x1200);
				break;
			case OV5642_2048x1536:
				(void) wrSensorRegs16_8(arduCam->sensor_addr, ov5642_2048x1536);
				break;
			case OV5642_2592x1944:
				(void) wrSensorRegs16_8(arduCam->sensor_addr, ov5642_2592x1944);
				break;
			default:
				result = ERROR;
			}
		}
		else
		{
			result = ERROR;
		}
	}

	return result;
}

void setCameraOption(uint8_t option, uint8_t value)
{
	write_reg(option, value);
}

void clearFifoFlag()
{
	write_reg(ARDUCHIP_FIFO, FIFO_CLEAR_MASK);
}
























