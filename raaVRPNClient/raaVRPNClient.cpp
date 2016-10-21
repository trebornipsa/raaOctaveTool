#include <iostream>
#include "raaVRPNClient.h"
#include "raaVRPNClient.moc"

raaVRPNClient::raaVRPNClient(std::string sTracker)
{
	m_sTracker = sTracker;
	m_mTrackerTransform.makeTranslate(0.0f, 2.0f, 1.5f);
}

raaVRPNClient::~raaVRPNClient()
{
}

void raaVRPNClient::addListener(raaVRPNClientListener* pListener)
{
	if (pListener && std::find(m_lListeners.begin(), m_lListeners.end(), pListener) == m_lListeners.end()) m_lListeners.push_back(pListener);
}

void raaVRPNClient::removeListener(raaVRPNClientListener* pListener)
{
	if (pListener && std::find(m_lListeners.begin(), m_lListeners.end(), pListener) != m_lListeners.end()) m_lListeners.remove(pListener);
}

osg::Matrixf& raaVRPNClient::sensorTransform()
{
	return m_mSensorTransform;
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
			Sleep(10);
		}
	}
}

void raaVRPNClient::track(const vrpn_TRACKERCB data)
{
	if (data.sensor == 0)
	{
		osg::Matrixf mR, mT;
		mR.makeRotate(osg::Quat(data.quat[0], data.quat[1], data.quat[2], data.quat[3]));
		mR = mR.rotate(osg::DegreesToRadians(-90.0f), osg::Vec3f(1.0f, 0.0f, 0.0f));
		mR = mR.scale(osg::Vec3f(1.0f, -1.0f, 1.0f));
		mT.makeTranslate(data.pos[0], -data.pos[2], data.pos[1]); 

		//		m_mSensorTransform = (mR*mT)*m_mTrackerTransform;
		m_mSensorTransform = mR*mT;

		for (raaVRPNClientListeners::iterator it = m_lListeners.begin(); it != m_lListeners.end(); it++) (*it)->updatedTracker(this);
	}
}
