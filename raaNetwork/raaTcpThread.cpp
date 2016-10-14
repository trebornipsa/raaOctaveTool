#include <QtCore/QCoreApplication>

#include "raaTcpMsg.h"
#include "raaTcpThread.moc"
#include "raaTcpThread.h"
#include <iostream>

raaNet::raaTcpThread::raaTcpThread(raaNet::raaNetwork* pNetwork, QString sName, QString sAddress, quint16 uiPort, QObject* pParent) : QThread(pParent)
{
	m_sName = sName;
	m_pNetwork = pNetwork;
	m_sAddress = sAddress;
	m_uiPort = uiPort;
	m_bServer = false;
	m_pSocket = 0;
}

raaNet::raaTcpThread::raaTcpThread(raaNet::raaNetwork* pNetwork, qintptr piSocketDescriptor, QObject* pParent) : QThread(pParent)
{
	m_sName = "pending";
	m_pNetwork = pNetwork;
	m_piSocketDescriptor = piSocketDescriptor;
	m_bServer = true;
	m_pSocket = 0;
}

void raaNet::raaTcpThread::close()
{
	m_pSocket->disconnectFromHost();
}

QString raaNet::raaTcpThread::name()
{
	return m_sName;
}

void raaNet::raaTcpThread::write(raaTcpMsg* pMsg)
{
	if (m_pSocket)
		QCoreApplication::postEvent(m_pSocket, pMsg);
	else
		QCoreApplication::postEvent(this, pMsg);
}

void raaNet::raaTcpThread::readyRead()
{
	while (m_pSocket->bytesAvailable())
	{
		while (m_pSocket->bytesAvailable() < sizeof(int)) m_pSocket->waitForReadyRead(1);
		int iSize = 0;
		m_pSocket->read((char*)&iSize, sizeof(unsigned int));

//		std::cout << "Size -> " << iSize << std::endl;

		if (iSize > 0)
		{
			while (m_pSocket->bytesAvailable() < iSize) m_pSocket->waitForReadyRead(1);

			QCoreApplication::postEvent(m_pNetwork, new raaTcpMsg(this, m_pSocket->read((qint64)iSize), raaNetwork::tcpReadEvent()));
		}
	}
}

void raaNet::raaTcpThread::disconnected()
{
	m_pSocket->close();
	m_pSocket->deleteLater();
	m_pSocket = 0;
	quit();
}

void raaNet::raaTcpThread::stateChanged(QAbstractSocket::SocketState state)
{
	unsigned int uiState;

	switch(state)
	{
	case QAbstractSocket::UnconnectedState:
		uiState = csm_uiUnconnectedState;
		break;
	case QAbstractSocket::HostLookupState:
		uiState = csm_uiHostLookupState;
		break;
	case QAbstractSocket::ConnectingState:
		uiState = csm_uiConnectingState;
		break;
	case QAbstractSocket::ConnectedState:
		uiState = csm_uiConnectedState;
		break;
	case QAbstractSocket::BoundState:
		uiState = csm_uiBoundState;
		break;
	case QAbstractSocket::ClosingState:
		uiState = csm_uiClosingState;
		break;
	case QAbstractSocket::ListeningState:
		uiState = csm_uiListeningState;
		break;
	default:
		uiState = csm_uiUnconnectedState;
		break;
	}

	emit stateChanged(this, uiState);
}

void raaNet::raaTcpThread::run()
{
	if (!m_pSocket)
	{
		m_pSocket = new raaTcpSocket();
		connect(m_pSocket, SIGNAL(disconnected()), this, SLOT(disconnected()));
		connect(m_pSocket, SIGNAL(stateChanged(QAbstractSocket::SocketState)), this, SLOT(stateChanged(QAbstractSocket::SocketState)));

		if (m_bServer)
		{
			if (m_pSocket->setSocketDescriptor(m_piSocketDescriptor))
			{
				m_pSocket->waitForReadyRead();
				QByteArray data = m_pSocket->readAll();
				m_sName = data.trimmed();
				emit stateChanged(this, csm_uiNameConnectedState);
				connect(m_pSocket, SIGNAL(readyRead()), this, SLOT(readyRead()), Qt::DirectConnection);
				exec();
			}
		}
		else
		{
			m_pSocket->connectToHost(m_sAddress, m_uiPort);
			m_pSocket->write(m_sName.toLocal8Bit());
			emit stateChanged(this, csm_uiNameConnectedState);
			connect(m_pSocket, SIGNAL(readyRead()), this, SLOT(readyRead()), Qt::DirectConnection);
			exec();
		}
	}
}

raaNet::raaTcpThread::~raaTcpThread()
{
	if (m_pSocket)
	{
		m_pSocket->close();
		m_pSocket->deleteLater();
		m_pSocket = 0;
	}
}

