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

raaScreen::raaScreen(std::string sName, osg::Vec3f vBL, osg::Vec3f vBR, osg::Vec3f vTR, osg::Vec3f vTL, raaOctaveViewPoint *pViewpoint)
{
	initialise();
	m_fNear = 0.5f;
	m_fFar = 1000.0f;
	m_pListener = 0;
	m_uiCurrentScreenUpdate = 0;
	m_uiCurrentViewpointUpdate = 0;
	m_uiScreenUpdateCount = 0;
	setName(sName);
	setScreen(vBL, vBR, vTR, vTL, pViewpoint);
	pViewpoint->addListener(this);
}

raaScreen::~raaScreen()
{
}

void raaScreen::setListener(raaScreenListener* pListener)
{
	m_pListener = pListener;
}

void raaScreen::removeListener(raaScreenListener* pListener)
{
	if (m_pListener == pListener) m_pListener = 0;
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
			if (m_pListener)m_pListener->screenChanged(this);
			break;
		case raaOctaveControllerTypes::csm_uiBR:
			m_vScreen[raaOctaveControllerTypes::csm_uiBR] = v;
			if (m_pListener)m_pListener->screenChanged(this);
			break;
		case raaOctaveControllerTypes::csm_uiTL:
			m_vScreen[raaOctaveControllerTypes::csm_uiTL] = v;
			if (m_pListener)m_pListener->screenChanged(this);
			break;
		case raaOctaveControllerTypes::csm_uiTR:
			m_vScreen[raaOctaveControllerTypes::csm_uiTR] = v;
			if (m_pListener)m_pListener->screenChanged(this);
			break;
	}
}

void raaScreen::setScreenVerts(osg::Vec3f vBL, osg::Vec3f vBR, osg::Vec3f vTL, osg::Vec3f vTR)
{
	m_vScreen[raaOctaveControllerTypes::csm_uiBL] = vBL;
	m_vScreen[raaOctaveControllerTypes::csm_uiBR] = vBR;
	m_vScreen[raaOctaveControllerTypes::csm_uiTL] = vTL;
	m_vScreen[raaOctaveControllerTypes::csm_uiTR] = vTR;
	if (m_pListener)m_pListener->screenChanged(this);
}

osg::Vec3f raaScreen::normal()
{
	return m_vNormal;
}

osg::Matrixf raaScreen::screenProjection()
{
	return m_mScreenProjection;
}

void raaScreen::physicalViewpointChanged(raaOctaveViewPoint* pViewpoint)
{
	if(pViewpoint->currentScreenUpdate()!=m_uiCurrentViewpointUpdate || m_uiCurrentScreenUpdate!=m_uiScreenUpdateCount)
	{
		m_uiCurrentViewpointUpdate = pViewpoint->currentScreenUpdate();
		m_uiCurrentScreenUpdate = m_uiScreenUpdateCount;
		calcProjectionMatrix(pViewpoint);
	}
}

void raaScreen::virtualViewpointChanged(raaOctaveViewPoint* pViewpoint)
{
}

void raaScreen::setScreen(osg::Vec3f vBL, osg::Vec3f vBR, osg::Vec3f vTR, osg::Vec3f vTL, raaOctaveViewPoint *pViewpoint)
{
	m_vScreen[raaOctaveControllerTypes::csm_uiBL] = vBL;
	m_vScreen[raaOctaveControllerTypes::csm_uiBR] = vBR;
	m_vScreen[raaOctaveControllerTypes::csm_uiTR] = vTR;
	m_vScreen[raaOctaveControllerTypes::csm_uiTL] = vTL;

	m_vEdgeNorm[raaOctaveControllerTypes::csm_uiLeft] = m_vScreen[raaOctaveControllerTypes::csm_uiBL] - m_vScreen[raaOctaveControllerTypes::csm_uiTL];
	m_vEdgeNorm[raaOctaveControllerTypes::csm_uiRight] =  m_vScreen[raaOctaveControllerTypes::csm_uiTR] - m_vScreen[raaOctaveControllerTypes::csm_uiBR];
	m_vEdgeNorm[raaOctaveControllerTypes::csm_uiTop] =  m_vScreen[raaOctaveControllerTypes::csm_uiTL] - m_vScreen[raaOctaveControllerTypes::csm_uiTR];
	m_vEdgeNorm[raaOctaveControllerTypes::csm_uiBottom]  = m_vScreen[raaOctaveControllerTypes::csm_uiBR] - m_vScreen[raaOctaveControllerTypes::csm_uiBL];


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

	if (m_pListener)m_pListener->screenChanged(this);
	m_uiScreenUpdateCount++;

	calcProjectionMatrix(pViewpoint);
}

