#include <QtCore/QCoreApplication>
#include <QtCore/QEvent>

#include "raaTcpMsg.h"
#include "raaTcpThread.h"

#include "raaUdpMsg.h"
#include "raaUdpThread.h"


#include "raaNetwork.h"
#include "raaNetwork.moc"
#include <iostream>
bool raaNet::raaNetwork::sm_bInit = false;
int raaNet::raaNetwork::sm_iTcpRead;
int raaNet::raaNetwork::sm_iTcpWrite;
int raaNet::raaNetwork::sm_iUdpRead;
int raaNet::raaNetwork::sm_iUdpWrite;


raaNet::raaNetwork::raaNetwork::raaNetwork(quint16 uiTcpServerPort, QObject *pParent) : QTcpServer(pParent)
{
	initEvents();

	if (uiTcpServerPort) listen(QHostAddress::Any, uiTcpServerPort);
}

raaNet::raaNetwork::raaNetwork::~raaNetwork()
{
}

QEvent::Type raaNet::raaNetwork::raaNetwork::tcpReadEvent()
{
	return QEvent::Type(sm_iTcpRead);
}

QEvent::Type raaNet::raaNetwork::raaNetwork::tcpWriteEvent()
{
	return QEvent::Type(sm_iTcpWrite);
}

QEvent::Type raaNet::raaNetwork::udpReadEvent()
{
	return QEvent::Type(sm_iUdpRead);
}

QEvent::Type raaNet::raaNetwork::udpWriteEvent()
{
	return QEvent::Type(sm_iUdpWrite);
}

bool raaNet::raaNetwork::raaNetwork::createTcpClient(QString sName, QString sIP, quint16 uiPort)
{
	if (sName.length() && m_mTcpThreads.find(sName) == m_mTcpThreads.end())
	{
		raaTcpThread *pThread = new raaTcpThread(this, sName, sIP, uiPort, this);
		connect(pThread, SIGNAL(finished()), pThread, SLOT(deleteLater()));
		connect(pThread, SIGNAL(stateChanged(raaTcpThread*, unsigned int)), SLOT(tcpStateChanged(raaTcpThread*, unsigned int)));
		pThread->start();
		return true;
	}
	return false;
}

bool raaNet::raaNetwork::raaNetwork::closeTcpConnection(QString sName)
{
	if (sName.length() && m_mTcpThreads.find(sName) != m_mTcpThreads.end())
	{
		m_mTcpThreads[sName]->close();
		return true;
	}
	return false;
}

bool raaNet::raaNetwork::createUdpClient(QString sName, QString sIP, quint16 uiPort)
{
	if (sName.length() && m_mUdpThreads.find(sName) == m_mUdpThreads.end())
	{
		raaUdpThread *pThread = new raaUdpThread(this, sName, sIP, uiPort, false, this);
		connect(pThread, SIGNAL(finished()), pThread, SLOT(deleteLater()));
		connect(pThread, SIGNAL(stateChanged(raaUdpThread*, unsigned int)), SLOT(udpStateChanged(raaUdpThread*, unsigned int)));
		m_lUdpThreads.push_back(pThread);
		m_mUdpThreads[pThread->name()] = pThread;
		pThread->start();
		return true;
	}
	return false;
	
}

bool raaNet::raaNetwork::createUdpServer(QString sName, QString sIP, quint16 uiPort)
{
	if (sName.length() && m_mUdpThreads.find(sName) == m_mUdpThreads.end())
	{
		raaUdpThread *pThread = new raaUdpThread(this, sName, sIP, uiPort, true, this);
		connect(pThread, SIGNAL(finished()), pThread, SLOT(deleteLater()));
		connect(pThread, SIGNAL(stateChanged(raaUdpThread*, unsigned int)), SLOT(udpStateChanged(raaUdpThread*, unsigned int)));
		m_lUdpThreads.push_back(pThread);
		m_mUdpThreads[pThread->name()] = pThread;
		pThread->start();
		return true;
	}
	return false;
}

bool raaNet::raaNetwork::closeUdpConnection(QString sName)
{
	if (sName.length() && m_mUdpThreads.find(sName) != m_mUdpThreads.end())
	{
		m_mUdpThreads[sName]->close();
		return true;
	}
	return false;
}

void raaNet::raaNetwork::raaNetwork::writeTcp(QString sName, raaTcpMsg *pMsg)
{
	if (pMsg && sName.length() && m_mTcpThreads.find(sName) != m_mTcpThreads.end())
	{
		m_mTcpThreads[sName]->write(pMsg); 
	}
}

void raaNet::raaNetwork::writeUdp(QString sName, raaUdpMsg *pMsg)
{
	if (sName.length() && m_mUdpThreads.find(sName) != m_mUdpThreads.end())
	{
		m_mUdpThreads[sName]->write(pMsg);
	}
}

bool raaNet::raaNetwork::write(raaTcpThread* pThread, raaTcpMsg *pMsg)
{
	if(pMsg && pThread && std::find(m_lTcpThreads.begin(), m_lTcpThreads.end(), pThread)!=m_lTcpThreads.end())
	{
		pThread->write(pMsg);
		return true;
	}
	return false;
}

