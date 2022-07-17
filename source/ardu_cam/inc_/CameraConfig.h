#ifndef ARDU_CAM_INC__CAMERACONFIG_H_
#define ARDU_CAM_INC__CAMERACONFIG_H_

#include "CameraTypes.h"

/****************************************************/
/* Sensor related definition 						*/
/****************************************************/

#define OV5642		3

#define OV5642_CHIPID_HIGH		0x300a
#define OV5642_CHIPID_LOW 		0x300b

#define OV5642_320x240		0	//320x240
#define OV5642_640x480		1	//640x480
#define OV5642_1024x768		2	//1024x768
#define OV5642_1280x960 	3	//1280x960
#define OV5642_1600x1200	4	//1600x1200
#define OV5642_2048x1536	5	//2048x1536
#define OV5642_2592x1944	6	//2592x1944
#define OV5642_1920x1080  	7	//1920x1080

//Light Mode

#define Auto				0
#define Sunny				1
#define Cloudy				2
#define Office				3
#define Home				4

#define Advanced_AWB		0
#define Simple_AWB			1
#define Manual_day			2
#define Manual_A			3
#define Manual_cwf			4
#define Manual_cloudy		5

//Color Saturation

#define Saturation4			0
#define Saturation3			1
#define Saturation2			2
#define Saturation1			3
#define Saturation0			4
#define Saturation_1		5
#define Saturation_2		6
#define Saturation_3		7
#define Saturation_4		8

//Brightness

#define Brightness4			0
#define Brightness3			1
#define Brightness2			2
#define Brightness1			3
#define Brightness0			4
#define Brightness_1		5
#define Brightness_2		6
#define Brightness_3		7
#define Brightness_4		8

//Contrast

#define Contrast4			0
#define Contrast3			1
#define Contrast2			2
#define Contrast1			3
#define Contrast0			4
#define Contrast_1			5
#define Contrast_2			6
#define Contrast_3			7
#define Contrast_4			8

#define degree_180			0
#define degree_150			1
#define degree_120			2
#define degree_90			3
#define degree_60			4
#define degree_30			5
#define degree_0			6
#define degree30			7
#define degree60			8
#define degree90			9
#define degree120			10
#define degree150			11

//Special effects

#define Antique				0
#define Bluish				1
#define Greenish			2
#define Reddish				3
#define BW					4
#define Negative			5
#define BWnegative			6
#define Normal				7
#define Sepia				8
#define Overexposure		9
#define Solarize			10
#define Blueish				11
#define Yellowish			12

#define Exposure_17_EV				0
#define Exposure_13_EV				1
#define Exposure_10_EV				2
#define Exposure_07_EV				3
#define Exposure_03_EV				4
#define Exposure_default			5
#define Exposure03_EV				6
#define Exposure07_EV				7
#define Exposure10_EV				8
#define Exposure13_EV				9
#define Exposure17_EV				10

#define Auto_Sharpness_default		0
#define Auto_Sharpness1				1
#define Auto_Sharpness2				2
#define Manual_Sharpnessoff			3
#define Manual_Sharpness1			4
#define Manual_Sharpness2			5
#define Manual_Sharpness3			6
#define Manual_Sharpness4			7
#define Manual_Sharpness5			8

#define Sharpness1			0
#define Sharpness2			1
#define Sharpness3			2
#define Sharpness4			3
#define Sharpness5			4
#define Sharpness6			5
#define Sharpness7			6
#define Sharpness8			7
#define Sharpness_auto		8

#define EV3					0
#define EV2					1
#define EV1					2
#define EV0					3
#define EV_1				4
#define EV_2				5
#define EV_3				6

#define MIRROR				0
#define FLIP				1
#define MIRROR_FLIP			2

#define high_quality		0
#define default_quality		1
#define low_quality			2

#define Color_bar			0
#define Color_square		1
#define BW_square			2
#define DLI					3

#define Night_Mode_On		0
#define Night_Mode_Off		1

#define Off					0
#define Manual_50HZ			1
#define Manual_60HZ			2
#define Auto_Detection		3

/* Register initialization tables for SENSORs */
/* Terminating list entry for reg */
#define SENSOR_REG_TERM_8BIT			0xFF
#define SENSOR_REG_TERM_16BIT			0xFFFF
/* Terminating list entry for val */
#define SENSOR_VAL_TERM_8BIT			0xFF
#define SENSOR_VAL_TERM_16BIT			0xFFFF

//Define maximum frame buffer size
#if (defined OV5642_MINI_5MP_PLUS)
#define MAX_FIFO_SIZE	0x7FFFFF		//8MByte
#endif

/****************************************************/
/* ArduChip registers definition 					*/
/****************************************************/
#define RWBIT					0x80		//READ AND WRITE BIT IS BIT[7]

#define ARDUCHIP_TEST1			0x00		//TEST register

#define ARDUCHIP_FRAMES			0x01		//FRAME control register, Bit[2:0] = Number of frames to be captured

#define ARDUCHIP_MODE			0x02		//Mode register
#define MCU2LCD_MODE			0x00
#define CAM2LCD_MODE			0x01
#define LCD2MCU_MODE			0x02

#define ARDUCHIP_TIM			0x03		//Timming control
#define HREF_LEVEL_MASK			0x01		//0 = High active , 		1 = Low active
#define VSYNC_LEVEL_MASK		0x02		//0 = High active , 		1 = Low active
#define LCD_BKEN_MASK			0x04		//0 = Enable, 				1 = Disable
#define PCLK_DELAY_MASK			0x08		//0 = data no delay,		1 = data delayed one PCLK

#define ARDUCHIP_FIFO      		0x04		//FIFO and I2C control
#define FIFO_CLEAR_MASK    		0x01
#define FIFO_START_MASK    		0x02
#define FIFO_RDPTR_RST_MASK		0x10
#define FIFO_WRPTR_RST_MASK		0x20

#define ARDUCHIP_GPIO			0x06		//GPIO Write Register
#define GPIO_RESET_MASK			0x01		//0 = Sensor reset,				1 =  Sensor normal operation
#define GPIO_PWDN_MASK			0x02		//0 = Sensor normal operation, 	1 = Sensor standby
#define GPIO_PWREN_MASK			0x04		//0 = Sensor LDO disable,		1 = sensor LDO enable

#define BURST_FIFO_READ			0x3C		//Burst FIFO read operation
#define SINGLE_FIFO_READ		0x3D		//Single FIFO read operation

#define ARDUCHIP_REV			0x40		//ArduCHIP revision
#define VER_LOW_MASK			0x3F
#define VER_HIGH_MASK			0xC0

#define ARDUCHIP_TRIG			0x41		//Trigger source
#define VSYNC_MASK				0x01
#define SHUTTER_MASK			0x02
#define CAP_DONE_MASK			0x08

#define FIFO_SIZE1				0x42		//Camera write FIFO size[7:0] for burst to read
#define FIFO_SIZE2				0x43		//Camera write FIFO size[15:8]
#define FIFO_SIZE3				0x44		//Camera write FIFO size[18:16]

#endif /* ARDU_CAM_INC__CAMERACONFIG_H_ */
