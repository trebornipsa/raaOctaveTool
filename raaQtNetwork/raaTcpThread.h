#pragma once

#include <QtCore/QThread>
#include <QtNetwork/QTcpSocket>
#include <QtNetwork/QHostAddress>


#include "raaTcpMsg.h"
#include "raaNetworkDefs.h"

class raaNetworkServer;
class raaNetworkClient;

class RAANETWORK_DLL_DEF raaTcpThread : public QThread
{
	Q_OBJECT
public:
	raaTcpThread(QString sAddress, quint16 uiPort, QObject *pParent = 0);
	raaTcpThread(qintptr piSocketDescriptor, QObject *pParent = 0);
	virtual ~raaTcpThread();
	
	void run();

	QString name();

signals:
	void stateChanged(raaTcpThread*, QAbstractSocket::SocketState);

public slots:
	void readyRead();
	void disconnected();
	void stateChanged(QAbstractSocket::SocketState);

protected:
	QTcpSocket *m_pSocket;
	qintptr m_piSocketDescriptor;
	QString m_sAddress;
	quint16 m_uiPort;
	bool m_bServer;
	raaNetworkServer *m_pServer;
	raaNetworkClient *m_pClient;
	QString m_sName;

	virtual void customEvent(QEvent *pEvent);
};

