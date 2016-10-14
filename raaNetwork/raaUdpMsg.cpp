#include "raaUdpThread.h"
#include "raaUdpMsg.h"

raaNet::raaUdpMsg::raaUdpMsg(unsigned short usMsgType) : raaMsg(usMsgType, raaNetwork::udpWriteEvent())
{
}

raaNet::raaUdpMsg::raaUdpMsg(raaUdpThread* pThread, QByteArray& data, QEvent::Type t) : raaMsg(pThread, data, t)
{
}

raaNet::raaUdpMsg::~raaUdpMsg()
{
}
