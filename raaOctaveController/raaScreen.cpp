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

/*
m_avScreen[csm_uiScreenRight]=m_avScreen[csm_uiScreenBR]-m_avScreen[csm_uiScreenBL];
m_avScreen[csm_uiScreenUp]=m_avScreen[csm_uiScreenTL]-m_avScreen[csm_uiScreenBL];

m_avScreen[csm_uiScreenRight].normalize();
m_avScreen[csm_uiScreenUp].normalize();

m_avScreen[csm_uiScreenNormal]=m_avScreen[csm_uiScreenUp]^m_avScreen[csm_uiScreenRight];
m_avScreen[csm_uiScreenNormal].normalize();

osg::Vec3d vPos=sm_avPhysical[csm_uiCamEye]-m_avScreen[csm_uiScreenBL];

double dL, dR, dT, dB, dD, dW, dH;

dD=vPos*m_avScreen[csm_uiScreenNormal];
dW=(m_avScreen[csm_uiScreenBR]-m_avScreen[csm_uiScreenBL]).length();
dH=(m_avScreen[csm_uiScreenTL]-m_avScreen[csm_uiScreenBL]).length();
dL=vPos*m_avScreen[csm_uiScreenRight];
dB=vPos*m_avScreen[csm_uiScreenUp];
dT=dH-dB;
dR=dW-dL;

m_adView[csm_uiViewLeft]=-(dL*m_adView[csm_uiViewNear])/dD;
m_adView[csm_uiViewRight]=(dR*m_adView[csm_uiViewNear])/dD;
m_adView[csm_uiViewTop]=(dT*m_adView[csm_uiViewNear])/dD;
m_adView[csm_uiViewBottom]=-(dB*m_adView[csm_uiViewNear])/dD;

m_mPersp.makeFrustum(m_adView[csm_uiViewLeft], m_adView[csm_uiViewRight], m_adView[csm_uiViewBottom], m_adView[csm_uiViewTop], m_adView[csm_uiViewNear], m_adView[csm_uiViewFar]);

osg::Matrixd mTrans, mRot;

mRot.set(m_avScreen[csm_uiScreenRight][0], m_avScreen[csm_uiScreenRight][1], m_avScreen[csm_uiScreenRight][2], 0.0,
m_avScreen[csm_uiScreenUp][0], m_avScreen[csm_uiScreenUp][1], m_avScreen[csm_uiScreenUp][2], 0.0,
m_avScreen[csm_uiScreenNormal][0], m_avScreen[csm_uiScreenNormal][1], m_avScreen[csm_uiScreenNormal][2], 0.0,
0.0, 0.0, 0.0, 1.0);


mTrans.makeTranslate(osg::Vec3d(-sm_avPhysical[csm_uiCamEye][0], -sm_avPhysical[csm_uiCamEye][1], -sm_avPhysical[csm_uiCamEye][2]));
mTrans=osg::Matrixd::inverse(mTrans);

m_mScreenRotation=mRot*mTrans;
*/

