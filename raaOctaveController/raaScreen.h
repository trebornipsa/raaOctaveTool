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
	virtual void screenChanged(raaScreen *pScreen) = 0;
	virtual void screenMatrixChanged(raaScreen *pScreen)=0;
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
	osg::Matrixf screenProjectionRotation();
	osg::Matrixf screenProjectionTranslation();
	osg::Matrixf screenTransform();


protected:
	void viewpointChanged(raaOctaveViewPoint *pViewpoint);

	void setScreen(osg::Vec3f vBL, osg::Vec3f vBR, osg::Vec3f vTR, osg::Vec3f vTL, raaOctaveViewPoint *pViewpoint);
	void setName(std::string sName);

	void calcProjectionMatrix(raaOctaveViewPoint *pViewpoint);

	osg::Vec3f m_vScreen[4];
	osg::Vec3f m_vEdge[4];
	osg::Vec3f m_vEdgeNorm[4];
	float m_fEdgeLen[4];
	float m_fNear;
	float m_fFar;

	osg::Vec3f m_vNormal;
	osg::Vec3f m_vCentre;
	std::string m_sName;
	raaScreenListener *m_pListener;

	osg::Matrixf m_mScreenProjection;
	osg::Matrixf m_mScreenProjectionRotation;
	osg::Matrixf m_mScreenProjectionTranslation;
	osg::Matrixf m_mScreenTransform;

	unsigned int m_uiScreenUpdateCount;
	unsigned int m_uiCurrentScreenUpdate;
	unsigned int m_uiCurrentViewpointUpdate;
};

