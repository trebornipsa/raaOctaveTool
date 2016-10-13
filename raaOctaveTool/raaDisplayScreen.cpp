#include "stdafx.h"
#include <windows.h>
#include <osg/Geode>
#include <osg/Geometry>
#include <osgUtil/SmoothingVisitor>
#include <osg/MatrixTransform>

#include <raaOctaveController/raaOctaveControllerTypes.h>

#include "raaDisplayScreen.h"
#include <iostream>
#include <osg/FrameBufferObject>

raaDisplayScreen::raaDisplayScreen(raaScreen *pScreen, osg::Group *pScene, raaOctaveViewPoint *pViewpoint)
{
	m_pScreen = pScreen;
	m_pRoot = new osg::Group();
	m_pRoot->ref();

	m_pVerts = new osg::Vec3Array();
	m_pNorms = new osg::Vec3Array();
	m_pTexCoords = new osg::Vec2Array();
	m_pGeode = new osg::Geode();
	m_pGeom = new osg::Geometry();
	m_vScreenRotation.set(0.0f, 0.0f, 1.0f);

	if(pScreen)
	{
		m_pVerts->push_back(pScreen->screenVert(raaOctaveControllerTypes::csm_uiBL));
		m_pTexCoords->push_back(osg::Vec2(0.0f, 0.0f));
		m_pVerts->push_back(pScreen->screenVert(raaOctaveControllerTypes::csm_uiBR));
		m_pTexCoords->push_back(osg::Vec2(1.0f, 0.0f));
		m_pVerts->push_back(pScreen->screenVert(raaOctaveControllerTypes::csm_uiTR));
		m_pTexCoords->push_back(osg::Vec2(1.0f, 1.0f));
		m_pVerts->push_back(pScreen->screenVert(raaOctaveControllerTypes::csm_uiTL));
		m_pTexCoords->push_back(osg::Vec2(0.0f, 1.0f));

		m_pNorms->push_back(pScreen->normal());
		m_pNorms->push_back(pScreen->normal());
		m_pNorms->push_back(pScreen->normal());
		m_pNorms->push_back(pScreen->normal());
		m_pRoot->setName(pScreen->name());
	}
	else
	{
		m_pVerts->push_back(osg::Vec3f(-1.0f, 0.0f, 0.0f));
		m_pTexCoords->push_back(osg::Vec2(0.0f, 0.0f));
		m_pVerts->push_back(osg::Vec3f(1.0f, 0.0f, 0.0f));
		m_pTexCoords->push_back(osg::Vec2(1.0f, 0.0f));
		m_pVerts->push_back(osg::Vec3f(1.0f, 0.0f, 1.0f));
		m_pTexCoords->push_back(osg::Vec2(1.0f, 1.0f));
		m_pVerts->push_back(osg::Vec3f(-1.0f, 0.0f, 1.0f));
		m_pTexCoords->push_back(osg::Vec2(0.0f, 1.0f));

		m_pNorms->push_back(osg::Vec3f(0.0f, 1.0f, 0.0f));
		m_pNorms->push_back(osg::Vec3f(0.0f, 1.0f, 0.0f));
		m_pNorms->push_back(osg::Vec3f(0.0f, 1.0f, 0.0f));
		m_pNorms->push_back(osg::Vec3f(0.0f, 1.0f, 0.0f));
		m_pRoot->setName("default");
	}

	m_pTexture = new osg::Texture2D();
	m_pTexture->setTextureSize(512, 512);
	m_pTexture->setInternalFormat(GL_RGBA);
	m_pTexture->setFilter(osg::Texture2D::MIN_FILTER, osg::Texture2D::LINEAR);
	m_pTexture->setFilter(osg::Texture2D::MAG_FILTER, osg::Texture2D::LINEAR);

	m_pRoot->addChild(m_pGeode);
	m_pGeode->addDrawable(m_pGeom);
	m_pGeom->setVertexArray(m_pVerts);
	m_pGeom->setNormalArray(m_pNorms, osg::Array::BIND_PER_VERTEX);
	m_pGeom->setTexCoordArray(0, m_pTexCoords);
	m_pGeom->getOrCreateStateSet()->setTextureAttributeAndModes(0, m_pTexture, osg::StateAttribute::ON);
	m_pGeom->addPrimitiveSet(new osg::DrawArrays(osg::PrimitiveSet::QUADS, 0, m_pVerts->size()));
	m_pGeom->setUseDisplayList(false);

	osg::FrameBufferObject *pFBO = new osg::FrameBufferObject();
	pFBO->setAttachment(osg::FrameBufferObject::BufferComponent::COLOR_BUFFER, osg::FrameBufferAttachment(m_pTexture));
	pFBO->setAttachment(osg::FrameBufferObject::BufferComponent::DEPTH_BUFFER, osg::FrameBufferAttachment(new osg::RenderBuffer(512, 512, GL_DEPTH_COMPONENT24)));

	m_pCamera = new osg::Camera();
	m_pCamera->setClearColor(osg::Vec4(0.1f, 0.3f, 0.3f, 1.0f));
	m_pCamera->setClearMask(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	m_pCameraView = new osg::MatrixTransform();
	m_pCameraView->setDataVariance(osg::Object::DYNAMIC);


	const osg::BoundingSphere& bs = pScene->getBound();
	float fZNear = 1.0f*bs.radius();
	float fZFar = 3.0f*bs.radius();

	float fProjTop = 0.5f*fZNear;
	float fProjRight = 0.5f*fZNear;

	fZNear *= 0.9f;
	fZFar *= 1.1f;

	m_pCamera->setViewport(0, 0, 512, 512);
	m_pCamera->setReferenceFrame(osg::Transform::ABSOLUTE_RF);
	m_pCamera->setProjectionMatrix(pScreen->screenProjection());
	m_pCamera->setViewMatrix(pViewpoint->virtualMatrix());
	m_pCamera->setRenderOrder(osg::Camera::PRE_RENDER);
	m_pCamera->setClearColor(osg::Vec4f(0.0f, 0.0f, 0.0f, 0.7f));
	m_pCamera->setRenderTargetImplementation(osg::Camera::RenderTargetImplementation::FRAME_BUFFER_OBJECT);
	m_pCamera->attach(osg::Camera::COLOR_BUFFER, m_pTexture);
	m_pCamera->addChild(m_pCameraView);
	m_pCameraView->addChild(pScene);
	m_pRoot->addChild(m_pCamera);

	pViewpoint->addListener(this);
	pScreen->setListener(this);
}

raaDisplayScreen::~raaDisplayScreen()
{
	if (m_pRoot) m_pRoot->unref();
}

void raaDisplayScreen::nameChanged(raaScreen* pScreen)
{
	if (m_pRoot) m_pRoot->setName(pScreen->name());
}

void raaDisplayScreen::screenChanged(raaScreen* pScreen)
{
	m_pCamera->setProjectionMatrix(pScreen->screenProjection());
}

void raaDisplayScreen::screenMatrixChanged(raaScreen* pScreen)
{
	m_pCamera->setProjectionMatrix(pScreen->screenProjection());
}

osg::Group* raaDisplayScreen::root()
{
	return m_pRoot;
}

void raaDisplayScreen::physicalViewpointChanged(raaOctaveViewPoint* pViewpoint)
{
}

void raaDisplayScreen::virtualViewpointChanged(raaOctaveViewPoint* pViewpoint)
{
	m_pCamera->setViewMatrix(pViewpoint->virtualMatrix());
}
