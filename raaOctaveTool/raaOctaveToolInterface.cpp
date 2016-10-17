#include "stdafx.h"
#include <windows.h>

#include <osgDB/ReadFile>
#include <osg/Drawable>
#include <osg/ShapeDrawable>

//#include <raaOctaveController/raaOctaveController.h>
#include <raaNetwork/raaTcpMsg.h>
#include <raaNetwork/raaTcpThread.h>

#include <raaOctaveKernel/raaOctaveKernel.h>

#include "raaDisplayScreen.h"
#include "raaOctaveToolInterface.h"
#include "raaOctaveToolInterface.moc"
#include <osg/MatrixTransform>


raaOctaveToolInterface::raaOctaveToolInterface()
{
	m_pUdpClient= 0;
	m_pTcpClient = 0;
	m_bScreenUpdate = true;
//	m_pCurrentScreen = 0;
	m_pPhysicalViewpoint = 0;
	for(int i=0;i<8;i++) m_abPhysical[i] = m_abVirtual[i] = false;
	m_abPhysical[csm_uiTranslate] = m_abVirtual[csm_uiTranslate] = true;

	m_avPhysical[csm_uiPos] = osg::Vec3f(0.0f, 0.0f, 1.5f);
	m_avPhysical[csm_uiUp] = osg::Vec3f(0.0f, 0.0f, 1.0f);
	m_avPhysical[csm_uiRight] = osg::Vec3f(1.0f, 0.0f, 0.0f);
	m_avPhysical[csm_uiDir] = osg::Vec3f(0.0f, 1.0f, 0.0f);

	m_avVirtual[csm_uiPos] = osg::Vec3f(0.0f, 0.0f, 0.0f);
	m_avVirtual[csm_uiUp] = osg::Vec3f(0.0f, 0.0f, 1.0f);
	m_avVirtual[csm_uiRight] = osg::Vec3f(1.0f, 0.0f, 0.0f);
	m_avVirtual[csm_uiDir] = osg::Vec3f(0.0f, 1.0f, 0.0f);

	setupUi(this);

	m_pNetwork = new raaNet::raaNetwork(0, this);
	m_pNetwork->createTcpClient("raaOctaveTool", "localhost", 65204);

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

	updateView();
	m_bLockCamera = false;

	connect(m_pNetwork, SIGNAL(tcpRead(raaTcpMsg*)), SLOT(tcpRead(raaTcpMsg*)));
	connect(m_pNetwork, SIGNAL(tcpState(raaTcpThread*, unsigned int)), SLOT(tcpState(raaTcpThread*, unsigned int)));

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
	connect(toolBox, SIGNAL(currentChanged(int)), SLOT(currentToolboxChanged(int)));
	connect(screen_combo, SIGNAL(currentIndexChanged(const QString&)), SLOT(currentScreenChanged(const QString&)));

	connect(screen_continual_update_check, SIGNAL(stateChanged(int)), SLOT(screenContUpdate(int)));
	connect(screen_updtae_button, SIGNAL(pressed()), SLOT(screenAllChanged()));


	connect(screen_bl_x_spin, SIGNAL(valueChanged(double)), SLOT(screenBLChanged(double)));
	connect(screen_bl_y_spin, SIGNAL(valueChanged(double)), SLOT(screenBLChanged(double)));
	connect(screen_bl_z_spin, SIGNAL(valueChanged(double)), SLOT(screenBLChanged(double)));

	connect(screen_br_x_spin, SIGNAL(valueChanged(double)), SLOT(screenBRChanged(double)));
	connect(screen_br_y_spin, SIGNAL(valueChanged(double)), SLOT(screenBRChanged(double)));
	connect(screen_br_z_spin, SIGNAL(valueChanged(double)), SLOT(screenBRChanged(double)));

	connect(screen_tl_x_spin, SIGNAL(valueChanged(double)), SLOT(screenTLChanged(double)));
	connect(screen_tl_y_spin, SIGNAL(valueChanged(double)), SLOT(screenTLChanged(double)));
	connect(screen_tl_z_spin, SIGNAL(valueChanged(double)), SLOT(screenTLChanged(double)));

	connect(screen_tr_x_spin, SIGNAL(valueChanged(double)), SLOT(screenTRChanged(double)));
	connect(screen_tr_y_spin, SIGNAL(valueChanged(double)), SLOT(screenTRChanged(double)));
	connect(screen_tr_z_spin, SIGNAL(valueChanged(double)), SLOT(screenTRChanged(double)));
	connect(screen_near_spin, SIGNAL(valueChanged(double)), SLOT(screenNFChanged(double)));
	connect(screen_far_spin, SIGNAL(valueChanged(double)), SLOT(screenNFChanged(double)));

	connect(screen_image_rot_spin, SIGNAL(valueChanged(double)), SLOT(screenRotChanged(double)));
	connect(screen_x_flip_check, SIGNAL(stateChanged(int)), SLOT(screenFlipChanged(int)));
	connect(screen_y_flip_check, SIGNAL(stateChanged(int)), SLOT(screenFlipChanged(int)));
	connect(screen_z_flip_check, SIGNAL(stateChanged(int)), SLOT(screenFlipChanged(int)));

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
	bool bPhysicalChanged = false;

	if(m_abPhysical[csm_uiTranslate])
	{
		if (m_abPhysical[csm_uiUpY])
		{
			m_avPhysical[csm_uiPos] += m_avPhysical[csm_uiDir] * 0.03f;
			bPhysicalChanged = true;
		}
		if (m_abPhysical[csm_uiDownY])
		{
			m_avPhysical[csm_uiPos] -= m_avPhysical[csm_uiDir] * 0.03f;
			bPhysicalChanged = true;
		}
		
		if (m_abPhysical[csm_uiUpX])
		{
			m_avPhysical[csm_uiPos] += m_avPhysical[csm_uiRight] * 0.03f;
			bPhysicalChanged = true;
		}

		if (m_abPhysical[csm_uiDownX])
		{
			m_avPhysical[csm_uiPos] -= m_avPhysical[csm_uiRight] * 0.03f;
			bPhysicalChanged = true;
		}
		
		if (m_abPhysical[csm_uiUpZ])
		{
			m_avPhysical[csm_uiPos] += m_avPhysical[csm_uiUp] * 0.03f;
			bPhysicalChanged = true;
		}
		
		if (m_abPhysical[csm_uiDownZ])
		{
			m_avPhysical[csm_uiPos] -= m_avPhysical[csm_uiUp] * 0.03f;
			bPhysicalChanged = true;
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
			bPhysicalChanged = true;
		}
		
		if (m_abPhysical[csm_uiDownY])
		{
			m_avPhysical[csm_uiUp] = m_avPhysical[csm_uiUp] + (m_avPhysical[csm_uiDir] * 0.1f);
			m_avPhysical[csm_uiUp].normalize();

			m_avPhysical[csm_uiDir] = m_avPhysical[csm_uiUp] ^ m_avPhysical[csm_uiRight];
			m_avPhysical[csm_uiDir].normalize();
			bPhysicalChanged = true;
		}
		
		if (m_abPhysical[csm_uiUpX])
		{
			m_avPhysical[csm_uiDir] = m_avPhysical[csm_uiDir] + (m_avPhysical[csm_uiRight] * 0.1f);
			m_avPhysical[csm_uiDir].normalize();

			m_avPhysical[csm_uiRight] = m_avPhysical[csm_uiDir] ^ m_avPhysical[csm_uiUp];
			m_avPhysical[csm_uiRight].normalize();
			bPhysicalChanged = true;
		}
		
		if (m_abPhysical[csm_uiDownX])
		{
			m_avPhysical[csm_uiDir] = m_avPhysical[csm_uiDir] - (m_avPhysical[csm_uiRight] * 0.1f);
			m_avPhysical[csm_uiDir].normalize();

			m_avPhysical[csm_uiRight] = m_avPhysical[csm_uiDir] ^ m_avPhysical[csm_uiUp];
			m_avPhysical[csm_uiRight].normalize();
			bPhysicalChanged = true;
		}
		
		if (m_abPhysical[csm_uiUpZ])
		{
			m_avPhysical[csm_uiUp] = m_avPhysical[csm_uiUp] + (m_avPhysical[csm_uiDir] * 0.1f);
			m_avPhysical[csm_uiUp].normalize();

			m_avPhysical[csm_uiDir] = m_avPhysical[csm_uiUp] ^ m_avPhysical[csm_uiRight];
			m_avPhysical[csm_uiDir].normalize();
			bPhysicalChanged = true;
		}
		
		if (m_abPhysical[csm_uiDownZ])
		{
			m_avPhysical[csm_uiPos] -= m_avPhysical[csm_uiUp] * 0.03f;
			bPhysicalChanged = true;
		}
	}

	if (bPhysicalChanged) updateView();


	bool bVirtualChanged = false;

	if (m_abVirtual[csm_uiTranslate])
	{
		if (m_abVirtual[csm_uiUpY])
		{
			m_avVirtual[csm_uiPos] += m_avVirtual[csm_uiDir] * 0.03f;
			bVirtualChanged = true;
		}
		if (m_abVirtual[csm_uiDownY])
		{
			m_avVirtual[csm_uiPos] -= m_avVirtual[csm_uiDir] * 0.03f;
			bVirtualChanged = true;
		}

		if (m_abVirtual[csm_uiUpX])
		{
			m_avVirtual[csm_uiPos] += m_avVirtual[csm_uiRight] * 0.03f;
			bVirtualChanged = true;
		}

		if (m_abVirtual[csm_uiDownX])
		{
			m_avVirtual[csm_uiPos] -= m_avVirtual[csm_uiRight] * 0.03f;
			bVirtualChanged = true;
		}

		if (m_abVirtual[csm_uiUpZ])
		{
			m_avVirtual[csm_uiPos] += m_avVirtual[csm_uiUp] * 0.03f;
			bVirtualChanged = true;
		}

		if (m_abVirtual[csm_uiDownZ])
		{
			m_avVirtual[csm_uiPos] -= m_avVirtual[csm_uiUp] * 0.03f;
			bVirtualChanged = true;
		}

	}

	if (m_abVirtual[csm_uiRotate])
	{
		if (m_abVirtual[csm_uiUpY])
		{
			m_avVirtual[csm_uiUp] = m_avVirtual[csm_uiUp] - (m_avVirtual[csm_uiDir] * 0.1f);
			m_avVirtual[csm_uiUp].normalize();

			m_avVirtual[csm_uiDir] = m_avVirtual[csm_uiUp] ^ m_avVirtual[csm_uiRight];
			m_avVirtual[csm_uiDir].normalize();
			bVirtualChanged = true;
		}

		if (m_abVirtual[csm_uiDownY])
		{
			m_avVirtual[csm_uiUp] = m_avVirtual[csm_uiUp] + (m_avVirtual[csm_uiDir] * 0.1f);
			m_avVirtual[csm_uiUp].normalize();

			m_avVirtual[csm_uiDir] = m_avVirtual[csm_uiUp] ^ m_avVirtual[csm_uiRight];
			m_avVirtual[csm_uiDir].normalize();
			bVirtualChanged = true;
		}

		if (m_abVirtual[csm_uiUpX])
		{
			m_avVirtual[csm_uiDir] = m_avVirtual[csm_uiDir] + (m_avVirtual[csm_uiRight] * 0.1f);
			m_avVirtual[csm_uiDir].normalize();

			m_avVirtual[csm_uiRight] = m_avVirtual[csm_uiDir] ^ m_avVirtual[csm_uiUp];
			m_avVirtual[csm_uiRight].normalize();
			bVirtualChanged = true;
		}

		if (m_abVirtual[csm_uiDownX])
		{
			m_avVirtual[csm_uiDir] = m_avVirtual[csm_uiDir] - (m_avVirtual[csm_uiRight] * 0.1f);
			m_avVirtual[csm_uiDir].normalize();

			m_avVirtual[csm_uiRight] = m_avVirtual[csm_uiDir] ^ m_avVirtual[csm_uiUp];
			m_avVirtual[csm_uiRight].normalize();
			bVirtualChanged = true;
		}

		if (m_abVirtual[csm_uiUpZ])
		{
			m_avVirtual[csm_uiUp] = m_avVirtual[csm_uiUp] + (m_avVirtual[csm_uiDir] * 0.1f);
			m_avVirtual[csm_uiUp].normalize();

			m_avVirtual[csm_uiDir] = m_avVirtual[csm_uiUp] ^ m_avVirtual[csm_uiRight];
			m_avVirtual[csm_uiDir].normalize();
			bVirtualChanged = true;
		}

		if (m_abVirtual[csm_uiDownZ])
		{
			m_avVirtual[csm_uiPos] -= m_avVirtual[csm_uiUp] * 0.03f;
			bVirtualChanged = true;
		}
	}

	if (bVirtualChanged) updateView();
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

void raaOctaveToolInterface::currentToolboxChanged(int iVal)
{

	if(iVal==1)
	{
		raaNet::raaTcpMsg *pMsg = new raaNet::raaTcpMsg(raaNet::csm_usTcpMsgRequest);
		pMsg->add(raaOctaveKernel::csm_uiOCControllerRequestScreenNames);
		if (m_pTcpClient) m_pTcpClient->write(pMsg);
	}
}

void raaOctaveToolInterface::currentScreenChanged(const QString& s)
{
	m_sCurrentScreen = s.toStdString();

	if(m_sCurrentScreen.length() && m_mDisplays.find(m_sCurrentScreen)!=m_mDisplays.end())
	{
		raaNet::raaTcpMsg *pMsg = new raaNet::raaTcpMsg(raaNet::csm_usTcpMsgRequest);
		pMsg->add(raaOctaveKernel::csm_uiOCScreenInfo);
		pMsg->add(m_sCurrentScreen);
		if (m_pTcpClient) m_pTcpClient->write(pMsg);
	}
}

void raaOctaveToolInterface::screenBLChanged(double)
{
	if (m_bScreenUpdate && m_sCurrentScreen.length())
	{
		raaNet::raaTcpMsg *pMsg = new raaNet::raaTcpMsg(raaNet::csm_usTcpMsgInfo);
		pMsg->add(raaOctaveKernel::csm_uiOCScreenVertex);
		pMsg->add(raaOctaveKernel::csm_uiOCBL);
		pMsg->add(m_sCurrentScreen);
		pMsg->add(osg::Vec3f(screen_bl_x_spin->value(), screen_bl_y_spin->value(), screen_bl_z_spin->value()));
		if (m_pTcpClient) m_pTcpClient->write(pMsg);
	}
}

void raaOctaveToolInterface::screenBRChanged(double)
{
	if (m_bScreenUpdate && m_sCurrentScreen.length())
	{
		raaNet::raaTcpMsg *pMsg = new raaNet::raaTcpMsg(raaNet::csm_usTcpMsgInfo);
		pMsg->add(raaOctaveKernel::csm_uiOCScreenVertex);
		pMsg->add(raaOctaveKernel::csm_uiOCBR);
		pMsg->add(m_sCurrentScreen);
		pMsg->add(osg::Vec3f(screen_br_x_spin->value(), screen_br_y_spin->value(), screen_br_z_spin->value()));
		if (m_pTcpClient) m_pTcpClient->write(pMsg);
	}
}

void raaOctaveToolInterface::screenTLChanged(double)
{
	if (m_bScreenUpdate && m_sCurrentScreen.length())
	{
		raaNet::raaTcpMsg *pMsg = new raaNet::raaTcpMsg(raaNet::csm_usTcpMsgInfo);
		pMsg->add(raaOctaveKernel::csm_uiOCScreenVertex);
		pMsg->add(raaOctaveKernel::csm_uiOCTL);
		pMsg->add(m_sCurrentScreen);
		pMsg->add(osg::Vec3f(screen_tl_x_spin->value(), screen_tl_y_spin->value(), screen_tl_z_spin->value()));
		if (m_pTcpClient) m_pTcpClient->write(pMsg);
	}
}

void raaOctaveToolInterface::screenTRChanged(double)
{
	if (m_bScreenUpdate && m_sCurrentScreen.length())
	{
		raaNet::raaTcpMsg *pMsg = new raaNet::raaTcpMsg(raaNet::csm_usTcpMsgInfo);
		pMsg->add(raaOctaveKernel::csm_uiOCScreenVertex);
		pMsg->add(raaOctaveKernel::csm_uiOCTR);
		pMsg->add(m_sCurrentScreen);
		pMsg->add(osg::Vec3f(screen_tr_x_spin->value(), screen_tr_y_spin->value(), screen_tr_z_spin->value()));
		if (m_pTcpClient) m_pTcpClient->write(pMsg);
	}
}

void raaOctaveToolInterface::screenNFChanged(double)
{
	if (m_bScreenUpdate && m_sCurrentScreen.length())
	{
		raaNet::raaTcpMsg *pMsg = new raaNet::raaTcpMsg(raaNet::csm_usTcpMsgInfo);
		pMsg->add(raaOctaveKernel::csm_uiOCScreenNearFar);
		pMsg->add(m_sCurrentScreen);
		pMsg->add((float)screen_near_spin->value());
		pMsg->add((float)screen_far_spin->value());
		if (m_pTcpClient) m_pTcpClient->write(pMsg);
	}
}

void raaOctaveToolInterface::screenRotChanged(double)
{
	if (m_bScreenUpdate && m_sCurrentScreen.length())
	{
		raaNet::raaTcpMsg *pMsg = new raaNet::raaTcpMsg(raaNet::csm_usTcpMsgInfo);
		pMsg->add(raaOctaveKernel::csm_uiOCScreenImageRotationInfo);
		pMsg->add(m_sCurrentScreen);
		pMsg->add((float)screen_image_rot_spin->value());
		if (m_pTcpClient) m_pTcpClient->write(pMsg);
	}
}

void raaOctaveToolInterface::screenFlipChanged(int)
{
	if (m_bScreenUpdate && m_sCurrentScreen.length())
	{
		raaNet::raaTcpMsg *pMsg = new raaNet::raaTcpMsg(raaNet::csm_usTcpMsgInfo);
		pMsg->add(raaOctaveKernel::csm_uiOCScreenImageFlipInfo);
		pMsg->add(m_sCurrentScreen);
		pMsg->add(screen_x_flip_check->isChecked());
		pMsg->add(screen_y_flip_check->isChecked());
		pMsg->add(screen_z_flip_check->isChecked());
		if (m_pTcpClient) m_pTcpClient->write(pMsg);
	}
}

void raaOctaveToolInterface::screenAllChanged()
{
	if (m_sCurrentScreen.length())
	{
		raaNet::raaTcpMsg *pMsg = new raaNet::raaTcpMsg(raaNet::csm_usTcpMsgInfo);
		pMsg->add(raaOctaveKernel::csm_uiOCScreenVertexAll);
		pMsg->add(m_sCurrentScreen);
		pMsg->add(osg::Vec3f(screen_bl_x_spin->value(), screen_bl_y_spin->value(), screen_bl_z_spin->value()));
		pMsg->add(osg::Vec3f(screen_br_x_spin->value(), screen_br_y_spin->value(), screen_br_z_spin->value()));
		pMsg->add(osg::Vec3f(screen_tl_x_spin->value(), screen_tl_y_spin->value(), screen_tl_z_spin->value()));
		pMsg->add(osg::Vec3f(screen_tr_x_spin->value(), screen_tr_y_spin->value(), screen_tr_z_spin->value()));
		if (m_pTcpClient) m_pTcpClient->write(pMsg);

		pMsg = new raaNet::raaTcpMsg(raaNet::csm_usTcpMsgInfo);
		pMsg->add(raaOctaveKernel::csm_uiOCScreenNearFar);
		pMsg->add(m_sCurrentScreen);
		pMsg->add((float)screen_near_spin->value());
		pMsg->add((float)screen_far_spin->value());
		if (m_pTcpClient) m_pTcpClient->write(pMsg);

		pMsg = new raaNet::raaTcpMsg(raaNet::csm_usTcpMsgInfo);
		pMsg->add(raaOctaveKernel::csm_uiOCScreenImageRotationInfo);
		pMsg->add(m_sCurrentScreen);
		pMsg->add((float)screen_image_rot_spin->value());
		if (m_pTcpClient) m_pTcpClient->write(pMsg);

		pMsg = new raaNet::raaTcpMsg(raaNet::csm_usTcpMsgInfo);
		pMsg->add(raaOctaveKernel::csm_uiOCScreenImageFlipInfo);
		pMsg->add(m_sCurrentScreen);
		pMsg->add(screen_x_flip_check->isChecked());
		pMsg->add(screen_y_flip_check->isChecked());
		pMsg->add(screen_z_flip_check->isChecked());
		if (m_pTcpClient) m_pTcpClient->write(pMsg);
	}
}

void raaOctaveToolInterface::screenContUpdate(int iVal)
{
	if (iVal == Qt::Checked)
		m_bScreenUpdate = true;
	else
		m_bScreenUpdate = false;
}

void raaOctaveToolInterface::tcpRead(raaNet::raaTcpMsg* pMsg)
{
	if(pMsg)
	{
		switch(pMsg->msgType())
		{
			case raaNet::csm_usTcpMsgReply:
			{
				if(pMsg->asUInt(2)==raaOctaveKernel::csm_uiOCHasConfigTrue)
				{
					raaNet::raaTcpMsg *pM = new raaNet::raaTcpMsg(raaNet::csm_usTcpMsgRequest);
					pM->add(raaOctaveKernel::csm_uiOCControllerRequestScreenAll);
					m_pTcpClient->write(pM);

					updateView();
				}
				else if (pMsg->asUInt(2) == raaOctaveKernel::csm_uiOCHasConfigFalse)
				{
					raaNet::raaTcpMsg *pM = new raaNet::raaTcpMsg(raaNet::csm_usTcpMsgRequest);
					pM->add(raaOctaveKernel::csm_uiOCLoadConfig);
					pM->add(std::string("C:\\robbie\\data\\octave_config.raa"));
					m_pTcpClient->write(pM);

					updateView();
				}
			}
			break;
			case raaNet::csm_usTcpMsgInfo:
			{
				switch(pMsg->asUInt(2))
				{
					case raaOctaveKernel::csm_uiOCControllerScreenAdded:
					{
						std::string sName = pMsg->asString(3);
						osg::Vec3f vbl = pMsg->asVector(4);
						osg::Vec3f vbr = pMsg->asVector(5);
						osg::Vec3f vtl = pMsg->asVector(6);
						osg::Vec3f vtr = pMsg->asVector(7);
						osg::Vec3f vn = pMsg->asVector(8); 
						float fN = pMsg->asFloat(9);
						float fF = pMsg->asFloat(10);
						float fR = pMsg->asFloat(11);
						bool bX = pMsg->asBool(12);
						bool bY = pMsg->asBool(13);
						bool bZ = pMsg->asBool(14);
						osg::Matrixf mPersp =pMsg->asMatrix(15);

						m_mDisplays[sName] = new raaDisplayScreen(m_pVirtualScene, sName, vbl, vbr, vtl, vtr, vn, mPersp);
						gl_widget->addToTranspScene(0, m_mDisplays[sName]->root());
					}
					break;
					case raaOctaveKernel::csm_uiOCControllerScreenRemoved:
					{
						std::string sName = pMsg->asString(3);
						std::cout << "Screen Removed -> " << sName << std::endl;
					}
					break;
					case raaOctaveKernel::csm_uiOCViewpointPhysicalChanged:
					{
						osg::Matrixf m = pMsg->asMatrix(3);
						if (m_pPhysicalViewpoint)m_pPhysicalViewpoint->setMatrix(m);
					}
					break;
					case raaOctaveKernel::csm_uiOCViewpointVirtualChanged:
					{
						osg::Matrixf m = pMsg->asMatrix(3);

						for (raaDisplayScreens::iterator it = m_mDisplays.begin(); it != m_mDisplays.end(); it++)
							it->second->setViewMatrix(m);
					}
					break;
					case raaOctaveKernel::csm_uiOCScreenMatrixChanged:
					{
						std::string sName = pMsg->asString(3);
						osg::Matrixf m = pMsg->asMatrix(4);

						if (sName.length() && m_mDisplays.find(sName) != m_mDisplays.end()) m_mDisplays[sName]->screenMatrixChanged(m);
					}
					break;
					case raaOctaveKernel::csm_uiOCScreenChanged:
					{
						std::string sName = pMsg->asString(3);
						osg::Vec3f vbl = pMsg->asVector(4);
						osg::Vec3f vbr = pMsg->asVector(5);
						osg::Vec3f vtl = pMsg->asVector(6);
						osg::Vec3f vtr = pMsg->asVector(7);
						osg::Vec3f vn = pMsg->asVector(8);
						float fN = pMsg->asFloat(9);
						float fF = pMsg->asFloat(10);
						float fR = pMsg->asFloat(11);
						bool bX = pMsg->asBool(12);
						bool bY = pMsg->asBool(13);
						bool bZ = pMsg->asBool(14);

						if (sName.length() && m_mDisplays.find(sName) != m_mDisplays.end()) m_mDisplays[sName]->screenChanged(vbl ,vbr, vtl, vtr, vn);

						if(m_sCurrentScreen==sName)
						{
							bool b = m_bScreenUpdate;
							m_bScreenUpdate = false;
							screen_bl_x_spin->setValue(vbl[0]);
							screen_bl_y_spin->setValue(vbl[1]);
							screen_bl_z_spin->setValue(vbl[2]);
							screen_br_x_spin->setValue(vbr[0]);
							screen_br_y_spin->setValue(vbr[1]);
							screen_br_z_spin->setValue(vbr[2]);
							screen_tl_x_spin->setValue(vtl[0]);
							screen_tl_y_spin->setValue(vtl[1]);
							screen_tl_z_spin->setValue(vtl[2]);
							screen_tr_x_spin->setValue(vtr[0]);
							screen_tr_y_spin->setValue(vtr[1]);
							screen_tr_z_spin->setValue(vtr[2]);
							screen_near_spin->setValue(fN);
							screen_far_spin->setValue(fF);
							screen_image_rot_spin->setValue(fR);
							screen_x_flip_check->setCheckState(bX ? Qt::Checked : Qt::Unchecked);
							screen_y_flip_check->setCheckState(bY ? Qt::Checked : Qt::Unchecked);
							screen_z_flip_check->setCheckState(bZ ? Qt::Checked : Qt::Unchecked);

							m_bScreenUpdate = b;
						}
					}
					break;
					case raaOctaveKernel::csm_uiOCScreenInfo:
					{
						std::string sName = pMsg->asString(3);
						osg::Vec3f vbl = pMsg->asVector(4);
						osg::Vec3f vbr = pMsg->asVector(5);
						osg::Vec3f vtl = pMsg->asVector(6);
						osg::Vec3f vtr = pMsg->asVector(7);
						osg::Vec3f vn = pMsg->asVector(8);
						float fN = pMsg->asFloat(9);
						float fF = pMsg->asFloat(10);
						float fR = pMsg->asFloat(11);
						bool bX = pMsg->asBool(12);
						bool bY = pMsg->asBool(13);
						bool bZ = pMsg->asBool(14);


						if (sName.length() && m_mDisplays.find(sName) != m_mDisplays.end()) m_mDisplays[sName]->screenChanged(vbl, vbr, vtl, vtr, vn);

						if (m_sCurrentScreen == sName)
						{
							bool b = m_bScreenUpdate;
							m_bScreenUpdate = false;
							screen_bl_x_spin->setValue(vbl[0]);
							screen_bl_y_spin->setValue(vbl[1]);
							screen_bl_z_spin->setValue(vbl[2]);
							screen_br_x_spin->setValue(vbr[0]);
							screen_br_y_spin->setValue(vbr[1]);
							screen_br_z_spin->setValue(vbr[2]);
							screen_tl_x_spin->setValue(vtl[0]);
							screen_tl_y_spin->setValue(vtl[1]);
							screen_tl_z_spin->setValue(vtl[2]);
							screen_tr_x_spin->setValue(vtr[0]);
							screen_tr_y_spin->setValue(vtr[1]);
							screen_tr_z_spin->setValue(vtr[2]);
							screen_near_spin->setValue(fN);
							screen_far_spin->setValue(fF);
							screen_image_rot_spin->setValue(fR);
							screen_x_flip_check->setCheckState(bX ? Qt::Checked : Qt::Unchecked);
							screen_y_flip_check->setCheckState(bY ? Qt::Checked : Qt::Unchecked);
							screen_z_flip_check->setCheckState(bZ ? Qt::Checked : Qt::Unchecked);
							m_bScreenUpdate = b;
						}
					}
					break;
					case raaOctaveKernel::csm_uiOCControllerRequestScreenNames:
					{
						screen_combo->clear();
						unsigned int uiNum = pMsg->asUInt(3);
						for (unsigned int i = 0; i < uiNum; i++)
							screen_combo->addItem(pMsg->asString(4 + i).c_str());
					}
					break;
				}
			}
		}
	}
}

void raaOctaveToolInterface::tcpState(raaNet::raaTcpThread* pThread, unsigned uiState)
{
	QString msg;
	msg += pThread->name();
	switch (uiState)
	{
	case raaNet::csm_uiUnconnectedState:
		msg += " -> StateChanged::UnconnectedState";
		break;
	case raaNet::csm_uiHostLookupState:
		msg += " -> StateChanged::HostLookupState";
		break;
	case raaNet::csm_uiConnectingState:
		msg += " -> StateChanged::ConnectingState";
		break;
	case raaNet::csm_uiConnectedState:
		msg += " -> StateChanged::ConnectedState";
		break;
	case raaNet::csm_uiNameConnectedState:
		msg += " -> StateChanged::NameConnectedState";
		if (pThread->name() == "raaOctaveTool")
		{
			m_pTcpClient = pThread;

			raaNet::raaTcpMsg *pM0 = new raaTcpMsg(raaNet::csm_usTcpMsgRequest);
			pM0->add(raaOctaveKernel::csm_uiOCAttachControllerListener);
			m_pTcpClient->write(pM0);

			raaNet::raaTcpMsg *pM1 = new raaTcpMsg(raaNet::csm_usTcpMsgRequest);
			pM1->add(raaOctaveKernel::csm_uiOCAttachViewpointListener);
			m_pTcpClient->write(pM1);

			raaNet::raaTcpMsg *pM2 = new raaTcpMsg(raaNet::csm_usTcpMsgRequest);
			pM2->add(raaOctaveKernel::csm_uiOCAttachScreenListener);
			m_pTcpClient->write(pM2);

			raaNet::raaTcpMsg *pM3 = new raaTcpMsg(raaNet::csm_usTcpMsgRequest);
			pM3->add(raaOctaveKernel::csm_uiOCHasConfig);
			m_pTcpClient->write(pM3);
		}
		break;
	case raaNet::csm_uiBoundState:
		msg += " -> StateChanged::BoundState";
		break;
	case raaNet::csm_uiClosingState:
		msg += " -> StateChanged::ClosingState";
		if (pThread->name() == "clientInterface")
		{
			m_pTcpClient = 0;
		}
		break;
	case raaNet::csm_uiListeningState:
		msg += " -> StateChanged::ListeningState";
		break;
	default:
		msg += " -> StateChanged::UnknownState";
		break;
	}
	std::cout << msg.toStdString() << std::endl;
}

void raaOctaveToolInterface::udpRead(raaNet::raaTcpMsg*)
{
}

void raaOctaveToolInterface::udpState(raaNet::raaTcpThread*, unsigned)
{
}

raaOctaveToolInterface::~raaOctaveToolInterface()
{
	if(m_pNetwork)
	{
		m_pNetwork->closeTcpConnection("raaOctaveTool");
		delete m_pNetwork;
	}
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
	osg::Matrixf mRot, mTrans, m; 

	mRot.set(m_avPhysical[csm_uiRight][0], m_avPhysical[csm_uiRight][1], m_avPhysical[csm_uiRight][2], 0.0f,m_avPhysical[csm_uiDir][0], m_avPhysical[csm_uiDir][1], m_avPhysical[csm_uiDir][2], 0.0f,m_avPhysical[csm_uiUp][0], m_avPhysical[csm_uiUp][1], m_avPhysical[csm_uiUp][2], 0.0f,0.0f, 0.0f, 0.0f, 1.0f);
	mTrans.makeTranslate(m_avPhysical[csm_uiPos]);
	m = mRot*mTrans;

	raaNet::raaTcpMsg *pM0 = new raaNet::raaTcpMsg(raaNet::csm_usTcpMsgInfo);
	pM0->add(raaOctaveKernel::csm_uiOCViewpointUpdatePhysical);
	pM0->add(m);
	if(m_pTcpClient) m_pTcpClient->write(pM0);

	mRot.set(m_avVirtual[csm_uiRight][0], m_avVirtual[csm_uiRight][1], m_avVirtual[csm_uiRight][2], 0.0f, m_avVirtual[csm_uiDir][0], m_avVirtual[csm_uiDir][1], m_avVirtual[csm_uiDir][2], 0.0f, m_avVirtual[csm_uiUp][0], m_avVirtual[csm_uiUp][1], m_avVirtual[csm_uiUp][2], 0.0f, 0.0f, 0.0f, 0.0f, 1.0f);
	mTrans.makeTranslate(m_avVirtual[csm_uiPos]);
	m = mRot*mTrans;

	raaNet::raaTcpMsg *pM1 = new raaNet::raaTcpMsg(raaNet::csm_usTcpMsgInfo);
	pM1->add(raaOctaveKernel::csm_uiOCViewpointUpdateVirtual);
	pM1->add(m);
	if (m_pTcpClient) m_pTcpClient->write(pM1);
}

