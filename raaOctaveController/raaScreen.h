#pragma once

#include <osg/Vec3f>
#include <osg/Matrix>
#include <string>
#include <list>


#include "raaOctaveViewPoint.h"
#include "raaOctaveControllerDefs.h"
#include <QtCore/QMutex>


class raaScreen;
class raaOctaveViewPoint;


class RAAOCTAVECONTROLLER_DLL_DEF raaScreenListener
{
	friend raaScreen;
public:
	raaScreenListener();
	virtual ~raaScreenListener();

	virtual void nameChanged(raaScreen *pScreen) = 0;
	virtual void screenProjMatrixChanged(raaScreen *pScreen) = 0;
	virtual void screenChanged(raaScreen *pScreen) = 0;
	virtual void windowChanged(raaScreen *pScreen) = 0;
	virtual void displayChanged(raaScreen *pScreen) = 0;
	virtual void stereoChanged(raaScreen *pScreen) = 0;
};

typedef std::list<raaScreenListener*>raaScreenListeners;

class RAAOCTAVECONTROLLER_DLL_DEF raaScreen: public raaOctaveViewPointListener
{
public:
	raaScreen(std::string sName, int iScreen, int iStereo, osg::Vec3f vBL, osg::Vec3f vBR, osg::Vec3f vTR, osg::Vec3f vTL, float fNear, float fFar, float fRot, bool bFlipX, bool bFlipY, bool bFlipZ, int iX, int iY, int iW, int iH, raaOctaveViewPoint *pViewpoint);
	virtual ~raaScreen();

	void addListener(raaScreenListener *pListener);
	void removeListener(raaScreenListener *pListener);

	std::string name();
	osg::Vec3f screenVert(unsigned int uiVert);
	void setScreenVert(unsigned int uiVert, osg::Vec3f v);
	void setScreenVerts(osg::Vec3f vBL, osg::Vec3f vBR, osg::Vec3f vTL, osg::Vec3f vTR);
	osg::Vec3f normal();
	osg::Matrixf screenProjection();
	osg::Matrixf screenLeftProjection();
	osg::Matrixf screenRightProjection();
	osg::Matrixf screenView();
	osg::Matrixf screenLeftView();
	osg::Matrixf screenRightView();
	float nearClip();
	float farClip();
	void setNearFar(float fNear, float fFar);
	bool flipped(unsigned int uiAxis);
	float rotation();
	void setFlipped(bool bX, bool bY, bool bZ);
	void setRotation(float fRot);

	void setWindow(int iX, int iY, int iW, int iH);
	int window(unsigned int uiParam);

	float projParam(unsigned int uiIndex);

	int screen();
	void setScreen(int iScreen);

	int stereo();
	void setStereo(int iStereo);

protected:
	virtual void physicalViewpointChanged(raaOctaveViewPoint *pViewpoint);
	virtual void virtualViewpointChanged(raaOctaveViewPoint* pViewpoint);
	void setScreen(osg::Vec3f vBL, osg::Vec3f vBR, osg::Vec3f vTR, osg::Vec3f vTL, raaOctaveViewPoint *pViewpoint);
	void setName(std::string sName);
	void calcProjectionMatrix(raaOctaveViewPoint *pViewpoint);
	static void initialise();
	void updateScreen(raaOctaveViewPoint *pViewpoint);

	osg::Vec3f m_vScreen[4];
	osg::Vec3f m_vEdgeNorm[4];
	osg::Vec3f m_vScreenUp;
	osg::Vec3f m_vScreenRight;
	float m_fNear;
	float m_fFar;
	bool m_abFlip[3];
	float m_fRotation;
	int m_aiWindow[4];
	int m_iScreen;
	int m_iStereo;

	float m_afProjParam[6];

	osg::Vec3f m_vNormal;
	std::string m_sName;
	raaScreenListeners m_lListeners;

	osg::Matrixf m_mScreenProjection;
	osg::Matrixf m_mScreenLeftProjection;
	osg::Matrixf m_mScreenRightProjection;
	osg::Matrixf m_mScreenProjectionRotation;
	osg::Matrixf m_mScreenProjectionTranslation;
	osg::Matrixf m_mScreenProjectionImageRotation;
	osg::Matrixf m_mScreenProjectionImageFlipScale;
	osg::Matrixf m_mScreenViewMatrix;
	osg::Matrixf m_mScreenLeftViewMatrix;
	osg::Matrixf m_mScreenRightViewMatrix;

	static osg::Matrixf sm_RotationScale;
	static bool sm_bIntialised;

	unsigned int m_uiScreenUpdateCount;
	unsigned int m_uiCurrentScreenUpdate;
	unsigned int m_uiCurrentViewpointUpdate;

	raaOctaveViewPoint *m_pLastViewpoint;

	QMutex m_Mutex;
};

