#pragma once



#include <osg/Group>
#include <osg/Texture2D>

//#include <raaOctaveController/raaScreen.h>

#include <raaOctaveController/raaOctaveViewPoint.h>

class raaScreen;
/*
class raaGeomCallback: public osg::Drawable::UpdateCallback, public osg::Drawable::AttributeFunctor
{
public:
	raaGeomCallback(osg::Vec3 vOrigin, osg::Vec3 vXAxis, osg::Vec3 vYAxis, osg::Vec3 vZAxis);
	virtual ~raaGeomCallback();

	virtual void update(osg::NodeVisitor* pNV, osg::Drawable* pDrawable);
	virtual void apply(osg::Drawable::AttributeType type, unsigned int uiCount, osg::Vec3* pBegin);
protected:
	osg::Vec3 m_vOrigin;
	osg::Vec3 m_vXAxis;
	osg::Vec3 m_vYAxis;
	osg::Vec3 m_vZAxis;
	bool m_bFirst;
	double m_dStartTime;
	double m_dTime;
};
*/

struct MyCameraPostDrawCallback : public osg::Camera::DrawCallback
{
	MyCameraPostDrawCallback(osg::Image* image) :
		_image(image)
	{
	}

	virtual void operator () (const osg::Camera& /*camera*/) const
	{
		if (_image && _image->getPixelFormat() == GL_RGBA && _image->getDataType() == GL_UNSIGNED_BYTE)
		{
			// we'll pick out the center 1/2 of the whole image,
			int column_start = _image->s() / 4;
			int column_end = 3 * column_start;

			int row_start = _image->t() / 4;
			int row_end = 3 * row_start;


			// and then invert these pixels
			for (int r = row_start; r<row_end; ++r)
			{
				unsigned char* data = _image->data(column_start, r);
				for (int c = column_start; c<column_end; ++c)
				{
					(*data) = 255 - (*data); ++data;
					(*data) = 255 - (*data); ++data;
					(*data) = 255 - (*data); ++data;
					(*data) = 255; ++data;
				}
			}


			// dirty the image (increments the modified count) so that any textures
			// using the image can be informed that they need to update.
			_image->dirty();
		}
		else if (_image && _image->getPixelFormat() == GL_RGBA && _image->getDataType() == GL_FLOAT)
		{
			// we'll pick out the center 1/2 of the whole image,
			int column_start = _image->s() / 4;
			int column_end = 3 * column_start;

			int row_start = _image->t() / 4;
			int row_end = 3 * row_start;

			// and then invert these pixels
			for (int r = row_start; r<row_end; ++r)
			{
				float* data = (float*)_image->data(column_start, r);
				for (int c = column_start; c<column_end; ++c)
				{
					(*data) = 1.0f - (*data); ++data;
					(*data) = 1.0f - (*data); ++data;
					(*data) = 1.0f - (*data); ++data;
					(*data) = 0.5f; ++data;
				}
			}

			// dirty the image (increments the modified count) so that any textures
			// using the image can be informed that they need to update.
			_image->dirty();
		}

	}

	osg::Image* _image;
};


class raaDisplayScreen
{
public:
//	raaDisplayScreen(raaScreen *pScreen, osg::Group *pScene, raaOctaveViewPoint *pViewpoint);

	raaDisplayScreen(osg::Group *pVirtualScene, std::string sName, osg::Vec3f vbl, osg::Vec3f vbr, osg::Vec3f vtl, osg::Vec3f vtr, osg::Vec3f vNormal, osg::Matrixf mPersp);

	virtual ~raaDisplayScreen();

//	virtual void nameChanged(raaScreen* pScreen);
//	virtual void screenChanged(raaScreen* pScreen);
//	virtual void screenMatrixChanged(raaScreen* pScreen);

	void screenChanged(osg::Vec3f vbl, osg::Vec3f vbr, osg::Vec3f vtl, osg::Vec3f vtr, osg::Vec3f vn);
	void screenMatrixChanged(osg::Matrixf &m);
	void screenStereoMatrixChanged(osg::Matrixf &mLeft, osg::Matrixf &mRight);

	void setDisplayScreen(int iScreen);
	int displayScreen();

	osg::Group* root();

	void setViewMatrix(osg::Matrixf &m);
	void setStereoViewMatrix(osg::Matrixf &mLeft, osg::Matrixf &mRight);

protected:

	osg::Group *m_pRoot;
	osg::Geode *m_pGeode;
	osg::Geometry *m_pGeom;
	osg::Geometry *m_pGeomR;
	osg::Vec3Array *m_pVerts;
	osg::Vec2Array *m_pTexCoords;
	osg::Vec3Array *m_pNorms;
	osg::Texture2D *m_pTexture;
	osg::Camera *m_pCamera;
	osg::MatrixTransform *m_pCameraView;
	osg::Vec3f m_vScreenRotation;
	int m_iScreen;

	raaScreen *m_pScreen;
};

