#pragma once

#include "raaOctaveKernelDefs.h"

class RAAOCTAVEKERNEL_DLL_DEF raaOctaveKernel
{
public:
	const static unsigned int csm_uiOCHasConfig = 0;
	const static unsigned int csm_uiOCHasConfigTrue = 1;
	const static unsigned int csm_uiOCHasConfigFalse = 2;
	const static unsigned int csm_uiOCLoadConfig = 3;
	const static unsigned int csm_uiOCSaveConfig = 4;
	const static unsigned int csm_uiOCSaveConfigAs = 5;
	const static unsigned int csm_uiOCAttachControllerListener = 6;
	const static unsigned int csm_uiOCDetachControllerListener = 7;
	const static unsigned int csm_uiOCAttachScreenListener = 8;
	const static unsigned int csm_uiOCDetachScreenListener = 9;
	const static unsigned int csm_uiOCAttachViewpointListener = 10;
	const static unsigned int csm_uiOCDetachViewpointListener = 11;
	const static unsigned int csm_uiOCControllerScreenAdded = 12;
	const static unsigned int csm_uiOCControllerScreenRemoved = 13;
	const static unsigned int csm_uiOCControllerScreenUpdated = 14;
	const static unsigned int csm_uiOCViewpointPhysicalChanged = 15;
	const static unsigned int csm_uiOCViewpointVirtualChanged = 16;
	const static unsigned int csm_uiOCViewpointUpdatePhysical = 17;
	const static unsigned int csm_uiOCViewpointUpdateVirtual = 18;
	const static unsigned int csm_uiOCScreenNameChanged = 19;
	const static unsigned int csm_uiOCScreenMatrixChanged = 20;
	const static unsigned int csm_uiOCScreenChanged = 21;
	const static unsigned int csm_uiOCScreenInfo = 22;
	const static unsigned int csm_uiOCScreenVertex = 23;
	const static unsigned int csm_uiOCScreenNearFar = 24;
	const static unsigned int csm_uiOCScreenVertexAll = 25;
	const static unsigned int csm_uiOCControllerRequestScreenNames = 26;
	const static unsigned int csm_uiOCControllerRequestScreen = 27;
	const static unsigned int csm_uiOCControllerRequestScreenAll = 28;
	const static unsigned int csm_uiOCScreenRequestImageRotation = 29;
	const static unsigned int csm_uiOCScreenImageRotationInfo = 30;
	const static unsigned int csm_uiOCScreenRequestImageFlip = 31;
	const static unsigned int csm_uiOCScreenImageFlipInfo = 32;
	const static unsigned int csm_uiOCWindowInfo = 33;
	const static unsigned int csm_uiOCDisplayChanged = 34;
	const static unsigned int csm_uiOCStereoChanged = 35;

	const static unsigned int csm_uiOCTrackerAllNames = 60;
	const static unsigned int csm_uiOCTrackerInfo = 61;
	const static unsigned int csm_uiOCTrackerSensors = 62;
	const static unsigned int csm_uiOCTrackerOriginTransform = 63;
	const static unsigned int csm_uiOCTrackerSensorTransform = 64;
	const static unsigned int csm_uiOCTrackerSensorTimeout = 65;
	const static unsigned int csm_uiOCTrackerEyeTracker = 66;
	const static unsigned int csm_uiOCTrackerAddRemoveListener = 67;

	const static unsigned int csm_uiOCScreenAdd = 70;
	const static unsigned int csm_uiOCScreenRemove = 71;

	const static unsigned int csm_uiOCListConfigs = 80;

	const static unsigned int csm_uiOCDisplays = 90;

	const static unsigned int csm_uiOCBL = 0;
	const static unsigned int csm_uiOCBR = 1;
	const static unsigned int csm_uiOCTL = 2;
	const static unsigned int csm_uiOCTR = 3;

	const static unsigned int csm_uiOCMono = 0;
	const static unsigned int csm_uiOCLeftEye = 1;
	const static unsigned int csm_uiOCRightEye = 2;
	const static unsigned int csm_uiOCAnaglyphic = 3;
	const static unsigned int csm_uiOCQuadBuffer = 4;
};

