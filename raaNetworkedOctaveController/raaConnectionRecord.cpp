#include "stdafx.h"
#include "raaConnectionRecord.h"


raaConnectionRecord::raaConnectionRecord(std::string sName)
{
	m_sName = sName;
	m_pTcpThread = 0;
	m_pUdpThread = 0;

}


raaConnectionRecord::~raaConnectionRecord()
{
	if (m_pTcpThread) m_pTcpThread->close();
	if (m_pUdpThread) m_pUdpThread->close();
}

std::string raaConnectionRecord::name()
{
	return m_sName;
}

raaNet::raaTcpThread* raaConnectionRecord::tcpThread()
{
	return m_pTcpThread;
}

raaNet::raaUdpThread* raaConnectionRecord::udpThread()
{
	return m_pUdpThread;
}

void raaConnectionRecord::setTcpThread(raaNet::raaTcpThread* pThread)
{
	m_pTcpThread = pThread;
}

void raaConnectionRecord::setUdpThread(raaNet::raaUdpThread* pThread)
{
	m_pUdpThread = pThread;
}