void raaScreen::setName(std::string sName)
{
	m_sName = sName;
	if (m_pListener) m_pListener->nameChanged(this);
}

void raaScreen::calcProjectionMatrix(raaOctaveViewPoint* pViewpoint)
{
	osg::Vec3f va, pe;
	float m[16];

	pe = pe*pViewpoint->physicalMatrix();
	va = m_vScreen[raaOctaveControllerTypes::csm_uiBL] - pe;
	float d = -(m_vNormal*va);

//	m_mScreenProjection.makeFrustum((m_vEdgeNorm[raaOctaveControllerTypes::csm_uiBottom] * va)*m_fNear / d, (m_vEdgeNorm[raaOctaveControllerTypes::csm_uiBottom] * (m_vScreen[raaOctaveControllerTypes::csm_uiBR] - pe))*m_fNear / d, (m_vEdgeNorm[raaOctaveControllerTypes::csm_uiRight] * va)*m_fNear / d, (m_vEdgeNorm[raaOctaveControllerTypes::csm_uiRight] * (m_vScreen[raaOctaveControllerTypes::csm_uiTL] - pe))*m_fNear / d, m_fNear, m_fFar);

	m_mScreenProjection.makeFrustum((m_vScreenRight * va)*m_fNear / d, (m_vScreenRight * (m_vScreen[raaOctaveControllerTypes::csm_uiBR] - pe))*m_fNear / d, (m_vScreenUp * va)*m_fNear / d, (m_vScreenUp * (m_vScreen[raaOctaveControllerTypes::csm_uiTL] - pe))*m_fNear / d, m_fNear, m_fFar);

	m[0] = 1.0f;m[1] = 0.0f;m[2] = 0.0f;m[3] = 0.0f;m[4] = 0.0f;m[5] = 1.0f;m[6] = 0.0f;m[7] = 0.0f;m[8] = 0.0f;m[9] = 0.0f;m[10] = 1.0f;m[11] = 0.0f;m[12] = pe[0];m[13] = -pe[1];m[14] =- pe[2];m[15] = 1.0f;
	m_mScreenProjectionTranslation.set(m);

//	m[0] = m_vEdgeNorm[raaOctaveControllerTypes::csm_uiBottom][0]; m[1] = m_vEdgeNorm[raaOctaveControllerTypes::csm_uiBottom][1]; m[2] = m_vEdgeNorm[raaOctaveControllerTypes::csm_uiBottom][2]; m[3] = 0.0f; m[4] = -m_vNormal[0]; m[5] = -m_vNormal[1]; m[6] = -m_vNormal[2]; m[7] = 0.0f; m[8] = m_vEdgeNorm[raaOctaveControllerTypes::csm_uiRight][0]; m[9] = m_vEdgeNorm[raaOctaveControllerTypes::csm_uiRight][1]; m[10] = m_vEdgeNorm[raaOctaveControllerTypes::csm_uiRight][2]; m[11] = 0.0f; m[12] = 0.0f; m[13] = 0.0f; m[14] = 0.0f; m[15] = 1.0f;
	m[0] = m_vScreenRight[0]; m[1] = m_vScreenRight[1]; m[2] = m_vScreenRight[2]; m[3] = 0.0f; m[4] = -m_vNormal[0]; m[5] = -m_vNormal[1]; m[6] = -m_vNormal[2]; m[7] = 0.0f; m[8] = m_vScreenUp[0]; m[9] = m_vScreenUp[1]; m[10] = m_vScreenUp[2]; m[11] = 0.0f; m[12] = 0.0f; m[13] = 0.0f; m[14] = 0.0f; m[15] = 1.0f;
	m_mScreenProjectionRotation.set(m);

	m_mScreenProjection = m_mScreenProjectionTranslation*m_mScreenProjectionRotation*sm_RotationScale*m_mScreenProjection;

	if (m_pListener)m_pListener->screenMatrixChanged(this);
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
