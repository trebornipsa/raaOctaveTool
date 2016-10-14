#pragma once

#include <QtCore/QObject>

#include "raaNetworkDefs.h"

class raaTcpThread;
class raaTcpMsg;

class RAANETWORK_DLL_DEF raaNetworkClient: public QObject
{
	Q_OBJECT
public:
	raaNetworkClient(QString sName);
	virtual ~raaNetworkClient();

	void start(QString sAddress, quint16 uiPort);
	void stop();

	void write(raaTcpMsg *pMsg);
	
signals:
	void read(raaTcpMsg*);
	void stream();
	void socketStarted();
	void socketFinished();

protected:
	raaTcpThread *m_pTcpSocket;
	QString m_sName;
	QString m_sAddress;
	quint16 m_uiPort;
};

