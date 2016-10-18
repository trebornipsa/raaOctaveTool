#include "stdafx.h"

#include <iostream>

#include <raaOctaveKernel/raaOctaveKernel.h>

#include <raaOctaveController/raaScreen.h>

#include "raaConnectionRecord.h"
#include "raaOctaveControl.h"
#include "raaOctaveControl.moc"

raaOctaveControl::raaOctaveControl()
{
	srand(0);
	m_iTimer = 0;
	m_pNetwork = 0;
	m_uiTcpCounter = 0;
	m_pController = new raaOctaveController(this);

	m_pNetwork = new raaNet::raaNetwork(65204, this);

	connect(m_pNetwork, SIGNAL(tcpRead(raaTcpMsg *)), SLOT(tcpRead(raaTcpMsg *)));
	connect(m_pNetwork, SIGNAL(tcpState(raaTcpThread*, unsigned int)), this, SLOT(tcpState(raaTcpThread*, unsigned int)));
	connect(m_pNetwork, SIGNAL(udpRead(raaUdpMsg *)), SLOT(udpRead(raaUdpMsg *)));
	connect(m_pNetwork, SIGNAL(udpState(raaUdpThread*, unsigned int)), this, SLOT(udpState(raaUdpThread*, unsigned int)));
}

raaOctaveControl::~raaOctaveControl()
{
	if (m_pNetwork) delete m_pNetwork;
}

