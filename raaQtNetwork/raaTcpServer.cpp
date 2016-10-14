
#include <iostream>

#include "raaTcpThread.h"

#include "raaNetworkServer.h"
#include "raaTcpServer.h"
#ifndef QTCREATOR
    #include "raaTcpServer.moc"
#endif
raaTcpServer::raaTcpServer(QObject *pParent) : QTcpServer(pParent)
{
	m_pServer = dynamic_cast<raaNetworkServer*>(pParent);
}

raaTcpServer::~raaTcpServer()
{
	for (raaTcpThreads::iterator it = m_Threads.begin(); it != m_Threads.end();it++) it->second->quit();
}

void raaTcpServer::startServer(quint16 uiPort)
{
	bool bLiistening=listen(QHostAddress::Any, uiPort);
}

raaTcpThread* raaTcpServer::socketByName(QString sName)
{
	if (m_Threads.find(sName) != m_Threads.end()) return m_Threads[sName];
	return 0;
}

void raaTcpServer::stateChanged(raaTcpThread* pThread, QAbstractSocket::SocketState state)
{
	switch (state)
	{
		//	case QAbstractSocket::UnconnectedState: break;
		//	case QAbstractSocket::HostLookupState: break;
		//	case QAbstractSocket::ConnectingState: break;
		case QAbstractSocket::ConnectedState:
			m_Threads[pThread->name()] = pThread;
			if(m_pServer) emit m_pServer->tcpSocketAdded(pThread->name());
			break;
		//	case QAbstractSocket::BoundState: break;
		case QAbstractSocket::ClosingState:
			m_Threads.erase(pThread->name());
			if(m_pServer) m_pServer->tcpSocketRemoved(pThread->name());
			break;
		//	case QAbstractSocket::ListeningState: break;
		default:
			break;
	}
}

void raaTcpServer::incomingConnection(qintptr piSocketDescriptor)
{
	raaTcpThread *pThread = new raaTcpThread(piSocketDescriptor, this);
	connect(pThread, SIGNAL(finished()), pThread, SLOT(deleteLater()));
	connect(pThread, SIGNAL(stateChanged(raaTcpThread*, QAbstractSocket::SocketState)), this, SLOT(stateChanged(raaTcpThread*, QAbstractSocket::SocketState)));
	pThread->start();
}

