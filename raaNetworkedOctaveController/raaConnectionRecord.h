#pragma once
#include <string>
#include <raaNetwork/raaTcpThread.h>
#include <raaNetwork/raaUdpThread.h>

class raaConnectionRecord
{
public:
	raaConnectionRecord(std::string sName);
	virtual ~raaConnectionRecord();

	std::string name();
	raaNet::raaTcpThread *tcpThread();
	raaNet::raaUdpThread *udpThread();

	void setTcpThread(raaNet::raaTcpThread *pThread);
	void setUdpThread(raaNet::raaUdpThread *pThread);

protected:
	std::string m_sName;
	raaNet::raaTcpThread *m_pTcpThread;
	raaNet::raaUdpThread *m_pUdpThread;
};

