#include "raaTcpThread.h"
#include "raaMsg.h"

unsigned int raaNet::raaMsg::sm_uiID=0;

raaNet::raaMsg::raaMsg(unsigned short usMsgType, QEvent::Type t) : QEvent(t)
{
	add(m_usMsgType = usMsgType);
	add(m_uiMsgID = sm_uiID++);
	m_pTcpThread = 0;
	m_usMsgType = usMsgType;
	m_uiBuildLen = 0;
	m_pTcpThread = 0;
	m_pUdpThread = 0;
}

raaNet::raaMsg::raaMsg(raaTcpThread* pThread, QByteArray& data, QEvent::Type t) : QEvent(t)
{
	m_Data = data;
	unpack();
	m_usMsgType = asUShort(0);
	m_uiMsgID = asUInt(1);
	m_pTcpThread = pThread;
	m_pUdpThread = 0;
	m_uiBuildLen = 0;
}

raaNet::raaMsg::raaMsg(raaUdpThread* pThread, QByteArray& data, QEvent::Type t) : QEvent(t)
{
	m_Data = data;
	unpack();
	m_usMsgType = asUShort(0);
	m_uiMsgID = asUInt(1);
	m_pUdpThread = pThread;
	m_pTcpThread = 0;
	m_uiBuildLen = 0;
}

raaNet::raaMsg::~raaMsg()
{
}

void raaNet::raaMsg::add(unsigned short usVal)
{
	m_DataList.push_back(QByteArray((const char*)&usVal, sizeof(unsigned short)));
}

void raaNet::raaMsg::add(unsigned uiVal)
{
	m_DataList.push_back(QByteArray((const char*)&uiVal, sizeof(unsigned int)));
}

void raaNet::raaMsg::add(std::string sVal)
{
	m_DataList.push_back(sVal.c_str());
}

void raaNet::raaMsg::add(unsigned int* puiVal, unsigned uiCount)
{
	if (puiVal && uiCount)
	{
		QByteArray data;
		for (unsigned int i = 0; i<uiCount; i++) data.append((const char*)puiVal + i, sizeof(unsigned int));
		m_DataList.push_back(data);
	}
}

void raaNet::raaMsg::add(float* pfVal, unsigned uiCount)
{
	if (pfVal && uiCount)
	{
		QByteArray data;
		for (unsigned int i = 0; i<uiCount; i++) data.append((const char*)pfVal + i, sizeof(float));
		m_DataList.push_back(data);
	}
}

unsigned short raaNet::raaMsg::asUShort(unsigned uiIndex)
{
	return *((unsigned short*)m_DataList[uiIndex].data());
}

unsigned int raaNet::raaMsg::asUInt(unsigned uiIndex)
{
	return *((unsigned int*)m_DataList[uiIndex].data());
}

std::string raaNet::raaMsg::asString(unsigned uiIndex)
{
	std::string s = m_DataList[uiIndex].data();
	return s;
}

float* raaNet::raaMsg::asFloatArray(unsigned uiIndex)
{
	return (float*)m_DataList[uiIndex].data();
}

unsigned* raaNet::raaMsg::asUIntArray(unsigned uiIndex)
{
	return (unsigned int*)m_DataList[uiIndex].data();
}

const char* raaNet::raaMsg::asConstChar(unsigned uiIndex)
{
	return m_DataList[uiIndex].data();
}

bool raaNet::raaMsg::asBool(unsigned uiIndex)
{
	if (m_DataList[uiIndex].contains('t')) return true;
	return false;
}

float raaNet::raaMsg::asFloat(unsigned uiIndex)
{
	return *((float*)m_DataList[uiIndex].data());
}

unsigned raaNet::raaMsg::length()
{
	return m_DataList.length();
}

void raaNet::raaMsg::add(const char* pccVal, unsigned int uiSize)
{
	m_DataList.push_back(QByteArray(pccVal, uiSize));
}

void raaNet::raaMsg::add(bool bVal)
{
	if (bVal)
		m_DataList.push_back(QByteArray(1, 't'));
	else
		m_DataList.push_back(QByteArray(1, 'f'));
}

void raaNet::raaMsg::add(float f)
{
	m_DataList.push_back(QByteArray((const char*)&f, sizeof(float)));
}

QByteArray raaNet::raaMsg::data()
{
	if (m_uiBuildLen != m_DataList.length())
	{
		m_Data.clear();

		for (QByteArrayList::iterator it = m_DataList.begin(); it != m_DataList.end();)
		{
			m_Data.append((*it));
			it++;
			if (it != m_DataList.end()) m_Data.append(csm_pcSepperator);
		}
	
		m_uiBuildLen = m_DataList.length();
	}
	return m_Data;
}

raaNet::raaTcpThread* raaNet::raaMsg::tcpThread()
{
	return m_pTcpThread;
}

raaNet::raaUdpThread* raaNet::raaMsg::udpThread()
{
	return m_pUdpThread;
}

unsigned short raaNet::raaMsg::msgType()
{
	return m_usMsgType;
}

unsigned raaNet::raaMsg::msgID()
{
	return m_uiMsgID;
}

void raaNet::raaMsg::unpack()
{
	unsigned int uiIndex = 0, i;
	m_DataList.clear();

	do
	{
		i = m_Data.indexOf(csm_pcSepperator, uiIndex);

		if (i != -1)
		{
			m_DataList.push_back(QByteArray(m_Data.data() + uiIndex, i - uiIndex));
			uiIndex = i + 5;
		}
		else
		{
			m_DataList.push_back(QByteArray(m_Data.data() + uiIndex, m_Data.length() - uiIndex));
			uiIndex = -1;
		}
	} while (uiIndex != -1);
}
