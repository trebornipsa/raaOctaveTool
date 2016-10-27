#pragma once

#include <QtNetwork/QTcpSocket>
#include <QtCore/QMutex>

#include "raaNetworkDefs.h"
//#include <QtCore/QMutex>

namespace raaNet
{
	class RAANETWORK_DLL_DEF raaTcpSocket : public QTcpSocket
	{
		Q_OBJECT
	public:
		raaTcpSocket(QObject *pParent=0);
		virtual ~raaTcpSocket();
	protected:
		virtual void customEvent(QEvent *pEvent);
		QMutex m_Mutex;

	};
}
