#pragma once

#include <QtCore/QObject>

#include <raaNetwork/raaNetwork.h>
#include <raaNetwork/raaTcpMsg.h>
#include <raaNetwork/raaTcpThread.h>
#include <raaNetwork/raaUdpMsg.h>
#include <raaNetwork/raaUdpThread.h>
#include <raaNetwork/raaNetworkTypes.h>
#include <raaVRPNClient/raaVRPNClient.h>
#include <raaOctaveController/raaOctaveController.h>

using namespace raaNet;

class raaConnectionRecord;

typedef std::map<raaTcpThread*, raaConnectionRecord*> raaConnectionRecordMap;

class raaOctaveControl: public QObject, public raaOctaveControllerListener, public raaVRPNClientListener
{
	Q_OBJECT
public:
	raaOctaveControl(std::string sTracker="");
	virtual ~raaOctaveControl();

	virtual void updatedTracker(raaVRPNClient* pClient);

public slots:
	void tcpRead(raaTcpMsg*);
	void tcpState(raaTcpThread*, unsigned int);
	void udpState(raaUdpThread*, unsigned int);
	void send();

	void udpConnect();
	void udpStream(int);
	void udpRead(raaUdpMsg*);

protected:
	raaNet::raaNetwork *m_pNetwork;

	virtual void originChanged(raaOctaveController* pController);
	virtual void screenAdded(raaOctaveController* pController, raaScreen* pScreen);
	virtual void screenRemoved(raaOctaveController* pController, raaScreen* pScreen);
	virtual void screenUpdated(raaOctaveController* pController, raaScreen* pScreen);


	void timerEvent(QTimerEvent *pEvent);
	int m_iTimer;


	unsigned int m_uiTcpCounter;

	raaConnectionRecordMap m_mConnections;

	raaOctaveController *m_pController;
};

