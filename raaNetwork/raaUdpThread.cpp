#include <QtCore/QCoreApplication>

#include "raaUdpThread.moc"
#include "raaUdpThread.h"


raaNet::raaUdpThread::raaUdpThread(raaNetwork* pNetwork, QString sName, QString sAddress, quint16 uiPort, bool bServer, QObject* pParent)
{
	m_pNetwork = pNetwork;
	m_sName = sName;
	m_sAddress = sAddress;
	m_uiPort = uiPort;
	m_bServer = bServer;
	m_pSocket = 0; 
}

void raaNet::raaUdpThread::close()
{
	m_pSocket->disconnectFromHost();
}

void raaNet::raaUdpThread::write(raaUdpMsg* pMsg)
{
	if (m_pSocket)
		QCoreApplication::postEvent(m_pSocket, pMsg);
	else
		QCoreApplication::postEvent(this, pMsg);
}

QString raaNet::raaUdpThread::name()
{
	return m_sName;
}

void raaNet::raaUdpThread::readyRead()
{
/*
	QByteArray data;
	data.resize(m_pSocket->pendingDatagramSize());
	m_pSocket->readDatagram(data.data(), data.size());
	QCoreApplication::postEvent(m_pNetwork, new raaUdpMsg(this, data, raaNetwork::udpReadEvent()));
	*/
/*
	while (m_pSocket->bytesAvailable())
	{
		while (m_pSocket->bytesAvailable() < sizeof(int)) m_pSocket->waitForReadyRead(1);
		int iSize = 0;
		m_pSocket->read((char*)&iSize, sizeof(unsigned int));

		//		std::cout << "Size -> " << iSize << std::endl;

		if (iSize > 0)
		{
			while (m_pSocket->bytesAvailable() < iSize) m_pSocket->waitForReadyRead(1);

			QCoreApplication::postEvent(m_pNetwork, new raaUdpMsg(this, m_pSocket->read((qint64)iSize), raaNetwork::udpReadEvent()));
		}
	}
	*/

	while(m_pSocket->hasPendingDatagrams())
	{
		QByteArray data;
		data.resize(m_pSocket->pendingDatagramSize());
		QHostAddress address;
		quint16 uiPort;
		m_pSocket->readDatagram(data.data(), data.size(), &address, &uiPort);
		QCoreApplication::postEvent(m_pNetwork, new raaUdpMsg(this, data, raaNetwork::udpReadEvent()));
	}
}

void raaNet::raaUdpThread::disconnected()
{
	m_pSocket->close();
	m_pSocket->deleteLater();
	m_pSocket = 0;
	quit();
}

void raaNet::raaUdpThread::stateChanged(QAbstractSocket::SocketState state)
{
	unsigned int uiState;

	switch (state)
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

void raaNet::raaUdpThread::run()
{
	if(!m_pSocket)
	{
		m_pSocket = new raaUdpSocket();
		connect(m_pSocket, SIGNAL(disconnected()), this, SLOT(disconnected()));
		connect(m_pSocket, SIGNAL(stateChanged(QAbstractSocket::SocketState)), SLOT(stateChanged(QAbstractSocket::SocketState)), Qt::DirectConnection);
		if (m_bServer)
		{
			m_pSocket->setConnection(QHostAddress::LocalHost, m_uiPort);
		}
		else
		{
			m_pSocket->bind(QHostAddress::LocalHost, m_uiPort);
		}
		emit stateChanged(this, csm_uiNameConnectedState);		
		connect(m_pSocket, SIGNAL(readyRead()), SLOT(readyRead()), Qt::DirectConnection);
		exec();
		 
	}
}

raaNet::raaUdpThread::~raaUdpThread()
{
	if (m_pSocket)
	{
		m_pSocket->close();
		m_pSocket->deleteLater();
		m_pSocket = 0;
	}
}
