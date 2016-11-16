#include "raaOctaveViewPoint.h"


raaOctaveViewPointListener::raaOctaveViewPointListener()
{
}

raaOctaveViewPointListener::~raaOctaveViewPointListener()
{
}

raaOctaveViewPoint::raaOctaveViewPoint()
{
	m_fEyeSeperation = 0.08f;
	m_uiViewpointUpdateCount = 0;
}

raaOctaveViewPoint::~raaOctaveViewPoint()
{
}

void raaOctaveViewPoint::setVirtualMatrix(osg::Matrixf m)
{
	m_Virtual = m;
	m_uiViewpointUpdateCount++;
	//m_Mutex.lock();
	for (raaOctaveViewPointListeners::iterator it = m_lListener.begin(); it != m_lListener.end(); it++) (*it)->virtualViewpointChanged(this);
	//m_Mutex.unlock();
}

void raaOctaveViewPoint::setPhysicalMatrix(osg::Matrixf m)
{
	m_Physical = m;
	m_uiViewpointUpdateCount++;

	calcStereoEyePostions();

	for (raaOctaveViewPointListeners::iterator it = m_lListener.begin(); it != m_lListener.end(); it++) (*it)->physicalViewpointChanged(this);
}

void raaOctaveViewPoint::setDefaultPhysicalMatrix(osg::Matrixf m)
{
	m_DefaultPhysical = m;
	setPhysicalMatrix(m);
}

void raaOctaveViewPoint::setDefaultPhysicalMatrix()
{
	m_DefaultPhysical = m_Physical;
}

osg::Matrixf raaOctaveViewPoint::defaultPhysicalMatrix()
{
	return m_DefaultPhysical;
}

osg::Matrixf raaOctaveViewPoint::physicalLeftMatrix()
{
	return m_LeftEye;
}

osg::Matrixf raaOctaveViewPoint::physicalRightMatrix()
{
	return m_RightEye;
}

osg::Matrixf raaOctaveViewPoint::physicalMatrix()
{
	return m_Physical;
}

osg::Matrixf raaOctaveViewPoint::virtualMatrix()
{
	return m_Virtual;
}

void raaOctaveViewPoint::addListener(raaOctaveViewPointListener* pListener)
{
	//m_Mutex.lock();
	if(pListener && std::find(m_lListener.begin(), m_lListener.end(), pListener)==m_lListener.end()) m_lListener.push_back(pListener);
	//m_Mutex.unlock();
}

void raaOctaveViewPoint::removeListener(raaOctaveViewPointListener* pListener)
{
	//m_Mutex.lock();
	if (pListener && std::find(m_lListener.begin(), m_lListener.end(), pListener) != m_lListener.end()) m_lListener.remove(pListener);
	//m_Mutex.unlock();
}

unsigned raaOctaveViewPoint::currentScreenUpdate()
{
	return m_uiViewpointUpdateCount;
}

void raaOctaveViewPoint::calcStereoEyePostions()
{
	osg::Matrixf mR, mL;
	mR.makeTranslate(osg::Vec3f(m_fEyeSeperation*0.5f, 0.0f, 0.0f));
	mL.makeTranslate(osg::Vec3f(-m_fEyeSeperation*0.5f, 0.0f, 0.0f));

	m_LeftEye = mL*m_Physical;
	m_RightEye = mR*m_Physical;
}
