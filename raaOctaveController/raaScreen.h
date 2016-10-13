#pragma once

#include <osg/Vec3f>
#include <osg/Matrix>
#include <string>


#include "raaOctaveViewPoint.h"
#include "raaOctaveControllerDefs.h"


class raaScreen;
class raaOctaveViewPoint;


class RAAOCTAVECONTROLLER_DLL_DEF raaScreenListener
{
	friend raaScreen;
public:
	raaScreenListener();
	virtual ~raaScreenListener();

	virtual void nameChanged(raaScreen *pScreen) = 0;
	virtual void screenMatrixChanged(raaScreen *pScreen) = 0;
	virtual void screenChanged(raaScreen *pScreen) = 0;
};

class RAAOCTAVECONTROLLER_DLL_DEF raaScreen: public raaOctaveViewPointListener
{
public:
	raaScreen(std::string sName, osg::Vec3f vBL, osg::Vec3f vBR, osg::Vec3f vTR, osg::Vec3f vTL, raaOctaveViewPoint *pViewpoint);
	virtual ~raaScreen();

	void setListener(raaScreenListener *pListener);
	void removeListener(raaScreenListener *pListener);

	std::string name();
	osg::Vec3f screenVert(unsigned int uiVert);
	osg::Vec3f normal();
	osg::Matrixf screenProjection();

protected:
	virtual void physicalViewpointChanged(raaOctaveViewPoint *pViewpoint);
	virtual void virtualViewpointChanged(raaOctaveViewPoint* pViewpoint);
	void setScreen(osg::Vec3f vBL, osg::Vec3f vBR, osg::Vec3f vTR, osg::Vec3f vTL, raaOctaveViewPoint *pViewpoint);
	void setName(std::string sName);
	void calcProjectionMatrix(raaOctaveViewPoint *pViewpoint);
	static void initialise();

	osg::Vec3f m_vScreen[4];
	osg::Vec3f m_vEdgeNorm[4];
	float m_fNear;
	float m_fFar;

	osg::Vec3f m_vNormal;
	std::string m_sName;
	raaScreenListener *m_pListener;

	osg::Matrixf m_mScreenProjection;
	osg::Matrixf m_mScreenProjectionRotation;
	osg::Matrixf m_mScreenProjectionTranslation;
	static osg::Matrixf sm_RotationScale;
	static bool sm_bIntialised;

	unsigned int m_uiScreenUpdateCount;
	unsigned int m_uiCurrentScreenUpdate;
	unsigned int m_uiCurrentViewpointUpdate;
};

