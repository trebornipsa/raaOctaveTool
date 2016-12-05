#pragma once
#include <string>
#include <QtCore/QObject>
#include <raaNetwork/raaTcpThread.h>
#include <raaNetwork/raaUdpThread.h>

#include <raaVRPNClient/raaVRPNClient.h>

#include <raaOctaveController/raaOctaveController.h>
#include <raaOctaveController/raaOctaveViewPoint.h>
#include <raaOctaveController/raaScreen.h>

using namespace raaNet;

class raaConnectionRecord: public raaOctaveControllerListener, public raaOctaveViewPointListener, public raaScreenListener, public raaVRPNClientListener
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

	void sendScreenAll(raaOctaveController *pController);

	virtual void updatedSensor(raaVRPNClient* pClient, unsigned uiSensor);
	virtual void updatedOrigin(raaVRPNClient* pClient);
	virtual void updatedSensors(raaVRPNClient* pClient);
	virtual void timerSensorUpdate(raaVRPNClient* pClient);


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
	virtual void physicalViewpointChanged(raaOctaveViewPoint* pViewpoint);
	virtual void virtualViewpointChanged(raaOctaveViewPoint* pViewpoint);
	virtual void nameChanged(raaScreen* pScreen);
	virtual void screenProjMatrixChanged(raaScreen* pScreen);
	virtual void screenChanged(raaScreen* pScreen);
	virtual void windowChanged(raaScreen* pScreen);
	virtual void displayChanged(raaScreen* pScreen);
	virtual void stereoChanged(raaScreen* pScreen);
	virtual void displayScreensChanged(raaOctaveController* pController);


};

