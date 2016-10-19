#include <iostream>
#include <raaOctaveKernel/raaOctaveKernel.h>
#include <raaNetwork/raaTcpMsg.h>

#include "raaOctaveSystem.h"
#include "raaOctaveSystem.moc"

raaOctaveSystem::raaOctaveSystem()
{
	m_iScreen = 0;
	m_pScene = new osg::Group();
	m_pScene->ref();


	m_pNetwork = new raaNet::raaNetwork(0, this);
	connect(m_pNetwork, SIGNAL(tcpRead(raaTcpMsg*)), SLOT(tcpRead(raaTcpMsg*)));
	connect(m_pNetwork, SIGNAL(tcpState(raaTcpThread*, unsigned int)), SLOT(tcpState(raaTcpThread*, unsigned int)));
	m_pNetwork->createTcpClient("raaOctaveSystem", "localhost", 65204);


	m_pTimer = new QTimer(this);
	connect(m_pTimer, SIGNAL(timeout()), SLOT(timerUpdate()));
	m_pTimer->start(30);
//	system.run();

	realize();
//	addDisplay(0, "test", 0, 0, 200, 200);
}

raaOctaveSystem::~raaOctaveSystem()
{
	if (m_pScene) m_pScene->unref();
}

void raaOctaveSystem::addSceneData(osg::Node* pNode)
{
	m_pScene->addChild(pNode);
}

