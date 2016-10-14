#include "raaNetwork.h"

#include "raaTcpThread.h"

#include <QtCore/QCoreApplication>

#ifndef QTCREATOR
    #include "raaNetworkClient.moc"
#endif
#include "raaNetworkClient.h"



raaNetworkClient::raaNetworkClient(QString sName)
{
	raaNetwork::start();

	m_sName = sName;
	m_pTcpSocket = 0;
}


raaNetworkClient::~raaNetworkClient()
{
	if (m_pTcpSocket) m_pTcpSocket->quit();
}

void raaNetworkClient::start(QString sAddress, quint16 uiPort)
{
	m_sAddress = sAddress;
	m_uiPort = uiPort;

	if (m_pTcpSocket) delete m_pTcpSocket;
	m_pTcpSocket = new raaTcpThread(m_sAddress, m_uiPort, this);
	connect(m_pTcpSocket, SIGNAL(finished()), m_pTcpSocket, SLOT(deleteLater()));
	connect(m_pTcpSocket, SIGNAL(initialised(raaTcpThread*)), SLOT(threadInitialised(raaTcpThread*)));
	connect(m_pTcpSocket, SIGNAL(finalised(raaTcpThread*)), SLOT(threadFinalised(raaTcpThread*)));
	m_pTcpSocket->start();
}

void raaNetworkClient::stop()
{
	if(m_pTcpSocket)
	{
		m_pTcpSocket->quit();

    }
}

void raaNetworkClient::write(raaTcpMsg *pMsg)
{
    if(pMsg && m_pTcpSocket)
    {
        QCoreApplication::postEvent(m_pTcpSocket, pMsg);
    }

}