void raaOctaveControl::tcpRead(raaTcpMsg* pMsg)
{
	std::cout << pMsg->tcpThread()->name().toStdString() << raaNet::tcpMsgTypeToString(pMsg->msgType()).toStdString() << pMsg->msgID() << QString(pMsg->data()).toStdString() << std::endl;

	switch(pMsg->msgType())
	{
		case raaNet::csm_usTcpMsgRequest:
		{
			switch(pMsg->asUInt(2))
			{
				case raaOctaveKernel::csm_uiOCHasConfig:
				{
					raaNet::raaTcpMsg *pM = new raaNet::raaTcpMsg(raaNet::csm_usTcpMsgReply);
					pM->add(m_pController->hasConfig() ? raaOctaveKernel::csm_uiOCHasConfigTrue : raaOctaveKernel::csm_uiOCHasConfigFalse);
					pMsg->tcpThread()->write(pM);
				}
				break;
				case raaOctaveKernel::csm_uiOCLoadConfig:
				{
	//				std::cout << "Load Config -> " << pMsg->asString(3).c_str() << std::endl;
					m_pController->readConfig(pMsg->asString(3).c_str());
				}
				break;
				case raaOctaveKernel::csm_uiOCAttachControllerListener:
				{
					m_mConnections[pMsg->tcpThread()]->setControllerListener(true);
				}
				break;
				case raaOctaveKernel::csm_uiOCDetachControllerListener:
				{
					m_mConnections[pMsg->tcpThread()]->setControllerListener(false);
				}
				break;
				case raaOctaveKernel::csm_uiOCAttachViewpointListener:
				{
					m_mConnections[pMsg->tcpThread()]->setViewpointListener(true);
				}
				break;
				case raaOctaveKernel::csm_uiOCDetachViewpointListener:
				{
					m_mConnections[pMsg->tcpThread()]->setViewpointListener(false);
				}
				break;
				case raaOctaveKernel::csm_uiOCAttachScreenListener:
				{
					m_mConnections[pMsg->tcpThread()]->setScreenListener(true);
				}
				break;
				case raaOctaveKernel::csm_uiOCDetachScreenListener:
				{
					m_mConnections[pMsg->tcpThread()]->setScreenListener(false);
				}
				break;
				case raaOctaveKernel::csm_uiOCScreenInfo:
				{
					std::string sName = pMsg->asString(3);

					raaNet::raaTcpMsg *pM = new raaTcpMsg(raaNet::csm_usTcpMsgInfo);
					pM->add(raaOctaveKernel::csm_uiOCScreenInfo);
					pM->add(sName);
					pM->add(m_pController->getScreen(sName)->screenVert(raaOctaveControllerTypes::csm_uiBL));
					pM->add(m_pController->getScreen(sName)->screenVert(raaOctaveControllerTypes::csm_uiBR));
					pM->add(m_pController->getScreen(sName)->screenVert(raaOctaveControllerTypes::csm_uiTL));
					pM->add(m_pController->getScreen(sName)->screenVert(raaOctaveControllerTypes::csm_uiTR));
					pM->add(m_pController->getScreen(sName)->normal());
					pM->add(m_pController->getScreen(sName)->near());
					pM->add(m_pController->getScreen(sName)->far());
					pM->add(m_pController->getScreen(sName)->rotation());
					pM->add(m_pController->getScreen(sName)->flipped(0));
					pM->add(m_pController->getScreen(sName)->flipped(1));
					pM->add(m_pController->getScreen(sName)->flipped(2));
					pMsg->tcpThread()->write(pM);
				}
				break;
				case raaOctaveKernel::csm_uiOCControllerRequestScreenNames:
				{
					raaNet::raaTcpMsg *pM = new raaNet::raaTcpMsg(raaNet::csm_usTcpMsgInfo);
					pM->add(raaOctaveKernel::csm_uiOCControllerRequestScreenNames);
					pM->add((unsigned int)m_pController->getScreens().size());
					for (raaStringScreenMap::const_iterator it = m_pController->getScreens().begin(); it != m_pController->getScreens().end(); it++) pM->add(it->first);
					pMsg->tcpThread()->write(pM);
				}
				break;
				case raaOctaveKernel::csm_uiOCWindowInfo:
				{
					std::string sName = pMsg->asString(3);

					raaNet::raaTcpMsg *pM = new raaNet::raaTcpMsg(raaNet::csm_usTcpMsgInfo);
					pM->add(raaOctaveKernel::csm_uiOCWindowInfo);
					pM->add(sName);
					pM->add(m_pController->getScreen(sName)->window(0));
					pM->add(m_pController->getScreen(sName)->window(1));
					pM->add(m_pController->getScreen(sName)->window(2));
					pM->add(m_pController->getScreen(sName)->window(3));
					pMsg->tcpThread()->write(pM);
				}
				break;
			}
		}
		break;
		case raaNet::csm_usTcpMsgInfo:
		{
			switch(pMsg->asUInt(2))
			{
				case raaOctaveKernel::csm_uiOCViewpointUpdatePhysical:
				{
					osg::Matrixf m=pMsg->asMatrix(3);
					m_pController->viewpoint()->setPhysicalMatrix(m);
				}
				break;
				case raaOctaveKernel::csm_uiOCViewpointUpdateVirtual:
				{
					osg::Matrixf m = pMsg->asMatrix(3);
					m_pController->viewpoint()->setVirtualMatrix(m);
				}
				break;
				case raaOctaveKernel::csm_uiOCScreenVertex:
				{
					std::string sName = pMsg->asString(4);
					if (sName.length() && m_pController->getScreen(sName))
					{
						osg::Vec3f v = pMsg->asVector(5);

						switch (pMsg->asUInt(3))
						{
							case raaOctaveKernel::csm_uiOCBL:
								m_pController->getScreen(sName)->setScreenVert(raaOctaveControllerTypes::csm_uiBL, v);
								break;
							case raaOctaveKernel::csm_uiOCBR:
								m_pController->getScreen(sName)->setScreenVert(raaOctaveControllerTypes::csm_uiBR, v);
								break;
							case raaOctaveKernel::csm_uiOCTL:
								m_pController->getScreen(sName)->setScreenVert(raaOctaveControllerTypes::csm_uiTL, v);
								break;
							case raaOctaveKernel::csm_uiOCTR:
								m_pController->getScreen(sName)->setScreenVert(raaOctaveControllerTypes::csm_uiTR, v);
								break;
						}
					}
				}
				break;
				case raaOctaveKernel::csm_uiOCScreenVertexAll:
				{
					std::string sName = pMsg->asString(3);
					if (sName.length() && m_pController->getScreen(sName))
					{
						m_pController->getScreen(sName)->setScreenVert(raaOctaveControllerTypes::csm_uiBL, pMsg->asVector(4));
						m_pController->getScreen(sName)->setScreenVert(raaOctaveControllerTypes::csm_uiBR, pMsg->asVector(5));
						m_pController->getScreen(sName)->setScreenVert(raaOctaveControllerTypes::csm_uiTL, pMsg->asVector(6));
						m_pController->getScreen(sName)->setScreenVert(raaOctaveControllerTypes::csm_uiTR, pMsg->asVector(7));
					}
				}
				break;
				case raaOctaveKernel::csm_uiOCScreenNearFar:
				{
					std::string sName = pMsg->asString(3);
					if (sName.length() && m_pController->getScreen(sName))
					{
						m_pController->getScreen(sName)->setNearFar(pMsg->asFloat(4), pMsg->asFloat(5));
					}
				}
				break;
				case raaOctaveKernel::csm_uiOCScreenImageRotationInfo:
				{
					std::string sName = pMsg->asString(3);
					if (sName.length() && m_pController->getScreen(sName))
					{
						m_pController->getScreen(sName)->setRotation(pMsg->asFloat(4));
					}
				}
				break;
				case raaOctaveKernel::csm_uiOCScreenImageFlipInfo:
				{
					std::string sName = pMsg->asString(3);
					if (sName.length() && m_pController->getScreen(sName))
					{
						m_pController->getScreen(sName)->setFlipped(pMsg->asBool(4), pMsg->asBool(5), pMsg->asBool(6));
					}
				}
				break;
				case raaOctaveKernel::csm_uiOCWindowInfo:
				{
					std::string sName = pMsg->asString(3);
					if (sName.length() && m_pController->getScreen(sName))
					{
						m_pController->getScreen(sName)->setWindow(pMsg->asInt(4), pMsg->asInt(5), pMsg->asInt(6), pMsg->asInt(7));
					}
				}
				break;

			}
		}
	}
}



