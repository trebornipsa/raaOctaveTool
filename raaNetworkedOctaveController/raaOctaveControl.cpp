#include "stdafx.h"

#include <QtCore/QMutex>
#include <QtCore/QDir>

#include <iostream>

#include <raaOctaveKernel/raaOctaveKernel.h>

#include <raaOctaveController/raaScreen.h>
#include <raaVRPNClient/raaVRPNKinect2.h>

#include "raaConnectionRecord.h"
#include "raaOctaveControl.h"
#include "raaOctaveControl.moc"

raaOctaveControl::raaOctaveControl(std::string sTracker, std::string sConfDir): m_sConfDir(sConfDir)
{
	std::cout << "Conf Dir -> " << m_sConfDir << std::endl;
	srand(0);
//	m_pEyeTracker = 0;
	m_iTimer = 0;
	m_pNetwork = 0;
	m_uiTcpCounter = 0;
	m_pController = new raaOctaveController(this);

	m_pNetwork = new raaNet::raaNetwork(65204, this);
	connect(m_pNetwork, SIGNAL(tcpRead(raaTcpMsg *)), SLOT(tcpRead(raaTcpMsg *)));
	connect(m_pNetwork, SIGNAL(tcpState(raaTcpThread*, unsigned int)), this, SLOT(tcpState(raaTcpThread*, unsigned int)));
	connect(m_pNetwork, SIGNAL(udpRead(raaUdpMsg *)), SLOT(udpRead(raaUdpMsg *)));
	connect(m_pNetwork, SIGNAL(udpState(raaUdpThread*, unsigned int)), this, SLOT(udpState(raaUdpThread*, unsigned int)));

	osg::Vec3f vPos(0.0f, 2.0f, 1.8f);
	osg::Vec3f vDir(0.0f, -1.0f, 0.0f); 
	osg::Vec3f vUp(0.0f, 0.0f, 1.0f);

//	osg::Vec3f vPos(0.0f, 1.8f, -2.0f);
//	osg::Vec3f vDir(0.0f, 0.0f, 1.0f);
//	osg::Vec3f vUp(0.0f, 1.0f, 0.0f);

/*
	raaVRPNClient *pClient=addClient("raaTest", "raaKinect2", sTracker, vPos, vDir, vUp, 30, 0x00000001);
	pClient->setActiveSensors(0x0000ffff);
	pClient->start();
	*/
}

raaOctaveControl::~raaOctaveControl()
{
	if (m_pNetwork) delete m_pNetwork;
}
/*
void raaOctaveControl::updatedSensor(raaVRPNClient* pClient, unsigned uiSensor)
{
	if (m_pEyeTracker && pClient == m_pEyeTracker && m_pController && (m_pEyeTracker->eyeTracker() & 1 << uiSensor))
	{
		m_pController->viewpoint()->setPhysicalMatrix(pClient->sensorTransform(uiSensor));
	}
}

void raaOctaveControl::updatedSensors(raaVRPNClient* pClient)
{
}

void raaOctaveControl::updatedOrigin(raaVRPNClient* pClient)
{
}

void raaOctaveControl::timerSensorUpdate(raaVRPNClient* pClient)
{
}
*/

