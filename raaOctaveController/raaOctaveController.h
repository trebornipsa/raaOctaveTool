#pragma once

#include <string>

#include <QtCore/QString>
#include <osg/Vec3>

#include "raaOctaveViewPoint.h"
#include "raaOctaveControllerTypes.h"
#include "raaOctaveControllerDefs.h"

class raaOctaveController;
class raaScreen;

class RAAOCTAVECONTROLLER_DLL_DEF raaOctaveControllerListener
{
	friend raaOctaveController;
public:
	raaOctaveControllerListener();
	virtual ~raaOctaveControllerListener();

protected:
	virtual void originChanged(raaOctaveController *pController)=0;
	virtual void screenAdded(raaOctaveController *pController, raaScreen *pScreen)=0;
	virtual void screenRemoved(raaOctaveController *pController, raaScreen *pScreen)=0;
	virtual void screenUpdated(raaOctaveController *pController, raaScreen *pScreen) = 0;
};

class RAAOCTAVECONTROLLER_DLL_DEF raaOctaveController
{
public:
	raaOctaveController(raaOctaveControllerListener *pListener=0);
	virtual ~raaOctaveController();

	void readConfig(QString sConfig);

	void setListener(raaOctaveControllerListener *pListener);
	void removeListener(raaOctaveControllerListener *pListener);

	raaOctaveViewPoint *viewpoint();

	void update();

	raaStringScreenList getScreenNames();
	raaScreen* getScreen(std::string sName);

protected:
	std::string m_sName;
	osg::Vec3f m_vOrigin;

	raaStringScreenMap m_mScreens;
	raaOctaveControllerListener *m_pListener;
	raaOctaveViewPoint m_ViewPoint;
};

