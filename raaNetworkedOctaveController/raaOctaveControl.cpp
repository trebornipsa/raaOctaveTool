#include "stdafx.h"

#include <iostream>

#include <raaOctaveKernel/raaOctaveKernel.h>

#include <raaOctaveController/raaScreen.h>

#include "raaConnectionRecord.h"
#include "raaOctaveControl.h"
#include "raaOctaveControl.moc"

raaOctaveControl::raaOctaveControl()
{
	srand(0);
	m_iTimer = 0;
	m_pNetwork = 0;
	m_uiTcpCounter = 0;
	m_pController = new raaOctaveController(this);

	m_pNetwork = new raaNet::raaNetwork(65204, this);

	connect(m_pNetwork, SIGNAL(tcpRead(raaTcpMsg *)), SLOT(tcpRead(raaTcpMsg *)));
	connect(m_pNetwork, SIGNAL(tcpState(raaTcpThread*, unsigned int)), this, SLOT(tcpState(raaTcpThread*, unsigned int)));
	connect(m_pNetwork, SIGNAL(udpRead(raaUdpMsg *)), SLOT(udpRead(raaUdpMsg *)));
	connect(m_pNetwork, SIGNAL(udpState(raaUdpThread*, unsigned int)), this, SLOT(udpState(raaUdpThread*, unsigned int)));
}

raaOctaveControl::~raaOctaveControl()
{
	if (m_pNetwork) delete m_pNetwork;
}

void raaOctaveControl::tcpRead(raaTcpMsg* pMsg)
{
	std::cout << pMsg->tcpThread()->name().toStdString() << raaNet::tcpMsgTypeToString(pMsg->msgType()).toStdString() << pMsg->msgID() << QString(pMsg->data()).toStdString() << std::endl;

	switch(pMsg->msgType())
	{
		case raaNet::csm_usTcpMsgRequest:
		{
			switch(pMsg->asUInt(2))
			{
			case raaOctaveKernel::csm_uiOCHasConfig:
			{
				raaNet::raaTcpMsg *pM = new raaNet::raaTcpMsg(raaNet::csm_usTcpMsgReply);
				pM->add(m_pController->hasConfig() ? raaOctaveKernel::csm_uiOCHasConfigTrue : raaOctaveKernel::csm_uiOCHasConfigFalse);
				pMsg->tcpThread()->write(pM);
			}
			break;
			case raaOctaveKernel::csm_uiOCLoadConfig:
			{
//				std::cout << "Load Config -> " << pMsg->asString(3).c_str() << std::endl;
				m_pController->readConfig(pMsg->asString(3).c_str());
			}
			break;
			case raaOctaveKernel::csm_uiOCAttachControllerListener:
			{
				m_mConnections[pMsg->tcpThread()]->setControllerListener(true);
			}
			break;
			case raaOctaveKernel::csm_uiOCDetachControllerListener:
			{
				m_mConnections[pMsg->tcpThread()]->setControllerListener(false);
			}
			break;

			}
		}
		break;
	}


}

void raaOctaveControl::tcpState(raaTcpThread* pThread, unsigned uiState)
{
	QString msg;
	msg += pThread->name();
	switch (uiState)
	{
	case raaNet::csm_uiUnconnectedState:
		msg += " -> StateChanged::UnconnectedState";
		if (m_mConnections.find(pThread) != m_mConnections.end())
		{
			delete m_mConnections[pThread];
			m_mConnections.erase(pThread);
		}
		break;
	case raaNet::csm_uiHostLookupState:
		msg += " -> StateChanged::HostLookupState";
		break;
	case raaNet::csm_uiConnectingState:
		msg += " -> StateChanged::ConnectingState";
		break;
	case raaNet::csm_uiConnectedState:
		msg += " -> StateChanged::ConnectedState";
		break;
	case raaNet::csm_uiNameConnectedState:
		msg += " -> StateChanged::NameConnectedState";
		m_mConnections[pThread] = new raaConnectionRecord(pThread->name().toStdString(), m_pController);
		m_mConnections[pThread]->setTcpThread(pThread);

		break;
	case raaNet::csm_uiBoundState:
		msg += " -> StateChanged::BoundState";
		break;
	case raaNet::csm_uiClosingState:
		msg += " -> StateChanged::ClosingState";
		break;
	case raaNet::csm_uiListeningState:
		msg += " -> StateChanged::ListeningState";
		break;
	default:
		msg += " -> StateChanged::UnknownState";
		break;
	}

	std::cout << msg.toStdString() << std::endl;
}

void raaOctaveControl::udpState(raaUdpThread* pThread, unsigned uiState)
{
	QString msg;
	msg += pThread->name();
	switch (uiState)
	{
	case raaNet::csm_uiUnconnectedState:
		msg += " -> udpStateChanged::UnconnectedState";
		break;
	case raaNet::csm_uiHostLookupState:
		msg += " -> udpStateChanged::HostLookupState";
		break;
	case raaNet::csm_uiConnectingState:
		msg += " -> udpStateChanged::ConnectingState";
		break;
	case raaNet::csm_uiConnectedState:
		msg += " -> udpStateChanged::ConnectedState";
		break;
	case raaNet::csm_uiNameConnectedState:
		msg += " -> udpStateChanged::NameConnectedState";
		break;
	case raaNet::csm_uiBoundState:
		msg += " -> udpStateChanged::BoundState";
		break;
	case raaNet::csm_uiClosingState:
		msg += " ->udp StateChanged::ClosingState";
		break;
	case raaNet::csm_uiListeningState:
		msg += " -> udpStateChanged::ListeningState";
		break;
	default:
		msg += " -> udpStateChanged::UnknownState";
		break;
	}

	std::cout << msg.toStdString() << std::endl;

}

void raaOctaveControl::send()
{
//	m_pNetwork->writeTcp(raaNet::csm_usTcpMsgInfo, m_uiTcpCounter++, sSocket, sMsg.toLocal8Bit());
}

void raaOctaveControl::udpConnect()
{
	if (m_pNetwork)
	{
		m_pNetwork->createUdpServer("raaOctaveControl", QString("localhost"), 65723);
	}
}

void raaOctaveControl::udpStream(int iState)
{
	if (iState == Qt::Checked)
	{
		m_iTimer = startTimer(10);
	}
	else if (m_iTimer)
	{
		killTimer(m_iTimer);
		m_iTimer = 0;
	}

}

void raaOctaveControl::udpRead(raaUdpMsg* pMsg)
{
	std::cout << pMsg->udpThread()->name().toStdString() << QString(pMsg->data()).toStdString() << std::endl;
}

void raaOctaveControl::originChanged(raaOctaveController* pController)
{
}

void raaOctaveControl::screenAdded(raaOctaveController* pController, raaScreen* pScreen)
{

}

void raaOctaveControl::screenRemoved(raaOctaveController* pController, raaScreen* pScreen)
{
}

void raaOctaveControl::screenUpdated(raaOctaveController* pController, raaScreen* pScreen)
{
}

void raaOctaveControl::timerEvent(QTimerEvent* pEvent)
{
	if (m_pNetwork)
	{
		QByteArray data;
		int i = rand();
		data.append((const char*)&i, sizeof(int));

//		m_pNetwork->writeUdp("raaOctaveControl",new raa data);
	}
}
