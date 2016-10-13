#include "raaOctaveViewPoint.h"
#include "raaOctaveControllerTypes.h"
#include "raaScreen.h"

raaScreenListener::raaScreenListener()
{
}

raaScreenListener::~raaScreenListener()
{
}

raaScreen::raaScreen(std::string sName, osg::Vec3f vBL, osg::Vec3f vBR, osg::Vec3f vTR, osg::Vec3f vTL, raaOctaveViewPoint *pViewpoint)
{
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

osg::Vec3f raaScreen::normal()
{
	return m_vNormal;
}

osg::Matrixf raaScreen::screenProjection()
{
	return m_mScreenProjection;
}

osg::Matrixf raaScreen::screenProjectionRotation()
{
	return m_mScreenProjectionRotation;
}

osg::Matrixf raaScreen::screenProjectionTranslation()
{
	return m_mScreenProjectionTranslation;
}

osg::Matrixf raaScreen::screenTransform()
{
	return m_mScreenTransform;
}

void raaScreen::viewpointChanged(raaOctaveViewPoint* pViewpoint)
{
	if(pViewpoint->currentScreenUpdate()!=m_uiCurrentViewpointUpdate || m_uiCurrentScreenUpdate!=m_uiScreenUpdateCount)
	{
		m_uiCurrentViewpointUpdate = pViewpoint->currentScreenUpdate();
		m_uiCurrentScreenUpdate = m_uiScreenUpdateCount;
		calcProjectionMatrix(pViewpoint);
	}
}

void raaScreen::setScreen(osg::Vec3f vBL, osg::Vec3f vBR, osg::Vec3f vTR, osg::Vec3f vTL, raaOctaveViewPoint *pViewpoint)
{
	m_vScreen[raaOctaveControllerTypes::csm_uiBL] = vBL;
	m_vScreen[raaOctaveControllerTypes::csm_uiBR] = vBR;
	m_vScreen[raaOctaveControllerTypes::csm_uiTR] = vTR;
	m_vScreen[raaOctaveControllerTypes::csm_uiTL] = vTL;

	m_vCentre = m_vScreen[0];
	m_vCentre += m_vScreen[1];
	m_vCentre += m_vScreen[2];
	m_vCentre += m_vScreen[3];
	m_vCentre /= 4.0f;

	m_vEdgeNorm[raaOctaveControllerTypes::csm_uiLeft] = m_vEdge[raaOctaveControllerTypes::csm_uiLeft] = m_vScreen[raaOctaveControllerTypes::csm_uiBL] - m_vScreen[raaOctaveControllerTypes::csm_uiTL];
	m_vEdgeNorm[raaOctaveControllerTypes::csm_uiRight] = m_vEdge[raaOctaveControllerTypes::csm_uiRight] = m_vScreen[raaOctaveControllerTypes::csm_uiTR] - m_vScreen[raaOctaveControllerTypes::csm_uiBR];
	m_vEdgeNorm[raaOctaveControllerTypes::csm_uiTop] = m_vEdge[raaOctaveControllerTypes::csm_uiTop] = m_vScreen[raaOctaveControllerTypes::csm_uiTL] - m_vScreen[raaOctaveControllerTypes::csm_uiTR];
	m_vEdgeNorm[raaOctaveControllerTypes::csm_uiBottom] = m_vEdge[raaOctaveControllerTypes::csm_uiBottom] = m_vScreen[raaOctaveControllerTypes::csm_uiBR] - m_vScreen[raaOctaveControllerTypes::csm_uiBL];

	m_fEdgeLen[raaOctaveControllerTypes::csm_uiLeft] = m_vEdgeNorm[raaOctaveControllerTypes::csm_uiLeft].normalize();
	m_fEdgeLen[raaOctaveControllerTypes::csm_uiRight] = m_vEdgeNorm[raaOctaveControllerTypes::csm_uiRight].normalize();
	m_fEdgeLen[raaOctaveControllerTypes::csm_uiTop] = m_vEdgeNorm[raaOctaveControllerTypes::csm_uiTop].normalize();
	m_fEdgeLen[raaOctaveControllerTypes::csm_uiBottom] = m_vEdgeNorm[raaOctaveControllerTypes::csm_uiBottom].normalize();

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

	float m[16];

	osg::Vec3f va, vb, vc, vr, vu, vn;
	osg::Vec3f pa, pb, pc, pe;

	pa.set(m_vScreen[raaOctaveControllerTypes::csm_uiBL][0], m_vScreen[raaOctaveControllerTypes::csm_uiBL][1], m_vScreen[raaOctaveControllerTypes::csm_uiBL][2]);
	pb.set(m_vScreen[raaOctaveControllerTypes::csm_uiBR][0], m_vScreen[raaOctaveControllerTypes::csm_uiBR][1], m_vScreen[raaOctaveControllerTypes::csm_uiBR][2]);
	pc.set(m_vScreen[raaOctaveControllerTypes::csm_uiTL][0], m_vScreen[raaOctaveControllerTypes::csm_uiTL][1], m_vScreen[raaOctaveControllerTypes::csm_uiTL][2]);
	pe = pe*pViewpoint->physicalMatrix();
	float n = m_fNear;
	float f = m_fFar;

	vr = pb - pa;
	vu = pc - pa;
	vr.normalize();
	vu.normalize();
	vn = vr^vu;
	vn.normalize();

	va = pa - pe;
	vb = pb - pe;
	vc = pc - pe;
	n = 0.1f;
	float d = -(vn*va);
	double l = (vr * va)*n/d;
	double r = (vr * vb)*n/d;
	double b =(vu * va)*n/d;
	double t = (vu * vc)*n/d;

	m_mScreenProjection.makeFrustum(l, r, b, t, n, f);

	float mt[16];
	mt[0] = 1.0f;
	mt[1] = 0.0f;
	mt[2] = 0.0f;
	mt[3] = 0.0f;

	mt[4] = 0.0f;
	mt[5] = 1.0f;
	mt[6] = 0.0f;
	mt[7] = 0.0f;

	mt[8] = 0.0f;
	mt[9] = 0.0f;
	mt[10] = 1.0f;
	mt[11] = 0.0f;

	mt[12] = -pe[0];
	mt[13] = -pe[1];
	mt[14] =- pe[2];
	mt[15] = 1.0f;

	m_mScreenProjectionTranslation.set(mt);

	float mr[16];

	mr[0] = m_vEdgeNorm[raaOctaveControllerTypes::csm_uiBottom][0];
	mr[1] = m_vEdgeNorm[raaOctaveControllerTypes::csm_uiBottom][1];
	mr[2] = m_vEdgeNorm[raaOctaveControllerTypes::csm_uiBottom][2];
	mr[3] = 0.0f;

	mr[4] = -m_vNormal[0];
	mr[5] = -m_vNormal[1];
	mr[6] = -m_vNormal[2];
	mr[7] = 0.0f;

	mr[8] = m_vEdgeNorm[raaOctaveControllerTypes::csm_uiRight][0];
	mr[9] = m_vEdgeNorm[raaOctaveControllerTypes::csm_uiRight][1];
	mr[10] = m_vEdgeNorm[raaOctaveControllerTypes::csm_uiRight][2];
	mr[11] = 0.0f;

	mr[12] = 0.0f;
	mr[13] = 0.0f;
	mr[14] = 0.0f;
	mr[15] = 1.0f;

	m_mScreenProjectionRotation.set(mr);
	osg::Matrixf mR;
	mR.makeRotate(osg::DegreesToRadians(90.0f), osg::Vec3f(1.0f, 0.0f, 0.0f));
	osg::Matrixf mS;
	mS.makeScale(-1.0f, -1.0f, -1.0f);

	m_mScreenProjection = m_mScreenProjectionTranslation*m_mScreenProjectionRotation*mR*mS*m_mScreenProjection;

	if (m_pListener)m_pListener->screenMatrixChanged(this);

}
