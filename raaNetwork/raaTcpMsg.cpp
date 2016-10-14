#include "raaTcpThread.h"
#include "raaTcpMsg.h"


raaNet::raaTcpMsg::raaTcpMsg(unsigned short usMsgType) : raaMsg(usMsgType, raaNetwork::tcpWriteEvent())
{
}

raaNet::raaTcpMsg::raaTcpMsg(raaTcpThread* pThread, QByteArray& data, QEvent::Type t) : raaMsg(pThread, data, t)
{
}

raaNet::raaTcpMsg::~raaTcpMsg()
{
}

