#include "stdafx.h"
#include <windows.h>

#include <osgDB/ReadFile>
#include <osg/Drawable>
#include <osg/ShapeDrawable>

#include <QtWidgets/QGraphicsRectItem>

//#include <raaOctaveController/raaOctaveController.h>
#include <raaNetwork/raaTcpMsg.h>
#include <raaNetwork/raaTcpThread.h>

#include <raaOctaveKernel/raaOctaveKernel.h>
#include <osg/MatrixTransform>
#include "raaDisplayScreen.h"
#include "raaTracker.h"
#include "raaOctaveToolInterface.h"
#include "raaOctaveToolInterface.moc"




raaOctaveToolInterface::raaOctaveToolInterface(std::string sConfig, std::string sName, std::string sIp, unsigned short int usiPort)
{
	m_uiMode = csm_uiTransform;
	m_sConfig = sConfig;
	m_pUdpClient= 0;
	m_pTcpClient = 0;
	m_bScreenUpdate = true;
	m_bWindowUpdate = true;
	m_bTrackerUpdate = true;
	m_bLockCamera = false;
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
	graphicsView->setScene(&m_Scene);

	gl_widget->addToScene(0, makeGrid(10.0f, 10.0f, 10, 10));
	m_pVirtualScene = new osg::MatrixTransform();
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

	m_sName=sName;
	m_pNetwork = new raaNet::raaNetwork(0, this);
	connect(m_pNetwork, SIGNAL(tcpRead(raaTcpMsg*)), SLOT(tcpRead(raaTcpMsg*)));
	connect(m_pNetwork, SIGNAL(tcpState(raaTcpThread*, unsigned int)), SLOT(tcpState(raaTcpThread*, unsigned int)));
	m_pTcpClient=m_pNetwork->createTcpClient(sName.c_str(), sIp.c_str(), usiPort);

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


	connect(tracker_combo, SIGNAL(currentIndexChanged(const QString&)), SLOT(trackerChanged(const QString&)));
	connect(tracker_update_check, SIGNAL(stateChanged(int)), SLOT(trackerContUpdate(int)));
	connect(tracker_update_button, SIGNAL(pressed()), SLOT(trackerUpdate()));
	connect(tracker_position_x_spin, SIGNAL(valueChanged(double)), SLOT(trackerTransformChanged(double)));
	connect(tracker_position_y_spin, SIGNAL(valueChanged(double)), SLOT(trackerTransformChanged(double)));
	connect(tracker_position_z_spin, SIGNAL(valueChanged(double)), SLOT(trackerTransformChanged(double)));
	connect(tracker_direction_x_spin, SIGNAL(valueChanged(double)), SLOT(trackerTransformChanged(double)));
	connect(tracker_direction_y_spin, SIGNAL(valueChanged(double)), SLOT(trackerTransformChanged(double)));
	connect(tracker_direction_z_spin, SIGNAL(valueChanged(double)), SLOT(trackerTransformChanged(double)));
	connect(tracker_up_x_spin, SIGNAL(valueChanged(double)), SLOT(trackerTransformChanged(double)));
	connect(tracker_up_y_spin, SIGNAL(valueChanged(double)), SLOT(trackerTransformChanged(double)));
	connect(tracker_up_z_spin, SIGNAL(valueChanged(double)), SLOT(trackerTransformChanged(double)));

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

	connect(window_combo, SIGNAL(currentIndexChanged(const QString&)), SLOT(currentWindowChanged(const QString&)));
	connect(window_update_button, SIGNAL(pressed()), SLOT(windowUpdate()));
	connect(window_continual_check, SIGNAL(stateChanged(int)), SLOT(windowUpdateMode(int)));
	connect(window_pos_x_spin, SIGNAL(valueChanged(int)), SLOT(windowParam(int)));
	connect(window_pos_y_spin, SIGNAL(valueChanged(int)), SLOT(windowParam(int)));
	connect(window_width_spin, SIGNAL(valueChanged(int)), SLOT(windowParam(int)));
	connect(window_height_spin, SIGNAL(valueChanged(int)), SLOT(windowParam(int)));

	connect(screen_add_button, SIGNAL(pressed()), SLOT(screenAdd()));
	connect(config_save_button, SIGNAL(pressed()), SLOT(configSave()));
	connect(config_list_widget, SIGNAL(itemSelectionChanged()), SLOT(configCurrentChanged()));

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

	m_abVirtual[csm_uiRotate] = !m_abVirtual[csm_uiTranslate];
}

