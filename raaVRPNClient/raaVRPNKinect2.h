#pragma once

#include "raaVRPNClient.h"

#include "raaVRPNClientDefs.h"

class RAAVRPNCLIENT_DLL_DEF raaVRPNKinect2: public raaVRPNClient
{
public:
	raaVRPNKinect2(std::string sTracker, unsigned int uiMSecPoll);
	virtual ~raaVRPNKinect2();

protected:
	virtual void track(const vrpn_TRACKERCB data);

};

