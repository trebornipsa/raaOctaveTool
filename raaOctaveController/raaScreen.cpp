#include "raaOctaveViewPoint.h"
#include "raaOctaveControllerTypes.h"
#include "raaScreen.h"

osg::Matrixf raaScreen::sm_RotationScale;
bool raaScreen::sm_bIntialised=false;

raaScreenListener::raaScreenListener()
{

}

raaScreenListener::~raaScreenListener()
{
}

raaScreen::raaScreen(std::string sName, int iScreen, int iStereo, osg::Vec3f vBL, osg::Vec3f vBR, osg::Vec3f vTR, osg::Vec3f vTL, float fNear, float fFar, float fRot, bool bFlipX, bool bFlipY, bool bFlipZ, int iX ,int iY, int iW, int iH, raaOctaveViewPoint *pViewpoint)
{
	initialise();
	m_iStereo = iStereo;
	m_fNear = fNear;
	m_fFar = fFar;
	m_uiCurrentScreenUpdate = 0;
	m_uiCurrentViewpointUpdate = 0;
	m_uiScreenUpdateCount = 0;
	m_abFlip[0] = bFlipX;
	m_abFlip[1] = bFlipY;
	m_abFlip[2] = bFlipZ;
	m_fRotation = fRot;
	m_iScreen = iScreen;
	setName(sName);
	setScreen(vBL, vBR, vTR, vTL, pViewpoint);
	pViewpoint->addListener(this);
	m_pLastViewpoint = pViewpoint;
	m_aiWindow[0] = iX;
	m_aiWindow[1] = iY;
	m_aiWindow[2] = iW;
	m_aiWindow[3] = iH;
}

raaScreen::~raaScreen()
{
	if (m_pLastViewpoint) m_pLastViewpoint->removeListener(this);
}

void raaScreen::addListener(raaScreenListener* pListener)
{
	//m_Mutex.lock();
	if (pListener && std::find(m_lListeners.begin(), m_lListeners.end(), pListener) == m_lListeners.end()) m_lListeners.push_back(pListener);
	//m_Mutex.unlock();
}

void raaScreen::removeListener(raaScreenListener* pListener)
{
	//m_Mutex.lock();
	if (pListener && std::find(m_lListeners.begin(), m_lListeners.end(), pListener) != m_lListeners.end()) m_lListeners.remove(pListener);
	//m_Mutex.unlock();
}

std::string raaScreen::name()
{
	return m_sName;
}

osg::Vec3f raaScreen::screenVert(unsigned uiVert)
{
	return m_vScreen[uiVert];
}

void raaScreen::setScreenVert(unsigned uiVert, osg::Vec3f v)
{
	switch(uiVert)
	{
		case raaOctaveControllerTypes::csm_uiBL:
			m_vScreen[raaOctaveControllerTypes::csm_uiBL] = v;
			updateScreen(m_pLastViewpoint);
			break;
		case raaOctaveControllerTypes::csm_uiBR:
			m_vScreen[raaOctaveControllerTypes::csm_uiBR] = v;
			updateScreen(m_pLastViewpoint);
			break;
		case raaOctaveControllerTypes::csm_uiTL:
			m_vScreen[raaOctaveControllerTypes::csm_uiTL] = v;
			updateScreen(m_pLastViewpoint);
			break;
		case raaOctaveControllerTypes::csm_uiTR:
			m_vScreen[raaOctaveControllerTypes::csm_uiTR] = v;
			updateScreen(m_pLastViewpoint);
			break;
	}
}

void raaScreen::setScreenVerts(osg::Vec3f vBL, osg::Vec3f vBR, osg::Vec3f vTL, osg::Vec3f vTR)
{
	m_vScreen[raaOctaveControllerTypes::csm_uiBL] = vBL;
	m_vScreen[raaOctaveControllerTypes::csm_uiBR] = vBR;
	m_vScreen[raaOctaveControllerTypes::csm_uiTL] = vTL;
	m_vScreen[raaOctaveControllerTypes::csm_uiTR] = vTR;
	updateScreen(m_pLastViewpoint);
}

osg::Vec3f raaScreen::normal()
{
	return m_vNormal;
}

