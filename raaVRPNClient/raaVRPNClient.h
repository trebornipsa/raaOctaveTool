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
	virtual void timerSensorUpdate(class raaVRPNClient *pClient)=0;
	virtual void updatedSensors(class raaVRPNClient* pClient) = 0;
	virtual void updatedOrigin(class raaVRPNClient* pClient) = 0;
};

typedef std::map<raaVRPNClientListener*, unsigned int>raaVRPNClientListeners;
typedef std::map<unsigned int, osg::Matrixf>raaSensors;

class RAAVRPNCLIENT_DLL_DEF raaVRPNClient: public QThread
{
	Q_OBJECT
public:

	raaVRPNClient(std::string sName, std::string sTracker, osg::Vec3f vTrackerPos, osg::Vec3f vTrackerDir, osg::Vec3f vTrackerUp, unsigned int uiMSecPoll, unsigned int uiEyeSensor);
	virtual ~raaVRPNClient();

	void addListener(raaVRPNClientListener *pListener, unsigned int uiSensorMask);
	void removeListener(raaVRPNClientListener *pListener);

	osg::Matrixf& sensorTransform(unsigned int uiSensor);
	osg::Matrixf& trackerTransform();

	void setTrackerTransform(osg::Matrixf &m);
	void setTrackerTransform(osg::Vec3f vPos, osg::Vec3f vDir, osg::Vec3f vUp);

	osg::Vec3f trackerPosition();
	osg::Vec3f trackerDirection();
	osg::Vec3f trackerUp();

	std::string name();
	unsigned int eyeTracker();
	void setEyeTracker(unsigned int uiSensor);

	unsigned int activeSensors();
	void setActiveSensors(unsigned int uiSensors);


	static void VRPN_CALLBACK tracker(void *pUsr, const vrpn_TRACKERCB data);

public slots:
	void timerSensorUpdate();

protected:
	virtual void run();
	virtual void track(const vrpn_TRACKERCB data)=0;

	void tellListenersSensor(unsigned int uiSensor);
	void tellListenersSensors();
	void tellListenersOrigin();
	void tellListenersTimerSensorUpdate();

	raaVRPNClientListeners m_lListeners;
	raaSensors m_mSensors;
	osg::Matrixf m_mTrackerTransform;
	std::string m_sTracker;
	std::string m_sName;
	vrpn_Tracker_Remote *m_pTracker;
	unsigned int m_uiSensors;
	unsigned int m_uiPoll;
	unsigned int m_uiEyeSensor;

	osg::Vec3f m_vPosition;
	osg::Vec3f m_vUp;
	osg::Vec3f m_vDir;
	osg::Vec3f m_vScale;

	void makeTrackerTransform();

	QMutex m_Mutex;
	QTimer *m_pTimer;
};

