#include "raaUdpThread.h"
#ifndef QTCREATOR
    #include "raaUdpThread.moc"
#endif
raaUdpThread::raaUdpThread(unsigned int uiPort, QObject *pParent) : QThread(pParent)
{
	m_uiPort = uiPort;
	m_pSocket = 0;
}

raaUdpThread::~raaUdpThread()
{
	if (m_pSocket) m_pSocket->close();
}

void raaUdpThread::run()
{
	if (!m_pSocket)
	{
		m_pSocket = new QUdpSocket();

//		if (m_pSocket->setSocketDescriptor(m_piSocketDescriptor))
//		{
			m_pSocket->bind(QHostAddress::Any, m_uiPort);
			connect(m_pSocket, SIGNAL(readyRead()), this, SLOT(readyRead()), Qt::DirectConnection);
			connect(m_pSocket, SIGNAL(disconnected()), this, SLOT(disconnected()));
			exec();
//		}
//		else emit error(m_pSocket->error());
	}
}

void raaUdpThread::readyRead()
{
	while(m_pSocket->hasPendingDatagrams())
	{
		QByteArray datagram;
		datagram.resize(m_pSocket->pendingDatagramSize());
		QHostAddress sender;
		quint16 uiSenderPort;

		m_pSocket->readDatagram(datagram.data(), datagram.size(), &sender, &uiSenderPort);
	}
}

void raaUdpThread::disconnected()
{
	m_pSocket->deleteLater();
	m_pSocket = 0;
}
