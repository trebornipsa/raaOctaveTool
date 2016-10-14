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
	m_pListener = pListener;
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
						if (m_pListener) m_pListener->originChanged(this);
					}
					else if (dE.nodeName() == "SCREEN")
					{
						osg::Vec3f vBL, vBR, vTR, vTL;

						for (QDomNode n = dN.firstChild(); !n.isNull(); n = n.nextSibling())
						{
							QDomElement e = n.toElement();
							if (e.nodeName() == "BL") readVec(e, vBL);
							else if (e.nodeName() == "BR") readVec(e, vBR);
							else if (e.nodeName() == "TL") readVec(e, vTL);
							else if (e.nodeName() == "TR") readVec(e, vTR);
						}

						std::string sName;

						if (dE.hasAttribute("name")) sName = dE.attribute("name").toStdString();

						if (sName.length())
						{
							m_mScreens[sName] = new raaScreen(sName, vBL, vBR, vTR, vTL, &m_ViewPoint);
							if (m_pListener) m_pListener->screenAdded(this, m_mScreens[sName]);
						}
					}
				}
			}
		}
	}
}

void raaOctaveController::setListener(raaOctaveControllerListener* pListener)
{
	m_pListener = pListener;
}

void raaOctaveController::removeListener(raaOctaveControllerListener* pListener)
{
	if (m_pListener == pListener) m_pListener = 0;
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
