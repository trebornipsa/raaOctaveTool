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
	const static unsigned int csm_uiOCScreenVertexAll = 24;
	const static unsigned int csm_uiOCControllerRequestScreenNames = 25;
	const static unsigned int csm_uiOCControllerRequestScreen = 26;
	const static unsigned int csm_uiOCControllerRequestScreenAll = 27;
	
	const static unsigned int csm_uiOCBL = 0;
	const static unsigned int csm_uiOCBR = 1;
	const static unsigned int csm_uiOCTL = 2;
	const static unsigned int csm_uiOCTR = 3;
};