bool raaNet::raaNetwork::write(raaUdpThread* pThread, raaUdpMsg *pMsg)
{
	if (pThread && std::find(m_lUdpThreads.begin(), m_lUdpThreads.end(), pThread) != m_lUdpThreads.end())
	{
		pThread->write(pMsg);
		return true;
	}
	return false;
}

bool raaNet::raaNetwork::write(raaTcpMsg* pMsg)
{
	for (raaTcpThreadList::iterator it = m_lTcpThreads.begin(); it != m_lTcpThreads.end(); it++)
	{
		(*it)->write(pMsg);
		return true;
	}
	return false;
}

raaNet::raaUdpThread* raaNet::raaNetwork::udpThread(std::string sName)
{
	if (sName.length() && m_mUdpThreads.find(QString(sName.c_str())) != m_mUdpThreads.end()) return m_mUdpThreads[QString(sName.c_str())];
	return 0;
}

raaNet::raaTcpThread* raaNet::raaNetwork::tcpThread(std::string sName)
{
	if (sName.length() && m_mTcpThreads.find(QString(sName.c_str())) != m_mTcpThreads.end()) return m_mTcpThreads[QString(sName.c_str())];
	return 0;
}

void raaNet::raaNetwork::raaNetwork::tcpStateChanged(raaTcpThread* pThread, unsigned int uiState)
{
	switch (uiState)
	{
	case csm_uiUnconnectedState:
		break;
	case csm_uiHostLookupState:
		break;
	case csm_uiConnectingState:
		break;
	case csm_uiConnectedState:
		break;
	case csm_uiNameConnectedState:
		if (pThread &&pThread->name().length() && (std::find(m_lTcpThreads.begin(), m_lTcpThreads.end(), pThread) == m_lTcpThreads.end()) && (m_mTcpThreads.find(pThread->name()) == m_mTcpThreads.end()))
		{
			m_lTcpThreads.push_back(pThread);
			m_mTcpThreads[pThread->name()] = pThread;
		}
		else
		{
			pThread->close();
		}		
		break;
	case csm_uiBoundState:
		break;
	case csm_uiClosingState:
		if (pThread && std::find(m_lTcpThreads.begin(), m_lTcpThreads.end(), pThread) != m_lTcpThreads.end())
		{
			m_lTcpThreads.remove(pThread);
			m_mTcpThreads.erase(pThread->name());
		}
		break;
	case csm_uiListeningState:
		break;
	default:
		break;
	}

	emit tcpState(pThread, uiState);
}

void raaNet::raaNetwork::udpStateChanged(raaUdpThread* pThread, unsigned int uiState)
{
	switch (uiState)
	{
	case csm_uiUnconnectedState:
		break;
	case csm_uiHostLookupState:
		break;
	case csm_uiConnectingState:
		break;
	case csm_uiConnectedState:
		break;
	case csm_uiBoundState:
	case csm_uiNameConnectedState:
/*
if (pThread)
			if(pThread->name().length())
				if(std::find(m_lUdpThreads.begin(), m_lUdpThreads.end(), pThread) == m_lUdpThreads.end())
					if(m_mUdpThreads.find(pThread->name()) == m_mUdpThreads.end())
					{
						m_lUdpThreads.push_back(pThread);
						m_mUdpThreads[pThread->name()] = pThread;
						std::cout << "added" << std::endl;
					}
					else
					{
						pThread->close();
						std::cout << "deleted" << std::endl;

					}
*/
		break;
//	case csm_uiBoundState:
//		break;
	case csm_uiClosingState:
		if (pThread && std::find(m_lUdpThreads.begin(), m_lUdpThreads.end(), pThread) != m_lUdpThreads.end())
		{
			m_lUdpThreads.remove(pThread);
			m_mUdpThreads.erase(pThread->name());
		}		
		break;
	case csm_uiListeningState:
		break;
	default:
		break;
	}

	emit udpState(pThread, uiState);

}



void raaNet::raaNetwork::raaNetwork::initEvents()
{
	if (!sm_bInit)
	{
		sm_iTcpRead = QEvent::registerEventType();
		sm_iTcpWrite = QEvent::registerEventType();
		sm_iUdpRead = QEvent::registerEventType();
		sm_iUdpWrite = QEvent::registerEventType();
		qRegisterMetaType<QAbstractSocket::SocketState>();
		sm_bInit = true;
	}
}

void raaNet::raaNetwork::raaNetwork::customEvent(QEvent* pEvent)
{
	if (pEvent)
	{
		if (pEvent->type() == raaNetwork::tcpReadEvent()) emit tcpRead((raaNet::raaTcpMsg*)pEvent);
		else if (pEvent->type() == raaNetwork::udpReadEvent())
		{
			emit udpRead((raaNet::raaUdpMsg*)pEvent);
		}
	}
}

void raaNet::raaNetwork::raaNetwork::incomingConnection(qintptr piSocketDescriptor)
{
	raaTcpThread *pThread = new raaTcpThread(this, piSocketDescriptor, this); 
	connect(pThread, SIGNAL(finished()), pThread, SLOT(deleteLater()));
	connect(pThread, SIGNAL(stateChanged(raaTcpThread*, unsigned int)), SLOT(tcpStateChanged(raaTcpThread*, unsigned int)));
	pThread->start();
}
