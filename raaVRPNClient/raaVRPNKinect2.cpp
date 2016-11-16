#include "raaVRPNKinect2.h"



raaVRPNKinect2::raaVRPNKinect2(std::string sName, std::string sTracker, osg::Vec3f vTrackerPos, osg::Vec3f vTrackerDir, osg::Vec3f vTrackerUp, unsigned int uiMSecPoll, unsigned int uiEyeSensor): raaVRPNClient(sName, sTracker, vTrackerPos, vTrackerDir, vTrackerUp, uiMSecPoll, uiEyeSensor)
{
	m_sType = "raaKinect2";
}

raaVRPNKinect2::~raaVRPNKinect2()
{
}

void raaVRPNKinect2::track(const vrpn_TRACKERCB data)
{
	if (m_uiSensors & 1 << data.sensor)
	{
		osg::Matrixf mR, mT;
//		mR.makeRotate(osg::Quat(data.quat[0], data.quat[1], data.quat[2], data.quat[3]));
//		mR = mR.rotate(osg::DegreesToRadians(-90.0f), osg::Vec3f(1.0f, 0.0f, 0.0f));
////		mR = mR.scale(osg::Vec3f(1.0f, -1.0f, 1.0f));
		mT.makeTranslate(data.pos[0], -data.pos[2], data.pos[1]);

//		mT.makeTranslate(data.pos[0], data.pos[1], data.pos[2]);

		mR.set(1.0f, 0.0f, 0.0f, 0.0f,
			1.0f, -1.0f, 0.0f, 0.0f,
			1.0f, 0.0f, 1.0f, 0.0f,
			0.0f, 0.0f, 0.0f, 1.0f);



		m_mSensors[data.sensor] = mT;

		tellListenersSensor(data.sensor);
	}
}
