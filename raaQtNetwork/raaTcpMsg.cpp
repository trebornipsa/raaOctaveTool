#include <iostream>

#include "raaTcpMsg.h"

raaTcpMsg::raaTcpMsg(QByteArray &data, QEvent::Type t) : QEvent(t)
{
	m_Data = data;
    m_pSocket = 0;
}

raaTcpMsg::raaTcpMsg(QString str, QEvent::Type t) : QEvent(t)
{
    m_Data = str.toUtf8();
    m_pSocket = 0;
}

raaTcpMsg::raaTcpMsg(raaTcpThread* pSocket, QString sSocketName, QByteArray &data, QEvent::Type t) : QEvent(t)
{
	m_Data = data;
	m_pSocket = pSocket;
	m_sSocketName = sSocketName;
}

raaTcpMsg::raaTcpMsg(raaTcpThread* pSocket, QString sSocketName, const QByteArray &data, QEvent::Type t) : QEvent(t)
{
    m_Data = data;
    m_pSocket = pSocket;
    m_sSocketName = sSocketName;
}


raaTcpMsg::~raaTcpMsg()
{
}

QByteArray raaTcpMsg::data()
{
	return m_Data;
}

raaTcpThread* raaTcpMsg::socket()
{
	return m_pSocket;
}

QString raaTcpMsg::socketName()
{
	return m_sSocketName;
}
