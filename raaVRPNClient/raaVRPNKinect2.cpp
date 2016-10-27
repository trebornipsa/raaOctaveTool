#include "raaVRPNKinect2.h"



raaVRPNKinect2::raaVRPNKinect2(std::string sName, std::string sTracker, osg::Matrixf mOrigin, unsigned int uiMSecPoll, unsigned int uiEyeSensor): raaVRPNClient(sName, sTracker, mOrigin, uiMSecPoll, uiEyeSensor)
{
}

raaVRPNKinect2::~raaVRPNKinect2()
{
}

void raaVRPNKinect2::track(const vrpn_TRACKERCB data)
{
	if (m_uiSensors & 1 << data.sensor)
	{
		osg::Matrixf mR, mT;
		mR.makeRotate(osg::Quat(data.quat[0], data.quat[1], data.quat[2], data.quat[3]));
		mR = mR.rotate(osg::DegreesToRadians(-90.0f), osg::Vec3f(1.0f, 0.0f, 0.0f));
		mR = mR.scale(osg::Vec3f(1.0f, -1.0f, 1.0f));
		mT.makeTranslate(data.pos[0], -data.pos[2], data.pos[1]);

		m_mSensors[data.sensor] = mR*mT;

		tellListenersSensor(data.sensor);
	}
}
