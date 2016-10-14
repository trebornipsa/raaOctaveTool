#pragma once

#include <string>
#include <map>

#include <QtWidgets\QMainWindow>

#include "raaOctaveController/raaOctaveController.h"
#include "raaOctaveController/raaOctaveViewPoint.h"

#include <raaNetwork/raaNetwork.h>

#include "raaOctaveToolInterfaceQt.h"

class raaDisplayScreen;

typedef std::map<std::string, raaDisplayScreen*>raaDisplayScreens;

using namespace raaNet;

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
	void currentToolboxChanged(int);
	void currentScreenChanged(const QString&);
	void screenBLChanged(double);
	void screenBRChanged(double);
	void screenTLChanged(double);
	void screenTRChanged(double);
	void screenAllChanged();
	void screenContUpdate(int);

	void tcpRead(raaTcpMsg*);
	void tcpState(raaTcpThread*, unsigned int);
	void udpRead(raaTcpMsg*);
	void udpState(raaTcpThread*, unsigned int);

protected:
	raaOctaveController *m_pController;

	static osg::Geode* makeGrid(float fWidth, float fDepth, unsigned int uiWidthSegs, unsigned int uiDepthSegs);
	void updateView();

	virtual void originChanged(raaOctaveController* pController);
	virtual void screenAdded(raaOctaveController* pController, raaScreen* pScreen);
	virtual void screenRemoved(raaOctaveController* pController, raaScreen* pScreen);
	virtual void screenUpdated(raaOctaveController* pController, raaScreen* pScreen);
	virtual void physicalViewpointChanged(raaOctaveViewPoint* pViewpoint);
	virtual void virtualViewpointChanged(raaOctaveViewPoint* pViewpoint);
	void updateScreenInfo(raaScreen *pScreen);

	raaDisplayScreens m_mDisplays;

	osg::Group *m_pVirtualScene;
	osg::MatrixTransform *m_pPhysicalViewpoint;

	bool m_abPhysical[8];
	bool m_abVirtual[8];

	osg::Vec3f m_vTrans;
	osg::Vec3f m_avPhysical[4];
	osg::Vec3f m_avVirtual[4];
	osg::Matrix m_CameraManipulatorMatrix;
	bool m_bLockCamera;

	osgGA::CameraManipulator *m_pManpulator;
	std::string m_sCurrentScreen;
	raaScreen *m_pCurrentScreen;
	bool m_bScreenUpdate;

	raaNet::raaNetwork *m_pNetwork;
	raaNet::raaTcpThread *m_pTcpClient;
	raaNet::raaUdpThread *m_pUdpThread;
};

