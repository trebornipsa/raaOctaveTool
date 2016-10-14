#pragma once



#include <QtNetwork/QTcpServer>

#include <QtCore/QEvent>

#include "raaNetworkTypes.h"
#include "raaNetworkDefs.h"



namespace raaNet
{
	class raaUdpMsg;
	class raaTcpMsg;
	class raaTcpThread;

	class RAANETWORK_DLL_DEF raaNetwork: public QTcpServer
	{
		Q_OBJECT
	public:
		raaNetwork(quint16 uiTcpServerPort=0, QObject *pParent=0);
		virtual ~raaNetwork();

		static QEvent::Type tcpReadEvent();
		static QEvent::Type tcpWriteEvent();
		static QEvent::Type udpReadEvent();
		static QEvent::Type udpWriteEvent();

		bool createTcpClient(QString sName, QString sIP, quint16 uiPort);
		bool closeTcpConnection(QString sName);

		bool createUdpClient(QString sName, QString sIP, quint16 uiPort);
		bool createUdpServer(QString sName, QString sIP, quint16 uiPort);
		bool closeUdpConnection(QString sName);

		void writeTcp(QString sName, raaTcpMsg *pMsg);
		void writeUdp(QString sName, raaUdpMsg *pMsg);

		bool write(raaTcpThread* pThread, raaTcpMsg *pMsg);
		bool write(raaUdpThread* pThread, raaUdpMsg *pMsg);

		bool write(raaTcpMsg *pMsg); // write to all

		raaUdpThread* udpThread(std::string sName);
		raaTcpThread* tcpThread(std::string sName);

	public slots:
	void tcpStateChanged(raaTcpThread*, unsigned int);
	void udpStateChanged(raaUdpThread*, unsigned int);

	signals:
		void tcpRead(raaTcpMsg*);
		void tcpState(raaTcpThread*, unsigned int);
		void udpRead(raaUdpMsg*);
		void udpState(raaUdpThread*, unsigned int);

	protected:
		static int sm_iTcpRead;
		static int sm_iTcpWrite;
		static int sm_iUdpRead;
		static int sm_iUdpWrite;
		static bool sm_bInit;
		static void initEvents();

		raaStrngTcpThreadMap m_mTcpThreads;
		raaTcpThreadList m_lTcpThreads;
		raaStrngUdpThreadMap m_mUdpThreads;
		raaUdpThreadList m_lUdpThreads;

		void customEvent(QEvent *pEvent);
		void incomingConnection(qintptr piSocketDescriptor) Q_DECL_OVERRIDE;
	};
}
