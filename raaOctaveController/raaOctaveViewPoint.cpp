#include "raaOctaveViewPoint.h"


raaOctaveViewPointListener::raaOctaveViewPointListener()
{
}

raaOctaveViewPointListener::~raaOctaveViewPointListener()
{
}

raaOctaveViewPoint::raaOctaveViewPoint()
{
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
	//m_Mutex.lock();
	for (raaOctaveViewPointListeners::iterator it = m_lListener.begin(); it != m_lListener.end(); it++) (*it)->physicalViewpointChanged(this);
	//m_Mutex.unlock();
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

