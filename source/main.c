#include <stdio.h>
#include "board.h"
#include "peripherals.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "LPC55S69_cm33_core0.h"
#include "fsl_debug_console.h"

#include "ArduCAM.h"
#include "CameraConfig.h"
#include "CameraTypes.h"
#include "CBSCommunication.h"

#define IMAGE_SIZE 30000

void SysTick_Handler(void)
{
	GPIO_PortToggle(BOARD_INITPINS_LED_B_GPIO, BOARD_INITPINS_LED_B_PORT, 1u << BOARD_INITPINS_LED_B_PIN);
}

int main(void) {

    /* Init board hardware. */
    BOARD_InitBootPins();
    BOARD_InitBootClocks();
    BOARD_InitBootPeripherals();
#ifndef BOARD_INIT_DEBUG_CONSOLE_PERIPHERAL
    /* Init FSL debug console. */
    BOARD_InitDebugConsole();
#endif
    Error_Code_T status;

    ArduCAM* arduCam =  ArduCamInstance();

    status = initModel(arduCam, OV5642_MINI_5MP_PLUS);
    if(status == SUCCESS)
    {
    	PRINTF("Correct initial camera model %d\n\r", arduCam->sensor_model);

    	status = testCameraSPI();
		if (status == SUCCESS)
		{
			PRINTF("Success ArduCAM SPI test register return value\r\n");

			status = checkTypeCameraModule(arduCam);
			if(status == SUCCESS)
			{
				PRINTF("Camera detected\r\n");

				setCameraFormatImage(arduCam, JPEG);
				status = init(arduCam);
				if(status == SUCCESS)
				{
					PRINTF("Camera initialized success\r\n");

					setCameraOption(ARDUCHIP_TIM, VSYNC_LEVEL_MASK);
					clearFifoFlag();
					setCameraOption(ARDUCHIP_FRAMES, 0x01);
					clearFifoFlag();
					clearFifoFlag();
					status = setJPEGsize(arduCam, OV5642_320x240);
					delay(1000);
					if(status == SUCCESS)
					{
						PRINTF("Set camera properties success, camera ready to work\r\n");

						int photoCounter = 0;
						uint32_t length = 0;
						uint8_t image[IMAGE_SIZE] = {0};

						while(1)
						{
							PRINTF("Start Capture\r\n");
							makePhotoCapture();
							PRINTF("Capture Done\r\n");

							Error_Code_T getImgStatus = getImage(image, IMAGE_SIZE);
							PRINTF("check correct jpg format: %d\r\n", getImgStatus);

	//						SysTick_Config(SystemCoreClock / 100U);

							length = getlenghtJPEG();
							PRINTF("JPEG size = %d\r\n", length);

							PRINTF("number frame = %d\r\n", photoCounter);
							if(photoCounter == 100)
							{
								break;
							}
							photoCounter++;

							clearFifoFlag();
							clearFifoFlag();
							delay(1);
						}

						for(int i = 0; i < length; i++)
						{
							PRINTF("%d,", image[i]);
						}
						PRINTF("\r\n");
					}
					else
					{
						PRINTF("Set camera properties failed\r\n");
					}
				}
				else
				{
					PRINTF("Camera initialized error\r\n");
				}
			}
			else
			{
				PRINTF("Can't find camera module\r\n");
			}
		}
		else
		{
			SysTick_Config(SystemCoreClock / 10U);
			PRINTF("Incorrect ArduCAM SPI test register return value\r\n");
		}
    }
    else
    {
    	PRINTF("No recognized camera model\n\r");
    }

//    while(1) {
//        i++ ;
//        PRINTF("i = %d\r\n", i);
//    }

    return 0 ;
}