osg::Matrixf raaScreen::screenProjection()
{
	return m_mScreenProjection;
}

osg::Matrixf raaScreen::screenLeftProjection()
{
	return m_mScreenLeftProjection;
}

osg::Matrixf raaScreen::screenRightProjection()
{
	return m_mScreenRightProjection;
}

osg::Matrixf raaScreen::screenView()
{
	return m_mScreenViewMatrix;
}

osg::Matrixf raaScreen::screenLeftView()
{
	return m_mScreenLeftViewMatrix;
}

osg::Matrixf raaScreen::screenRightView()
{
	return m_mScreenRightViewMatrix;
}

float raaScreen::nearClip()
{
	return m_fNear;
}

float raaScreen::farClip()
{
	return m_fFar;
}

void raaScreen::setNearFar(float fNear, float fFar)
{
	m_fNear = fNear;
	m_fFar = fFar;

	calcProjectionMatrix(m_pLastViewpoint);
}

bool raaScreen::flipped(unsigned uiAxis)
{
	return m_abFlip[uiAxis];
}

float raaScreen::rotation()
{
	return m_fRotation;
}

void raaScreen::setFlipped(bool bX, bool bY, bool bZ)
{
	m_abFlip[0] = bX;
	m_abFlip[1] = bY;
	m_abFlip[2] = bZ;
	calcProjectionMatrix(m_pLastViewpoint);
}

void raaScreen::setRotation(float fRot)
{
	m_fRotation = fRot;
	calcProjectionMatrix(m_pLastViewpoint);
}

void raaScreen::setWindow(int iX, int iY, int iW, int iH)
{
	m_aiWindow[0] = iX;
	m_aiWindow[1] = iY;
	m_aiWindow[2] = iW;
	m_aiWindow[3] = iH;
	for (raaScreenListeners::iterator it = m_lListeners.begin(); it != m_lListeners.end(); it++) (*it)->windowChanged(this);
}

int raaScreen::window(unsigned uiParam)
{
	if (uiParam < 4) return m_aiWindow[uiParam];
	return 0;
}

float raaScreen::projParam(unsigned uiIndex)
{
	return m_afProjParam[uiIndex];
}

void raaScreen::setStereo(int iStereo)
{
	m_iStereo = iStereo;
	for (raaScreenListeners::iterator it = m_lListeners.begin(); it != m_lListeners.end(); it++) (*it)->stereoChanged(this);
}

int raaScreen::screen()
{
	return m_iScreen;
}

void raaScreen::setScreen(int iScreen)
{
	m_iStereo = iScreen;
	for (raaScreenListeners::iterator it = m_lListeners.begin(); it != m_lListeners.end(); it++) (*it)->displayChanged(this);
}

int raaScreen::stereo()
{
	return m_iStereo;
}

void raaScreen::physicalViewpointChanged(raaOctaveViewPoint* pViewpoint)
{
	m_pLastViewpoint = pViewpoint;
	if(pViewpoint->currentScreenUpdate()!=m_uiCurrentViewpointUpdate || m_uiCurrentScreenUpdate!=m_uiScreenUpdateCount)
	{
		m_uiCurrentViewpointUpdate = pViewpoint->currentScreenUpdate();
		m_uiCurrentScreenUpdate = m_uiScreenUpdateCount;
		calcProjectionMatrix(pViewpoint);
	}
}

void raaScreen::virtualViewpointChanged(raaOctaveViewPoint* pViewpoint)
{
	m_pLastViewpoint = pViewpoint;
}

void raaScreen::setScreen(osg::Vec3f vBL, osg::Vec3f vBR, osg::Vec3f vTR, osg::Vec3f vTL, raaOctaveViewPoint *pViewpoint)
{
	m_pLastViewpoint = pViewpoint;

	m_vScreen[raaOctaveControllerTypes::csm_uiBL] = vBL;
	m_vScreen[raaOctaveControllerTypes::csm_uiBR] = vBR;
	m_vScreen[raaOctaveControllerTypes::csm_uiTR] = vTR;
	m_vScreen[raaOctaveControllerTypes::csm_uiTL] = vTL;

	updateScreen(pViewpoint);
}

