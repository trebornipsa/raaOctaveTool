// raaOctaveViewer.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include <windows.h>
#include <gl/gl.h>
#include <QtCore/QCoreApplication>

#include <conio.h>
#include <osgViewer/Viewer>
#include <osgViewer/ViewerEventHandlers>
#include <osgDB/ReadFile>
#include <osgGA/TrackballManipulator>
#include <osg/Drawable>
#include <osg/ShapeDrawable>
#include <osg/MatrixTransform>

#include <raaOctaveSystem/raaOctaveSystem.h>


osg::Geode* makeGrid(float fWidth, float fDepth, unsigned uiWidthSegs, unsigned uiDepthSegs)
{
	osg::Geode *pGeode = new osg::Geode();
	osg::Geometry *pGeom = new osg::Geometry();

	float fWS = fWidth / (float)uiWidthSegs;
	float fDS = fDepth / (float)uiDepthSegs;

	float fW = -fWidth / 2.0f;
	float fWW = fW;
	float fD = -fDepth / 2.0f;

	osg::Vec3Array *pVerts = new osg::Vec3Array();
	osg::Vec3Array *pNorms = new osg::Vec3Array();
	osg::Vec3 vNorm(0.0f, 0.0f, 1.0f);

	for (unsigned int i = 0; i <= uiWidthSegs; i++, fW += fWS)
	{
		pVerts->push_back(osg::Vec3f(fW, fD, 0.0f));
		pVerts->push_back(osg::Vec3f(fW, -fD, 0.0f));
		pNorms->push_back(vNorm);
		pNorms->push_back(vNorm);
	}

	for (unsigned int i = 0; i <= uiDepthSegs; i++, fD += fDS)
	{
		pVerts->push_back(osg::Vec3f(fWW, fD, 0.0f));
		pVerts->push_back(osg::Vec3f(-fWW, fD, 0.0f));
		pNorms->push_back(vNorm);
		pNorms->push_back(vNorm);
	}

	pGeom->setVertexArray(pVerts);
	pGeom->setNormalArray(pNorms, osg::Array::BIND_PER_VERTEX);
	pGeom->addPrimitiveSet(new osg::DrawArrays(osg::PrimitiveSet::LINES, 0, pVerts->size()));
	pGeode->addChild(pGeom);

	return pGeode;
}

osg::Node* makeScene()
{
	osg::MatrixTransform *pVirtualScene = new osg::MatrixTransform();

	pVirtualScene->addChild(makeGrid(10.0f, 10.0f, 10, 10));
	osg::Light *pLight = new osg::Light();
	pLight->setLightNum(0);
	pLight->setAmbient(osg::Vec4f(0.3f, 0.3f, 0.3f, 1.0f));
	pLight->setDiffuse(osg::Vec4f(1.0f, 1.0f, 1.0f, 1.0f));
	osg::LightSource *pLS = new osg::LightSource();
	pLS->setLight(pLight);
	pLS->setLocalStateSetModes(osg::StateAttribute::ON);
	pVirtualScene->addChild(pLS);

	pVirtualScene->addChild(makeGrid(10.0f, 10.0f, 10, 10));

	osg::Geode *pG0 = new osg::Geode();
	osg::ShapeDrawable *pSD0 = new osg::ShapeDrawable(new osg::Cone(osg::Vec3f(0.0f, 4.0f, 1.0f), 1.0f, 1.0f));
	pG0->addDrawable(pSD0);
	pVirtualScene->addChild(pG0);

	osg::Geode *pG1 = new osg::Geode();
	osg::ShapeDrawable *pSD1 = new osg::ShapeDrawable(new osg::Sphere(osg::Vec3f(0.0f, 0.0f, -4.0f), 1.0f));
	pG1->addDrawable(pSD1);
	pVirtualScene->addChild(pG1);

	osg::MatrixTransform *pMTC = new osg::MatrixTransform();
	osg::Matrix mC;
	mC.makeRotate(osg::DegreesToRadians(-90.0f), osg::Vec3f(1.0f, 0.0f, 0.0f));
	pMTC->setMatrix(mC);
	osg::Geode *pG2 = new osg::Geode();
	osg::ShapeDrawable *pSD2 = new osg::ShapeDrawable(new osg::Box(osg::Vec3f(0.0f, 0.0f, 4.0f), 1.0f));
	pG2->addDrawable(pSD2);
	pMTC->addChild(pG2);

	pVirtualScene->addChild(pG2);
//	pVirtualScene->addChild(makeGrid(10.0f, 10.0f, 10, 10));
	return pVirtualScene;
}

int main(int argc, char* argv[])
{
	QCoreApplication a(argc, argv);

	raaOctaveSystem *pSystem = new raaOctaveSystem(makeScene());
//	pSystem->addSceneData(makeScene());
	a.exec();

    return 0;
}
