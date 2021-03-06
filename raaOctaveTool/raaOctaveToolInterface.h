#pragma once

#include <string>
#include <map>

#include <QtWidgets\QMainWindow>

#include "raaOctaveController/raaOctaveController.h"
#include "raaOctaveController/raaOctaveViewPoint.h"

#include "raaOctaveToolInterfaceQt.h"

class raaDisplayScreen;

typedef std::map<std::string, raaDisplayScreen*>raaDisplayScreens;

class raaOctaveToolInterface: public QMainWindow, public Ui::raaOctaveToolInterfaceQt, public raaOctaveControllerListener, public raaOctaveViewPointListener
{
	Q_OBJECT
public:
	const static unsigned int csm_uiTranslate = 0;
	const static unsigned int csm_uiRotate = 1;
	const static unsigned int csm_uiUpX = 2;
	const static unsigned int csm_uiDownX = 3;
	const static unsigned int csm_uiUpZ = 4;
	const static unsigned int csm_uiDownZ = 5;
	const static unsigned int csm_uiUpY = 6;
	const static unsigned int csm_uiDownY = 7;

	const static unsigned int csm_uiPos = 0;
	const static unsigned int csm_uiRight = 1;
	const static unsigned int csm_uiUp = 2;
	const static unsigned int csm_uiDir = 3;


	raaOctaveToolInterface();
	virtual ~raaOctaveToolInterface();

public slots:
	void phyTrans(bool);
	void phyRot(bool);
	void phyXUpPressed();
	void phyXUpReleased();
	void phyXDownPressed();
	void phyXDownReleased();
	void phyYUpPressed();
	void phyYUpReleased();
	void phyYDownPressed();
	void phyYDownReleased();
	void phyZUpPressed();
	void phyZUpReleased();
	void phyZDownPressed();
	void phyZDownReleased();
	void virTrans(bool);
	void virRot(bool);
	void virXUpPressed();
	void virXUpReleased();
	void virXDownPressed();
	void virXDownReleased();
	void virYUpPressed();
	void virYUpReleased();
	void virYDownPressed();
	void virYDownReleased();
	void virZUpPressed();
	void virZUpReleased();
	void virZDownPressed();
	void virZDownReleased();
	void timerUpdate();
	void lockCamera(int);
protected:
	raaOctaveController *m_pController;

	osg::Geode* makeGrid(float fWidth, float fDepth, unsigned int uiWidthSegs, unsigned int uiDepthSegs);
	void updateView();

	virtual void originChanged(raaOctaveController* pController);
	virtual void screenAdded(raaOctaveController* pController, raaScreen* pScreen);
	virtual void screenRemoved(raaOctaveController* pController, raaScreen* pScreen);
	virtual void screenUpdated(raaOctaveController* pController, raaScreen* pScreen);
	virtual void viewpointChanged(raaOctaveViewPoint* pViewpoint);

	raaDisplayScreens m_mDisplays;

	osg::Group *m_pVirtualScene;
	osg::MatrixTransform *m_pPhysicalViewpoint;

	bool m_abPhysical[8];
	bool m_abVirtual[8];

	osg::Vec3f m_vTrans;
	osg::Vec3f m_avPhysical[4];
	osg::Matrix m_CameraManipulatorMatrix;
	bool m_bLockCamera;

	osgGA::CameraManipulator *m_pManpulator;
};

