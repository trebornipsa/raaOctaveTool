#pragma once

#include <string>
#include <map>

#include "raaOctaveControllerDefs.h"

class raaScreen;

typedef std::map<std::string, raaScreen*>raaStringScreenMap;

class RAAOCTAVECONTROLLER_DLL_DEF raaOctaveControllerTypes
{
public:
	const static unsigned int csm_uiBL = 0;
	const static unsigned int csm_uiBR = 1;
	const static unsigned int csm_uiTR = 2;
	const static unsigned int csm_uiTL = 3;

	const static unsigned int csm_uiLeft = 0;
	const static unsigned int csm_uiRight = 1;
	const static unsigned int csm_uiBottom = 2;
	const static unsigned int csm_uiTop = 3;
};

