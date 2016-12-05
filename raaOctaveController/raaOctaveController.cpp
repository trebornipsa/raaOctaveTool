#include <iostream>

#include <QtXml/QDomDocument>
#include <QtXml/QDomNode>
#include <QtXml/QDomElement>
#include <QtCore/QFile>
#include <QtCore/QFileDevice>
#include <QtCore/QTextStream>

#include <osg/Matrixf>

#include "raaScreen.h"
#include "raaOctaveController.h"

void readVec(QDomElement &e, osg::Vec3f &v)
{
	if (e.hasAttribute("x")) v[0] = e.attribute("x").toFloat();
	if (e.hasAttribute("y")) v[1] = e.attribute("y").toFloat();
	if (e.hasAttribute("z")) v[2] = e.attribute("z").toFloat();
}

raaOctaveControllerListener::raaOctaveControllerListener()
{
}

raaOctaveControllerListener::~raaOctaveControllerListener()
{
}

raaOctaveController::raaOctaveController(raaOctaveControllerListener *pListener)
{
	m_bConfig = false;
	m_pEyeTracker = 0;
	addListener(pListener);
}

void raaOctaveController::trackerAdded(raaVRPNClient* pClient)
{
	if (pClient)
	{
		if (pClient->eyeTracker())
		{
			if (m_pEyeTracker) m_pEyeTracker->removeListener(this);
			m_pEyeTracker = pClient;
			m_pEyeTracker->addListener(this, m_pEyeTracker->eyeTracker());
		}
	}
}

void raaOctaveController::trackerRemoved(raaVRPNClient* pClient)
{
	if(pClient && pClient==m_pEyeTracker)
	{
		pClient->removeListener(this);
		m_pEyeTracker = 0;

		for(raaVRPNClients::iterator it =clients().begin();it!=clients().end();it++)
		{
			if(pClient!=it->second && it->second->eyeTracker())
			{
				m_pEyeTracker = it->second;
				m_pEyeTracker->addListener(this, m_pEyeTracker->eyeTracker());
			}
		}
	}
}

raaOctaveController::~raaOctaveController()
{
}

