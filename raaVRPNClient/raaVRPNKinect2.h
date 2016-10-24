#pragma once

#include "raaVRPNClient.h"

#include "raaVRPNClientDefs.h"

class RAAVRPNCLIENT_DLL_DEF raaVRPNKinect2: public raaVRPNClient
{
public:
	raaVRPNKinect2(std::string sName, std::string sTracker, osg::Matrixf mOrigin, unsigned int uiMSecPoll, unsigned int uiEyeSensor);
	virtual ~raaVRPNKinect2();

protected:
	virtual void track(const vrpn_TRACKERCB data);

};

