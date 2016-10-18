#pragma once

#include <windows.h>
#include <gl/gl.h>

#include <osgViewer/CompositeViewer>
#include "raaOctaveSystemDefs.h"

class RAAOCTAVESYSTEM_DLL_DEF raaOctaveSystem: public osgViewer::CompositeViewer
{
public:
	raaOctaveSystem();
	virtual ~raaOctaveSystem();

protected:

	void addDisplay(int iScreen, std::string sName, int iX, int iY, int iW, int iH);

	osg::Group *m_pScene;
};

