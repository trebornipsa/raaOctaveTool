#pragma once

#include <map>

#include <QtNetwork/QTcpServer>

#include "raaNetworkDefs.h"

class raaNetworkServer;

typedef std::map<QString, class raaTcpThread*> raaTcpThreads;

class RAANETWORK_DLL_DEF raaTcpServer : public QTcpServer
{
private:
	Q_OBJECT
public:

	raaTcpServer(QObject *pParent=0);
	virtual ~raaTcpServer();

	void startServer(quint16 uiPort);

	raaTcpThread* socketByName(QString sName);

public slots:
	void stateChanged(raaTcpThread*, QAbstractSocket::SocketState);

protected:
	void incomingConnection(qintptr piSocketDescriptor) Q_DECL_OVERRIDE;

	raaTcpThreads m_Threads;
	raaNetworkServer *m_pServer;
};

