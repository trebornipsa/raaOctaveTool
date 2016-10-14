#pragma once

#include "raaMsg.h"
#include "raaNetworkDefs.h"

namespace raaNet
{
	class raaTcpThread;

	class RAANETWORK_DLL_DEF raaTcpMsg : public raaMsg
	{
	public:
		raaTcpMsg(unsigned short usMsgType);
		raaTcpMsg(raaTcpThread* pThread, QByteArray &data, QEvent::Type t);
		virtual ~raaTcpMsg();

	};
}
