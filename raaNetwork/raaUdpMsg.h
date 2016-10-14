#pragma once

#include "raaMsg.h"
#include "raaNetworkDefs.h"

namespace raaNet
{
	class raaUdpThread;

	class RAANETWORK_DLL_DEF raaUdpMsg : public raaMsg
	{
	public:
		raaUdpMsg(unsigned short usMsgType);
		raaUdpMsg(raaUdpThread* pThread, QByteArray &data, QEvent::Type t);
		virtual ~raaUdpMsg();
	};
}
