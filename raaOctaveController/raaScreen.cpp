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

raaScreen::raaScreen(std::string sName, osg::Vec3f vBL, osg::Vec3f vBR, osg::Vec3f vTR, osg::Vec3f vTL, float fNear, float fFar, float fRot, bool bFlipX, bool bFlipY, bool bFlipZ, int iX ,int iY, int iH, int iW, raaOctaveViewPoint *pViewpoint)
{
	initialise();
	m_fNear = fNear;
	m_fFar = fFar;
	m_uiCurrentScreenUpdate = 0;
	m_uiCurrentViewpointUpdate = 0;
	m_uiScreenUpdateCount = 0;
	m_abFlip[0] = bFlipX;
	m_abFlip[1] = bFlipY;
	m_abFlip[2] = bFlipZ;
	m_fRotation = fRot;
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
}

void raaScreen::addListener(raaScreenListener* pListener)
{
	if (pListener && std::find(m_lListeners.begin(), m_lListeners.end(), pListener) == m_lListeners.end()) m_lListeners.push_back(pListener);
}

void raaScreen::removeListener(raaScreenListener* pListener)
{
	if (pListener && std::find(m_lListeners.begin(), m_lListeners.end(), pListener) != m_lListeners.end()) m_lListeners.remove(pListener);
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

osg::Matrixf raaScreen::screenView()
{
	return m_mScreenViewMatrix;
}

float raaScreen::near()
{
	return m_fNear;
}

float raaScreen::far()
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
	float m[16];

	pe = pe*pViewpoint->physicalMatrix();
//	pe = -pe;
//	pe.set(0.0f, 0.0f, 0.0f);
	va = m_vScreen[raaOctaveControllerTypes::csm_uiBL] - pe;
	vb = m_vScreen[raaOctaveControllerTypes::csm_uiBR] - pe;
	vc = m_vScreen[raaOctaveControllerTypes::csm_uiTL] - pe;

	float d = -(m_vNormal*va);

	m_mScreenProjection.makeFrustum((m_vScreenRight * va)*m_fNear / d,
		(m_vScreenRight * vb)*m_fNear / d,
		(m_vScreenUp * va)*m_fNear / d,
		(m_vScreenUp * vc)*m_fNear / d,
		m_fNear,
		m_fFar);
/*
	m_afProjParam[0] = (m_vScreenRight * va)*m_fNear / d;
	m_afProjParam[1] = (m_vScreenRight * (m_vScreen[raaOctaveControllerTypes::csm_uiBR] - pe))*m_fNear / d;
	m_afProjParam[2] = (m_vScreenUp * va)*m_fNear / d;
	m_afProjParam[3] = (m_vScreenUp * (m_vScreen[raaOctaveControllerTypes::csm_uiTL] - pe))*m_fNear / d;
	m_afProjParam[4] = m_fNear;
	m_afProjParam[5] = m_fFar;
*/
/*
	m[0] = 1.0f; m[1] = 0.0f; m[2] = 0.0f; m[3] = 0.0f; m[4] = 0.0f; m[5] = 1.0f; m[6] = 0.0f; m[7] = 0.0f; m[8] = 0.0f; m[9] = 0.0f; m[10] = 1.0f; m[11] = 0.0f; m[12] = pe[0]; m[13] = -pe[1]; m[14] = -pe[2]; m[15] = 1.0f;
	m_mScreenProjectionTranslation.set(m);
	m[0] = m_vScreenRight[0]; m[1] = m_vScreenRight[1]; m[2] = m_vScreenRight[2]; m[3] = 0.0f; m[4] = -m_vNormal[0]; m[5] = -m_vNormal[1]; m[6] = -m_vNormal[2]; m[7] = 0.0f; m[8] = m_vScreenUp[0]; m[9] = m_vScreenUp[1]; m[10] = m_vScreenUp[2]; m[11] = 0.0f; m[12] = 0.0f; m[13] = 0.0f; m[14] = 0.0f; m[15] = 1.0f;
	m_mScreenProjectionRotation.set(m);

	m_mScreenProjectionImageRotation.makeRotate(osg::DegreesToRadians(m_fRotation), osg::Vec3f(0.0f, 1.0f, 0.0f));
	m_mScreenProjectionImageFlipScale.makeScale(osg::Vec3f(m_abFlip[0]?-1.0f:1.0f, m_abFlip[1] ? -1.0f : 1.0f, m_abFlip[2] ? -1.0f : 1.0f));
*/
	// with depth peeling
//	m_mScreenProjection = m_mScreenProjectionTranslation*m_mScreenProjectionRotation*m_mScreenProjectionImageRotation*m_mScreenProjectionImageFlipScale*sm_RotationScale*m_mScreenProjection;
//	m_mScreenViewMatrix = m_mScreenProjectionTranslation*m_mScreenProjectionRotation*m_mScreenProjectionImageRotation*m_mScreenProjectionImageFlipScale*sm_RotationScale;

	// without depth peeling
//	m_mScreenViewMatrix = m_mScreenProjectionRotation*m_mScreenProjectionTranslation;

	m[0] = 1.0f; 
	m[1] = 0.0f; 
	m[2] = 0.0f; 
	m[3] = 0.0f; 
	m[4] = 0.0f; 
	m[5] = 1.0f; 
	m[6] = 0.0f; 
	m[7] = 0.0f; 
	m[8] = 0.0f; 
	m[9] = 0.0f;
	m[10] = 1.0f;
	m[11] = 0.0f; 
	m[12] = -pe[0]; 
	m[13] = -pe[1]; 
	m[14] = -pe[2]; 
	m[15] = 1.0f;
	m_mScreenProjectionTranslation.set(m);

	m[0] = m_vScreenRight[0]; 
	m[1] = m_vScreenRight[1]; 
	m[2] = m_vScreenRight[2]; 
	m[3] = 0.0f; 
	m[4] = -m_vNormal[0]; 
	m[5] = -m_vNormal[1]; 
	m[6] = -m_vNormal[2]; 
	m[7] = 0.0f; 
	m[8] = m_vScreenUp[0]; 
	m[9] = m_vScreenUp[1]; 
	m[10] = m_vScreenUp[2]; 
	m[11] = 0.0f; 
	m[12] = 0.0f; 
	m[13] = 0.0f; 
	m[14] = 0.0f; 
	m[15] = 1.0f;
/*
	m[0] = 0.0f;		m[1] = 0.0f;		m[2] = 1.0f;		m[3] = 0.0f;
	m[4] = 0.0f;		m[5] = 1.0f;		m[6] = 0.0f;		m[7] = 0.0f;
	m[8] = -1.0f;		m[9] = 0.0f;		m[10] = 0.0f;	m[11] = 0.0f;
	m[12] = 0.0f;	m[13] = 0.0f;	m[14] = 0.0f;	m[15] = 1.0f;
*/

	m_mScreenProjectionRotation.set(m);
//	m_mScreenProjectionRotation.makeRotate(osg::DegreesToRadians(-90.0f), osg::Vec3f(1.0f, 0.0f, 0.0f));

	osg::Matrixf mR, mT, mTO;
//	mT.makeLookAt(-pe, -m_vNormal+pe, m_vScreenUp);

//	osg::Vec3f e(pe[0], pe[2], -pe[1]);
	osg::Vec3f e(0.0f, 0.0f, 0.0f);
	osg::Vec3f n(m_vNormal[0], m_vNormal[1], m_vNormal[2]);
	osg::Vec3f o=n;
	osg::Vec3f u(m_vScreenUp[0], m_vScreenUp[1], m_vScreenUp[2]);

	mR.makeLookAt(e, -n, u);

//	mR.makeRotate(-90.0f, osg::Vec3f(1.0f, 0.0f, 0.0f));
	mT.makeTranslate(-pe[0], -pe[1], -pe[2]);
	mTO.makeTranslate(-pe[0], pe[2], -pe[1]);

	osg::Matrixf mIR, mIT;
	mIR.invert_4x4(mR);
	mIT.invert_4x4(mT);

	m_mScreenProjectionImageRotation.makeRotate(osg::DegreesToRadians(m_fRotation), osg::Vec3f(0.0f, 1.0f, 0.0f));
	m_mScreenProjectionImageFlipScale.makeScale(osg::Vec3f(m_abFlip[0] ? -1.0f : 1.0f, m_abFlip[1] ? -1.0f : 1.0f, m_abFlip[2] ? -1.0f : 1.0f));

	m_mScreenProjection = m_mScreenProjection;
	m_mScreenViewMatrix = mT*mR;


//	m_mScreenViewMatrix = mT;

	for (raaScreenListeners::iterator it = m_lListeners.begin(); it != m_lListeners.end(); it++) (*it)->screenProjMatrixChanged(this);
}

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

	for (raaScreenListeners::iterator it = m_lListeners.begin(); it != m_lListeners.end(); it++) (*it)->screenChanged(this);

	m_uiScreenUpdateCount++;

	if(pViewpoint) calcProjectionMatrix(pViewpoint);

}
