#include "stdafx.h"
#include <windows.h>

#include <osgDB/ReadFile>
#include <osg/Drawable>
#include <osg/ShapeDrawable>

#include <raaOctaveController/raaOctaveController.h>

#include "raaDisplayScreen.h"
#include "raaOctaveToolInterface.h"
#include "raaOctaveToolInterface.moc"
#include <osg/MatrixTransform>


raaOctaveToolInterface::raaOctaveToolInterface()
{
	m_pPhysicalViewpoint = 0;
	for(int i=0;i<8;i++) m_abPhysical[i] = m_abVirtual[i] = false;
	m_abPhysical[csm_uiTranslate] = m_abVirtual[csm_uiTranslate] = true;

	m_avPhysical[csm_uiPos] = osg::Vec3f(0.0f, 0.0f, 1.5f);
	m_avPhysical[csm_uiUp] = osg::Vec3f(0.0f, 0.0f, 1.0f);
	m_avPhysical[csm_uiRight] = osg::Vec3f(1.0f, 0.0f, 0.0f);
	m_avPhysical[csm_uiDir] = osg::Vec3f(0.0f, 1.0f, 0.0f);

	setupUi(this);

	gl_widget->addToScene(0, makeGrid(10.0f, 10.0f, 10, 10));
	m_pVirtualScene = new osg::Group();
	osg::Light *pLight = new osg::Light();
	pLight->setLightNum(0);
	pLight->setAmbient(osg::Vec4f(0.3f, 0.3f, 0.3f, 1.0f));
	pLight->setDiffuse(osg::Vec4f(1.0f, 1.0f, 1.0f, 1.0f));
	osg::LightSource *pLS = new osg::LightSource();
	pLS->setLight(pLight);
	pLS->setLocalStateSetModes(osg::StateAttribute::ON);
	m_pVirtualScene->addChild(pLS);

	m_pVirtualScene->addChild(makeGrid(10.0f, 10.0f, 10, 10));

	osg::Geode *pG0 = new osg::Geode();
	osg::ShapeDrawable *pSD0 = new osg::ShapeDrawable(new osg::Cone(osg::Vec3f(0.0f, 4.0f, 1.0f), 1.0f, 1.0f));
	pG0->addDrawable(pSD0);
	m_pVirtualScene->addChild(pG0);

	osg::Geode *pG1 = new osg::Geode();
	osg::ShapeDrawable *pSD1 = new osg::ShapeDrawable(new osg::Sphere(osg::Vec3f(0.0f, 0.0f, -4.0f), 1.0f));
	pG1->addDrawable(pSD1);
	m_pVirtualScene->addChild(pG1);

	osg::MatrixTransform *pMTC = new osg::MatrixTransform();
	osg::Matrix mC;
	mC.makeRotate(osg::DegreesToRadians(-90.0f), osg::Vec3f(1.0f, 0.0f, 0.0f));
	pMTC->setMatrix(mC);
	osg::Geode *pG2 = new osg::Geode();
	osg::ShapeDrawable *pSD2 = new osg::ShapeDrawable(new osg::Box(osg::Vec3f(0.0f, 0.0f, 4.0f), 1.0f));
	pG2->addDrawable(pSD2);
	pMTC->addChild(pG2);

	m_pVirtualScene->addChild(pG2);

	m_pController = new raaOctaveController(this);
	m_pController->readConfig("C:\\robbie\\data\\octave_config.raa");
	m_pController->viewpoint()->addListener(this);

	updateView();
	m_bLockCamera = false;

	connect(physical_translation_radio, SIGNAL(clicked(bool)), SLOT(phyTrans(bool)));
	connect(physical_rotation_radio, SIGNAL(clicked(bool)), SLOT(phyRot(bool)));
	connect(physical_xup_button, SIGNAL(pressed()), SLOT(phyXUpPressed()));
	connect(physical_xup_button, SIGNAL(released()), SLOT(phyXUpReleased()));
	connect(physical_xdown_button, SIGNAL(pressed()), SLOT(phyXDownPressed()));
	connect(physical_xdown_button, SIGNAL(released()), SLOT(phyXDownReleased()));
	connect(physical_yup_button, SIGNAL(pressed()), SLOT(phyYUpPressed()));
	connect(physical_yup_button, SIGNAL(released()), SLOT(phyYUpReleased()));
	connect(physical_ydown_button, SIGNAL(pressed()), SLOT(phyYDownPressed()));
	connect(physical_ydown_button, SIGNAL(released()), SLOT(phyYDownReleased()));
	connect(physical_zup_button, SIGNAL(pressed()), SLOT(phyZUpPressed()));
	connect(physical_zup_button, SIGNAL(released()), SLOT(phyZUpReleased()));
	connect(physical_zdown_button, SIGNAL(pressed()), SLOT(phyZDownPressed()));
	connect(physical_zdown_button, SIGNAL(released()), SLOT(phyZDownReleased()));

	connect(virtual_translation_radio, SIGNAL(clicked(bool)), SLOT(virTrans(bool)));
	connect(virtual_rotation_radio, SIGNAL(clicked(bool)), SLOT(virRot(bool)));
	connect(virtual_xup_button, SIGNAL(pressed()), SLOT(virXUpPressed()));
	connect(virtual_xup_button, SIGNAL(released()), SLOT(virXUpReleased()));
	connect(virtual_xdown_button, SIGNAL(pressed()), SLOT(virXDownPressed()));
	connect(virtual_xdown_button, SIGNAL(released()), SLOT(virXDownReleased()));
	connect(virtual_yup_button, SIGNAL(pressed()), SLOT(virYUpPressed()));
	connect(virtual_yup_button, SIGNAL(released()), SLOT(virYUpReleased()));
	connect(virtual_ydown_button, SIGNAL(pressed()), SLOT(virYDownPressed()));
	connect(virtual_ydown_button, SIGNAL(released()), SLOT(virYDownReleased()));
	connect(virtual_zup_button, SIGNAL(pressed()), SLOT(virZUpPressed()));
	connect(virtual_zup_button, SIGNAL(released()), SLOT(virZUpReleased()));
	connect(virtual_zdown_button, SIGNAL(pressed()), SLOT(virZDownPressed()));
	connect(virtual_zdown_button, SIGNAL(released()), SLOT(virZDownReleased()));
	connect(phy_lock_camera_check, SIGNAL(stateChanged(int)), SLOT(lockCamera(int)));

	m_pPhysicalViewpoint = new osg::MatrixTransform();
	osg::Geode *pVPGeode = new osg::Geode();
	osg::Geometry *pVPGeom = new osg::Geometry();
	osg::Vec3Array *pVPVerts = new osg::Vec3Array();
	osg::Vec4Array *pVPCols = new osg::Vec4Array();

	pVPVerts->push_back(osg::Vec3f(0.0f, 0.0f, 0.0f));
	pVPVerts->push_back(osg::Vec3f(0.3f, 0.0f, 0.0f));
	pVPVerts->push_back(osg::Vec3f(0.0f, 0.0f, 0.0f));
	pVPVerts->push_back(osg::Vec3f(0.0f, 0.3f, 0.0f));
	pVPVerts->push_back(osg::Vec3f(0.0f, 0.0f, 0.0f));
	pVPVerts->push_back(osg::Vec3f(0.0f, 0.0f, 0.3f));

	pVPCols->push_back(osg::Vec4(1.0f, 0.0f, 0.0f, 1.0f));
	pVPCols->push_back(osg::Vec4(1.0f, 0.0f, 0.0f, 0.3f));
	pVPCols->push_back(osg::Vec4(0.0f, 1.0f, 0.0f, 1.0f));
	pVPCols->push_back(osg::Vec4(0.0f, 1.0f, 0.0f, 0.3f));
	pVPCols->push_back(osg::Vec4(0.0f, 0.0f, 1.0f, 1.0f));
	pVPCols->push_back(osg::Vec4(0.0f, 0.0f, 1.0f, 0.3f));

	pVPGeom->setVertexArray(pVPVerts);
	pVPGeom->setColorArray(pVPCols, osg::Array::BIND_PER_VERTEX);
	pVPGeom->addPrimitiveSet(new osg::DrawArrays(osg::PrimitiveSet::LINES, 0, pVPVerts->size()));
	pVPGeode->getOrCreateStateSet()->setMode(GL_LIGHTING, osg::StateAttribute::OFF | osg::StateAttribute::OVERRIDE);
	pVPGeode->addDrawable(pVPGeom);
	m_pPhysicalViewpoint->addChild(pVPGeode);
	gl_widget->addToScene(0, m_pPhysicalViewpoint);
	gl_widget->addToScene(0, m_pVirtualScene);

	updateView();

	QTimer *pTimer = new QTimer(this);
	connect(pTimer, SIGNAL(timeout()), SLOT(timerUpdate()));
	pTimer->start(30);
}