void raaOctaveSystem::addDisplay(int iScreen, std::string sName, int iX, int iY, int iW, int iH)
{
	osg::GraphicsContext::Traits *pTraits = new osg::GraphicsContext::Traits();
	pTraits->screenNum=iScreen;
	pTraits->x = iX;
	pTraits->y = iY;
	pTraits->width = iW;
	pTraits->height = iH;
	pTraits->windowDecoration = true;
	pTraits->doubleBuffer = true;
	pTraits->sharedContext = getNumViews()? pTraits->sharedContext=getView(0)->getCamera()->getGraphicsContext():0;

	osg::GraphicsContext *pGC = osg::GraphicsContext::createGraphicsContext(pTraits);
	osgViewer::View *pView = new osgViewer::View;
	m_mViews[sName] = pView;
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

void raaOctaveSystem::tcpRead(raaNet::raaTcpMsg* pMsg)
{
	if (pMsg)
	{
		switch (pMsg->msgType())
		{
		case raaNet::csm_usTcpMsgReply:
		{
			if (pMsg->asUInt(2) == raaOctaveKernel::csm_uiOCHasConfigTrue)
			{
				std::cout << "Read Reply -> raaOctaveKernel::csm_uiOCHasConfigTrue" << std::endl;
				raaNet::raaTcpMsg *pM = new raaNet::raaTcpMsg(raaNet::csm_usTcpMsgRequest);
				pM->add(raaOctaveKernel::csm_uiOCControllerRequestScreenAll);
				m_pTcpClient->write(pM);
			}
			else if (pMsg->asUInt(2) == raaOctaveKernel::csm_uiOCHasConfigFalse)
			{
				std::cout << "Read Reply -> raaOctaveKernel::csm_uiOCHasConfigFalse" << std::endl;
				raaNet::raaTcpMsg *pM = new raaNet::raaTcpMsg(raaNet::csm_usTcpMsgRequest);
				pM->add(raaOctaveKernel::csm_uiOCLoadConfig);
				pM->add(std::string("C:\\robbie\\data\\octave_config.raa"));
				m_pTcpClient->write(pM);
			}
		}
		break;
		case raaNet::csm_usTcpMsgInfo:
		{
			switch (pMsg->asUInt(2))
			{
			case raaOctaveKernel::csm_uiOCControllerScreenAdded:
			{
				//std::cout << "Read Info -> raaOctaveKernel::csm_uiOCControllerScreenAdded" << std::endl;

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
				osg::Matrixf mPersp = pMsg->asMatrix(15);

				addDisplay(0, sName, 0, 0, 200, 200);
				realize();

//				Sleep(100);

				raaNet::raaTcpMsg *pM = new raaNet::raaTcpMsg(raaNet::csm_usTcpMsgRequest);
				pM->add(raaOctaveKernel::csm_uiOCWindowInfo);
				pM->add(sName);
				m_pTcpClient->write(pM);

//				addDisplay(0, "test", 0, 0, 200, 200);

			}
			break;
			case raaOctaveKernel::csm_uiOCControllerScreenRemoved:
			{
				//std::cout << "Read Info -> raaOctaveKernel::csm_uiOCControllerScreenRemoved" << std::endl;
				std::string sName = pMsg->asString(3);
			}
			break;
			case raaOctaveKernel::csm_uiOCViewpointPhysicalChanged:
			{
				//std::cout << "Read Info -> raaOctaveKernel::csm_uiOCViewpointPhysicalChanged" << std::endl;
				osg::Matrixf m = pMsg->asMatrix(3);
//				if (m_pPhysicalViewpoint)m_pPhysicalViewpoint->setMatrix(m);
			}
			break;
			case raaOctaveKernel::csm_uiOCViewpointVirtualChanged:
			{
				//std::cout << "Read Info -> raaOctaveKernel::csm_uiOCViewpointVirtualChanged" << std::endl;
				osg::Matrixf m = pMsg->asMatrix(3);

//				for (raaDisplayScreens::iterator it = m_mDisplays.begin(); it != m_mDisplays.end(); it++)
//					it->second->setViewMatrix(m);
			}
			break;
			case raaOctaveKernel::csm_uiOCScreenMatrixChanged:
			{
				//std::cout << "Read Info -> raaOctaveKernel::csm_uiOCScreenMatrixChanged" << std::endl;
				std::string sName = pMsg->asString(3);
				osg::Matrixf m = pMsg->asMatrix(4);

//				if (sName.length() && m_mDisplays.find(sName) != m_mDisplays.end()) m_mDisplays[sName]->screenMatrixChanged(m);
			}
			break;
			case raaOctaveKernel::csm_uiOCScreenChanged:
			{
				//std::cout << "Read Info -> raaOctaveKernel::csm_uiOCScreenChanged" << std::endl;
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
			}
			break;
			case raaOctaveKernel::csm_uiOCScreenInfo:
			{
				//std::cout << "Read Info -> raaOctaveKernel::csm_uiOCScreenInfo" << std::endl;
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
			}
			break;
			case raaOctaveKernel::csm_uiOCWindowInfo:
			{
				//std::cout << "Read Info -> raaOctaveKernel::csm_uiOCWindowInfo" << std::endl;
				std::string sName = pMsg->asString(3);
				int iX = pMsg->asInt(4);
				int iY = pMsg->asInt(5);
				int iW = pMsg->asInt(6);
				int iH = pMsg->asInt(7);

				osgViewer::ViewerBase::Windows wins;
				this->getWindows(wins);


				if(m_mViews.find(sName)!=m_mViews.end())
				{
					osgViewer::GraphicsWindow *pWindow = dynamic_cast<osgViewer::GraphicsWindow*>(m_mViews[sName]->getCamera()->getGraphicsContext());
					if(pWindow) pWindow->setWindowRectangle(iX, iY, iW, iH);
				}
				
/*
				m_mWindows[sName].sName = sName;
				m_mWindows[sName].m_aiParam[0] = iX;
				m_mWindows[sName].m_aiParam[1] = iY;
				m_mWindows[sName].m_aiParam[2] = iW;
				m_mWindows[sName].m_aiParam[3] = iH;

				m_mWindows[sName].m_pItem->setRect(iX, iY, iW, iH);

				bool bMode = m_bWindowUpdate;
				m_bWindowUpdate = false;

				window_pos_x_spin->setValue(iX);
				window_pos_y_spin->setValue(iY);
				window_width_spin->setValue(iW);
				window_height_spin->setValue(iH);

				m_bWindowUpdate = bMode;
*/
			}
			break;
			}
		}
		}
	}
}

void raaOctaveSystem::tcpState(raaNet::raaTcpThread* pThread, unsigned uiState)
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
		if (pThread->name() == "raaOctaveSystem")
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
		if (pThread->name() == "raaOctaveSystem")
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

void raaOctaveSystem::udpRead(raaNet::raaTcpMsg*)
{
}

void raaOctaveSystem::udpState(raaNet::raaTcpThread*, unsigned)
{
}

void raaOctaveSystem::timerUpdate()
{
	advance(0.1f);

	eventTraversal();
	updateTraversal();
	renderingTraversals();

}
