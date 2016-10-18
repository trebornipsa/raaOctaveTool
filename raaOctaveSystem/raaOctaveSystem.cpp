#include "raaOctaveSystem.h"

raaOctaveSystem::raaOctaveSystem()
{
	m_pScene = new osg::Group();
	m_pScene->ref();
}

raaOctaveSystem::~raaOctaveSystem()
{
	if (m_pScene) m_pScene->unref();
}

void raaOctaveSystem::addDisplay(int iScreen, std::string sName, int iX, int iY, int iW, int iH)
{
	osg::GraphicsContext::Traits *pTraits = new osg::GraphicsContext::Traits();
	pTraits->screenNum(iScreen);
	pTraits->x = iX;
	pTraits->y = iY;
	pTraits->width = iW;
	pTraits->height = iH;
	pTraits->windowDecoration = false;
	pTraits->doubleBuffer = true;
	pTraits->sharedContext = getNumViews()? pTraits->sharedContext=getView(0)->getCamera()->getGraphicsContext():0;

	osg::GraphicsContext *pGC = osg::GraphicsContext::createGraphicsContext(pTraits);
	osgViewer::View *pView = new osgViewer::View;
	pView->setName(sName);
	osg::Camera *pCamera = pView->getCamera();
	pCamera->setGraphicsContext(pGC);
	pCamera->setClearColor(osg::Vec4f(0.3f, 0.3f, 0.8f, 1.0f));
	pCamera->setName(sName);
	pCamera->setViewport(0, 0, iW, iH);
	pCamera->setProjectionResizePolicy(osg::Camera::FIXED);

	addView(pView);
	pView->setSceneData(m_pScene);
}
