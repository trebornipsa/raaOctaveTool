#pragma once

#include <list>

#include <osg/Matrixf>

#include <QtCore\QThread>

#include <vrpn_tracker.h>

#include "raaVRPNClientDefs.h"

class RAAVRPNCLIENT_DLL_DEF raaVRPNClientListener
{
public:
	virtual void updatedTracker(class raaVRPNClient* pClient) = 0;
};

typedef std::list<raaVRPNClientListener*>raaVRPNClientListeners;

class RAAVRPNCLIENT_DLL_DEF raaVRPNClient: public QThread
{
	Q_OBJECT
public:
	raaVRPNClient(std::string sTracker);
	virtual ~raaVRPNClient();

	void addListener(raaVRPNClientListener *pListener);
	void removeListener(raaVRPNClientListener *pListener);

	osg::Matrixf& sensorTransform();
	osg::Matrixf& trackerTransform();

	static void VRPN_CALLBACK tracker(void *pUsr, const vrpn_TRACKERCB data);
protected:
	virtual void run();
	void track(const vrpn_TRACKERCB data);
	raaVRPNClientListeners m_lListeners;

	osg::Matrixf m_mSensorTransform;
	osg::Matrixf m_mTrackerTransform;
	std::string m_sTracker;
};

