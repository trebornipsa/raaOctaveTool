#pragma once

#include <QtNetwork/QTcpSocket>

#include "raaNetworkDefs.h"

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

	};
}
