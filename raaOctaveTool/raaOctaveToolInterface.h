#pragma once

#include <string>
#include <map>

#include <QtWidgets\QMainWindow>

//#include "raaOctaveController/raaOctaveController.h"
//#include "raaOctaveController/raaOctaveViewPoint.h"

#include <raaNetwork/raaNetwork.h>

#include "raaOctaveToolInterfaceQt.h"

class raaOctaveController;
class raaDisplayScreen;

typedef std::map<std::string, raaDisplayScreen*>raaDisplayScreens;

using namespace raaNet;

typedef struct _raaWindow
{
	std::string sName;
	int m_aiParam[4];
	QGraphicsRectItem *m_pItem;
} raaWindow;

typedef std::map<std::string, raaWindow>raaWindows;

class raaOctaveToolInterface: public QMainWindow, public Ui::raaOctaveToolInterfaceQt/*, public raaOctaveControllerListener, public raaOctaveViewPointListener*/
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

	const static unsigned int csm_uiTransform = 0;
	const static unsigned int csm_uiScreen = 1;
	const static unsigned int csm_uiWindow = 2;


	raaOctaveToolInterface(std::string sConfig, std::string sName, std::string sIp, unsigned short int usiPort);
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
	void screenNFChanged(double);
	void screenRotChanged(double);
	void screenFlipChanged(int);
	void screenAllChanged();
	void screenContUpdate(int);

	void windowUpdate();
	void windowUpdateMode(int);
	void windowParam(int);
	void currentWindowChanged(const QString&);

	void tcpRead(raaTcpMsg*);
	void tcpState(raaTcpThread*, unsigned int);
	void udpRead(raaTcpMsg*);
	void udpState(raaTcpThread*, unsigned int);

protected:

	static osg::Geode* makeGrid(float fWidth, float fDepth, unsigned int uiWidthSegs, unsigned int uiDepthSegs);
	void updateView();

	raaDisplayScreens m_mDisplays;

	osg::MatrixTransform *m_pVirtualScene;
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
	bool m_bScreenUpdate;

	std::string m_sCurrentWindow;
	bool m_bWindowUpdate;

	raaNet::raaNetwork *m_pNetwork;
	raaNet::raaTcpThread *m_pTcpClient;
	raaNet::raaUdpThread *m_pUdpClient;


	unsigned int m_uiMode;
	QGraphicsScene m_Scene;
	raaWindows m_mWindows;

	std::string m_sConfig;
	std::string m_sName;
};

