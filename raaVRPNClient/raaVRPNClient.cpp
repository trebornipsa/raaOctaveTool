#include <iostream>
#include "raaVRPNClient.h"
#include "raaVRPNClient.moc"

raaVRPNClient::raaVRPNClient(std::string sName, std::string sTracker, osg::Vec3f vTrackerPos, osg::Vec3f vTrackerDir, osg::Vec3f vTrackerUp, unsigned int uiMSecPoll, unsigned int uiEyeSensor)
{
	m_sTracker = sTracker;
	m_uiSensors = 0;
	m_uiPoll = uiMSecPoll;
	m_sName = sName;
	m_uiEyeSensor = uiEyeSensor;
	m_vPosition = vTrackerPos;
	m_vDir = vTrackerDir;
	m_vUp = vTrackerUp;
	makeTrackerTransform();

	m_pTimer = new QTimer(this);
	connect(m_pTimer, SIGNAL(timeout()), SLOT(timerSensorUpdate()));
	m_pTimer->start(10);
}

raaVRPNClient::~raaVRPNClient()
{
	
}

void raaVRPNClient::addListener(raaVRPNClientListener* pListener, unsigned int uiSensorMask)
{
	//m_Mutex.lock();

	if (pListener && m_lListeners.find(pListener) == m_lListeners.end())
	{
		m_lListeners[pListener] = uiSensorMask;
		m_uiSensors = m_uiSensors | uiSensorMask;
	}

	//m_Mutex.unlock();
}

void raaVRPNClient::removeListener(raaVRPNClientListener* pListener)
{
	//m_Mutex.lock();
	if (pListener && m_lListeners.find(pListener) != m_lListeners.end()) m_lListeners.erase(pListener);
	//m_Mutex.unlock();
}

osg::Matrixf& raaVRPNClient::sensorTransform(unsigned uiSensor)
{
	return (m_uiSensors & 1 << uiSensor) ? m_mTrackerTransform*m_mSensors[uiSensor] : osg::Matrixf();
}

osg::Matrixf& raaVRPNClient::trackerTransform()
{
	return m_mTrackerTransform;
}

void raaVRPNClient::setTrackerTransform(osg::Matrixf& m)
{
	m_mTrackerTransform = m;
	tellListenersOrigin();
}

void raaVRPNClient::setTrackerTransform(osg::Vec3f vPos, osg::Vec3f vDir, osg::Vec3f vUp)
{
	m_vPosition = vPos;
	m_vDir = vDir;
	m_vUp = vUp;
	makeTrackerTransform();
}

osg::Vec3f raaVRPNClient::trackerPosition()
{
	return m_vPosition;
}

osg::Vec3f raaVRPNClient::trackerDirection()
{
	return m_vDir;
}

osg::Vec3f raaVRPNClient::trackerUp()
{
	return m_vUp;
}

std::string raaVRPNClient::name()
{
	return m_sName;
}

unsigned raaVRPNClient::activeSensors()
{
	return m_uiSensors;
}

void raaVRPNClient::setActiveSensors(unsigned uiSensors)
{
	m_uiSensors = uiSensors;
	tellListenersSensors();
}

unsigned raaVRPNClient::eyeTracker()
{
	return m_uiEyeSensor;
}

void raaVRPNClient::setEyeTracker(unsigned uiSensor)
{
	m_uiEyeSensor = uiSensor;
}

void raaVRPNClient::tracker(void* pUsr, const vrpn_TRACKERCB data)
{
	((raaVRPNClient*)pUsr)->m_Mutex.lock();
	((raaVRPNClient*)pUsr)->track(data);
	((raaVRPNClient*)pUsr)->m_Mutex.unlock();
}

void raaVRPNClient::timerSensorUpdate()
{
	tellListenersTimerSensorUpdate();
}

void raaVRPNClient::run()
{
	if (m_sTracker.length())
	{
		m_pTracker = new vrpn_Tracker_Remote(m_sTracker.c_str());
		m_pTracker->register_change_handler(this, tracker);

		while (1)
		{ 
			m_pTracker->mainloop();
			Sleep(m_uiPoll);
		}
	}
}

void raaVRPNClient::tellListenersSensor(unsigned uiSensor)
{
	//m_Mutex.lock();
	for (raaVRPNClientListeners::iterator it = m_lListeners.begin(); it != m_lListeners.end(); it++) if (it->second & 1 << uiSensor) it->first->updatedSensor(this, uiSensor);
	//m_Mutex.unlock();
}

void raaVRPNClient::tellListenersOrigin()
{
	//m_Mutex.lock();
	for (raaVRPNClientListeners::iterator it = m_lListeners.begin(); it != m_lListeners.end(); it++) it->first->updatedOrigin(this);
	//m_Mutex.unlock();
}

void raaVRPNClient::tellListenersTimerSensorUpdate()
{
	m_Mutex.lock();
	for (raaVRPNClientListeners::iterator it = m_lListeners.begin(); it != m_lListeners.end(); it++) it->first->timerSensorUpdate(this);
	m_Mutex.unlock();
}

void raaVRPNClient::makeTrackerTransform()
{
	m_mTrackerTransform.makeLookAt(m_vPosition, m_vPosition + m_vDir, m_vUp);

	osg::Matrixf mT, mR;
	mT.makeTranslate(m_vPosition);

	osg::Vec3f vRight;
	vRight = m_vUp^m_vDir;
	vRight.normalize();


	mR.set(vRight[0], m_vDir[0], m_vUp[0], 0.0f,
		vRight[1], m_vDir[1], m_vUp[1], 0.0f,
		vRight[2], m_vDir[2], m_vUp[2], 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f);

/*
	mR.set(vRight[0], vRight[1], vRight[2], 0.0f,
		m_vDir[0], m_vDir[1], m_vDir[2], 0.0f,
		m_vUp[0], m_vUp[1], m_vUp[2], 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f);
*/

	m_mTrackerTransform = mR*mT;


//	m_mTrackerTransform.invert(m_mTrackerTransform);
	tellListenersOrigin();
}

void raaVRPNClient::tellListenersSensors()
{
	//m_Mutex.lock();
	for (raaVRPNClientListeners::iterator it = m_lListeners.begin(); it != m_lListeners.end(); it++) it->first->updatedSensors(this);
	//m_Mutex.unlock();
}

