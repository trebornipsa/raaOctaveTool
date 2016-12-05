#pragma once

#include "raaVRPNClient.h"

#include "raaVRPNClientDefs.h"

class RAAVRPNCLIENT_DLL_DEF raaVRPNViconHead : public raaVRPNClient
{
public:
	raaVRPNViconHead(std::string sName, std::string sTracker, osg::Vec3f vTrackerPos, osg::Vec3f vTrackerDir, osg::Vec3f vTrackerUp, unsigned uiMSecPoll, unsigned uiEyeSensor);
	virtual ~raaVRPNViconHead();
protected:
	virtual void track(const vrpn_TRACKERCB data);
};