void raaOctaveToolInterface::phyTrans(bool bVal)
{
	if (bVal) m_abPhysical[csm_uiTranslate] = true;
	else m_abPhysical[csm_uiTranslate] = false;

	m_abPhysical[csm_uiRotate] = !m_abPhysical[csm_uiTranslate];
}

void raaOctaveToolInterface::phyRot(bool bVal)
{
	if (bVal) m_abPhysical[csm_uiRotate] = true;
	else m_abPhysical[csm_uiRotate] = false;

	m_abPhysical[csm_uiTranslate] = !m_abPhysical[csm_uiRotate];
}

void raaOctaveToolInterface::phyXUpPressed()
{
	m_abPhysical[csm_uiUpX] = true;
}

void raaOctaveToolInterface::phyXUpReleased()
{
	m_abPhysical[csm_uiUpX] = false;
}

void raaOctaveToolInterface::phyXDownPressed()
{
	m_abPhysical[csm_uiDownX] = true;
}

void raaOctaveToolInterface::phyXDownReleased()
{
	m_abPhysical[csm_uiDownX] = false;
}

void raaOctaveToolInterface::phyYUpPressed()
{
	m_abPhysical[csm_uiUpY] = true;
}

void raaOctaveToolInterface::phyYUpReleased()
{
	m_abPhysical[csm_uiUpY] = false;
}

