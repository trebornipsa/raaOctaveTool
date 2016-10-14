#pragma once

#include <QtCore/QThread>
#include <QtNetwork/QAbstractSocket>
#include <QtNetwork/QTcpSocket>

#include "raaNetwork.h"
#include "raaTcpSocket.h"
#include "raaNetworkDefs.h"

namespace raaNet
{
	class RAANETWORK_DLL_DEF raaTcpThread: public QThread
	{
		Q_OBJECT
	public:
		raaTcpThread(raaNetwork *pNetwork, QString sName, QString sAddress, quint16 uiPort, QObject *pParent = 0);
		raaTcpThread(raaNetwork *pNetwork, qintptr piSocketDescriptor, QObject *pParent = 0);

		void close();

		QString name();

		void write(raaTcpMsg* pMsg);

	public slots:
		void readyRead();
		void disconnected();
		void stateChanged(QAbstractSocket::SocketState); 

	signals:
		void stateChanged(raaTcpThread*, unsigned int);

	protected:
		QString m_sName;
		raaTcpSocket *m_pSocket;
		qintptr m_piSocketDescriptor;
		QString m_sAddress;
		quint16 m_uiPort;
		bool m_bServer;
		raaNetwork *m_pNetwork;



//		virtual void customEvent(QEvent *pEvent);
		virtual void run();
		virtual ~raaTcpThread();

	};
}