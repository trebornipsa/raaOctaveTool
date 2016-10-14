#include <QtCore/QEvent>

#include "raaNetwork.h"
#include <QtCore/qmetatype.h>
#include <QtNetwork/QTcpSocket>


unsigned int raaNetwork::sm_uiInstance=0;
raaNetwork* raaNetwork::sm_pInstance=0;

int raaNetwork::sm_iTcpReadEvent = 0;
int raaNetwork::sm_iTcpWriteEvent = 0;
int raaNetwork::sm_iUdpReadEvent = 0;
int raaNetwork::sm_iUdpWriteEvent = 0;

void raaNetwork::start()
{
	if (!sm_uiInstance++ && !sm_pInstance) new raaNetwork();
}

void raaNetwork::stop()
{
	if (!--sm_uiInstance && sm_pInstance) delete sm_pInstance;
}

raaNetwork* raaNetwork::instance()
{
	return sm_pInstance;
}

QEvent::Type raaNetwork::tcpWrite()
{
	return QEvent::Type(sm_iTcpWriteEvent);
}

QEvent::Type raaNetwork::tcpRead()
{
	return QEvent::Type(sm_iTcpReadEvent);
}

QEvent::Type raaNetwork::udpWrite()
{
	return QEvent::Type(sm_iUdpWriteEvent);
}

QEvent::Type raaNetwork::udpRead()
{
	return QEvent::Type(sm_iUdpReadEvent);
}

void raaNetwork::init()
{
	sm_iTcpReadEvent = QEvent::registerEventType();
	sm_iTcpWriteEvent = QEvent::registerEventType();
	sm_iUdpReadEvent = QEvent::registerEventType();
	sm_iUdpWriteEvent = QEvent::registerEventType();
	qRegisterMetaType<QAbstractSocket::SocketState>();
}

raaNetwork::raaNetwork()
{
	init();
	if (!sm_pInstance) sm_pInstance = this;
}

raaNetwork::~raaNetwork()
{
	if (sm_pInstance == this) sm_pInstance = 0;
}