void raaScreen::setName(std::string sName)
{
	m_sName = sName;
	for (raaScreenListeners::iterator it = m_lListeners.begin(); it != m_lListeners.end(); it++) (*it)->nameChanged(this);
}

void raaScreen::calcProjectionMatrix(raaOctaveViewPoint* pViewpoint)
{
	m_pLastViewpoint = pViewpoint;
	osg::Vec3f va, vb, vc, pe;
	float d;
	m_mScreenViewMatrix.makeLookAt(pe, pe - m_vNormal, m_vScreenUp);
	osg::Matrixf mS, mR;
	mS.makeScale(m_abFlip[0] ? -1.0 : 1.0, m_abFlip[1] ? -1.0 : 1.0, m_abFlip[2] ? -1.0 : 1.0);
	mR.makeRotate(osg::DegreesToRadians(m_fRotation), osg::Vec3f(0.0f, 1.0f, 0.0f));

	if (!m_iStereo)
	{
		pe = pe*pViewpoint->physicalMatrix();
		va = m_vScreen[raaOctaveControllerTypes::csm_uiBL] - pe;
		vb = m_vScreen[raaOctaveControllerTypes::csm_uiBR] - pe;
		vc = m_vScreen[raaOctaveControllerTypes::csm_uiTL] - pe;
		d = -(m_vNormal*va);

		osg::Matrixf mS, mR;
		mS.makeScale(m_abFlip[0] ? -1.0 : 1.0, m_abFlip[1] ? -1.0 : 1.0, m_abFlip[2] ? -1.0 : 1.0);
		mR.makeRotate(osg::DegreesToRadians(m_fRotation), osg::Vec3f(0.0f, 1.0f, 0.0f));

		m_mScreenProjection.makeFrustum((m_vScreenRight * va)*m_fNear / d, (m_vScreenRight * vb)*m_fNear / d, (m_vScreenUp * va)*m_fNear / d, (m_vScreenUp * vc)*m_fNear / d, m_fNear, m_fFar);
		m_mScreenProjection = mR*m_mScreenProjection*mS;
		m_mScreenViewMatrix.makeLookAt(pe, pe - m_vNormal, m_vScreenUp);
	}
	else
	{
		osg::Vec3f peL, peR;

		peL = peL*pViewpoint->physicalLeftMatrix();
		va = m_vScreen[raaOctaveControllerTypes::csm_uiBL] - peL;
		vb = m_vScreen[raaOctaveControllerTypes::csm_uiBR] - peL;
		vc = m_vScreen[raaOctaveControllerTypes::csm_uiTL] - peL;
		d = -(m_vNormal*va);
		m_mScreenLeftProjection.makeFrustum((m_vScreenRight * va)*m_fNear / d, (m_vScreenRight * vb)*m_fNear / d, (m_vScreenUp * va)*m_fNear / d, (m_vScreenUp * vc)*m_fNear / d, m_fNear, m_fFar);
		m_mScreenLeftProjection = mR*m_mScreenLeftProjection*mS;
		m_mScreenLeftViewMatrix.makeLookAt(peL, peL - m_vNormal, m_vScreenUp);

		peR = peR*pViewpoint->physicalRightMatrix();
		va = m_vScreen[raaOctaveControllerTypes::csm_uiBL] - peR;
		vb = m_vScreen[raaOctaveControllerTypes::csm_uiBR] - peR;
		vc = m_vScreen[raaOctaveControllerTypes::csm_uiTL] - peR;
		d = -(m_vNormal*va);
		m_mScreenRightProjection.makeFrustum((m_vScreenRight * va)*m_fNear / d, (m_vScreenRight * vb)*m_fNear / d, (m_vScreenUp * va)*m_fNear / d, (m_vScreenUp * vc)*m_fNear / d, m_fNear, m_fFar);
		m_mScreenRightProjection = mR*m_mScreenRightProjection*mS;
		m_mScreenRightViewMatrix.makeLookAt(peR, peR - m_vNormal, m_vScreenUp);
	}
	for (raaScreenListeners::iterator it = m_lListeners.begin(); it != m_lListeners.end(); it++) (*it)->screenProjMatrixChanged(this);
}

