#pragma once

#include <QtCore/QThread>


#include "raaUdpSocket.h"
#include "raaUdpMsg.h"
#include "raaNetwork.h"
#include "raaNetworkDefs.h"

namespace raaNet
{
	class RAANETWORK_DLL_DEF raaUdpThread : public QThread
	{
		Q_OBJECT
	public:
		raaUdpThread(raaNetwork *pNetwork, QString sName, QString sAddress, quint16 uiPort, bool bServer, QObject *pParent=0);


		void close();

		void write(raaUdpMsg* pMsg);

		QString name();

	public slots:
		void readyRead();
		void disconnected();
		void stateChanged(QAbstractSocket::SocketState);

	signals:
		void stateChanged(raaUdpThread*, unsigned int);

	protected:
		QString m_sName;
		raaUdpSocket *m_pSocket;
		QString m_sAddress;
		quint16 m_uiPort;
		raaNetwork *m_pNetwork;
		bool m_bServer;

		virtual void run();
		virtual ~raaUdpThread();

	};
}
