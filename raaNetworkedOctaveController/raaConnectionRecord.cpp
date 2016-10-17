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
	if (bListen)
	{
		if (m_pController->viewpoint()) m_pController->viewpoint()->addListener(this);
	}
	else
	{
		if (m_pController->viewpoint()) m_pController->viewpoint()->removeListener(this);
	}
	m_bViewpointListener = bListen;
}

void raaConnectionRecord::setScreenListener(bool bListen)
{
	if (bListen)
	{
		for (raaStringScreenMap::const_iterator it = m_pController->getScreens().begin(); it != m_pController->getScreens().end(); it++) it->second->addListener(this);
	}
	else
	{
		for (raaStringScreenMap::const_iterator it = m_pController->getScreens().begin(); it != m_pController->getScreens().end(); it++) it->second->removeListener(this);
	}
	m_bScreenListener = bListen;
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
		pMsg->add(pScreen->screenVert(raaOctaveControllerTypes::csm_uiBL));
		pMsg->add(pScreen->screenVert(raaOctaveControllerTypes::csm_uiBR));
		pMsg->add(pScreen->screenVert(raaOctaveControllerTypes::csm_uiTL));
		pMsg->add(pScreen->screenVert(raaOctaveControllerTypes::csm_uiTR));
		pMsg->add(pScreen->normal());
		pMsg->add(pScreen->near());
		pMsg->add(pScreen->far());
		pMsg->add(pScreen->rotation());
		pMsg->add(pScreen->flipped(0));
		pMsg->add(pScreen->flipped(1));
		pMsg->add(pScreen->flipped(2));

		pMsg->add(pScreen->screenProjection());

		m_pTcpThread->write(pMsg);
	}

	if (m_bScreenListener) pScreen->addListener(this);
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
	if (m_bScreenListener) pScreen->removeListener(this);
}

void raaConnectionRecord::screenUpdated(raaOctaveController* pController, raaScreen* pScreen)
{
}

void raaConnectionRecord::physicalViewpointChanged(raaOctaveViewPoint* pViewpoint)
{
	if (m_pTcpThread)
	{
		raaNet::raaTcpMsg *pMsg = new raaNet::raaTcpMsg(raaNet::csm_usTcpMsgInfo);
		pMsg->add(raaOctaveKernel::csm_uiOCViewpointPhysicalChanged);
		pMsg->add(pViewpoint->physicalMatrix());
		m_pTcpThread->write(pMsg);
	}
}

void raaConnectionRecord::virtualViewpointChanged(raaOctaveViewPoint* pViewpoint)
{
	if (m_pTcpThread)
	{
		raaNet::raaTcpMsg *pMsg = new raaNet::raaTcpMsg(raaNet::csm_usTcpMsgInfo);
		pMsg->add(raaOctaveKernel::csm_uiOCViewpointVirtualChanged);
		pMsg->add(pViewpoint->virtualMatrix());
		m_pTcpThread->write(pMsg);
	}
}

void raaConnectionRecord::nameChanged(raaScreen* pScreen)
{
}

void raaConnectionRecord::screenMatrixChanged(raaScreen* pScreen)
{
	if (m_pTcpThread)
	{
		raaNet::raaTcpMsg *pMsg = new raaNet::raaTcpMsg(raaNet::csm_usTcpMsgInfo);
		pMsg->add(raaOctaveKernel::csm_uiOCScreenMatrixChanged);
		pMsg->add(pScreen->name());
		pMsg->add(pScreen->screenProjection());
		m_pTcpThread->write(pMsg);
	}
}

void raaConnectionRecord::screenChanged(raaScreen* pScreen)
{
	if (m_pTcpThread)
	{
		raaNet::raaTcpMsg *pMsg = new raaNet::raaTcpMsg(raaNet::csm_usTcpMsgInfo);
		pMsg->add(raaOctaveKernel::csm_uiOCScreenChanged);
		pMsg->add(pScreen->name());
		pMsg->add(pScreen->screenVert(raaOctaveControllerTypes::csm_uiBL));
		pMsg->add(pScreen->screenVert(raaOctaveControllerTypes::csm_uiBR));
		pMsg->add(pScreen->screenVert(raaOctaveControllerTypes::csm_uiTL));
		pMsg->add(pScreen->screenVert(raaOctaveControllerTypes::csm_uiTR));
		pMsg->add(pScreen->normal());
		pMsg->add(pScreen->near());
		pMsg->add(pScreen->far());
		pMsg->add(pScreen->rotation());
		pMsg->add(pScreen->flipped(0));
		pMsg->add(pScreen->flipped(1));
		pMsg->add(pScreen->flipped(2));
		m_pTcpThread->write(pMsg);
	}
}

