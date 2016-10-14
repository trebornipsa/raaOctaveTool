#pragma once

#include <QtCore/QEvent>

#include "raaNetworkDefs.h"
class RAANETWORK_DLL_DEF raaNetwork
{

public:
	static int sm_iTcpReadEvent;
	static int sm_iTcpWriteEvent;
    static int sm_iUdpReadEvent;
	static int sm_iUdpWriteEvent;

	static void start();
	static void stop();
	static raaNetwork* instance();

	static QEvent::Type tcpWrite();
	static QEvent::Type tcpRead();
	static QEvent::Type udpWrite();
	static QEvent::Type udpRead();

protected:
	static unsigned int sm_uiInstance;
	static raaNetwork* sm_pInstance;

	static void init();
	raaNetwork();
	virtual ~raaNetwork();
};

