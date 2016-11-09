#include "stdafx.h"

#include <windows.h>

#include <osg/Geode>
#include <osg/Geometry>
#include <osg/LineWidth>

#include "raaTracker.h"


raaTracker::raaTracker(std::string sName)
{
	m_sName = sName;

	m_pRoot = new osg::Group();
	m_pRoot->ref();

	m_pOrigin = new osg::MatrixTransform();
	m_pRoot->addChild(m_pOrigin);

	m_vCol.set((float)rand() / RAND_MAX, (float)rand() / RAND_MAX, (float)rand() / RAND_MAX, 0.5f);
	m_vOriginCol.set(m_vCol[0]*2.0f, m_vCol[1] * 2.0f, m_vCol[2] * 2.0f, 1.0f);

	osg::Geode *pGeode = new osg::Geode();
	osg::Geometry *pGeom = new osg::Geometry();
	osg::Vec3Array *pVerts = new osg::Vec3Array();
	osg::Vec4Array *pCols = new osg::Vec4Array();

	pVerts->push_back(osg::Vec3f(0.0f, 0.0f, 0.0f));
	pVerts->push_back(osg::Vec3f(0.15f, 0.0f, 0.0f));
	pVerts->push_back(osg::Vec3f(0.0f, 0.0f, 0.0f));
	pVerts->push_back(osg::Vec3f(0.0f, 0.15f, 0.0f));
	pVerts->push_back(osg::Vec3f(0.0f, 0.0f, 0.0f));
	pVerts->push_back(osg::Vec3f(0.0f, 0.0f, 0.15f));

	pCols->push_back(m_vOriginCol);
	pCols->push_back(m_vOriginCol);
	pCols->push_back(m_vOriginCol);
	pCols->push_back(m_vOriginCol);
	pCols->push_back(m_vOriginCol);
	pCols->push_back(m_vOriginCol);

	pGeom->setVertexArray(pVerts);
	pGeom->setColorArray(pCols, osg::Array::BIND_PER_VERTEX);
	pGeom->addPrimitiveSet(new osg::DrawArrays(osg::PrimitiveSet::LINES, 0, pVerts->size()));
	pGeode->getOrCreateStateSet()->setMode(GL_LIGHTING, osg::StateAttribute::OFF | osg::StateAttribute::OVERRIDE);
	pGeode->getOrCreateStateSet()->setAttribute(new osg::LineWidth(3.0f), osg::StateAttribute::ON);
	pGeode->addDrawable(pGeom);
	m_pOrigin->addChild(pGeode);

	osg::Geode *pGeodeS = new osg::Geode();
	osg::Geometry *pGeomS = new osg::Geometry();
	osg::Vec3Array *pVertsS = new osg::Vec3Array();

	pVertsS->push_back(osg::Vec3f(0.0f, 0.0f, 0.0f));
	pVertsS->push_back(osg::Vec3f(0.1f, 0.0f, 0.0f));
	pVertsS->push_back(osg::Vec3f(0.0f, 0.0f, 0.0f));
	pVertsS->push_back(osg::Vec3f(0.0f, -0.1f, 0.0f));
	pVertsS->push_back(osg::Vec3f(0.0f, 0.0f, 0.0f));
	pVertsS->push_back(osg::Vec3f(0.0f, 0.0f, 0.1f));

	pGeomS->setVertexArray(pVertsS);
	pGeomS->setColorArray(pCols, osg::Array::BIND_PER_VERTEX);
	pGeomS->addPrimitiveSet(new osg::DrawArrays(osg::PrimitiveSet::LINES, 0, pVertsS->size()));
	pGeodeS->getOrCreateStateSet()->setMode(GL_LIGHTING, osg::StateAttribute::OFF | osg::StateAttribute::OVERRIDE);
	pGeodeS->addDrawable(pGeomS);

	for(int i=0;i<32;i++)
	{
		m_paSensor[i] = new osg::MatrixTransform();
		m_paSensorSwitch[i] = new osg::Switch();
		m_paSensorSwitch[i]->setAllChildrenOff();
		m_pRoot->addChild(m_paSensorSwitch[i]);
		m_paSensorSwitch[i]->addChild(m_paSensor[i]);
		m_paSensor[i]->addChild(pGeodeS);
	}
}

void raaTracker::setOrigin(osg::Matrixf& m)
{
	m_pOrigin->setMatrix(m);;
}

void raaTracker::setSensor(unsigned uiSensor, osg::Matrixf& m)
{
	if (uiSensor < 32) m_paSensor[uiSensor]->setMatrix(m);
}

raaTracker::~raaTracker()
{
	if (m_pRoot) m_pRoot->unref();
}

std::string raaTracker::name()
{
	return m_sName;
}

osg::Group* raaTracker::root()
{
	return m_pRoot;
}

void raaTracker::setSensors(unsigned uiSensors)
{
	if(m_pRoot)
	{
		for(unsigned int i=0;i<32;i++)
		{
			if (uiSensors & 1 << i)
				m_paSensorSwitch[i]->setAllChildrenOn();
			else
				m_paSensorSwitch[i]->setAllChildrenOff();
		}
	}
}