void raaOctaveToolInterface::virRot(bool bVal)
{
	if (bVal) m_abVirtual[csm_uiRotate] = true;
	else m_abVirtual[csm_uiRotate] = false;

	m_abVirtual[csm_uiTranslate] = !m_abVirtual[csm_uiRotate];
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
		gl_widget->getView(0)->getCameraManipulator()->setByMatrix(m_pPhysicalViewpoint->getMatrix());
		m_bLockCamera = true;
//		updateView();
	}
	else
	{
		gl_widget->getView(0)->getCameraManipulator()->setByMatrix(m_CameraManipulatorMatrix);
		m_bLockCamera = false;
	}
}

void raaOctaveToolInterface::currentToolboxChanged(int iVal)
{

	if (iVal == 0)
	{
		m_uiMode = csm_uiTransform;
		stackedWidget->setCurrentIndex(0);

	}
	else if (iVal == 1)
	{
		m_uiMode = csm_uiScreen;
		raaNet::raaTcpMsg *pMsg = new raaNet::raaTcpMsg(raaNet::csm_usTcpMsgRequest);
		pMsg->add(raaOctaveKernel::csm_uiOCControllerRequestScreenNames);
		if (m_pTcpClient) m_pTcpClient->write(pMsg);
		stackedWidget->setCurrentIndex(0);
	}
	else if (iVal == 2)
	{
		m_uiMode = csm_uiWindow;
		stackedWidget->setCurrentIndex(1);
		raaNet::raaTcpMsg *pMsg1 = new raaNet::raaTcpMsg(raaNet::csm_usTcpMsgRequest);
		pMsg1->add(raaOctaveKernel::csm_uiOCControllerRequestScreenNames);
		raaNet::raaTcpMsg *pMsg2 = new raaNet::raaTcpMsg(raaNet::csm_usTcpMsgRequest);
		pMsg2->add(raaOctaveKernel::csm_uiOCDisplays); 
		if (m_pTcpClient)
		{
			m_pTcpClient->write(pMsg2);
			m_pTcpClient->write(pMsg1);
		}
	}
	else if (iVal == 3)
	{
		m_uiMode = csm_uiScreen;
		raaNet::raaTcpMsg *pMsg = new raaNet::raaTcpMsg(raaNet::csm_usTcpMsgRequest);
		pMsg->add(raaOctaveKernel::csm_uiOCTrackerAllNames);
		if (m_pTcpClient) m_pTcpClient->write(pMsg);
		stackedWidget->setCurrentIndex(0);
	}
	else if (iVal == 4)
	{
		m_uiMode = csm_uiScreen;
		raaNet::raaTcpMsg *pMsg = new raaNet::raaTcpMsg(raaNet::csm_usTcpMsgRequest);
		pMsg->add(raaOctaveKernel::csm_uiOCListConfigs);
		if (m_pTcpClient) m_pTcpClient->write(pMsg);
		stackedWidget->setCurrentIndex(0);
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

void raaOctaveToolInterface::screenAdd()
{
	if (m_pTcpClient)
	{
		raaTcpMsg *pMsg = new raaTcpMsg(raaNet::csm_usTcpMsgRequest);
		pMsg->add(raaOctaveKernel::csm_uiOCScreenAdd);
		pMsg->add(screen_add_edit->text().toStdString());

		m_pTcpClient->write(pMsg);
	}

}

void raaOctaveToolInterface::configSave()
{
	if (m_pTcpClient)
	{
		raaTcpMsg *pMsg = new raaTcpMsg(raaNet::csm_usTcpMsgRequest);
		pMsg->add(raaOctaveKernel::csm_uiOCSaveConfigAs);
		pMsg->add(config_name_edit->text().toStdString());

		m_pTcpClient->write(pMsg);
	}
}

void raaOctaveToolInterface::configCurrentChanged()
{
	QList<QListWidgetItem*> items = config_list_widget->selectedItems();

	if (items.length())
	{
		for (QList<QListWidgetItem*>::iterator it = items.begin(); it != items.end(); it++)
			config_name_edit->setText((*it)->text());
	}
	else
		config_name_edit->clear();
}

void raaOctaveToolInterface::trackerContUpdate(int iVal)
{
	if (iVal == Qt::Checked)
		m_bTrackerUpdate = true;
	else
		m_bTrackerUpdate = false;
}

void raaOctaveToolInterface::trackerChanged(const QString& s)
{
	if(s.length())
	{
		raaTcpMsg *pMsg = new raaNet::raaTcpMsg(raaNet::csm_usTcpMsgRequest);
		pMsg->add(raaOctaveKernel::csm_uiOCTrackerOriginTransform);
		pMsg->add(s.toStdString());
		m_pTcpClient->write(pMsg);
	}
}

void raaOctaveToolInterface::trackerUpdate()
{
	osg::Vec3f vPos, vDir, vUp, vRight;

	vPos[0] = (float)tracker_position_x_spin->value();
	vPos[1] = (float)tracker_position_y_spin->value();
	vPos[2] = (float)tracker_position_z_spin->value();
	vDir[0] = (float)tracker_direction_x_spin->value();
	vDir[1] = (float)tracker_direction_y_spin->value();
	vDir[2] = (float)tracker_direction_z_spin->value();
	vUp[0] = (float)tracker_up_x_spin->value();
	vUp[1] = (float)tracker_up_y_spin->value();
	vUp[2] = (float)tracker_up_z_spin->value();
	vRight[0] = tracker_right_x_label->text().toFloat();
	vRight[1] = tracker_right_x_label->text().toFloat();
	vRight[2] = tracker_right_x_label->text().toFloat();

	if (sender() == tracker_direction_x_spin || sender() == tracker_direction_y_spin || sender() == tracker_direction_z_spin)
	{
		tracker_direction_x_spin->blockSignals(true);
		tracker_direction_y_spin->blockSignals(true);
		tracker_direction_z_spin->blockSignals(true);

		vDir.normalize();

		tracker_direction_x_spin->setValue(vDir[0]);
		tracker_direction_y_spin->setValue(vDir[1]);
		tracker_direction_z_spin->setValue(vDir[2]);
		
		tracker_direction_x_spin->blockSignals(false);
		tracker_direction_y_spin->blockSignals(false);
		tracker_direction_z_spin->blockSignals(false);

		if (sender() == tracker_direction_y_spin)
		{
			tracker_up_x_spin->blockSignals(true);
			tracker_up_y_spin->blockSignals(true);
			tracker_up_z_spin->blockSignals(true);

			vUp = vRight^vDir;
			vUp.normalize();

			tracker_up_x_spin->setValue(vUp[0]);
			tracker_up_y_spin->setValue(vUp[1]);
			tracker_up_z_spin->setValue(vUp[2]);

			tracker_up_x_spin->blockSignals(false);
			tracker_up_y_spin->blockSignals(false);
			tracker_up_z_spin->blockSignals(false);
		}

		vRight = vDir^vUp;
		vRight.normalize();

		tracker_right_x_label->setText(QString::number(vRight[0]));
		tracker_right_y_label->setText(QString::number(vRight[1]));
		tracker_right_z_label->setText(QString::number(vRight[2]));
	}

	if (sender() == tracker_up_x_spin || sender() == tracker_up_y_spin || sender() == tracker_up_z_spin)
	{
		tracker_up_x_spin->blockSignals(true);
		tracker_up_y_spin->blockSignals(true);
		tracker_up_z_spin->blockSignals(true);

		vUp.normalize();

		tracker_up_x_spin->setValue(vUp[0]);
		tracker_up_y_spin->setValue(vUp[1]);
		tracker_up_z_spin->setValue(vUp[2]);

		tracker_up_x_spin->blockSignals(false);
		tracker_up_y_spin->blockSignals(false);
		tracker_up_z_spin->blockSignals(false);

		if (sender() == tracker_up_y_spin)
		{
			tracker_direction_x_spin->blockSignals(true);
			tracker_direction_y_spin->blockSignals(true);
			tracker_direction_z_spin->blockSignals(true);

			vDir= vUp^vRight;
			vDir.normalize();

			tracker_direction_x_spin->setValue(vDir[0]);
			tracker_direction_y_spin->setValue(vDir[1]);
			tracker_direction_z_spin->setValue(vDir[2]);

			tracker_direction_x_spin->blockSignals(false);
			tracker_direction_y_spin->blockSignals(false);
			tracker_direction_z_spin->blockSignals(false);
		}
		vRight = vDir^vUp;
		vRight.normalize();

		tracker_right_x_label->setText(QString::number(vRight[0]));
		tracker_right_y_label->setText(QString::number(vRight[1]));
		tracker_right_z_label->setText(QString::number(vRight[2]));
	}

	
	raaTcpMsg *pMsg = new raaNet::raaTcpMsg(raaNet::csm_usTcpMsgInfo);
	pMsg->add(raaOctaveKernel::csm_uiOCTrackerOriginTransform);
	pMsg->add(tracker_combo->currentText().toStdString());
	pMsg->add(vPos);
	pMsg->add(vDir);
	pMsg->add(vUp);
	m_pTcpClient->write(pMsg);
}

void raaOctaveToolInterface::trackerTransformChanged(double)
{

	if (m_bTrackerUpdate)
	{
		trackerUpdate();
	}
}


void raaOctaveToolInterface::windowUpdate()
{
	if(m_sCurrentWindow.length())
	{
		raaNet::raaTcpMsg *pMsg = new raaNet::raaTcpMsg(raaNet::csm_usTcpMsgInfo);
		pMsg->add(raaOctaveKernel::csm_uiOCWindowInfo);
		pMsg->add(m_sCurrentWindow);
		pMsg->add(window_pos_x_spin->value());
		pMsg->add(window_pos_y_spin->value());
		pMsg->add(window_width_spin->value());
		pMsg->add(window_height_spin->value());
		if (m_pTcpClient) m_pTcpClient->write(pMsg);
	}
}

void raaOctaveToolInterface::windowUpdateMode(int iVal)
{
	if (iVal == Qt::Checked)
		m_bWindowUpdate = true;
	else
		m_bWindowUpdate = false;
}

void raaOctaveToolInterface::windowParam(int)
{
	if(m_bWindowUpdate) windowUpdate();
}

void raaOctaveToolInterface::currentWindowChanged(const QString& s)
{
	m_sCurrentWindow = s.toStdString();

	if (m_sCurrentWindow.length() && m_mWindows.find(m_sCurrentWindow) != m_mWindows.end())
	{
		raaNet::raaTcpMsg *pMsg = new raaNet::raaTcpMsg(raaNet::csm_usTcpMsgRequest);
		pMsg->add(raaOctaveKernel::csm_uiOCWindowInfo);
		pMsg->add(m_sCurrentWindow);
		if (m_pTcpClient) m_pTcpClient->write(pMsg);
	}
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
					pM->add(m_sConfig);
					m_pTcpClient->write(pM);
					updateView();
				}
			}
			break;
			case raaNet::csm_usTcpMsgInfo:
			{
				switch(pMsg->asUInt(2))
				{
					case raaOctaveKernel::csm_uiOCDisplayChanged:
					{
						std::string sName = pMsg->asString(3);
						int iScreen = pMsg->asInt(4);

						if(m_mDisplays.find(sName)!=m_mDisplays.end())
						{
							m_mDisplays[sName]->setDisplayScreen(iScreen);
						}
					}
					break;
					case raaOctaveKernel::csm_uiOCDisplays:
					{
						for(raaDisplayPanelMap::iterator it=m_mDisplayPanels.begin();it!=m_mDisplayPanels.end();it++)
						{
							m_Scene.removeItem(it->second.m_pItem);
							delete it->second.m_pItem;
							m_Scene.removeItem(it->second.m_pText);
							delete it->second.m_pText;
						}

						m_mDisplayPanels.clear();

						unsigned int iCount = pMsg->asUInt(3);
						int iCurrentX = 0;

						for(unsigned int i=0;i<iCount;i++)
						{
							int iScreen = pMsg->asInt(i * 3 + 4);
							m_mDisplayPanels[iScreen].m_iScreen = iScreen;
							m_mDisplayPanels[iScreen].m_iWidth = pMsg->asInt(i * 3 + 5);
							m_mDisplayPanels[iScreen].m_iHeight = pMsg->asInt(i * 3 + 6);

							m_mDisplayPanels[iScreen].m_iX = iCurrentX;
							iCurrentX += m_mDisplayPanels[iScreen].m_iWidth + 40;
							m_mDisplayPanels[iScreen].m_iY = 0;


							m_mDisplayPanels[iScreen].m_pItem = m_Scene.addRect(m_mDisplayPanels[iScreen].m_iX / csm_iWindowScale, m_mDisplayPanels[iScreen].m_iY / csm_iWindowScale, m_mDisplayPanels[iScreen].m_iWidth / csm_iWindowScale, m_mDisplayPanels[iScreen].m_iHeight / csm_iWindowScale, QPen(), QBrush(QColor(10, 150, 250, 127), Qt::SolidPattern));
							m_mDisplayPanels[iScreen].m_pText = m_Scene.addText(QString("Display Screen::%1").arg(iScreen));

							m_mDisplayPanels[iScreen].m_pText->setPos(QPoint(m_mDisplayPanels[iScreen].m_iX / csm_iWindowScale, (m_mDisplayPanels[iScreen].m_iY / csm_iWindowScale) - 40));

						}
					}
					break;
					case raaOctaveKernel::csm_uiOCListConfigs:
					{
						try
						{
							config_list_widget->clear();

							int iNum = pMsg->asInt(3);

							if(iNum)
							{
								for(int i=0;i<iNum;i++)
								{
									config_list_widget->addItem(pMsg->asString(i + 4).c_str());
								}
							}
						
						}
						catch (unsigned int e)
						{
							if (e == raaNet::raaMsg::csm_uiMsgBadIndex) std::cout << "raaOctaveKernel::csm_uiOCListConfigs -> Bad Read Index" << std::endl;
						}

					}
					break;
					case raaOctaveKernel::csm_uiOCControllerScreenAdded:
					{
						try
						{
							std::string sName = pMsg->asString(3);
							int iScreen = pMsg->asInt(4);
							osg::Vec3f vbl = pMsg->asVector(5);
							osg::Vec3f vbr = pMsg->asVector(6);
							osg::Vec3f vtl = pMsg->asVector(7);
							osg::Vec3f vtr = pMsg->asVector(8);
							osg::Vec3f vn = pMsg->asVector(9);
							float fN = pMsg->asFloat(10);
							float fF = pMsg->asFloat(11);
							float fR = pMsg->asFloat(12);
							bool bX = pMsg->asBool(13);
							bool bY = pMsg->asBool(14);
							bool bZ = pMsg->asBool(15);
							osg::Matrixf mPersp = pMsg->asMatrix(16);
							osg::Matrixf mView = pMsg->asMatrix(17);

							m_mDisplays[sName] = new raaDisplayScreen(m_pVirtualScene, sName, vbl, vbr, vtl, vtr, vn, mPersp);
							m_mDisplays[sName]->setViewMatrix(mView);

							gl_widget->addToTranspScene(0, m_mDisplays[sName]->root());

							m_mWindows[sName].sName = sName;
							m_mWindows[sName].m_aiParam[0] = 0;
							m_mWindows[sName].m_aiParam[1] = 0;
							m_mWindows[sName].m_aiParam[2] = 200;
							m_mWindows[sName].m_aiParam[3] = 200;
							m_mWindows[sName].m_pItem = m_Scene.addRect((m_mWindows[sName].m_aiParam[0]+m_mDisplayPanels[iScreen].m_iX)/ csm_iWindowScale, (m_mWindows[sName].m_aiParam[1] + m_mDisplayPanels[iScreen].m_iY)/ csm_iWindowScale, m_mWindows[sName].m_aiParam[2]/ csm_iWindowScale, m_mWindows[sName].m_aiParam[3]/ csm_iWindowScale, QPen(), QBrush(QColor(250, 150, 10, 100), Qt::SolidPattern));
							m_mWindows[sName].m_pText = m_Scene.addText(QString("Screen::%0").arg(sName.c_str()));
							m_mWindows[sName].m_pText->setPos(QPoint(((m_mWindows[sName].m_aiParam[0] + m_mDisplayPanels[iScreen].m_iX) / csm_iWindowScale)+20,
								((m_mWindows[sName].m_aiParam[1] + m_mDisplayPanels[iScreen].m_iY) / csm_iWindowScale)+35));
							


							raaNet::raaTcpMsg *pM = new raaNet::raaTcpMsg(raaNet::csm_usTcpMsgRequest);
							pM->add(raaOctaveKernel::csm_uiOCWindowInfo);
							pM->add(sName);
							m_pTcpClient->write(pM);
						}
						catch(unsigned int e)
						{
							if (e == raaNet::raaMsg::csm_uiMsgBadIndex) std::cout << "raaOctaveKernel::csm_uiOCControllerScreenAdded -> Bad Read Index" << std::endl;
						}
					}
					break;
					case raaOctaveKernel::csm_uiOCControllerScreenRemoved:
					{
					}
					break;
					case raaOctaveKernel::csm_uiOCViewpointPhysicalChanged:
					{
						try
						{
							osg::Matrixf m = pMsg->asMatrix(3);
							if (m_pPhysicalViewpoint)m_pPhysicalViewpoint->setMatrix(m);
						}
						catch(unsigned int e)
						{
							if (e == raaNet::raaMsg::csm_uiMsgBadIndex) std::cout << "raaOctaveKernel::csm_uiOCViewpointPhysicalChanged -> bad read index" << std::endl;
						}
					}
					break;
					case raaOctaveKernel::csm_uiOCViewpointVirtualChanged:
					{
						try
						{
							osg::Matrixf m;
							m.invert_4x4(pMsg->asMatrix(3));
							if (m_pVirtualScene)m_pVirtualScene->setMatrix(m);
						}
						catch (unsigned int e)
						{
							if (e == raaNet::raaMsg::csm_uiMsgBadIndex) std::cout << "raaOctaveKernel::csm_uiOCViewpointVirtualChanged -> bad read index" << std::endl;
						}
					}
					break;
					case raaOctaveKernel::csm_uiOCScreenMatrixChanged:
					{
						try
						{
							std::string sName = pMsg->asString(3);

							if(pMsg->asInt(4))
							{
								osg::Matrixf mLeftPersp = pMsg->asMatrix(5);
								osg::Matrixf mRightPersp = pMsg->asMatrix(6);
								osg::Matrixf mLeftView = pMsg->asMatrix(7);
								osg::Matrixf mRightView = pMsg->asMatrix(8);

								if (sName.length() && m_mDisplays.find(sName) != m_mDisplays.end())
								{
									m_mDisplays[sName]->screenStereoMatrixChanged(mLeftPersp, mRightPersp);
									m_mDisplays[sName]->setStereoViewMatrix(mLeftView, mRightView);
									if (m_bLockCamera) gl_widget->getView(0)->getCameraManipulator()->setByMatrix(osg::Matrix::inverse(mRightView));
								}

							}
							else
							{
								osg::Matrixf mPersp = pMsg->asMatrix(5);
								osg::Matrixf mView = pMsg->asMatrix(6);

								if (sName.length() && m_mDisplays.find(sName) != m_mDisplays.end())
								{
									m_mDisplays[sName]->screenMatrixChanged(mPersp);
									m_mDisplays[sName]->setViewMatrix(mView);
									if (m_bLockCamera) gl_widget->getView(0)->getCameraManipulator()->setByMatrix(osg::Matrix::inverse(mView));
								}
							}
						}
						catch (unsigned int e)
						{
							if (e == raaNet::raaMsg::csm_uiMsgBadIndex) std::cout << "raaOctaveKernel::csm_uiOCScreenMatrixChanged -> bad read index" << std::endl;
						}

					}
					break;
					case raaOctaveKernel::csm_uiOCScreenChanged:
					{
						try
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
						catch (unsigned int e)
						{
							if (e == raaNet::raaMsg::csm_uiMsgBadIndex) std::cout << "raaOctaveKernel::csm_uiOCScreenChanged -> bad read index" << std::endl;
						}
					}
					break;
					case raaOctaveKernel::csm_uiOCScreenInfo:
					{
						try
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
						catch (unsigned int e)
						{
							if (e == raaNet::raaMsg::csm_uiMsgBadIndex) std::cout << "raaOctaveKernel::csm_uiOCScreenInfo -> bad read index" << std::endl;
						}

					}
					break;
					case raaOctaveKernel::csm_uiOCControllerRequestScreenNames:
					{
						try
						{
							switch (m_uiMode)
							{
							case csm_uiScreen:
							{
								screen_combo->clear();
								unsigned int uiNum = pMsg->asUInt(3);
								for (unsigned int i = 0; i < uiNum; i++) screen_combo->addItem(pMsg->asString(4 + i).c_str());
							}
							break;
							case csm_uiWindow:
							{
								window_combo->clear();
								unsigned int uiNum = pMsg->asUInt(3);
								for (unsigned int i = 0; i < uiNum; i++) window_combo->addItem(pMsg->asString(4 + i).c_str());
							}
							break;
							}
						}
						catch (unsigned int e)
						{
							if (e == raaNet::raaMsg::csm_uiMsgBadIndex) std::cout << "raaOctaveKernel::csm_uiOCControllerRequestScreenNames -> bad read index" << std::endl;
						}
					}
					break;
					case raaOctaveKernel::csm_uiOCWindowInfo:
					{
						try
						{
							std::string sName = pMsg->asString(3);
							int iScreen = pMsg->asInt(4);
							int iX = pMsg->asInt(5);
							int iY = pMsg->asInt(6);
							int iW = pMsg->asInt(7);
							int iH = pMsg->asInt(8);

//							std::cout << "Window -> " << sName << "   " << iScreen << std::endl;

							m_mWindows[sName].sName = sName;
							m_mWindows[sName].m_aiParam[0] = iX;
							m_mWindows[sName].m_aiParam[1] = iY;
							m_mWindows[sName].m_aiParam[2] = iW;
							m_mWindows[sName].m_aiParam[3] = iH; 

							m_mWindows[sName].m_pItem->setRect((iX+m_mDisplayPanels[iScreen].m_iX)/ csm_iWindowScale, (iY+m_mDisplayPanels[iScreen].m_iY)/ csm_iWindowScale, iW/csm_iWindowScale, iH/csm_iWindowScale);
							m_mWindows[sName].m_pText->setPos(QPoint(((m_mWindows[sName].m_aiParam[0] + m_mDisplayPanels[iScreen].m_iX) / csm_iWindowScale) + 20,
								((m_mWindows[sName].m_aiParam[1] + m_mDisplayPanels[iScreen].m_iY) / csm_iWindowScale) + 35));

							if (m_sCurrentWindow == sName)
							{
								bool bMode = m_bWindowUpdate;
								m_bWindowUpdate = false;
								window_pos_x_spin->setValue(iX);
								window_pos_y_spin->setValue(iY);
								window_width_spin->setValue(iW);
								window_height_spin->setValue(iH);
								m_bWindowUpdate = bMode;
							}
						}
						catch (unsigned int e)
						{
							if (e == raaNet::raaMsg::csm_uiMsgBadIndex) std::cout << "raaOctaveKernel::csm_uiOCWindowInfo -> bad read index" << std::endl;
						}
					}
					break;
					case raaOctaveKernel::csm_uiOCTrackerAllNames:
					{
						try
						{
							tracker_combo->clear();
							for (unsigned int i = 0; i < pMsg->asUInt(3); i++)
							{
								try
								{
									tracker_combo->addItem(pMsg->asString(i + 4).c_str());

									if (m_mTrackers.find(pMsg->asString(i + 4)) == m_mTrackers.end())
									{
										m_mTrackers[pMsg->asString(i + 4)] = new raaTracker(pMsg->asString(i + 4));
										gl_widget->addToScene(0,m_mTrackers[pMsg->asString(i + 4)]->root());
										raaTcpMsg *pM = new raaTcpMsg(raaNet::csm_usTcpMsgRequest);
										pM->add(raaOctaveKernel::csm_uiOCTrackerAddRemoveListener);
										pM->add(true);
										pM->add(pMsg->asString(i + 4));
										pMsg->tcpThread()->write(pM);
									}
								}
								catch (unsigned int e)
								{
									if (e == raaNet::raaMsg::csm_uiMsgBadIndex) std::cout << "raaOctaveKernel::csm_uiOCTrackerAllNames " << i << " -> bad read index" << std::endl;
								}
							}
						}
						catch (unsigned int e)
						{
							if (e == raaNet::raaMsg::csm_uiMsgBadIndex) std::cout << "raaOctaveKernel::csm_uiOCTrackerAllNames -> bad read index" << std::endl;
						}
					}
					break;
					case raaOctaveKernel::csm_uiOCTrackerInfo:
					{
						try
						{
							if(m_mTrackers.find(pMsg->asString(3))!=m_mTrackers.end())
							{
								raaTracker *pTracker = m_mTrackers[pMsg->asString(3)];
								pTracker->setOrigin(pMsg->asMatrix(4));
								unsigned int uiSensors = pMsg->asUInt(5);
								pTracker->setSensors(uiSensors);

								unsigned int iCount = 0;
								for(unsigned int i=0;i<32;i++, iCount++)
								{
									try
									{
										if (uiSensors & 1 << i) pTracker->setSensor(i, pMsg->asMatrix(6 + iCount));
									}
									catch (unsigned int e)
									{
										if (e == raaNet::raaMsg::csm_uiMsgBadIndex) std::cout << "raaOctaveKernel::csm_uiOCTrackerInfo " << i << " -> bad read index" << std::endl;
									}
								}
							}
						}
						catch (unsigned int e)
						{
							if (e == raaNet::raaMsg::csm_uiMsgBadIndex) std::cout << "raaOctaveKernel::csm_uiOCTrackerInfo -> bad read index" << std::endl;
						}
					}
					break;
					case raaOctaveKernel::csm_uiOCTrackerOriginTransform:
					{
						try
						{
							if (m_mTrackers.find(pMsg->asString(3)) != m_mTrackers.end() )
							{
								osg::Vec3f vPos, vDir, vUp, vRight;

								vPos = pMsg->asVector(4);
								vDir = pMsg->asVector(5);
								vUp = pMsg->asVector(6);
								vRight = vDir^vUp;
								vRight.normalize();

								osg::Matrixf m(vRight[0], vDir[0], vUp[0], 0.0f, vRight[1], vDir[1], vUp[1], 0.0f, vRight[2], vDir[2], vUp[2], 0.0f, vPos[0], vPos[2], vPos[2], 1.0f);

								m_mTrackers[pMsg->asString(3)]->setOrigin(m);

								if(tracker_combo->currentText() == pMsg->asString(3).c_str())
								{
									bool bUpdate = m_bTrackerUpdate;
									m_bTrackerUpdate = false;

									tracker_position_x_spin->setValue(vPos[0]);
									tracker_position_y_spin->setValue(vPos[1]);
									tracker_position_z_spin->setValue(vPos[2]);

									tracker_direction_x_spin->setValue(vDir[0]);
									tracker_direction_y_spin->setValue(vDir[1]);
									tracker_direction_z_spin->setValue(vDir[2]);

									tracker_up_x_spin->setValue(vUp[0]);
									tracker_up_y_spin->setValue(vUp[1]);
									tracker_up_z_spin->setValue(vUp[2]);
									
									tracker_right_x_label->setText(QString::number(vRight[0]));
									tracker_right_y_label->setText(QString::number(vRight[1]));
									tracker_right_z_label->setText(QString::number(vRight[2]));

									m_bTrackerUpdate = bUpdate;
								}
							}
						}
						catch (unsigned int e)
						{
							if (e == raaNet::raaMsg::csm_uiMsgBadIndex) std::cout << "raaOctaveKernel::csm_uiOCTrackerOriginTransform -> bad read index" << std::endl;
						}
					}
					break;
					case  raaOctaveKernel::csm_uiOCTrackerSensors:
					{
						try
						{
							if (m_mTrackers.find(pMsg->asString(3)) != m_mTrackers.end())
							{
								m_mTrackers[pMsg->asString(3)]->setSensors(pMsg->asUInt(4));
							}
						}
						catch (unsigned int e)
						{
							if (e == raaNet::raaMsg::csm_uiMsgBadIndex) std::cout << "raaOctaveKernel::csm_uiOCTrackerSensors -> bad read index" << std::endl;
						}
					}
					break;
					case  raaOctaveKernel::csm_uiOCTrackerSensorTimeout:
					{
						try
						{
							if (m_mTrackers.find(pMsg->asString(3)) != m_mTrackers.end())
							{
								unsigned int uiSensors = pMsg->asUInt(4);

								unsigned int uiCount = 5;
								for(unsigned int i=0;i<32;i++, uiCount++)
								{
									if(uiSensors & 1<<i)
									{
										osg::Matrixf m = pMsg->asMatrix(uiCount);
										m_mTrackers[pMsg->asString(3)]->setSensor(i, m);
									}
								}

								m_mTrackers[pMsg->asString(3)]->setSensors(pMsg->asUInt(4));
							}
						}
						catch (unsigned int e)
						{
							if (e == raaNet::raaMsg::csm_uiMsgBadIndex) std::cout << "raaOctaveKernel::csm_uiOCTrackerSensorTimeout -> bad read index" << std::endl;
						}
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
		if (pThread->name() == m_sName.c_str())
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
	//std::cout << msg.toStdString() << std::endl;
}

void raaOctaveToolInterface::udpRead(raaNet::raaTcpMsg*)
{
}

void raaOctaveToolInterface::udpState(raaNet::raaTcpThread*, unsigned)
{
}

raaOctaveToolInterface::~raaOctaveToolInterface()
{
	if(m_pNetwork && m_pTcpClient)
	{
//		m_pNetwork->closeTcpConnection("raaOctaveTool");
		m_pNetwork->closeTcpConnection(m_pTcpClient);
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
	osg::Vec4Array *pCols = new osg::Vec4Array();
	osg::Vec3 vNorm(0.0f, 0.0f, 1.0f);

	for(unsigned int i=0;i<=uiWidthSegs;i++, fW+=fWS)
	{
		pVerts->push_back(osg::Vec3f(fW, fD, 0.0f));
		pVerts->push_back(osg::Vec3f(fW, -fD, 0.0f));
		pNorms->push_back(vNorm);
		pNorms->push_back(vNorm);
		pCols->push_back(osg::Vec4f(1.0f, 0.0f, 0.0f, 1.0f));
		pCols->push_back(osg::Vec4f(1.0f, 0.0f, 0.0f, 1.0f));
	}

	for (unsigned int i = 0; i <= uiDepthSegs; i++, fD += fDS)
	{
		pVerts->push_back(osg::Vec3f(fWW, fD, 0.0f));
		pVerts->push_back(osg::Vec3f(-fWW, fD, 0.0f));
		pNorms->push_back(vNorm);
		pNorms->push_back(vNorm);
		pCols->push_back(osg::Vec4f(1.0f, 0.0f, 0.0f, 1.0f));
		pCols->push_back(osg::Vec4f(1.0f, 0.0f, 0.0f, 1.0f));
	}

	pGeom->getOrCreateStateSet()->setMode(GL_LIGHTING, osg::StateAttribute::OFF | osg::StateAttribute::OVERRIDE);
	pGeom->setVertexArray(pVerts);
	pGeom->setNormalArray(pNorms, osg::Array::BIND_PER_VERTEX);
	pGeom->setColorArray(pCols, osg::Array::BIND_PER_VERTEX);
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

