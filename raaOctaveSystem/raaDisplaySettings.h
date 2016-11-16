#pragma once

#include <osg/DisplaySettings>

#include "raaOctaveSystemDefs.h"

class RAAOCTAVESYSTEM_DLL_DEF raaDisplaySettings: public osg::DisplaySettings
{
public:
	raaDisplaySettings();
	virtual ~raaDisplaySettings();

	void setLeftEye(osg::Matrixf &mProj, osg::Matrixf &mView);
	void setRightEye(osg::Matrixf &mProj, osg::Matrixf &mView);

	virtual osg::Matrixd computeLeftEyeProjectionImplementation(const osg::Matrixd& projection) const;
	virtual osg::Matrixd computeLeftEyeViewImplementation(const osg::Matrixd& view, double eyeSeperationScale) const;
	virtual osg::Matrixd computeRightEyeProjectionImplementation(const osg::Matrixd& projection) const;
	virtual osg::Matrixd computeRightEyeViewImplementation(const osg::Matrixd& view, double eyeSeperationScale) const;

	osg::Matrixf m_LeftView;
	osg::Matrixf m_RightView;
	osg::Matrixf m_LeftProj;
	osg::Matrixf m_RightProj;

};

