#include <osg/Matrixf>

#include "raaDisplaySettings.h"

raaDisplaySettings::raaDisplaySettings()
{
}


osg::Matrixd raaDisplaySettings::computeLeftEyeProjectionImplementation(const osg::Matrixd& projection) const
{
	return m_LeftProj;
}

osg::Matrixd raaDisplaySettings::computeLeftEyeViewImplementation(const osg::Matrixd& view, double eyeSeperationScale) const
{
	return m_LeftView;
}

osg::Matrixd raaDisplaySettings::computeRightEyeProjectionImplementation(const osg::Matrixd& projection) const
{
	return m_RightProj;
}

osg::Matrixd raaDisplaySettings::computeRightEyeViewImplementation(const osg::Matrixd& view, double eyeSeperationScale) const
{
	return m_RightView;
}

raaDisplaySettings::~raaDisplaySettings()
{
}

void raaDisplaySettings::setLeftEye(osg::Matrixf& mProj, osg::Matrixf& mView)
{
	m_LeftProj = mProj;
	m_LeftView = mView;
}

void raaDisplaySettings::setRightEye(osg::Matrixf& mProj, osg::Matrixf& mView)
{
	m_RightProj = mProj;
	m_RightView = mView;
}
