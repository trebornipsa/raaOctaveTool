#pragma once

#include <string>
#include <osg/Group>
#include <osg/Switch>
#include <osg/MatrixTransform>

class raaTracker
{
public:
	raaTracker(std::string sName);
	virtual ~raaTracker();

	void setOrigin(osg::Matrixf &m);
	void setSensor(unsigned int uiSensor, osg::Matrixf &m);

	std::string name();

	osg::Group *root();

	void setSensors(unsigned int uiSensors);

protected:
	std::string m_sName;

	osg::MatrixTransform *m_pOrigin;
	osg::MatrixTransform *m_paSensor[32];
	osg::Switch *m_paSensorSwitch[32];

	osg::Group *m_pRoot;

	osg::Vec4f m_vCol;
	osg::Vec4f m_vOriginCol;
};

