#pragma once
#include <list>
#include "raaOctaveControllerDefs.h"
#include <osg/Matrix>
#include <QtCore/QMutex>

class raaOctaveViewPoint;


class RAAOCTAVECONTROLLER_DLL_DEF raaOctaveViewPointListener
{
	friend raaOctaveViewPoint;
public:
	raaOctaveViewPointListener();
	virtual ~raaOctaveViewPointListener();

protected:
	virtual void physicalViewpointChanged(raaOctaveViewPoint* pViewpoint) = 0;
	virtual void virtualViewpointChanged(raaOctaveViewPoint* pViewpoint) = 0;
};

typedef std::list<raaOctaveViewPointListener*> raaOctaveViewPointListeners;

class RAAOCTAVECONTROLLER_DLL_DEF raaOctaveViewPoint
{
public:
	raaOctaveViewPoint();
	virtual ~raaOctaveViewPoint();

	void setVirtualMatrix(osg::Matrixf m);
	void setPhysicalMatrix(osg::Matrixf m);
	void setDefaultPhysicalMatrix(osg::Matrixf m);
	void setDefaultPhysicalMatrix();

	osg::Matrixf defaultPhysicalMatrix();
	osg::Matrixf physicalMatrix();
	osg::Matrixf physicalLeftMatrix();
	osg::Matrixf physicalRightMatrix();
	osg::Matrixf virtualMatrix();

	void addListener(raaOctaveViewPointListener *pListener);
	void removeListener(raaOctaveViewPointListener *pListener);

	unsigned int currentScreenUpdate();
protected:
	osg::Matrixf m_Virtual;
	osg::Matrixf m_Physical;
	osg::Matrixf m_DefaultPhysical;
	osg::Matrixf m_LeftEye;
	osg::Matrixf m_RightEye;

	float m_fEyeSeperation;

	raaOctaveViewPointListeners m_lListener;
	unsigned int m_uiViewpointUpdateCount;

	QMutex m_Mutex;

	void calcStereoEyePostions();
};

