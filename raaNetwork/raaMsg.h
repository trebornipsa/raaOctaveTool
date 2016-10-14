#pragma once

#include <QtCore/QEvent>
#include <QtCore/QByteArray>
#include <QtCore/QByteArrayList>
#include <QtCore/QString>

#include "raaNetworkDefs.h"

namespace raaNet
{
	class raaTcpThread;
	class raaUdpThread;

	class RAANETWORK_DLL_DEF raaMsg : public QEvent
	{
	public:
		raaMsg(unsigned short usMsgType, QEvent::Type t);
		raaMsg(raaTcpThread* pThread, QByteArray &data, QEvent::Type t);
		raaMsg(raaUdpThread* pThread, QByteArray &data, QEvent::Type t);

		virtual ~raaMsg();

		void add(unsigned short usVal);
		void add(unsigned int uiVal);
		void add(std::string sVal);
		void add(unsigned int *puiVal, unsigned int uiCount);
		void add(float *pfVal, unsigned int uiCount);
		void add(const char* pccVal, unsigned int uiSize);
		void add(bool bVal);
		void add(float f);

		unsigned short asUShort(unsigned int uiIndex);
		unsigned int asUInt(unsigned int uiIndex);
		std::string asString(unsigned int uiIndex);
		float* asFloatArray(unsigned int uiIndex);
		unsigned int* asUIntArray(unsigned int uiIndex);
		const char* asConstChar(unsigned int uiIndex);
		bool asBool(unsigned int uiIndex);
		float asFloat(unsigned int uiIndex);

		unsigned int length();

		QByteArray data();
		raaTcpThread* tcpThread();
		raaUdpThread* udpThread();

		unsigned short msgType();
		unsigned int msgID();
	protected:
		void unpack();

		QByteArray m_Data;
		QByteArrayList m_DataList;
		raaTcpThread *m_pTcpThread;
		raaUdpThread *m_pUdpThread;
		unsigned short m_usMsgType;
		unsigned int m_uiMsgID;

		static unsigned int sm_uiID;
		unsigned int m_uiBuildLen;
	};
}
