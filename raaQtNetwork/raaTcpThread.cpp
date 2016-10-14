#include <QtCore/QEvent>
#include <QtCore/QCoreApplication>
#include <iostream>

#include "raaNetwork.h"
#include "raaNetworkServer.h"
#include "raaNetworkClient.h"
#ifndef QTCREATOR
    #include "raaTcpThread.moc"
#endif
#include "raaTcpThread.h"
#include "raaNetworkServer.h"
#include "raaTcpServer.h"
#include "raaTcpMsg.h"

raaTcpThread::raaTcpThread(QString sAddress, quint16 uiPort, QObject* pParent) : QThread(pParent)
{
	m_sAddress = sAddress;
	m_uiPort = uiPort;
	m_pServer = 0;
	m_pClient = dynamic_cast<raaNetworkClient*>(pParent->parent());
}

raaTcpThread::raaTcpThread(qintptr piSocketDescriptor, QObject *pParent) : QThread(pParent)
{
	m_piSocketDescriptor = piSocketDescriptor;
	m_pServer = dynamic_cast<raaNetworkServer*>(pParent->parent());
	m_pClient = 0;
}

raaTcpThread::~raaTcpThread()
{
	if (m_pSocket)
	{
		m_pSocket->close();
		m_pSocket->deleteLater();
		m_pSocket = 0;
	}
	exit();
}

void raaTcpThread::run()
{
	if (!m_pSocket)
	{
		m_pSocket = new QTcpSocket();

		if (m_pServer)
		{
			if (m_pSocket->setSocketDescriptor(m_piSocketDescriptor))
			{
				connect(m_pSocket, SIGNAL(disconnected()), this, SLOT(disconnected()));
				connect(m_pSocket, SIGNAL(stateChanged(QAbstractSocket::SocketState)), this, SLOT(stateChanged(QAbstractSocket::SocketState)));
				m_pSocket->waitForReadyRead();
				QByteArray data = m_pSocket->readAll();
				m_sName = data.trimmed();
				connect(m_pSocket, SIGNAL(readyRead()), this, SLOT(readyRead()), Qt::DirectConnection);
				exec();
			}
		}
		else if(m_pClient)
		{
			connect(m_pSocket, SIGNAL(disconnected()), this, SLOT(disconnected()));
			connect(m_pSocket, SIGNAL(stateChanged(QAbstractSocket::SocketState)), this, SLOT(stateChanged(QAbstractSocket::SocketState)));
			m_pSocket->connectToHost(m_sAddress, m_uiPort);
			m_pSocket->write(m_sName.toLocal8Bit());
			connect(m_pSocket, SIGNAL(readyRead()), this, SLOT(readyRead()), Qt::DirectConnection);
			exec();
		}
	}
}

QString raaTcpThread::name()
{
	return m_sName;
}

void raaTcpThread::readyRead()
{
	if (m_pServer) QCoreApplication::postEvent(m_pServer, new raaTcpMsg(this, m_sName, m_pSocket->readAll(), raaNetwork::tcpRead()));
	else if (m_pClient) QCoreApplication::postEvent(m_pClient, new raaTcpMsg(this, m_sName, m_pSocket->readAll(), raaNetwork::tcpRead()));
}

void raaTcpThread::disconnected()
{
	m_pSocket->deleteLater();
	m_pSocket = 0;
	exit();
}

void raaTcpThread::customEvent(QEvent* pEvent)
{
	if(pEvent)
	{
		if (pEvent->type() == raaNetwork::tcpWrite())
		{
			if (m_pSocket)
			{
				raaTcpMsg *pTcpMsg = static_cast<raaTcpMsg*>(pEvent);
				m_pSocket->write(pTcpMsg->data().data(), pTcpMsg->data().length());
			}
		}
	}
}

void raaTcpThread::stateChanged(QAbstractSocket::SocketState state)
{

	switch (state)
	{
		case QAbstractSocket::UnconnectedState:
			std::cout << "StateChanged::UnconnectedState" << std::endl;
			break;
		case QAbstractSocket::HostLookupState:
			std::cout << "StateChanged::HostLookupState" << std::endl;
			break;
		case QAbstractSocket::ConnectingState:
			std::cout << "StateChanged::ConnectingState" << std::endl;
			break;
		case QAbstractSocket::ConnectedState:
			std::cout << "StateChanged::ConnectedState" << std::endl;
			break;
		case QAbstractSocket::BoundState:
			std::cout << "StateChanged::BoundState" << std::endl;
			break;
		case QAbstractSocket::ClosingState:
			std::cout << "StateChanged::ClosingState" << std::endl;
			break;
		case QAbstractSocket::ListeningState:
			std::cout << "StateChanged::ListeningState" << std::endl;
			break;
		default:
			std::cout << "StateChanged::UnknownState" << std::endl;
			break;
	}
	emit stateChanged(this, state);
}
