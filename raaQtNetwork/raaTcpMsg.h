#pragma once

#include <QtCore/QEvent>
#include <QtCore/QByteArray>
#include <QtCore/QString>


#include "raaNetworkDefs.h"

class raaTcpThread;

class RAANETWORK_DLL_DEF raaTcpMsg : public QEvent
{
public:
	raaTcpMsg(QByteArray &data, QEvent::Type t);
    raaTcpMsg(QString str, QEvent::Type t);
    raaTcpMsg(raaTcpThread* pSocket, QString sSocketName, QByteArray &data, QEvent::Type t);
    raaTcpMsg(raaTcpThread* pSocket, QString sSocketName, const QByteArray &data, QEvent::Type t);
    virtual ~raaTcpMsg();

	QByteArray data();
	raaTcpThread* socket();
	QString socketName();

protected:
	QByteArray m_Data;
	QString m_sSocketName;
	raaTcpThread*m_pSocket;
};

