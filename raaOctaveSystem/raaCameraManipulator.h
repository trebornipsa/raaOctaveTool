#pragma once
#include <windows.h>
#include <gl/gl.h>
#include <osgGA/CameraManipulator>

#include "raaOctaveSystemDefs.h"

class RAAOCTAVESYSTEM_DLL_DEF raaCameraManipulator: public osgGA::CameraManipulator
{
public:
	raaCameraManipulator();
	virtual ~raaCameraManipulator();

	virtual void setByMatrix(const osg::Matrixd& matrix);
	virtual void setByInverseMatrix(const osg::Matrixd& matrix);
	virtual osg::Matrixd getMatrix() const;
	virtual osg::Matrixd getInverseMatrix() const;
};

