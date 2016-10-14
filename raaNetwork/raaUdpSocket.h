#pragma once

#include <QtNetwork/QUdpSocket>

#include "raaNetworkDefs.h"

namespace raaNet
{
	class RAANETWORK_DLL_DEF raaUdpSocket: public QUdpSocket
	{
		Q_OBJECT
	public:
		raaUdpSocket(QHostAddress &address, quint16 uiPort, QObject *pParent = 0);
		raaUdpSocket(QObject *pParent = 0);
		virtual ~raaUdpSocket();

		void setConnection(const QHostAddress &address, quint16 uiPort);

	protected:
		virtual void customEvent(QEvent* pEvent);

		QHostAddress m_Address;
		quint16 m_uiPort;
	};
}

