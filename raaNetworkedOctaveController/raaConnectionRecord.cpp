#include "stdafx.h"

#include <raaOctaveKernel/raaOctaveKernel.h>
#include <raaNetwork/raaTcpMsg.h>
#include <raaOctaveController/raaScreen.h>

#include "raaConnectionRecord.h"


raaConnectionRecord::raaConnectionRecord(std::string sName, raaOctaveController *pController)
{
	m_pController = pController;
	m_sName = sName;
	m_pTcpThread = 0;
	m_pUdpThread = 0;
	m_bControllerListener = false;
	m_bViewpointListener = false;
	m_bScreenListener = false;
}


raaConnectionRecord::~raaConnectionRecord()
{
	if (m_pTcpThread) m_pTcpThread->close();
	if (m_pUdpThread) m_pUdpThread->close();

	if (m_bControllerListener) m_pController->removeListener(this);
}

std::string raaConnectionRecord::name()
{
	return m_sName;
}

raaTcpThread* raaConnectionRecord::tcpThread()
{
	return m_pTcpThread;
}

raaUdpThread* raaConnectionRecord::udpThread()
{
	return m_pUdpThread;
}

void raaConnectionRecord::setTcpThread(raaTcpThread* pThread)
{
	m_pTcpThread = pThread;
}

void raaConnectionRecord::setUdpThread(raaUdpThread* pThread)
{
	m_pUdpThread = pThread;
}

void raaConnectionRecord::setControllerListener(bool bListen)
{
	if(bListen)
	{
		if (m_pController) m_pController->addListener(this);
	}
	else
	{	
		if (m_pController) m_pController->removeListener(this);
	}
	m_bControllerListener = bListen;
}

void raaConnectionRecord::setViewpointListener(bool bListen)
{
}

void raaConnectionRecord::setScreenListener(bool bListen)
{
}

void raaConnectionRecord::originChanged(raaOctaveController* pController)
{
}

void raaConnectionRecord::screenAdded(raaOctaveController* pController, raaScreen* pScreen)
{
	if (m_pTcpThread)
	{
		raaNet::raaTcpMsg *pMsg = new raaNet::raaTcpMsg(raaNet::csm_usTcpMsgInfo);
		pMsg->add(raaOctaveKernel::csm_uiOCControllerScreenAdded);
		pMsg->add(pScreen->name());
		pMsg->add(pScreen->screenVert(raaOctaveControllerTypes::csm_uiBL).ptr(), 3);
		pMsg->add(pScreen->screenVert(raaOctaveControllerTypes::csm_uiBR).ptr(), 3);
		pMsg->add(pScreen->screenVert(raaOctaveControllerTypes::csm_uiTL).ptr(), 3);
		pMsg->add(pScreen->screenVert(raaOctaveControllerTypes::csm_uiTR).ptr(), 3);
		pMsg->add(pScreen->normal().ptr(), 3);
		pMsg->add(pScreen->screenProjection().ptr(), 16);
		m_pTcpThread->write(pMsg);
	}
}

void raaConnectionRecord::screenRemoved(raaOctaveController* pController, raaScreen* pScreen)
{
	if (m_pTcpThread)
	{
		raaNet::raaTcpMsg *pMsg = new raaNet::raaTcpMsg(raaNet::csm_usTcpMsgInfo);
		pMsg->add(raaOctaveKernel::csm_uiOCControllerScreenRemoved);
		pMsg->add(pScreen->name());
		m_pTcpThread->write(pMsg);
	}
}

void raaConnectionRecord::screenUpdated(raaOctaveController* pController, raaScreen* pScreen)
{
}
