#include <iostream>

#include <QtXml/QDomDocument>
#include <QtXml/QDomNode>
#include <QtXml/QDomElement>
#include <QtCore/QFile>

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
							m_mScreens[sName] = new raaScreen(sName, vBL, vBR, vTR, vTL, fNear, fFar, fRot, bX, bY, bZ, iX, iY, iW, iH, &m_ViewPoint);
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