void raaOctaveController::readConfig(QString sConfig)
{
	if (sConfig.length())
	{
		QDomDocument doc("raaConfig");
		QFile file(sConfig);
		if (file.open(QIODevice::ReadOnly))
		{
			doc.setContent(&file);
			file.close();

			QDomElement dConfig = doc.documentElement();

			if (!dConfig.isNull())
			{
				if (dConfig.hasAttribute("name")) m_sName = dConfig.attribute("name").toStdString();


				for (QDomNode dN = dConfig.firstChild(); !dN.isNull(); dN = dN.nextSibling())
				{
					QDomElement dE = dN.toElement();

					if (dE.nodeName() == "ORIGIN")
					{
						readVec(dE, m_vOrigin);
						for (raaOctaveControllerListeners::iterator it = m_lListener.begin(); it != m_lListener.end(); it++)(*it)->originChanged(this);
					}
					else if (dE.nodeName() == "DISPLAYS")
					{
						for (QDomNode n = dE.firstChild(); !n.isNull(); n = n.nextSibling())
						{
							QDomElement eD = n.toElement();

							if(eD.nodeName()=="DISPLAY")
							{
								int iScreen = 0;
								int iWidth = 1024;
								int iHeight = 1024;

								if (eD.hasAttribute("screen")) iScreen = eD.attribute("screen").toInt();
								if (eD.hasAttribute("width")) iWidth = eD.attribute("width").toInt();
								if (eD.hasAttribute("height")) iHeight = eD.attribute("height").toInt();
							
								if (m_mDisplays.find(iScreen) == m_mDisplays.end())
								{
									m_mDisplays[iScreen].m_iScreen = iScreen;
									m_mDisplays[iScreen].m_iWidth = iWidth;
									m_mDisplays[iScreen].m_iHeight = iHeight;
								}
							}
						}

						for (raaOctaveControllerListeners::iterator it = m_lListener.begin(); it != m_lListener.end(); it++)(*it)->displayScreensChanged(this);
					}
					else if (dE.nodeName() == "VIEWPOINT")
					{
						for (QDomNode n = dE.firstChild(); !n.isNull(); n = n.nextSibling())
						{
							QDomElement eV = n.toElement();

							if (eV.nodeName() == "PHYSICAL")
							{
								osg::Vec3f vPos, vUp, vDir, vRight;
								for (QDomNode nV = eV.firstChild(); !nV.isNull(); nV = nV.nextSibling())
								{
									QDomElement eT = nV.toElement();
									if (eT.nodeName() == "POS") 
										readVec(eT, vPos);
									else if (eT.nodeName() == "DIR") 
										readVec(eT, vDir);
									else if (eT.nodeName() == "UP") 
										readVec(eT, vUp);
								}

								vRight = vDir^vUp;
								vRight.normalize();

								osg::Matrixf m(vRight[0], vDir[0], vUp[0], 0.0f,
									vRight[1], vDir[1], vUp[1], 0.0f,
									vRight[2], vDir[2], vUp[2], 0.0f,
									vPos[0], vPos[1], vPos[2], 1.0f);


								m_ViewPoint.setDefaultPhysicalMatrix(m);
							}
							else if (eV.nodeName() == "VIRTUAL")
							{
								osg::Vec3f vPos, vUp, vDir, vRight;
								for (QDomNode nV = eV.firstChild(); !nV.isNull(); nV = nV.nextSibling())
								{
									QDomElement eT = nV.toElement();
									if (eT.nodeName() == "POS")
										readVec(eT, vPos);
									else if (eT.nodeName() == "DIR")
										readVec(eT, vDir);
									else if (eT.nodeName() == "UP")
										readVec(eT, vUp);
								}
								vRight = vDir^vUp;
								vRight.normalize();

								osg::Matrixf m(vRight[0], vDir[0], vUp[0], 0.0f,
									vRight[1], vDir[1], vUp[1], 0.0f,
									vRight[2], vDir[1], vUp[2], 0.0f,
									vPos[0], vPos[1], vPos[2], 1.0f);


								m_ViewPoint.setVirtualMatrix(m);

							}
						}
					}
					else if (dE.nodeName() == "TRACKER")
					{
						osg::Vec3f vPos, vUp, vDir;
						QString sName, sType, sAddress;
						unsigned int uiPollTime = 30, uiEyeSensor = 0, uiActiveSensors = 0;

						if (dE.hasAttribute("name")) sName = dE.attribute("name");
						if (dE.hasAttribute("type")) sType = dE.attribute("type");
						if (dE.hasAttribute("address")) sAddress = dE.attribute("address");
						if (dE.hasAttribute("pollTime")) uiPollTime = dE.attribute("pollTime").toUInt();
						if (dE.hasAttribute("eyeSensor")) uiEyeSensor = dE.attribute("eyeSensor").toUInt();
						if (dE.hasAttribute("activeSensors")) uiActiveSensors = dE.attribute("activeSensors").toUInt();

						for (QDomNode n = dN.firstChild(); !n.isNull(); n = n.nextSibling())
						{
							QDomElement e = n.toElement();
							if (e.nodeName() == "POS") readVec(e, vPos);
							else if (e.nodeName() == "DIR") readVec(e, vDir);
							else if (e.nodeName() == "UP") readVec(e, vUp);
						}

						if(sName.length() && sType.length() && sAddress.length() && uiActiveSensors)
						{
							raaVRPNClient *pClient = addClient(sName.toStdString(), sType.toStdString(),sAddress.toStdString(), vPos, vDir, vUp, uiPollTime, uiEyeSensor);
							pClient->setActiveSensors(uiActiveSensors);
							pClient->start();
						}
					}
					else if (dE.nodeName() == "SCREEN")
					{
						osg::Vec3f vBL, vBR, vTR, vTL;
						float fNear = 0.01f, fFar = 100.0f, fRot=0.0f;
						bool bX=false, bY=false, bZ=false;
						int iX = 0, iY = 0, iW = 200, iH = 200;
						int iScreen = 0;
						int iStereo = 0;

						for (QDomNode n = dN.firstChild(); !n.isNull(); n = n.nextSibling())
						{
							QDomElement e = n.toElement();
							if (e.nodeName() == "BL") readVec(e, vBL);
							else if (e.nodeName() == "BR") readVec(e, vBR);
							else if (e.nodeName() == "TL") readVec(e, vTL);
							else if (e.nodeName() == "TR") readVec(e, vTR);
							else if (e.nodeName() == "CLIP")
							{
								if (e.hasAttribute("near")) fNear = e.attribute("near").toFloat();
								if (e.hasAttribute("far")) fFar = e.attribute("far").toFloat();
							}
							else if (e.nodeName() == "MOD")
							{
								if (e.hasAttribute("rot")) fRot = e.attribute("rot").toFloat();
								if (e.hasAttribute("flipx")) bX = (e.attribute("flipx") == "true") ? true : false;
								if (e.hasAttribute("flipy")) bY = (e.attribute("flipy") == "true") ? true : false;
								if (e.hasAttribute("fipz")) bZ = (e.attribute("flipz") == "true") ? true : false;
							}
							else if (e.nodeName() == "WINDOW")
							{
								if (e.hasAttribute("x")) iX = e.attribute("x").toInt();
								if (e.hasAttribute("y")) iY = e.attribute("y").toInt();
								if (e.hasAttribute("width")) iW = e.attribute("width").toInt();
								if (e.hasAttribute("height")) iH = e.attribute("height").toInt();
							}
						}

						std::string sName;

						if (dE.hasAttribute("name")) sName = dE.attribute("name").toStdString();
						if (dE.hasAttribute("screen")) iScreen = dE.attribute("screen").toInt();
						if (dE.hasAttribute("stereo"))
						{
							if (dE.attribute("stereo") == "mono") iStereo = 0;
							else if (dE.attribute("stereo") == "lefteye") iStereo = 1;
							else if (dE.attribute("stereo") == "righteye") iStereo = 2;
							else if (dE.attribute("stereo") == "anaglyphic") iStereo = 3;
							else if (dE.attribute("stereo") == "quad_buffer") iStereo = 4;
						}


						if (sName.length())
						{
							m_mScreens[sName] = new raaScreen(sName, iScreen, iStereo, vBL, vBR, vTR, vTL, fNear, fFar, fRot, bX, bY, bZ, iX, iY, iW, iH, &m_ViewPoint);
							for (raaOctaveControllerListeners::iterator it = m_lListener.begin(); it != m_lListener.end(); it++)(*it)->screenAdded(this, m_mScreens[sName]);
						}
					}
				}
			}
		}
		m_bConfig = true;
	}
}