void raaScreen::calcProjectionMatrix(raaOctaveViewPoint* pViewpoint)
{
/*
	osg::Vec3f vPos = osg::Vec3f(0.0f, 0.0f, 1.5f) - m_vScreen[raaOctaveControllerTypes::csm_uiBL];

	float fL, fR, fT, fB, fD, fW, fH;

	fD = vPos*m_vNormal;
	fW = (m_vScreen[raaOctaveControllerTypes::csm_uiBR] - m_vScreen[raaOctaveControllerTypes::csm_uiBL]).length();
	fH = (m_vScreen[raaOctaveControllerTypes::csm_uiTL] - m_vScreen[raaOctaveControllerTypes::csm_uiBL]).length();
	fL = vPos*m_vEdgeNorm[raaOctaveControllerTypes::csm_uiBottom];
	fB = vPos*m_vEdgeNorm[raaOctaveControllerTypes::csm_uiRight];
	fT = fH - fB;
	fR = fW - fL;

	float fViewLeft = -(fL*0.01f) / fD;
	float fViewRight = (fR*0.01f) / fD;
	float fViewTop = (fT*0.01f) / fD;
	float fViewBottom = -(fB*0.01f) / fD;

	m_mScreenProjection.makeFrustum(fViewLeft, fViewRight, fViewBottom, fViewTop, 0.01f, 100.0f);

	osg::Matrixf mTrans, mRot, mScreen;



	mRot.set(
		m_vEdgeNorm[raaOctaveControllerTypes::csm_uiBottom][0], m_vEdgeNorm[raaOctaveControllerTypes::csm_uiBottom][1], m_vEdgeNorm[raaOctaveControllerTypes::csm_uiBottom][2], 0.0,
		m_vEdgeNorm[raaOctaveControllerTypes::csm_uiRight][0], m_vEdgeNorm[raaOctaveControllerTypes::csm_uiRight][1], m_vEdgeNorm[raaOctaveControllerTypes::csm_uiRight][2], 0.0,
		m_vNormal[0], m_vNormal[1], m_vNormal[2], 0.0,
		0.0, 0.0, 0.0, 1.0);
*/

/*
osg::Vec3f va, vb, vc, vr, vu, vn;
osg::Vec3f pa, pb, pc, pe;

pa.set(m_vScreen[raaOctaveControllerTypes::csm_uiBL][0], -m_vScreen[raaOctaveControllerTypes::csm_uiBL][2], m_vScreen[raaOctaveControllerTypes::csm_uiBL][1]);
pb.set(m_vScreen[raaOctaveControllerTypes::csm_uiBR][0], -m_vScreen[raaOctaveControllerTypes::csm_uiBR][2], m_vScreen[raaOctaveControllerTypes::csm_uiBR][1]);
pc.set(m_vScreen[raaOctaveControllerTypes::csm_uiTL][0], -m_vScreen[raaOctaveControllerTypes::csm_uiTL][2], m_vScreen[raaOctaveControllerTypes::csm_uiTL][1]);
pe = pe*pViewpoint->physicalMatrix();
pe.set(pe[0], -pe[2], pe[1]);
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

float d = -(vn*va);
float l = (vr * va) * n / d;
float r = (vr * vb) * n / d;
float b = (vu * va) * n / d;
float t = (vu * vc) * n / d;

float m[16];

m[0] = (2.0f*n) / (r - l);
m[1] = 0.0f;
m[2] = (r + l) / (r - l);
m[3] = 0.0f;

m[4] = 0.0f;
m[5] = (2.0f*n) / (t - b);
m[6] = (t + b) /( t - b);
m[7] = 0.0f;

m[8] = 0.0f;
m[9] = 0.0f;
m[10] = -(f + n) / (f - n);
m[11] = -(2.0f*f*n) / (f - n);

m[12] = 0.0f;
m[13] = 0.0f;
m[14] = -1.0f;
m[15] = 0.0f;
*/
/*
	osg::Vec3f pe(0.0f, 0.0f, 0.0f), va, vb, vc;

	pe = pe*pViewpoint->physicalMatrix();
	va = m_vScreen[raaOctaveControllerTypes::csm_uiBL] - pe;
	vb = m_vScreen[raaOctaveControllerTypes::csm_uiBR] - pe;
	vc = m_vScreen[raaOctaveControllerTypes::csm_uiTL] - pe;
	float d = -(m_vNormal*va);
	float l = m_vEdgeNorm[raaOctaveControllerTypes::csm_uiBottom] * va *m_fNear / d;
	float r = m_vEdgeNorm[raaOctaveControllerTypes::csm_uiBottom] * vb * m_fNear / d;
	float b = m_vEdgeNorm[raaOctaveControllerTypes::csm_uiRight] * va * m_fNear / d;
	float t = m_vEdgeNorm[raaOctaveControllerTypes::csm_uiRight] * vc * m_fNear / d;
*/
	float m[16];
/*
	// ogl way
	m[0] = (2.0f*m_fNear) / (r - l);
	m[1] = 0.0f;
	m[2] = (r + l) / (r - l);
	m[3] = 0.0f;

	m[4] = 0.0f;
	m[5] = (2.0f*m_fNear) / (t - b);
	m[6] = (t + b) /( t - b);
	m[7] = 0.0f;

	m[8] = 0.0f;
	m[9] = 0.0f;
	m[10] = -(m_fFar + m_fNear) / (m_fFar - m_fNear);
	m[11] = -(2.0f*m_fFar*m_fNear) / (m_fFar - m_fNear);

	m[12] = 0.0f;
	m[13] = 0.0f;
	m[14] = -1.0f;
	m[15] = 0.0f;
*/

	osg::Vec3f va, vb, vc, vr, vu, vn;
	osg::Vec3f pa, pb, pc, pe;

	pa.set(m_vScreen[raaOctaveControllerTypes::csm_uiBL][0], m_vScreen[raaOctaveControllerTypes::csm_uiBL][1], m_vScreen[raaOctaveControllerTypes::csm_uiBL][2]);
	pb.set(m_vScreen[raaOctaveControllerTypes::csm_uiBR][0], m_vScreen[raaOctaveControllerTypes::csm_uiBR][1], m_vScreen[raaOctaveControllerTypes::csm_uiBR][2]);
	pc.set(m_vScreen[raaOctaveControllerTypes::csm_uiTL][0], m_vScreen[raaOctaveControllerTypes::csm_uiTL][1], m_vScreen[raaOctaveControllerTypes::csm_uiTL][2]);
	pe = pe*pViewpoint->physicalMatrix();
//	pe.set(pe[0], -pe[2], pe[1]);
//	pe.set(0.0f, 0.0f, 1.5f);
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

/*
	float d = 2.0f;
	float l = -0.5f*n/d;
	float r = 0.5f*n / d;
	float t = 0.25f*n / d;
	float b = -0.75f*n / d;
*/

/*	
	// ogl way
	m[0] = (2.0f*n) / (r - l);
	m[1] = 0.0f;
	m[2] = (r + l) / (r - l);
	m[3] = 0.0f;

	m[4] = 0.0f;
	m[5] = (2.0f*n) / (t - b);
	m[6] = (t + b) /( t - b);
	m[7] = 0.0f;

	m[8] = 0.0f;
	m[9] = 0.0f;
	m[10] = -(f + n) / (f - n);
	m[11] = -(2.0f*f*n) / (f - n);

	m[12] = 0.0f;
	m[13] = 0.0f;
	m[14] = -1.0f;
	m[15] = 0.0f;
*/	


/*
	// dx way lh
	m[0] = 2.0f*n / (r - l);
	m[1] = 0.0f;
	m[2] = 0.0f;
	m[3] = 0.0f;

	m[4] = 0.0f;
	m[5] = 2.0f*n / (t - b);
	m[6] = 0.0f;
	m[7] = 0.0f;

	m[8] = (l+r)/(l-r);
	m[9] = (t+b)/(b-t);
	m[10] = f/ (f - n);
	m[11] = 1.0f;

	m[12] = 0.0f;
	m[13] = 0.0f;
	m[14] = n*f/(n-f);
	m[15] = 0.0f;
*/

/*
	// dx way lh - transpose
	m[0] = 2.0f*n / (r - l);
	m[4] = 0.0f;
	m[8] = 0.0f;
	m[12] = 0.0f;

	m[1] = 0.0f;
	m[5] = 2.0f*n / (t - b);
	m[9] = 0.0f;
	m[13] = 0.0f;

	m[2] = (l+r)/(l-r);
	m[6] = (t+b)/(b-t);
	m[10] = f/ (f - n);
	m[14] = 1.0f;

	m[3] = 0.0f;
	m[7] = 0.0f;
	m[11] = n*f/(n-f);
	m[15] = 0.0f;
*/


/*
// dx way rh
	m[0] = 2.0f*n / (r - l);
	m[1] = 0.0f;
	m[2] = 0.0f;
	m[3] = 0.0f;

	m[4] = 0.0f;
	m[5] = 2.0f*n / (t - b);
	m[6] = 0.0f;
	m[7] = 0.0f;

	m[8] = (l + r) / (r-l);
	m[9] = (t + b) / (t-b);
	m[10] = f / (f - n);
	m[11] = -1.0f;

	m[12] = 0.0f;
	m[13] = 0.0f;
	m[14] = n*f / (n - f);
	m[15] = 0.0f;
*/


/*
	m[0] = (2.0f*m_fNear) / (r - l);
	m[1] = 0.0f;
	m[2] = (r + l) / (r - l);
	m[3] = 0.0f;

	m[4] = -0.0f;
	m[5] = -0.0f;
	m[6] = (m_fFar + m_fNear) / (m_fFar - m_fNear);
	m[7] = (2.0f*m_fFar*m_fNear) / (m_fFar - m_fNear);

	m[8] = 0.0f;
	m[9] = (2.0f*m_fNear) / (t - b);
	m[10] = (t + b) / (t - b);
	m[11] = 0.0f;

	m[12] = 0.0f;
	m[13] = 0.0f;
	m[14] = -1.0f;
	m[15] = 0.0f;
*/

//	m_mScreenProjection.set(m);
//	m_mScreenProjection = m_mScreenProjection;
	

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

//	m_mScreenProjectionRotation.tra

	m_mScreenProjection = m_mScreenProjectionTranslation*m_mScreenProjectionRotation*mR*mS*m_mScreenProjection;
//	m_mScreenProjection = m_mScreenProjection;

//	m_mScreenTransform = m_mScreenProjectionTranslation*m_mScreenProjectionRotation*mR*mS;


//	m_mScreenTransform = m_mScreenProjectionTranslation*mR*m_mScreenProjectionRotation;


//	m_mScreenProjectionTranslation.makeTranslate(osg::Vec3d(pe[0], -pe[2], pe[1]));
//	m_mScreenProjectionTranslation.makeTranslate(0.0f, 0.0f, 0.0f);
//	m_mScreenProjectionTranslation.makeTranslate(osg::Vec3d(pe[0], pe[1], pe[2]));
//	m_mScreenTransform = (mR*mS)*m_mScreenProjectionTranslation;
//	m_mScreenTransform.invert(m_mScreenTransform);
//	m_mScreenTransform = mR;

//	mTrans.makeTranslate(osg::Vec3d(-sm_avPhysical[csm_uiCamEye][0], -sm_avPhysical[csm_uiCamEye][1], -sm_avPhysical[csm_uiCamEye][2]));
//	mTrans.makeTranslate(osg::Vec3d(-0.0f, -0.0f, -1.5f));
/*
	osg::Vec3f v(0.0f, 0.0f, 0.0f);
	v = v*pViewpoint->physicalMatrix();
	mTrans.makeTranslate(v);

//	mTrans = osg::Matrixd::inverse(mTrans);
	mRot = osg::Matrixd::inverse(mRot);

//	m_mScreenTransform = mRot*mTrans;
	m_mScreenTransform.makeIdentity();

	//	m_mScreenProjection = m_mScreenTransform*m_mScreenProjection;
	m_mScreenProjection = m_mScreenProjection*mRot;

	if (m_pListener)m_pListener->screenMatrixChanged(this);

*/
/*
	osg::Vec3f pa, pb, pc, pe(0.0f, 0.0f, 0.0f);

	pa = m_vScreen[raaOctaveControllerTypes::csm_uiBL];
	pb = m_vScreen[raaOctaveControllerTypes::csm_uiBR];
	pc = m_vScreen[raaOctaveControllerTypes::csm_uiTL];
	pe = pe*pViewpoint->physicalMatrix();


	pa.set(m_vScreen[raaOctaveControllerTypes::csm_uiBL][0], m_vScreen[raaOctaveControllerTypes::csm_uiBL][1], m_vScreen[raaOctaveControllerTypes::csm_uiBL][2]);
	pb.set(m_vScreen[raaOctaveControllerTypes::csm_uiBR][0], m_vScreen[raaOctaveControllerTypes::csm_uiBR][1], m_vScreen[raaOctaveControllerTypes::csm_uiBR][2]);
	pc.set(m_vScreen[raaOctaveControllerTypes::csm_uiTL][0], m_vScreen[raaOctaveControllerTypes::csm_uiTL][1], m_vScreen[raaOctaveControllerTypes::csm_uiTL][2]);

//	pe.set(pe[0], pe[1], pe[2]);


	osg::Vec3f va, vb, vc, vr, vu, vn;
	float l, r, b, t, d;
	osg::Matrixf mR, mT;

	vr = pb - pa;
	vu = pc - pa;

	vr.normalize();
	vu.normalize();

	vn = vr^vu;
	vn.normalize();

	va = pa - pe;
	vb = pb - pe;
	vc = pc - pe;

	d = -(va*vn);
	l = (vr*va)*m_fNear / d;
	r = (vr*vb)*m_fNear / d;
	b = (vu*va)*m_fNear / d;
	t = (vu*vc)*m_fNear / d;

	m_mScreenProjection.makeFrustum(l,r,b,t, m_fNear, m_fFar);


//		m_mScreenProjection.set((2.0f*m_fNear) / (r - l), 0.0f, (r + l) / (r - l), 0.0f,
//		0.0f, (2.0f*m_fNear) / (t - b), (t + b) / (t - b), 0.0f,
//		0.0f, 0.0f, -(m_fFar + m_fNear) / (m_fFar - m_fNear), -(2.0f*m_fFar*m_fNear) / (m_fFar - m_fNear),
//		0.0f, 0.0f, -1.0f, 0.0f);


	m_mScreenProjectionRotation.set(vr[0], vr[1], vr[2], 0.0f,
		-vn[0], -vn[1], -vn[2], 0.0f,
		vu[0], vu[1], vu[2], 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f);
*/

//	m_mScreenProjectionTranslation.makeTranslate(pe[0], pe[1], pe[2]);
//	m_mScreenProjectionTranslation.makeTranslate(0.0f, 2.0f, 0.0f);
	//	m_mScreenProjection = m_mScreenProjectionTranslation*m_mScreenProjection;
//	m_mScreenProjection = m_mScreenProjectionRotation*m_mScreenProjection;

//	m_mScreenTransform = m_mScreenProjectionTranslation*m_mScreenProjectionRotation;
//	m_mScreenTransform = m_mScreenProjectionRotation;
//	m_mScreenTransform = m_mScreenProjectionTranslation;

	if (m_pListener)m_pListener->screenMatrixChanged(this);


/*
	float projectionMatrix[16];

	// width and height of viewport to display on (screen dimensions in case of fullscreen rendering)
	float ratio = ((m_fEdgeLen[raaOctaveControllerTypes::csm_uiBottom]+m_fEdgeLen[raaOctaveControllerTypes::csm_uiTop])/2.0f) / ((m_fEdgeLen[raaOctaveControllerTypes::csm_uiRight] + m_fEdgeLen[raaOctaveControllerTypes::csm_uiLeft]) / 2.0f);
	float left = -ratio;
	float right = ratio;
	float bottom = -1.0f;
	float top = 1.0f;
	float near = -1.0f;
	float far = 100.0f;
	float r_width = 1.0f / (right - left);
	float r_height = 1.0f / (top - bottom);
	float r_depth = 1.0f / (far - near);
	float x = 2.0f * (r_width);
	float y = 2.0f * (r_height);
	float z = 2.0f * (r_depth);
	float A = (right + left) * r_width;
	float B = (top + bottom) * r_height;
	float C = (far + near) * r_depth;

	float m[16];

	m[0] = x;
	m[3] = -A;
	m[5] = y;
	m[7] = -B;
	m[10] = -z;
	m[11] = -C;
	m[1] = 0.0f;
	m[2] = 0.0f;
	m[4] = 0.0f;
	m[6] = 0.0f;
	m[8] = 0.0f;
	m[9] = 0.0f;
	m[12] = 0.0f;
	m[13] = 0.0f;
	m[14] = 0.0f;
	m[15] = 1.0f;

	m_mScreenProjection.set(m);
	if (m_pListener)m_pListener->screenMatrixChanged(this);
*/
}
