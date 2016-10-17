// raaOctaveViewer.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include <windows.h>
#include <gl/gl.h>

#include <osgViewer/Viewer>
#include <osgViewer/ViewerEventHandlers>



int main(int argc, char* argv[])
{
	osg::ArgumentParser arguments(&argc, argv);
	osgViewer::Viewer viewer(arguments);
	viewer.addEventHandler(new osgViewer::StatsHandler);
	viewer.addEventHandler(new osgViewer::ThreadingHandler);

	osg::GraphicsContext::WindowingSystemInterface* wsi = osg::GraphicsContext::getWindowingSystemInterface();

	unsigned int width, height;
	wsi->getScreenResolution(osg::GraphicsContext::ScreenIdentifier(0), width, height);

	osg::ref_ptr<osg::GraphicsContext::Traits> traits = new osg::GraphicsContext::Traits;
	traits->x = 0;
	traits->y = 0;
	traits->width = width;
	traits->height = height;
	traits->windowDecoration = true;
	traits->doubleBuffer = true;
	traits->sharedContext = 0;

	osg::ref_ptr<osg::GraphicsContext> gc = osg::GraphicsContext::createGraphicsContext(traits.get());
	gc->setClearColor(osg::Vec4f(0.2f, 0.2f, 0.6f, 1.0f));
	gc->setClearMask(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	unsigned int numCameras = 2;
	double aspectRatioScale = 1.0;///(double)numCameras;
	for (unsigned int i = 0; i<numCameras; ++i)
	{
		osg::ref_ptr<osg::Camera> camera = new osg::Camera;
		camera->setGraphicsContext(gc.get());
		camera->setViewport(new osg::Viewport((i*width) / numCameras, (i*height) / numCameras, width / numCameras, height / numCameras));
		GLenum buffer = traits->doubleBuffer ? GL_BACK : GL_FRONT;
		camera->setDrawBuffer(buffer);
		camera->setReadBuffer(buffer);

		viewer.addSlave(camera.get(), osg::Matrixd(), osg::Matrixd::scale(aspectRatioScale, 1.0, 1.0));
	}


	viewer.realize();

    return 0;
}