void raaOctaveControl::tcpRead(raaTcpMsg* pMsg)
{
	switch(pMsg->msgType())
	{
		case raaNet::csm_usTcpMsgRequest:
		{
			switch(pMsg->asUInt(2))
			{
				case raaOctaveKernel::csm_uiOCHasConfig:
				{
					std::cout << "Request -> Has Config -> " << pMsg->tcpThread()->name().toStdString() << std::endl;
					raaNet::raaTcpMsg *pM = new raaNet::raaTcpMsg(raaNet::csm_usTcpMsgReply);
					pM->add(m_pController->hasConfig() ? raaOctaveKernel::csm_uiOCHasConfigTrue : raaOctaveKernel::csm_uiOCHasConfigFalse);
					pMsg->tcpThread()->write(pM);
				}
				break;
				case raaOctaveKernel::csm_uiOCScreenAdd:
					if(m_pController)
					{
						std::string sName = pMsg->asString(3).c_str();


						if(m_pController->getScreens().find(sName)==m_pController->getScreens().end())
						{
							m_pController->addScreen(sName);
						}
					}
					break;
				case raaOctaveKernel::csm_uiOCSaveConfigAs:
				{
					std::string sName = m_sConfDir;
					sName += "/";
					sName+=pMsg->asString(3);
				
					if(m_pController)
					{
						m_pController->writeConfig(sName.c_str(), "raaOctave");
					}
				}
				break;
				case raaOctaveKernel::csm_uiOCDisplays:
				{
					raaNet::raaTcpMsg *pM = new raaNet::raaTcpMsg(raaNet::csm_usTcpMsgInfo);
					pM->add(raaOctaveKernel::csm_uiOCDisplays);
					std::cout << "Request Displays" << std::endl;
					pM->add((unsigned int)m_pController->getDisplays().size());

					for (raaDisplayScreensMap::const_iterator cit = m_pController->getDisplays().begin(); cit != m_pController->getDisplays().end(); cit++)
					{
						pM->add(cit->second.m_iScreen);
						pM->add(cit->second.m_iWidth);
						pM->add(cit->second.m_iHeight);
					}
					pMsg->tcpThread()->write(pM);
				}
				break;
				case raaOctaveKernel::csm_uiOCListConfigs:
				{
					QDir d(m_sConfDir.c_str());

					if (d.exists())
					{
						QStringList sl;
						sl.push_back("*.raa");

						QFileInfoList fil = d.entryInfoList(sl, QDir::Files | QDir::NoDot | QDir::NoDotAndDotDot);

						if (fil.length())
						{
							raaTcpMsg *pM = new raaTcpMsg(raaNet::csm_usTcpMsgInfo);
							pM->add(raaOctaveKernel::csm_uiOCListConfigs);
							pM->add(fil.length());

							for (QFileInfoList::const_iterator it = fil.constBegin(); it != fil.constEnd(); it++)
								pM->add((*it).fileName().toStdString());

							pMsg->tcpThread()->write(pM);
						}

					}

				}
				break;
				case raaOctaveKernel::csm_uiOCLoadConfig:
				{
					std::cout << "Request -> Load Config -> " << pMsg->tcpThread()->name().toStdString() << std::endl;
					QString sConf = m_sConfDir.c_str();
					sConf += "//";
					sConf += pMsg->asString(3).c_str();

					std::cout << "\tConfig dir -> " << m_sConfDir << std::endl;
					std::cout << "\tConfig file -> " << pMsg->asString(3).c_str() << std::endl;


					std::cout << "\tConfig -> " << sConf.toStdString() << std::endl;
					m_pController->readConfig(sConf);
				}
				break;
				case raaOctaveKernel::csm_uiOCAttachControllerListener:
				{
					//std::cout << "Request -> Attache Controller Listener -> " << pMsg->tcpThread()->name().toStdString() << std::endl;
					m_mConnections[pMsg->tcpThread()]->setControllerListener(true);
				}
				break;
				case raaOctaveKernel::csm_uiOCDetachControllerListener:
				{
					//std::cout << "Request -> Detach Controller Listener -> " << pMsg->tcpThread()->name().toStdString() << std::endl;
					m_mConnections[pMsg->tcpThread()]->setControllerListener(false);
				}
				break;
				case raaOctaveKernel::csm_uiOCAttachViewpointListener:
				{
					//std::cout << "Request -> Attach Viewpoint Listener -> " << pMsg->tcpThread()->name().toStdString() << std::endl;
					m_mConnections[pMsg->tcpThread()]->setViewpointListener(true);
				}
				break;
				case raaOctaveKernel::csm_uiOCDetachViewpointListener:
				{
					//std::cout << "Request -> Detach Viewpoint Listener -> " << pMsg->tcpThread()->name().toStdString() << std::endl;

					m_mConnections[pMsg->tcpThread()]->setViewpointListener(false);
				}
				break;
				case raaOctaveKernel::csm_uiOCAttachScreenListener:
				{
					//std::cout << "Request -> Attach Screen Listener -> " << pMsg->tcpThread()->name().toStdString() << std::endl;

					m_mConnections[pMsg->tcpThread()]->setScreenListener(true);
				}
				break;
				case raaOctaveKernel::csm_uiOCDetachScreenListener:
				{
					//std::cout << "Request -> DEtach Screen Listener -> " << pMsg->tcpThread()->name().toStdString() << std::endl;
					m_mConnections[pMsg->tcpThread()]->setScreenListener(false);
				}
				break;
				case raaOctaveKernel::csm_uiOCControllerRequestScreenAll:
					m_mConnections[pMsg->tcpThread()]->sendScreenAll(m_pController);
					break;
				case raaOctaveKernel::csm_uiOCScreenInfo:
				{
					//std::cout << "Request -> Screen Info -> " << pMsg->tcpThread()->name().toStdString() << std::endl;
					std::string sName = pMsg->asString(3);

					raaNet::raaTcpMsg *pM = new raaTcpMsg(raaNet::csm_usTcpMsgInfo);
					pM->add(raaOctaveKernel::csm_uiOCScreenInfo);
					pM->add(sName);
					pM->add(m_pController->getScreen(sName)->screenVert(raaOctaveControllerTypes::csm_uiBL));
					pM->add(m_pController->getScreen(sName)->screenVert(raaOctaveControllerTypes::csm_uiBR));
					pM->add(m_pController->getScreen(sName)->screenVert(raaOctaveControllerTypes::csm_uiTL));
					pM->add(m_pController->getScreen(sName)->screenVert(raaOctaveControllerTypes::csm_uiTR));
					pM->add(m_pController->getScreen(sName)->normal());

					pM->add(m_pController->getScreen(sName)->nearClip());

					pM->add(m_pController->getScreen(sName)->farClip());
					pM->add(m_pController->getScreen(sName)->rotation());
					pM->add(m_pController->getScreen(sName)->flipped(0));
					pM->add(m_pController->getScreen(sName)->flipped(1));
					pM->add(m_pController->getScreen(sName)->flipped(2));
					pMsg->tcpThread()->write(pM);
				}
				break;
				case raaOctaveKernel::csm_uiOCControllerRequestScreenNames:
				{
					//std::cout << "Request -> Screen Names -> " << pMsg->tcpThread()->name().toStdString() << std::endl;

					raaNet::raaTcpMsg *pM = new raaNet::raaTcpMsg(raaNet::csm_usTcpMsgInfo);
					pM->add(raaOctaveKernel::csm_uiOCControllerRequestScreenNames);
					pM->add((unsigned int)m_pController->getScreens().size());
					for (raaStringScreenMap::const_iterator it = m_pController->getScreens().begin(); it != m_pController->getScreens().end(); it++) pM->add(it->first);
					pMsg->tcpThread()->write(pM);
				}
				break;
				case raaOctaveKernel::csm_uiOCWindowInfo:
				{
					//std::cout << "Request -> Window Info -> " << pMsg->tcpThread()->name().toStdString() << std::endl;
					std::string sName = pMsg->asString(3);

					raaNet::raaTcpMsg *pM = new raaNet::raaTcpMsg(raaNet::csm_usTcpMsgInfo);
					pM->add(raaOctaveKernel::csm_uiOCWindowInfo);
					pM->add(sName);
					pM->add(m_pController->getScreen(sName)->screen());
					pM->add(m_pController->getScreen(sName)->window(0));
					pM->add(m_pController->getScreen(sName)->window(1));
					pM->add(m_pController->getScreen(sName)->window(2));
					pM->add(m_pController->getScreen(sName)->window(3));
					pMsg->tcpThread()->write(pM);
				}
				break;
				case raaOctaveKernel::csm_uiOCTrackerAllNames:
				{
					if (m_pController)
					{
						raaNet::raaTcpMsg *pM = new raaNet::raaTcpMsg(raaNet::csm_usTcpMsgInfo);
						pM->add(raaOctaveKernel::csm_uiOCTrackerAllNames);
						pM->add((unsigned int)m_pController->clients().size());
						for (raaVRPNClients::const_iterator cit = m_pController->clients().begin(); cit != m_pController->clients().end(); cit++) pM->add(cit->second->name());
						pMsg->tcpThread()->write(pM);
					}
				}
				break;
				case raaOctaveKernel::csm_uiOCTrackerOriginTransform:
				{
					std::string sName = pMsg->asString(3);

					if (m_pController && m_pController->clients().find(sName) != m_pController->clients().end())
					{
						raaVRPNClient *pClient = m_pController->clients()[sName];
						raaTcpMsg *pM = new raaTcpMsg(raaNet::csm_usTcpMsgInfo);
						pM->add(raaOctaveKernel::csm_uiOCTrackerOriginTransform);
						pM->add(sName);
						pM->add(pClient->trackerPosition());
						pM->add(pClient->trackerDirection());
						pM->add(pClient->trackerUp());
						pMsg->tcpThread()->write(pM);
					}
				}
				break;
				case raaOctaveKernel::csm_uiOCTrackerAddRemoveListener:
				{
					if(pMsg->asBool(3) && m_pController)
					{
						if(m_pController->clients().find(pMsg->asString(4))!= m_pController->clients().end())
						{
							raaVRPNClient *pClient = m_pController->clients()[pMsg->asString(4)];
							unsigned int uiSensors = pClient->activeSensors();
							raaTcpMsg *pM = new raaTcpMsg(raaNet::csm_usTcpMsgInfo);
							pM->add(raaOctaveKernel::csm_uiOCTrackerInfo);
							pM->add(pMsg->asString(4));
							pM->add(pClient->trackerTransform());
							pM->add(uiSensors);
							for (int i = 0; i < 32; i++) if (uiSensors & 1 << i) pM->add(pClient->sensorTransform(i));
							pMsg->tcpThread()->write(pM);


							pClient->addListener(m_mConnections[pMsg->tcpThread()], uiSensors);
						}
					}
					else if(m_pController)
					{
						if (m_pController->clients().find(pMsg->asString(4)) != m_pController->clients().end())
						{
							raaVRPNClient *pClient = m_pController->clients()[pMsg->asString(4)];
							pClient->removeListener(m_mConnections[pMsg->tcpThread()]);
						}
					}
				}
				break;
			}
		}
		break;
		case raaNet::csm_usTcpMsgInfo:
		{
			switch(pMsg->asUInt(2))
			{
				case raaOctaveKernel::csm_uiOCTrackerOriginTransform:
				{
					std::string sName = pMsg->asString(3);

					if (m_pController && m_pController->clients().find(sName) != m_pController->clients().end())
					{
						raaVRPNClient *pClient = m_pController->clients()[sName];
						pClient->setTrackerTransform(pMsg->asVector(4), pMsg->asVector(5),pMsg->asVector(6));
					}
				}
				break;
				case raaOctaveKernel::csm_uiOCViewpointUpdatePhysical:
				{
					//std::cout << "Info -> Update Physical -> " << pMsg->tcpThread()->name().toStdString() << std::endl;
					osg::Matrixf m=pMsg->asMatrix(3);
					m_pController->viewpoint()->setPhysicalMatrix(m);
				}
				break;
				case raaOctaveKernel::csm_uiOCViewpointUpdateVirtual:
				{
					//std::cout << "Info -> Update Virtual -> " << pMsg->tcpThread()->name().toStdString() << std::endl;
					osg::Matrixf m = pMsg->asMatrix(3);
					m_pController->viewpoint()->setVirtualMatrix(m);
				}
				break;
				case raaOctaveKernel::csm_uiOCScreenVertex:
				{
					//std::cout << "Info -> Screen Vertex -> " << pMsg->tcpThread()->name().toStdString() << std::endl;
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
					//std::cout << "Info -> Screen Vertex All -> " << pMsg->tcpThread()->name().toStdString() << std::endl;
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
					//std::cout << "Info -> Screen Near Far -> " << pMsg->tcpThread()->name().toStdString() << std::endl;
					std::string sName = pMsg->asString(3);
					if (sName.length() && m_pController->getScreen(sName))
					{
						m_pController->getScreen(sName)->setNearFar(pMsg->asFloat(4), pMsg->asFloat(5));
					}
				}
				break;
				case raaOctaveKernel::csm_uiOCScreenImageRotationInfo:
				{
					//std::cout << "Info -> Screen Rot -> " << pMsg->tcpThread()->name().toStdString() << std::endl;
					std::string sName = pMsg->asString(3);
					if (sName.length() && m_pController->getScreen(sName))
					{
						m_pController->getScreen(sName)->setRotation(pMsg->asFloat(4));
					}
				}
				break;
				case raaOctaveKernel::csm_uiOCScreenImageFlipInfo:
				{
					//std::cout << "Info -> Screen Flip -> " << pMsg->tcpThread()->name().toStdString() << std::endl;
					std::string sName = pMsg->asString(3);
					if (sName.length() && m_pController->getScreen(sName))
					{
						m_pController->getScreen(sName)->setFlipped(pMsg->asBool(4), pMsg->asBool(5), pMsg->asBool(6));
					}
				}
				break;
				case raaOctaveKernel::csm_uiOCWindowInfo:
				{
					//std::cout << "Info -> Window Info -> " << pMsg->tcpThread()->name().toStdString() << std::endl;
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
		msg += " -> StateChanged::NameConnectedState -> " + pThread->name();
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

	//std::cout << msg.toStdString() << std::endl;

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
	//std::cout << pMsg->udpThread()->name().toStdString() << QString(pMsg->data()).toStdString() << std::endl;
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

void raaOctaveControl::displayScreensChanged(raaOctaveController* pController)
{
}

void raaOctaveControl::timerEvent(QTimerEvent* pEvent)
{
/*
	if (m_pNetwork)
	{
		QByteArray data;
		int i = rand();
		data.append((const char*)&i, sizeof(int));
	}
*/
}