void raaOctaveToolInterface::phyYDownPressed()
{
	m_abPhysical[csm_uiDownY] = true;
}

void raaOctaveToolInterface::phyYDownReleased()
{
	m_abPhysical[csm_uiDownY] = false;
}

void raaOctaveToolInterface::phyZUpPressed()
{
	m_abPhysical[csm_uiUpZ] = true;
}

void raaOctaveToolInterface::phyZUpReleased()
{
	m_abPhysical[csm_uiUpZ] = false;
}

void raaOctaveToolInterface::phyZDownPressed()
{
	m_abPhysical[csm_uiDownZ] = true;
}

void raaOctaveToolInterface::phyZDownReleased()
{
	m_abPhysical[csm_uiDownZ] = false;
}

void raaOctaveToolInterface::virTrans(bool bVal)
{
	if (bVal) m_abVirtual[csm_uiTranslate] = true;
	else m_abVirtual[csm_uiTranslate] = false;
}

void raaOctaveToolInterface::virRot(bool bVal)
{
	if (bVal) m_abVirtual[csm_uiRotate] = true;
	else m_abVirtual[csm_uiRotate] = false;
}

void raaOctaveToolInterface::virXUpPressed()
{
	m_abVirtual[csm_uiUpX] = true;
}

void raaOctaveToolInterface::virXUpReleased()
{
	m_abVirtual[csm_uiUpX] = false;
}

void raaOctaveToolInterface::virXDownPressed()
{
	m_abVirtual[csm_uiDownX] = true;
}

void raaOctaveToolInterface::virXDownReleased()
{
	m_abVirtual[csm_uiDownX] = false;
}

