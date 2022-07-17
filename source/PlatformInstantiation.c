#include "ArduCAM.h"
#include "CameraTypes.h"

ArduCAM* ArduCamInstance()
{
	static ArduCAM arduCam;
	return &arduCam;
}
