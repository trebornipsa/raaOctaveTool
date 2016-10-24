#pragma once

#include <list>
#include <map>

#include <osg/Matrixf>

#include <QtCore\QThread>

#include <vrpn_tracker.h>

#include "raaVRPNClientDefs.h"
#include <QtCore/QTimer>

class RAAVRPNCLIENT_DLL_DEF raaVRPNClientListener
{
public:
	virtual void updatedTracker(class raaVRPNClient* pClient, unsigned int uiSensor) = 0;
};

typedef std::map<raaVRPNClientListener*, unsigned int>raaVRPNClientListeners;
typedef std::map<unsigned int, osg::Matrixf>raaSensors;

class RAAVRPNCLIENT_DLL_DEF raaVRPNClient: public QThread
{
	Q_OBJECT
public:

	raaVRPNClient(std::string sName, std::string sTracker, osg::Matrixf mOrigin, unsigned int uiMSecPoll, unsigned int uiEyeSensor);
	virtual ~raaVRPNClient();

	void addListener(raaVRPNClientListener *pListener, unsigned int uiSensorMask);
	void removeListener(raaVRPNClientListener *pListener);

	osg::Matrixf& sensorTransform(unsigned int uiSensor);
	osg::Matrixf& trackerTransform();

	std::string name();
	unsigned int eyeTracker();

	static void VRPN_CALLBACK tracker(void *pUsr, const vrpn_TRACKERCB data);
protected:
	virtual void run();
	virtual void track(const vrpn_TRACKERCB data)=0;

	void tellListeners(unsigned int uiSensor);

	raaVRPNClientListeners m_lListeners;
	raaSensors m_mSensors;
	osg::Matrixf m_mTrackerTransform;
	std::string m_sTracker;
	std::string m_sName;
	vrpn_Tracker_Remote *m_pTracker;
	unsigned int m_uiSensors;
	unsigned int m_uiPoll;
	unsigned int m_uiEyeSensor;
};

