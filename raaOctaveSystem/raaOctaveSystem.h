#pragma once

#include <windows.h>
#include <gl/gl.h>
#include <map>
#include <QtCore/QTimer>
#include <osgViewer/CompositeViewer>
#include <raaNetwork/raaUdpThread.h>
#include <raaNetwork/raaTcpThread.h>
#include "raaOctaveSystemDefs.h"


using namespace raaNet;

typedef std::map<std::string, osgViewer::View*>raaViews;
typedef std::vector<class DepthPeeling*> raaDepthPeelings;


class RAAOCTAVESYSTEM_DLL_DEF raaOctaveSystem: public QObject, public osgViewer::CompositeViewer
{
	Q_OBJECT
public:
	raaOctaveSystem(osg::Node *pNode, std::string sConfig, std::string sName, std::string sIp, unsigned short int usiPort);
	virtual ~raaOctaveSystem();

	void addSceneData(osg::Node *pNode);

public slots:
	void tcpRead(raaTcpMsg*);
	void tcpState(raaTcpThread*, unsigned int);
	void udpRead(raaTcpMsg*);
	void udpState(raaTcpThread*, unsigned int);
	void timerUpdate();


protected:

	void addDisplay(int iScreen, std::string sName, int iX, int iY, int iW, int iH, osg::Matrixf mPersp);

	osg::MatrixTransform *m_pScene;

	raaNet::raaNetwork *m_pNetwork;
	raaNet::raaTcpThread *m_pTcpClient;
	raaNet::raaUdpThread *m_pUdpClient;

	raaDepthPeelings m_vPeelings;

	raaViews m_mViews;
	int m_iScreen;

	QTimer *m_pTimer;
	std::string m_sConfig;
	std::string m_sName;
};

