#include <iostream>
#include "raaVRPNClient.h"
#include "raaVRPNClient.moc"

raaVRPNClient::raaVRPNClient(std::string sTracker, unsigned int uiMSecPoll)
{
	m_sTracker = sTracker;
	m_mTrackerTransform.makeTranslate(0.0f, 2.0f, 1.5f);
	m_uiSensors = 0;
	m_uiPoll = uiMSecPoll;
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
	if (pListener && m_lListeners.find(pListener) != m_lListeners.end())
	{
		m_lListeners.erase(pListener);
	}
}

osg::Matrixf& raaVRPNClient::sensorTransform(unsigned uiSensor)
{
	if (m_uiSensors & 1 << uiSensor) return m_mSensors[uiSensor];
	return osg::Matrixf();
}

osg::Matrixf& raaVRPNClient::trackerTransform()
{
	return m_mTrackerTransform;
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
	for(raaVRPNClientListeners::iterator it=m_lListeners.begin();it!=m_lListeners.end();it++)
	{
		if (it->second & 1 << uiSensor) it->first->updatedTracker(this, uiSensor);
	}
}
