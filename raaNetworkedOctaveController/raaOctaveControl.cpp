#include "stdafx.h"

#include <iostream>

#include "raaOctaveControl.h"
#include "raaOctaveControl.moc"

raaOctaveControl::raaOctaveControl()
{
	srand(0);
	m_iTimer = 0;
	m_pNetwork = 0;
	m_uiTcpCounter = 0;

	m_pNetwork = new raaNet::raaNetwork(65204, this);

	connect(m_pNetwork, SIGNAL(tcpRead(raaTcpMsg *)), SLOT(tcpRead(raaTcpMsg *)));
	connect(m_pNetwork, SIGNAL(tcpState(raaTcpThread*, unsigned int)), this, SLOT(tcpState(raaTcpThread*, unsigned int)));
	connect(m_pNetwork, SIGNAL(udpRead(raaUdpMsg *)), SLOT(udpRead(raaUdpMsg *)));
	connect(m_pNetwork, SIGNAL(udpState(raaUdpThread*, unsigned int)), this, SLOT(udpState(raaUdpThread*, unsigned int)));
}

raaOctaveControl::~raaOctaveControl()
{
}

void raaOctaveControl::tcpRead(raaNet::raaTcpMsg* pMsg)
{
	std::cout << pMsg->tcpThread()->name().toStdString() << raaNet::tcpMsgTypeToString(pMsg->msgType()).toStdString() << pMsg->msgID() << QString(pMsg->data()).toStdString() << std::endl;
}

void raaOctaveControl::tcpState(raaNet::raaTcpThread* pThread, unsigned uiState)
{
	QString msg;
	msg += pThread->name();
	switch (uiState)
	{
	case raaNet::csm_uiUnconnectedState:
		msg += " -> StateChanged::UnconnectedState";
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

void raaOctaveControl::udpState(raaNet::raaUdpThread* pThread, unsigned uiState)
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

void raaOctaveControl::udpRead(raaNet::raaUdpMsg* pMsg)
{
	std::cout << pMsg->udpThread()->name().toStdString() << QString(pMsg->data()).toStdString() << std::endl;
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
