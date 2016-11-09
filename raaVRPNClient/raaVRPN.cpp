#include "raaVRPNKinect2.h"

#include "raaVRPN.h"



raaVRPN::raaVRPN()
{
}


raaVRPN::~raaVRPN()
{
}

raaVRPNClient* raaVRPN::addClient(std::string sName, std::string sType, std::string sTrackerID, osg::Vec3f vTrackerPos, osg::Vec3f vTrackerDir, osg::Vec3f vTrackerUp, unsigned int uiMSecPoll, unsigned UiEyeSensor)
{
	raaVRPNClient *pClient = 0;

	if(sName.length() && m_mClients.find(sName)==m_mClients.end())
	{

		if(sType=="raaKinect2") pClient = new raaVRPNKinect2(sName, sTrackerID, vTrackerPos, vTrackerDir, vTrackerUp, uiMSecPoll, UiEyeSensor);

		if (pClient)
		{
			m_mClients[sName] = pClient;
			trackerAdded(pClient);
		}
	}

	return pClient;
}

raaVRPNClients& raaVRPN::clients()
{
	return m_mClients;
}