void raaOctaveController::writeConfig(QString sConfig, QString sName)
{
	if(sConfig.length())
	{
		QDomDocument doc("raaConfig");
		QDomElement eDocElement = doc.createElement("CONFIG");
		doc.appendChild(eDocElement);
		eDocElement.setAttribute("name", sName);

		if (m_mDisplays.size())
		{
			QDomElement eDisplays = doc.createElement("DISPLAYS");
			eDocElement.appendChild(eDisplays);

			for (raaDisplayScreensMap::iterator it = m_mDisplays.begin(); it != m_mDisplays.end(); it++)
			{
				QDomElement eDisplay = doc.createElement("DISPLAY");
				eDisplays.appendChild(eDisplay);

				eDisplay.setAttribute("screen", it->second.m_iScreen);
				eDisplay.setAttribute("width", it->second.m_iWidth);
				eDisplay.setAttribute("height", it->second.m_iHeight);
			}
		}

		QDomElement eOrigin = doc.createElement("ORIGIN");
		eDocElement.appendChild(eOrigin);
		eOrigin.setAttribute("x", m_vOrigin[0]);
		eOrigin.setAttribute("y", m_vOrigin[1]);
		eOrigin.setAttribute("z", m_vOrigin[2]);

		QDomElement eViewpoint = doc.createElement("VIEWPOINT");
		QDomElement eViewpointPhysical = doc.createElement("PHYSICAL");
		QDomElement eViewpointVirtual = doc.createElement("VIRTUAL");
		QDomElement eViewpointPhysicalPos = doc.createElement("POS");
		QDomElement eViewpointPhysicalUp = doc.createElement("UP");
		QDomElement eViewpointPhysicalDir = doc.createElement("DIR");
		QDomElement eViewpointVirtualPos = doc.createElement("POS");
		QDomElement eViewpointVirtualUp = doc.createElement("UP");
		QDomElement eViewpointVirtualDir = doc.createElement("DIR");
		eDocElement.appendChild(eViewpoint);
		eViewpoint.appendChild(eViewpointPhysical);
		eViewpoint.appendChild(eViewpointVirtual);
		eViewpointPhysical.appendChild(eViewpointPhysicalPos);
		eViewpointPhysical.appendChild(eViewpointPhysicalUp);
		eViewpointPhysical.appendChild(eViewpointPhysicalDir);
		eViewpointVirtual.appendChild(eViewpointVirtualPos);
		eViewpointVirtual.appendChild(eViewpointVirtualUp);
		eViewpointVirtual.appendChild(eViewpointVirtualDir);

		eViewpointPhysicalPos.setAttribute("x", m_ViewPoint.defaultPhysicalMatrix().ptr()[12]);
		eViewpointPhysicalPos.setAttribute("y", m_ViewPoint.defaultPhysicalMatrix().ptr()[13]);
		eViewpointPhysicalPos.setAttribute("z", m_ViewPoint.defaultPhysicalMatrix().ptr()[14]);

		eViewpointPhysicalUp.setAttribute("x", m_ViewPoint.defaultPhysicalMatrix().ptr()[1]);
		eViewpointPhysicalUp.setAttribute("y", m_ViewPoint.defaultPhysicalMatrix().ptr()[5]);
		eViewpointPhysicalUp.setAttribute("z", m_ViewPoint.defaultPhysicalMatrix().ptr()[9]);

		eViewpointPhysicalDir.setAttribute("x", m_ViewPoint.defaultPhysicalMatrix().ptr()[2]);
		eViewpointPhysicalDir.setAttribute("y", m_ViewPoint.defaultPhysicalMatrix().ptr()[6]);
		eViewpointPhysicalDir.setAttribute("z", m_ViewPoint.defaultPhysicalMatrix().ptr()[10]);

		eViewpointVirtualPos.setAttribute("x", m_ViewPoint.virtualMatrix().ptr()[12]);
		eViewpointVirtualPos.setAttribute("y", m_ViewPoint.virtualMatrix().ptr()[13]);
		eViewpointVirtualPos.setAttribute("z", m_ViewPoint.virtualMatrix().ptr()[14]);

		eViewpointVirtualUp.setAttribute("x", m_ViewPoint.virtualMatrix().ptr()[1]);
		eViewpointVirtualUp.setAttribute("y", m_ViewPoint.virtualMatrix().ptr()[5]);
		eViewpointVirtualUp.setAttribute("z", m_ViewPoint.virtualMatrix().ptr()[9]);

		eViewpointVirtualDir.setAttribute("x", m_ViewPoint.virtualMatrix().ptr()[2]);
		eViewpointVirtualDir.setAttribute("y", m_ViewPoint.virtualMatrix().ptr()[6]);
		eViewpointVirtualDir.setAttribute("z", m_ViewPoint.virtualMatrix().ptr()[10]);

		for(raaStringScreenMap::iterator it=m_mScreens.begin(); it!=m_mScreens.end();it++)
		{
			QDomElement eScreen = doc.createElement("SCREEN");
			QDomElement eScreenClip = doc.createElement("CLIP");
			QDomElement eScreenBL = doc.createElement("BL");
			QDomElement eScreenBR = doc.createElement("BR");
			QDomElement eScreenTL = doc.createElement("TL");
			QDomElement eScreenTR = doc.createElement("TR");
			QDomElement eScreenMOD = doc.createElement("MOD");
			QDomElement eScreenWindow = doc.createElement("WINDOW");
			eDocElement.appendChild(eScreen);
			eScreen.appendChild(eScreenClip);
			eScreen.appendChild(eScreenBL);
			eScreen.appendChild(eScreenBR);
			eScreen.appendChild(eScreenTL);
			eScreen.appendChild(eScreenTR);
			eScreen.appendChild(eScreenMOD);
			eScreen.appendChild(eScreenWindow);


			QString sStereo = "mono";
			switch(it->second->stereo())
			{
				case 0:
					sStereo = "mono";
					break;
				case 1:
					sStereo = "lefteye";
					break;
				case 2:
					sStereo = "righteye";
					break;
				case 3:
					sStereo = "anaglyphic";
					break;
				case 4:
					sStereo = "quad_buffer";
					break;
			}


			eScreen.setAttribute("name", it->second->name().c_str());
			eScreen.setAttribute("screen", it->second->screen());
			eScreen.setAttribute("stereo", sStereo);

			eScreenClip.setAttribute("near", it->second->nearClip());
			eScreenClip.setAttribute("far", it->second->farClip());

			eScreenBL.setAttribute("x", it->second->screenVert(raaOctaveControllerTypes::csm_uiBL)[0]);
			eScreenBL.setAttribute("y", it->second->screenVert(raaOctaveControllerTypes::csm_uiBL)[1]);
			eScreenBL.setAttribute("z", it->second->screenVert(raaOctaveControllerTypes::csm_uiBL)[2]);
			eScreenBR.setAttribute("x", it->second->screenVert(raaOctaveControllerTypes::csm_uiBR)[0]);
			eScreenBR.setAttribute("y", it->second->screenVert(raaOctaveControllerTypes::csm_uiBR)[1]);
			eScreenBR.setAttribute("z", it->second->screenVert(raaOctaveControllerTypes::csm_uiBR)[2]);
			eScreenTL.setAttribute("x", it->second->screenVert(raaOctaveControllerTypes::csm_uiTL)[0]);
			eScreenTL.setAttribute("y", it->second->screenVert(raaOctaveControllerTypes::csm_uiTL)[1]);
			eScreenTL.setAttribute("z", it->second->screenVert(raaOctaveControllerTypes::csm_uiTL)[2]);
			eScreenTR.setAttribute("x", it->second->screenVert(raaOctaveControllerTypes::csm_uiTR)[0]);
			eScreenTR.setAttribute("y", it->second->screenVert(raaOctaveControllerTypes::csm_uiTR)[1]);
			eScreenTR.setAttribute("z", it->second->screenVert(raaOctaveControllerTypes::csm_uiTR)[2]);

			eScreenMOD.setAttribute("rot", it->second->rotation());
			eScreenMOD.setAttribute("flipx", it->second->flipped(0) ? "true" : "false");
			eScreenMOD.setAttribute("flipy", it->second->flipped(1) ? "true" : "false");
			eScreenMOD.setAttribute("flipz", it->second->flipped(2) ? "true" : "false");

			eScreenWindow.setAttribute("x", it->second->window(0));
			eScreenWindow.setAttribute("y", it->second->window(1));
			eScreenWindow.setAttribute("width", it->second->window(2));
			eScreenWindow.setAttribute("height", it->second->window(3));
		}

		for (raaVRPNClients::iterator it = clients().begin(); it != clients().end(); it++)
		{
			QDomElement eTracker = doc.createElement("TRACKER");
			eDocElement.appendChild(eTracker);

			eTracker.setAttribute("name", it->first.c_str());
			eTracker.setAttribute("type", it->second->type().c_str());
			eTracker.setAttribute("address", it->second->tracker().c_str());
			eTracker.setAttribute("pollTiime", it->second->pollTime());
			eTracker.setAttribute("eyeSensor", it->second->eyeTracker());
			eTracker.setAttribute("activeSensors", it->second->activeSensors());

			QDomElement eTrackerPos = doc.createElement("POS");
			QDomElement eTrackerUp = doc.createElement("UP");
			QDomElement eTrackerDir = doc.createElement("DIR");

			eTracker.appendChild(eTrackerPos);
			eTracker.appendChild(eTrackerUp);
			eTracker.appendChild(eTrackerDir);

			eTrackerPos.setAttribute("x", it->second->trackerPosition()[0]);
			eTrackerPos.setAttribute("y", it->second->trackerPosition()[1]);
			eTrackerPos.setAttribute("z", it->second->trackerPosition()[2]);

			eTrackerUp.setAttribute("x", it->second->trackerUp()[0]);
			eTrackerUp.setAttribute("y", it->second->trackerUp()[1]);
			eTrackerUp.setAttribute("z", it->second->trackerUp()[2]);

			eTrackerDir.setAttribute("x", it->second->trackerDirection()[0]);
			eTrackerDir.setAttribute("y", it->second->trackerDirection()[1]);
			eTrackerDir.setAttribute("z", it->second->trackerDirection()[2]);
		}

		std::cout << "write conf-> " << sName.toStdString() << std::endl;

		QFile file(sConfig);
		if(file.open(QIODevice::WriteOnly))
		{
			const int IndentSize = 4;

			QTextStream out(&file);
			doc.save(out, IndentSize);
			file.close();
		}
	}
}

