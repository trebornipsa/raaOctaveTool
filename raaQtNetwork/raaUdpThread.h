#pragma once

#include <QtCore/QThread>
#include <QtNetwork/QUdpSocket>

class raaUdpThread : public QThread
{
	Q_OBJECT
public:
	raaUdpThread(unsigned int uiPort, QObject *pParent=0);
	virtual ~raaUdpThread();

	void run();

signals:
	void error(QUdpSocket::SocketError socketError);

public slots:
	void readyRead();
	void disconnected();

protected:
	QUdpSocket *m_pSocket;
	unsigned int m_uiPort;
};

