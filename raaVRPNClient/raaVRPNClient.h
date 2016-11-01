#pragma once

#include <list>
#include <map>

#include <osg/Matrixf>

#include <QtCore\QThread>

#include <vrpn_tracker.h>

#include "raaVRPNClientDefs.h"
#include <QtCore/QTimer>
#include <QtCore/QMutex>

class RAAVRPNCLIENT_DLL_DEF raaVRPNClientListener
{
public:
	virtual void updatedSensor(class raaVRPNClient* pClient, unsigned int uiSensor) = 0;
	virtual void updatedSensors(class raaVRPNClient* pClient) = 0;
	virtual void updatedOrigin(class raaVRPNClient* pClient) = 0;
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

	void setTRackerTransform(osg::Matrixf &m);
	void setTrackerTranslation(osg::Vec3f &v);
	void setTrackerRotation(osg::Vec3f &v);

	std::string name();
	unsigned int eyeTracker();
	void setEyeTracker(unsigned int uiSensor);

	unsigned int activeSensors();
	void setActiveSensors(unsigned int uiSensors);

	static void VRPN_CALLBACK tracker(void *pUsr, const vrpn_TRACKERCB data);
protected:
	virtual void run();
	virtual void track(const vrpn_TRACKERCB data)=0;

	void tellListenersSensor(unsigned int uiSensor);
	void tellListenersSensors();
	void tellListenersOrigin();

	raaVRPNClientListeners m_lListeners;
	raaSensors m_mSensors;
	osg::Matrixf m_mTrackerTransform;
	std::string m_sTracker;
	std::string m_sName;
	vrpn_Tracker_Remote *m_pTracker;
	unsigned int m_uiSensors;
	unsigned int m_uiPoll;
	unsigned int m_uiEyeSensor;

	QMutex m_Mutex;
};