bool raaOctaveController::hasConfig()
{
	return m_bConfig;
}

void raaOctaveController::addListener(raaOctaveControllerListener* pListener)
{
	if (pListener && std::find(m_lListener.begin(), m_lListener.end(), pListener) == m_lListener.end()) m_lListener.push_back(pListener);
}

void raaOctaveController::removeListener(raaOctaveControllerListener* pListener)
{
	if (pListener && std::find(m_lListener.begin(), m_lListener.end(), pListener) != m_lListener.end()) m_lListener.remove(pListener);
}


raaOctaveViewPoint* raaOctaveController::viewpoint()
{
	return &m_ViewPoint;
}

void raaOctaveController::update()
{

}

raaStringScreenList raaOctaveController::getScreenNames()
{
	raaStringScreenList l;
	for (raaStringScreenMap::iterator it = m_mScreens.begin(); it != m_mScreens.end(); it++) l.push_back(it->first);
	return l;
}

raaScreen* raaOctaveController::getScreen(std::string sName)
{
	if (sName.length() && m_mScreens.find(sName) != m_mScreens.end()) return m_mScreens[sName];
	return 0;
}

const raaStringScreenMap& raaOctaveController::getScreens()
{
	return m_mScreens;
}

const raaDisplayScreensMap& raaOctaveController::getDisplays()
{
	return m_mDisplays;
}

