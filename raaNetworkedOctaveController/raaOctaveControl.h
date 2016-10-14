#pragma once

#include <QtCore/QObject>

#include <raaNetwork/raaNetwork.h>
#include <raaNetwork/raaTcpMsg.h>
#include <raaNetwork/raaTcpThread.h>
#include <raaNetwork/raaUdpMsg.h>
#include <raaNetwork/raaUdpThread.h>
#include <raaNetwork/raaNetworkTypes.h>

class raaOctaveControl: public QObject
{
	Q_OBJECT
public:
	raaOctaveControl();
	virtual ~raaOctaveControl();

public slots:
	void tcpRead(raaNet::raaTcpMsg*);
	void tcpState(raaNet::raaTcpThread*, unsigned int);
	void udpState(raaNet::raaUdpThread*, unsigned int);
	void send();

	void udpConnect();
	void udpStream(int);
	void udpRead(raaNet::raaUdpMsg*);

protected:
	raaNet::raaNetwork *m_pNetwork;

	void timerEvent(QTimerEvent *pEvent);
	int m_iTimer;

	unsigned int m_uiTcpCounter;

};

