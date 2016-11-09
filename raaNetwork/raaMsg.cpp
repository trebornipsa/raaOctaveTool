#include "raaTcpThread.h"
#include "raaMsg.h"
#include <iostream>

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
	//m_Mutex.lock();
	m_DataList.push_back(QByteArray((const char*)&usVal, sizeof(unsigned short)));
	//m_Mutex.unlock();
}

void raaNet::raaMsg::add(unsigned uiVal)
{
	//m_Mutex.lock();
	m_DataList.push_back(QByteArray((const char*)&uiVal, sizeof(unsigned int)));
	//m_Mutex.unlock();
}

void raaNet::raaMsg::add(int iVal)
{
	//m_Mutex.lock();
	m_DataList.push_back(QByteArray((const char*)&iVal, sizeof(int)));
	//m_Mutex.unlock();
}

void raaNet::raaMsg::add(std::string sVal)
{
	//m_Mutex.lock();
	m_DataList.push_back(sVal.c_str());
	//m_Mutex.unlock();
}

void raaNet::raaMsg::add(unsigned int* puiVal, unsigned uiCount)
{
	//m_Mutex.lock();

	if (puiVal && uiCount)
	{
		QByteArray data;
		for (unsigned int i = 0; i<uiCount; i++) data.append((const char*)puiVal + i, sizeof(unsigned int));
		m_DataList.push_back(data);
	}
	//m_Mutex.unlock();
}

void raaNet::raaMsg::add(float* pfVal, unsigned uiCount)
{
	//m_Mutex.lock();
	if (pfVal && uiCount)
	{
		QByteArray data;
		for (unsigned int i = 0; i<uiCount; i++) data.append((const char*)pfVal + i, sizeof(float));
		m_DataList.push_back(data);
	}
	//m_Mutex.unlock();
}

unsigned short raaNet::raaMsg::asUShort(unsigned uiIndex)
{ 
	if(uiIndex<m_DataList.size()) return *((unsigned short*)m_DataList[uiIndex].data());
	throw csm_uiMsgBadIndex;
}

unsigned int raaNet::raaMsg::asUInt(unsigned uiIndex)
{
	if (uiIndex<m_DataList.size()) return *((unsigned int*)m_DataList[uiIndex].data());
	throw csm_uiMsgBadIndex;
}

int raaNet::raaMsg::asInt(unsigned uiIndex)
{
	if (uiIndex<m_DataList.size()) return *((int*)m_DataList[uiIndex].data());
	throw csm_uiMsgBadIndex;
}

std::string raaNet::raaMsg::asString(unsigned uiIndex)
{
	if (uiIndex < m_DataList.size())
	{
		std::string s= m_DataList[uiIndex].data();
		return s;
	}
	throw csm_uiMsgBadIndex;
}

float* raaNet::raaMsg::asFloatArray(unsigned uiIndex)
{
	if (uiIndex<m_DataList.size()) return (float*)m_DataList[uiIndex].data();
	throw csm_uiMsgBadIndex;
}

unsigned* raaNet::raaMsg::asUIntArray(unsigned uiIndex)
{
	if (uiIndex<m_DataList.size()) return (unsigned int*)m_DataList[uiIndex].data();
	throw csm_uiMsgBadIndex;
}

const char* raaNet::raaMsg::asConstChar(unsigned uiIndex)
{
	if (uiIndex<m_DataList.size()) return m_DataList[uiIndex].data();
	throw csm_uiMsgBadIndex;
}

bool raaNet::raaMsg::asBool(unsigned uiIndex)
{
	if (uiIndex < m_DataList.size())
	{
		if (m_DataList[uiIndex].contains('t')) return true;
		return false;
	}
	throw csm_uiMsgBadIndex;
}

float raaNet::raaMsg::asFloat(unsigned uiIndex)
{
	if (uiIndex<m_DataList.size()) return *((float*)m_DataList[uiIndex].data());
	throw csm_uiMsgBadIndex;
}

osg::Vec3f raaNet::raaMsg::asVector(unsigned uiIndex)
{
	if (uiIndex < m_DataList.size())
	{
		float *pf = (float*)m_DataList[uiIndex].data();
		osg::Vec3f v(pf[0], pf[1], pf[2]);
		return v;
	}
	throw csm_uiMsgBadIndex;
}

osg::Matrixf raaNet::raaMsg::asMatrix(unsigned uiIndex)
{
	if (uiIndex < m_DataList.size())
	{
		float *pm = (float*)m_DataList[uiIndex].data();
		osg::Matrixf m(pm);
		return m;
	}
	throw csm_uiMsgBadIndex;
}

unsigned raaNet::raaMsg::length()
{
	return m_DataList.length();
}

void raaNet::raaMsg::add(const char* pccVal, unsigned int uiSize)
{
	//m_Mutex.lock();
	m_DataList.push_back(QByteArray(pccVal, uiSize));
	//m_Mutex.unlock();
}

void raaNet::raaMsg::add(bool bVal)
{
	//m_Mutex.lock();
	if (bVal)
		m_DataList.push_back(QByteArray(1, 't'));
	else
		m_DataList.push_back(QByteArray(1, 'f'));
	//m_Mutex.unlock();
}

void raaNet::raaMsg::add(float f)
{
	//m_Mutex.lock();
	m_DataList.push_back(QByteArray((const char*)&f, sizeof(float)));
	//m_Mutex.unlock();
}

void raaNet::raaMsg::add(osg::Vec3f& v)
{
	//m_Mutex.lock();
	m_DataList.push_back(QByteArray((const char*)v.ptr(), 3*sizeof(float)));
	//m_Mutex.unlock();
}

void raaNet::raaMsg::add(osg::Matrixf& m)
{
	//m_Mutex.lock();
	m_DataList.push_back(QByteArray((const char*)m.ptr(), 16 * sizeof(float)));
	//m_Mutex.unlock();
}

QByteArray& raaNet::raaMsg::data()
{

	if (m_uiBuildLen != m_DataList.length())
	{
		m_Data.clear();

		for (QByteArrayList::iterator it = m_DataList.begin(); it != m_DataList.end();)
		{
				m_Data.append((*it));
				it++;
				if(it!=m_DataList.end()) m_Data.append(csm_pcSepperator);
		}

		m_Data=qCompress(m_Data, 1);
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
	qint64 uiIndex = 0, i;

	m_DataList.clear();

	m_Data = qUncompress(m_Data);

	do
	{
		i = m_Data.indexOf(csm_pcSepperator, uiIndex);

		if (i != -1)
		{
			m_DataList.push_back(QByteArray(m_Data.data() + uiIndex, i - uiIndex));
			uiIndex = i + csm_pcSepperator.size();
		}
		else
		{
			m_DataList.push_back(QByteArray(m_Data.data() + uiIndex, m_Data.length() - uiIndex));
			uiIndex = -1;
		}
	} while (uiIndex != -1);
}