void raaOctaveToolInterface::virYUpPressed()
{
	m_abVirtual[csm_uiUpY] = true;
}

void raaOctaveToolInterface::virYUpReleased()
{
	m_abVirtual[csm_uiUpY] = false;
}

void raaOctaveToolInterface::virYDownPressed()
{
	m_abVirtual[csm_uiDownY] = true;
}

void raaOctaveToolInterface::virYDownReleased()
{
	m_abVirtual[csm_uiDownY] = false;
}

void raaOctaveToolInterface::virZUpPressed()
{
	m_abVirtual[csm_uiUpZ] = true;
}

void raaOctaveToolInterface::virZUpReleased()
{
	m_abVirtual[csm_uiUpZ] = false;
}

void raaOctaveToolInterface::virZDownPressed()
{
	m_abVirtual[csm_uiDownZ] = true;
}

void raaOctaveToolInterface::virZDownReleased()
{
	m_abVirtual[csm_uiDownZ] = false;
}

void raaOctaveToolInterface::timerUpdate()
{
	bool bChanged = false;

	if(m_abPhysical[csm_uiTranslate])
	{
		if (m_abPhysical[csm_uiUpY])
		{
			m_avPhysical[csm_uiPos] += m_avPhysical[csm_uiDir] * 0.03f;
			bChanged = true;
		}
		if (m_abPhysical[csm_uiDownY])
		{
			m_avPhysical[csm_uiPos] -= m_avPhysical[csm_uiDir] * 0.03f;
			bChanged = true;
		}
		
		if (m_abPhysical[csm_uiUpX])
		{
			m_avPhysical[csm_uiPos] += m_avPhysical[csm_uiRight] * 0.03f;
			bChanged = true;
		}

		if (m_abPhysical[csm_uiDownX])
		{
			m_avPhysical[csm_uiPos] -= m_avPhysical[csm_uiRight] * 0.03f;
			bChanged = true;
		}
		
		if (m_abPhysical[csm_uiUpZ])
		{
			m_avPhysical[csm_uiPos] += m_avPhysical[csm_uiUp] * 0.03f;
			bChanged = true;
		}
		
		if (m_abPhysical[csm_uiDownZ])
		{
			m_avPhysical[csm_uiPos] -= m_avPhysical[csm_uiUp] * 0.03f;
			bChanged = true;
		}

	}

	if (m_abPhysical[csm_uiRotate])
	{
		if (m_abPhysical[csm_uiUpY])
		{
			m_avPhysical[csm_uiUp] = m_avPhysical[csm_uiUp] - (m_avPhysical[csm_uiDir] * 0.1f);
			m_avPhysical[csm_uiUp].normalize();

			m_avPhysical[csm_uiDir] = m_avPhysical[csm_uiUp] ^ m_avPhysical[csm_uiRight];
			m_avPhysical[csm_uiDir].normalize();
			bChanged = true;
		}
		
		if (m_abPhysical[csm_uiDownY])
		{
			m_avPhysical[csm_uiUp] = m_avPhysical[csm_uiUp] + (m_avPhysical[csm_uiDir] * 0.1f);
			m_avPhysical[csm_uiUp].normalize();

			m_avPhysical[csm_uiDir] = m_avPhysical[csm_uiUp] ^ m_avPhysical[csm_uiRight];
			m_avPhysical[csm_uiDir].normalize();
			bChanged = true;
		}
		
		if (m_abPhysical[csm_uiUpX])
		{
			m_avPhysical[csm_uiDir] = m_avPhysical[csm_uiDir] + (m_avPhysical[csm_uiRight] * 0.1f);
			m_avPhysical[csm_uiDir].normalize();

			m_avPhysical[csm_uiRight] = m_avPhysical[csm_uiDir] ^ m_avPhysical[csm_uiUp];
			m_avPhysical[csm_uiRight].normalize();
			bChanged = true;
		}
		
		if (m_abPhysical[csm_uiDownX])
		{
			m_avPhysical[csm_uiDir] = m_avPhysical[csm_uiDir] - (m_avPhysical[csm_uiRight] * 0.1f);
			m_avPhysical[csm_uiDir].normalize();

			m_avPhysical[csm_uiRight] = m_avPhysical[csm_uiDir] ^ m_avPhysical[csm_uiUp];
			m_avPhysical[csm_uiRight].normalize();
			bChanged = true;
		}
		
		if (m_abPhysical[csm_uiUpZ])
		{
			m_avPhysical[csm_uiUp] = m_avPhysical[csm_uiUp] + (m_avPhysical[csm_uiDir] * 0.1f);
			m_avPhysical[csm_uiUp].normalize();

			m_avPhysical[csm_uiDir] = m_avPhysical[csm_uiUp] ^ m_avPhysical[csm_uiRight];
			m_avPhysical[csm_uiDir].normalize();
			bChanged = true;
		}
		
		if (m_abPhysical[csm_uiDownZ])
		{
			m_avPhysical[csm_uiPos] -= m_avPhysical[csm_uiUp] * 0.03f;
			bChanged = true;
		}
	}

	if (bChanged) updateView();
}

