#ifndef ARDU_CAM_INC__ARDUCAM_H_
#define ARDU_CAM_INC__ARDUCAM_H_

#include <stdint.h>
#include "common_types.h"

#define UNDEFINED_FORMAT 	0
#define BMP 				1
#define JPEG				2
#define RAW	  				3

struct sensor_reg {
	uint16_t reg;
	uint16_t val;
};

typedef struct ArduCAM
{
	uint8_t sensor_model;
	uint8_t sensor_addr;

	uint8_t m_fmt;
} ArduCAM;

Error_Code_T initModel(ArduCAM* cam, uint8_t model);
Error_Code_T init(ArduCAM* arduCam);

void makePhotoCapture();
uint32_t getlenghtJPEG();
Error_Code_T getImage(uint8_t *image, uint32_t size);

Error_Code_T testCameraSPI();
Error_Code_T checkTypeCameraModule(ArduCAM* arduCam);

Error_Code_T setCameraFormatImage(ArduCAM* arduCam, uint8_t fmt);
Error_Code_T setJPEGsize(ArduCAM* arduCam, uint8_t size);
void setCameraOption(uint8_t option, uint8_t value);

void clearFifoFlag();

void set_burst_fifo();

#endif /* ARDU_CAM_INC__ARDUCAM_H_ */