void raaOctaveControl::tcpState(raaTcpThread* pThread, unsigned uiState)
{
	QString msg;
	msg += pThread->name();
	switch (uiState)
	{
	case raaNet::csm_uiUnconnectedState:
		msg += " -> StateChanged::UnconnectedState";
		if (m_mConnections.find(pThread) != m_mConnections.end())
		{
			delete m_mConnections[pThread];
			m_mConnections.erase(pThread);
		}
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
		m_mConnections[pThread] = new raaConnectionRecord(pThread->name().toStdString(), m_pController);
		m_mConnections[pThread]->setTcpThread(pThread);

		break;
	case raaNet::csm_uiBoundState:
		msg += " -> StateChanged::BoundState";
		break;
	case raaNet::csm_uiClosingState:
		msg += " -> StateChanged::ClosingState";
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

void raaOctaveControl::udpState(raaUdpThread* pThread, unsigned uiState)
{
	QString msg;
	msg += pThread->name();
	switch (uiState)
	{
	case raaNet::csm_uiUnconnectedState:
		msg += " -> udpStateChanged::UnconnectedState";
		break;
	case raaNet::csm_uiHostLookupState:
		msg += " -> udpStateChanged::HostLookupState";
		break;
	case raaNet::csm_uiConnectingState:
		msg += " -> udpStateChanged::ConnectingState";
		break;
	case raaNet::csm_uiConnectedState:
		msg += " -> udpStateChanged::ConnectedState";
		break;
	case raaNet::csm_uiNameConnectedState:
		msg += " -> udpStateChanged::NameConnectedState";
		break;
	case raaNet::csm_uiBoundState:
		msg += " -> udpStateChanged::BoundState";
		break;
	case raaNet::csm_uiClosingState:
		msg += " ->udp StateChanged::ClosingState";
		break;
	case raaNet::csm_uiListeningState:
		msg += " -> udpStateChanged::ListeningState";
		break;
	default:
		msg += " -> udpStateChanged::UnknownState";
		break;
	}

	std::cout << msg.toStdString() << std::endl;

}

void raaOctaveControl::send()
{
//	m_pNetwork->writeTcp(raaNet::csm_usTcpMsgInfo, m_uiTcpCounter++, sSocket, sMsg.toLocal8Bit());
}

void raaOctaveControl::udpConnect()
{
	if (m_pNetwork)
	{
		m_pNetwork->createUdpServer("raaOctaveControl", QString("localhost"), 65723);
	}
}

void raaOctaveControl::udpStream(int iState)
{
	if (iState == Qt::Checked)
	{
		m_iTimer = startTimer(10);
	}
	else if (m_iTimer)
	{
		killTimer(m_iTimer);
		m_iTimer = 0;
	}

}

void raaOctaveControl::udpRead(raaUdpMsg* pMsg)
{
	std::cout << pMsg->udpThread()->name().toStdString() << QString(pMsg->data()).toStdString() << std::endl;
}

void raaOctaveControl::originChanged(raaOctaveController* pController)
{
}

void raaOctaveControl::screenAdded(raaOctaveController* pController, raaScreen* pScreen)
{

}

void raaOctaveControl::screenRemoved(raaOctaveController* pController, raaScreen* pScreen)
{
}

void raaOctaveControl::screenUpdated(raaOctaveController* pController, raaScreen* pScreen)
{
}

void raaOctaveControl::timerEvent(QTimerEvent* pEvent)
{
	if (m_pNetwork)
	{
		QByteArray data;
		int i = rand();
		data.append((const char*)&i, sizeof(int));

//		m_pNetwork->writeUdp("raaOctaveControl",new raa data);
	}
}
