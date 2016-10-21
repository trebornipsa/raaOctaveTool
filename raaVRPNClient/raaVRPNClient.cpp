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
		//	vrpn_Tracker_Remote *pTracker = new vrpn_Tracker_Remote("Tracker0@146.87.112.253");

	//	vrpn_Tracker_Remote *pTracker = new vrpn_Tracker_Remote("Tracker0@146.87.114.199");
	//		vrpn_Tracker_Remote *pTracker = new vrpn_Tracker_Remote("Tracker0@localhost");
			vrpn_Tracker_Remote *pTracker = new vrpn_Tracker_Remote(m_sTracker.c_str());

		pTracker->register_change_handler(this, tracker);

		while (1)
		{ 
			pTracker->mainloop();
		}
	}
}

void raaVRPNClient::track(const vrpn_TRACKERCB data)
{
	if (data.sensor == 0)
	{
//		std::cout << "Tracker-> " << data.sensor << " :: " << data.pos[0] << " :: " << data.pos[1] << " :: " << data.pos[2] << " -> " data.quat[0] << std::endl;

		osg::Matrixf mR, mT;
		mR.makeRotate(osg::Quat(data.quat[0], data.quat[2], -data.quat[1], data.quat[3]));
		mT.makeTranslate(data.pos[0], data.pos[1], data.pos[2]);
//		m_mSensorTransform = m_mTrackerTransform*(mR*mT);

		m_mSensorTransform = m_mTrackerTransform*mT;

		for (raaVRPNClientListeners::iterator it = m_lListeners.begin(); it != m_lListeners.end(); it++)
			(*it)->updatedTracker(this);
	}
}
