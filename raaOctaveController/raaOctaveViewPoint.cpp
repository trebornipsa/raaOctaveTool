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
	for (raaOctaveViewPointListeners::iterator it = m_lListener.begin(); it != m_lListener.end(); it++) (*it)->virtualViewpointChanged(this);
}

void raaOctaveViewPoint::setPhysicalMatrix(osg::Matrixf m)
{
	m_Physical = m;
	m_uiViewpointUpdateCount++;
	for (raaOctaveViewPointListeners::iterator it = m_lListener.begin(); it != m_lListener.end(); it++) (*it)->physicalViewpointChanged(this);
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
	if(pListener && std::find(m_lListener.begin(), m_lListener.end(), pListener)==m_lListener.end()) m_lListener.push_back(pListener);
}

void raaOctaveViewPoint::removeListener(raaOctaveViewPointListener* pListener)
{
	if (pListener && std::find(m_lListener.begin(), m_lListener.end(), pListener) != m_lListener.end()) m_lListener.remove(pListener);
}

unsigned raaOctaveViewPoint::currentScreenUpdate()
{
	return m_uiViewpointUpdateCount;
}