void raaOctaveToolInterface::lockCamera(int iVal)
{
	if(iVal==Qt::Checked)
	{
		m_CameraManipulatorMatrix = gl_widget->getView(0)->getCameraManipulator()->getMatrix();
		m_bLockCamera = true;
		updateView();
	}
	else
	{
		gl_widget->getView(0)->getCameraManipulator()->setByMatrix(m_CameraManipulatorMatrix);
		m_bLockCamera = false;
	}
}

raaOctaveToolInterface::~raaOctaveToolInterface()
{
}

osg::Geode* raaOctaveToolInterface::makeGrid(float fWidth, float fDepth, unsigned uiWidthSegs, unsigned uiDepthSegs)
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

	for(unsigned int i=0;i<=uiWidthSegs;i++, fW+=fWS)
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

void raaOctaveToolInterface::updateView()
{
	osg::Matrix mRot, mTrans;

	mRot.set(m_avPhysical[csm_uiRight][0], m_avPhysical[csm_uiDir][0], m_avPhysical[csm_uiUp][0], 0.0f,
			m_avPhysical[csm_uiRight][1], m_avPhysical[csm_uiDir][1], m_avPhysical[csm_uiUp][1], 0.0f,
			m_avPhysical[csm_uiRight][2], m_avPhysical[csm_uiDir][2], m_avPhysical[csm_uiUp][2], 0.0f,
			0.0f, 0.0f, 0.0f, 1.0f);

	mTrans.makeTranslate(m_avPhysical[csm_uiPos]);

	m_pController->viewpoint()->setPhysicalMatrix(mTrans);

	if (m_pPhysicalViewpoint)
		m_pPhysicalViewpoint->setMatrix(mRot*mTrans);

	if (m_bLockCamera)
		gl_widget->getView(0)->getCamera()->setViewMatrix(mRot*mTrans);

}

void raaOctaveToolInterface::originChanged(raaOctaveController* pController)
{
}

void raaOctaveToolInterface::screenAdded(raaOctaveController* pController, raaScreen* pScreen)
{
	raaDisplayScreen *pDisplay = new raaDisplayScreen(pScreen, m_pVirtualScene, m_pController->viewpoint());
	m_mDisplays[pScreen->name()] = pDisplay;
	gl_widget->addToTranspScene(0, pDisplay->root());
}

void raaOctaveToolInterface::screenRemoved(raaOctaveController* pController, raaScreen* pScreen)
{
}

void raaOctaveToolInterface::screenUpdated(raaOctaveController* pController, raaScreen* pScreen)
{
}

void raaOctaveToolInterface::viewpointChanged(raaOctaveViewPoint* pViewpoint)
{
//	if(m_pPhysicalViewpoint)
//		m_pPhysicalViewpoint->setMatrix(pViewpoint->physicalMatrix());
}
