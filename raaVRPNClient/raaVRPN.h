#pragma once

#include <map>

#include <osg/Matrixf>

#include "raaVRPNClientDefs.h"

class raaVRPNClient;

typedef std::map<std::string, raaVRPNClient*> raaVRPNClients;


class RAAVRPNCLIENT_DLL_DEF raaVRPN
{
public:
	raaVRPN();
	virtual ~raaVRPN();

	raaVRPNClient* addClient(std::string sName, std::string sType, std::string sTrackerID, osg::Vec3f vTrackerPos, osg::Vec3f vTrackerDir, osg::Vec3f vTrackerUp, unsigned int uiMSecPoll, unsigned int UiEyeSensor=0);

	raaVRPNClients& clients();

protected:
	raaVRPNClients m_mClients;

	virtual void trackerAdded(raaVRPNClient *pClient) = 0;
	virtual void trackerRemoved(raaVRPNClient *pClient)=0;

};