/* safe version
void raaScreen::calcProjectionMatrix(raaOctaveViewPoint* pViewpoint)
{
	m_pLastViewpoint = pViewpoint;

	osg::Vec3f va, vb, vc, pe;
	pe = pe*pViewpoint->physicalMatrix();
	va = m_vScreen[raaOctaveControllerTypes::csm_uiBL] - pe;
	vb = m_vScreen[raaOctaveControllerTypes::csm_uiBR] - pe;
	vc = m_vScreen[raaOctaveControllerTypes::csm_uiTL] - pe;
	float d = -(m_vNormal*va);

	m_mScreenProjection.makeFrustum((m_vScreenRight * va)*m_fNear / d, (m_vScreenRight * vb)*m_fNear / d, (m_vScreenUp * va)*m_fNear / d, (m_vScreenUp * vc)*m_fNear / d, m_fNear, m_fFar);
	m_mScreenViewMatrix.makeLookAt(pe, pe - m_vNormal, m_vScreenUp);

	for (raaScreenListeners::iterator it = m_lListeners.begin(); it != m_lListeners.end(); it++) (*it)->screenProjMatrixChanged(this);
}
*/

void raaScreen::initialise()
{
	if(!sm_bIntialised)
	{		
		osg::Matrix mR, mS;
		mR.makeRotate(osg::DegreesToRadians(90.0f), osg::Vec3f(1.0f, 0.0f, 0.0f));
		mS.makeScale(-1.0f, -1.0f, -1.0f);
		sm_RotationScale = mR*mS;
		sm_bIntialised = true;
	}
}

void raaScreen::updateScreen(raaOctaveViewPoint *pViewpoint)
{

	m_vEdgeNorm[raaOctaveControllerTypes::csm_uiLeft] = m_vScreen[raaOctaveControllerTypes::csm_uiBL] - m_vScreen[raaOctaveControllerTypes::csm_uiTL];
	m_vEdgeNorm[raaOctaveControllerTypes::csm_uiRight] = m_vScreen[raaOctaveControllerTypes::csm_uiTR] - m_vScreen[raaOctaveControllerTypes::csm_uiBR];
	m_vEdgeNorm[raaOctaveControllerTypes::csm_uiTop] = m_vScreen[raaOctaveControllerTypes::csm_uiTL] - m_vScreen[raaOctaveControllerTypes::csm_uiTR];
	m_vEdgeNorm[raaOctaveControllerTypes::csm_uiBottom] = m_vScreen[raaOctaveControllerTypes::csm_uiBR] - m_vScreen[raaOctaveControllerTypes::csm_uiBL];


	m_vScreenUp = m_vEdgeNorm[raaOctaveControllerTypes::csm_uiRight] - m_vEdgeNorm[raaOctaveControllerTypes::csm_uiLeft];
	m_vScreenRight = m_vEdgeNorm[raaOctaveControllerTypes::csm_uiBottom] - m_vEdgeNorm[raaOctaveControllerTypes::csm_uiTop];

	m_vEdgeNorm[raaOctaveControllerTypes::csm_uiLeft].normalize();
	m_vEdgeNorm[raaOctaveControllerTypes::csm_uiRight].normalize();
	m_vEdgeNorm[raaOctaveControllerTypes::csm_uiTop].normalize();
	m_vEdgeNorm[raaOctaveControllerTypes::csm_uiBottom].normalize();
	m_vScreenRight.normalize();
	m_vScreenUp.normalize();

	m_vNormal = m_vEdgeNorm[raaOctaveControllerTypes::csm_uiBottom] ^ (m_vEdgeNorm[raaOctaveControllerTypes::csm_uiLeft] * -1.0f);
	m_vNormal.normalize();

	//m_Mutex.lock();
	for (raaScreenListeners::iterator it = m_lListeners.begin(); it != m_lListeners.end(); it++) (*it)->screenChanged(this);
	//m_Mutex.unlock();

	m_uiScreenUpdateCount++;

	if(pViewpoint) calcProjectionMatrix(pViewpoint);

}
