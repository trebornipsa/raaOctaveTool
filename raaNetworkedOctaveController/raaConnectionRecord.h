#pragma once
#include <string>
#include <raaNetwork/raaTcpThread.h>
#include <raaNetwork/raaUdpThread.h>

#include <raaOctaveController/raaOctaveController.h>

using namespace raaNet;

class raaConnectionRecord: public raaOctaveControllerListener
{
public:
	raaConnectionRecord(std::string sName, raaOctaveController *pController);
	virtual ~raaConnectionRecord();

	std::string name();
	raaTcpThread *tcpThread();
	raaUdpThread *udpThread();

	void setTcpThread(raaTcpThread *pThread);
	void setUdpThread(raaUdpThread *pThread);

	void setControllerListener(bool bListen);
	void setViewpointListener(bool bListen);
	void setScreenListener(bool bListen);

protected:
	std::string m_sName;
	raaTcpThread *m_pTcpThread;
	raaUdpThread *m_pUdpThread;

	raaOctaveController *m_pController;

	bool m_bControllerListener;
	bool m_bViewpointListener;
	bool m_bScreenListener;

	virtual void originChanged(raaOctaveController* pController);
	virtual void screenAdded(raaOctaveController* pController, raaScreen* pScreen);
	virtual void screenRemoved(raaOctaveController* pController, raaScreen* pScreen);
	virtual void screenUpdated(raaOctaveController* pController, raaScreen* pScreen);

};

