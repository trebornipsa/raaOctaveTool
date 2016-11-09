#include <iostream>

#include <QtXml/QDomDocument>
#include <QtXml/QDomNode>
#include <QtXml/QDomElement>
#include <QtCore/QFile>
#include <QtCore/QTextStream>

#include "raaScreen.h"
#include "raaOctaveController.h"

void readVec(QDomElement e, osg::Vec3f &v)
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

raaOctaveControllerConfigListener::raaOctaveControllerConfigListener()
{
}

raaOctaveControllerConfigListener::~raaOctaveControllerConfigListener()
{
}

raaOctaveController::raaOctaveController(raaOctaveControllerListener *pListener)
{
	m_bConfig = false;

	addListener(pListener);
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
					if (dE.nodeName() == "VIEWPOINT")
					{
						osg::Vec3f vPos, vUp, vDir;
						for (QDomNode n = dN.firstChild(); !n.isNull(); n = n.nextSibling())
						{
							QDomElement e = n.toElement();
							if (e.nodeName() == "POS") readVec(e, vPos);
							else if (e.nodeName() == "DIR") readVec(e, vDir);
							else if (e.nodeName() == "UP") readVec(e, vUp);
						}
					}
					else if (dE.nodeName() == "SCREEN")
					{
						osg::Vec3f vBL, vBR, vTR, vTL;
						float fNear = 0.01f, fFar = 100.0f, fRot=0.0f;
						bool bX=false, bY=false, bZ=false;
						int iX = 0, iY = 0, iW = 200, iH = 200;

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

						if (sName.length())
						{
							m_mScreens[sName] = new raaScreen(sName, 0, vBL, vBR, vTR, vTL, fNear, fFar, fRot, bX, bY, bZ, iX, iY, iW, iH, &m_ViewPoint);
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
		eDocElement.setAttribute("name", sName);

		QDomElement eOrigin = doc.createElement("ORIGIN");
		eDocElement.appendChild(eOrigin);
		eOrigin.setAttribute("x", m_vOrigin[0]);
		eOrigin.setAttribute("y", m_vOrigin[1]);
		eOrigin.setAttribute("z", m_vOrigin[2]);

		QDomElement eViewpoint = doc.createElement("VIEWPOINT");
		QDomElement eViewpointPos = doc.createElement("POS");
		QDomElement eViewpointUp = doc.createElement("UP");
		QDomElement eViewpointDir = doc.createElement("DIR");
		eDocElement.appendChild(eViewpoint);
		eViewpoint.appendChild(eViewpointPos);
		eViewpoint.appendChild(eViewpointUp);
		eViewpoint.appendChild(eViewpointDir);
/*
		eViewpointPos.setAttribute("x", m_ViewPoint.physicalMatrix()[12]);
		eViewpointPos.setAttribute("y", m_ViewPoint.physicalMatrix()[13]);
		eViewpointPos.setAttribute("z", m_ViewPoint.physicalMatrix()[14]);

		eViewpointDir.setAttribute("x", m_ViewPoint.physicalMatrix()[1]);
		eViewpointDir.setAttribute("y", m_ViewPoint.physicalMatrix()[5]);
		eViewpointDir.setAttribute("z", m_ViewPoint.physicalMatrix()[9]);

		eViewpointUp.setAttribute("x", m_ViewPoint.physicalMatrix()[2]);
		eViewpointUp.setAttribute("y", m_ViewPoint.physicalMatrix()[6]);
		eViewpointUp.setAttribute("z", m_ViewPoint.physicalMatrix()[10]);
*/
		for(raaStringScreenMap::iterator it; it!=m_mScreens.end();it++)
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

			eScreen.setAttribute("name", it->second->name().c_str());
			eScreen.setAttribute("screen", it->second->screen());

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

		QFile file(sName);
		if(file.open(QIODevice::WriteOnly))
		{
			const int IndentSize = 4;

//			QTextStream out(file);
//			doc.save(out, IndentSize);
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

void raaOctaveController::addConfigListener(raaOctaveControllerConfigListener* pListener)
{
	if (pListener && std::find(m_lConfigListener.begin(), m_lConfigListener.end(), pListener) == m_lConfigListener.end()) m_lConfigListener.push_back(pListener);
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
