#include <iostream>
#include "raaVRPNClient.h"
#include "raaVRPNClient.moc"

raaVRPNClient::raaVRPNClient(std::string sName, std::string sTracker, osg::Matrixf mOrigin, unsigned int uiMSecPoll, unsigned int uiEyeSensor)
{
	m_sTracker = sTracker;
	m_uiSensors = 0;
	m_uiPoll = uiMSecPoll;
	m_sName = sName;
	m_uiEyeSensor = uiEyeSensor;
	m_mTrackerTransform = mOrigin;
}

raaVRPNClient::~raaVRPNClient()
{
}

void raaVRPNClient::addListener(raaVRPNClientListener* pListener, unsigned int uiSensorMask)
{
	if (pListener && m_lListeners.find(pListener) == m_lListeners.end())
	{
		m_lListeners[pListener] = uiSensorMask;
		m_uiSensors = m_uiSensors | uiSensorMask;
	}
}

void raaVRPNClient::removeListener(raaVRPNClientListener* pListener)
{
	if (pListener && m_lListeners.find(pListener) != m_lListeners.end()) m_lListeners.erase(pListener);
}

osg::Matrixf& raaVRPNClient::sensorTransform(unsigned uiSensor)
{
//	if (m_uiSensors & 1 << uiSensor) return m_mSensors[uiSensor] * m_mTrackerTransform;
//	return osg::Matrixf();

	return (m_uiSensors & 1 << uiSensor) ? m_mSensors[uiSensor] * m_mTrackerTransform : osg::Matrixf();
}

osg::Matrixf& raaVRPNClient::trackerTransform()
{
	return m_mTrackerTransform;
}

std::string raaVRPNClient::name()
{
	return m_sName;
}

unsigned raaVRPNClient::eyeTracker()
{
	return m_uiEyeSensor;
}

void raaVRPNClient::tracker(void* pUsr, const vrpn_TRACKERCB data)
{
	((raaVRPNClient*)pUsr)->track(data);
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

void raaVRPNClient::tellListeners(unsigned uiSensor)
{
	for(raaVRPNClientListeners::iterator it=m_lListeners.begin();it!=m_lListeners.end();it++) if (it->second & 1 << uiSensor) it->first->updatedTracker(this, uiSensor);
}