void raaOctaveController::updatedSensors(raaVRPNClient* pClient)
{
}

void raaOctaveController::updatedOrigin(raaVRPNClient* pClient)
{
}

void raaOctaveController::timerSensorUpdate(raaVRPNClient* pClient)
{
}

bool raaOctaveController::addScreen(std::string sName)
{
	if (sName.length() && m_mScreens.find(sName)==m_mScreens.end())
	{
		m_mScreens[sName] = new raaScreen(sName, 0, 0, osg::Vec3f(-1.0f, 0.0f, 0.0f), osg::Vec3f(1.0f, 0.0f, 0.0f), osg::Vec3f(1.0f, 0.0f, 1.0f), osg::Vec3f(-1.0f, 0.0f, 1.0f), 0.01f, 100.0f, 0.0f, false, false, false, 0, 0, 100, 100, &m_ViewPoint);
		for (raaOctaveControllerListeners::iterator it = m_lListener.begin(); it != m_lListener.end(); it++)(*it)->screenAdded(this, m_mScreens[sName]);
		return true;
	}
	return false;
}

void raaOctaveController::updatedSensor(raaVRPNClient* pClient, unsigned uiSensor)
{
	if (m_pEyeTracker && pClient == m_pEyeTracker && (m_pEyeTracker->eyeTracker() & 1 << uiSensor))
	{
		viewpoint()->setPhysicalMatrix(pClient->sensorTransform(uiSensor));
	}
}
