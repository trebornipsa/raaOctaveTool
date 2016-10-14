#pragma once

#include <QtCore/QObject>



#include "raaNetworkDefs.h"
class raaTcpServer;
class raaTcpMsg;

class RAANETWORK_DLL_DEF raaNetworkServer : public QObject
{
	Q_OBJECT
public:

	raaNetworkServer(unsigned int uiTcpPort, unsigned int uiUDPPort, QObject *pParent=0);
	virtual ~raaNetworkServer();

	void write(QString sSocket, QByteArray data);
	void stream(QByteArray &data);

	const raaTcpServer* tcpServer();

signals:
	void read(raaTcpMsg*);
	void tcpSocketAdded(QString);
	void tcpSocketRemoved(QString);


protected:
	unsigned int m_uiTcpPort;
	unsigned int m_uiUdpPort;

	raaTcpServer *m_pTcpServer;

	void customEvent(QEvent* pEvent);
};

