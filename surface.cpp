// Surface.cpp : Implementation of Surface
#include "stdafx.h"
//#include "VBATest.h"
#include "main.h"

/////////////////////////////////////////////////////////////////////////////
// Surface

Surface::Surface()
	{
	m_rgvT = NULL;

	m_menuid = IDR_SURFACEMENU;

	m_phitdrop = NULL;
	}

Surface::~Surface()
	{
	}

HRESULT Surface::Init(PinTable *ptable, float x, float y)
	{
	m_ptable = ptable;

	CComObject<DragPoint> *pdp;
	CComObject<DragPoint>::CreateInstance(&pdp);
	if (pdp)
		{
		pdp->AddRef();
		pdp->Init(this, x-50, y-50);
		m_vdpoint.AddElement(pdp);
		}
	CComObject<DragPoint>::CreateInstance(&pdp);
	if (pdp)
		{
		pdp->AddRef();
		pdp->Init(this, x-50, y+50);
		m_vdpoint.AddElement(pdp);
		}
	CComObject<DragPoint>::CreateInstance(&pdp);
	if (pdp)
		{
		pdp->AddRef();
		pdp->Init(this, x+50, y+50);
		m_vdpoint.AddElement(pdp);
		}
	CComObject<DragPoint>::CreateInstance(&pdp);
	if (pdp)
		{
		pdp->AddRef();
		pdp->Init(this, x+50, y-50);
		m_vdpoint.AddElement(pdp);
		}

	SetDefaults();

	return InitVBA(fTrue, 0, NULL);
	}

HRESULT Surface::InitTarget(PinTable *ptable, float x, float y)
	{
	m_ptable = ptable;

	CComObject<DragPoint> *pdp;
	CComObject<DragPoint>::CreateInstance(&pdp);
	if (pdp)
		{
		pdp->AddRef();
		pdp->Init(this, x-30, y-6);
		m_vdpoint.AddElement(pdp);
		}
	CComObject<DragPoint>::CreateInstance(&pdp);
	if (pdp)
		{
		pdp->AddRef();
		pdp->Init(this, x-30, y+6);
		pdp->m_fAutoTexture = fFalse;
		pdp->m_texturecoord = 0.0;
		m_vdpoint.AddElement(pdp);
		}
	CComObject<DragPoint>::CreateInstance(&pdp);
	if (pdp)
		{
		pdp->AddRef();
		pdp->Init(this, x+30, y+6);
		pdp->m_fAutoTexture = fFalse;
		pdp->m_texturecoord = 1.0;
		m_vdpoint.AddElement(pdp);
		}
	CComObject<DragPoint>::CreateInstance(&pdp);
	if (pdp)
		{
		pdp->AddRef();
		pdp->Init(this, x+30, y-6);
		m_vdpoint.AddElement(pdp);
		}

	SetDefaults();

	m_d.m_fHitEvent = fTrue;

	m_d.m_sidecolor = RGB(127,127,127);
	m_d.m_topcolor = RGB(127,127,127);
	m_d.m_slingshotColor = RGB(242,242,242);

	return InitVBA(fTrue, 0, NULL);
	}

void Surface::SetDefaults()
	{
	m_d.m_tdr.m_fTimerEnabled = fFalse;
	m_d.m_tdr.m_TimerInterval = 100;

	m_d.m_fHitEvent = fFalse;
	m_d.m_threshold = 1;

	m_d.m_fInner = fTrue;

	m_d.m_ia = ImageAlignCenter;
	m_d.m_sidecolor = RGB(255,255,255);
	m_d.m_szImage[0] = 0;
	m_d.m_slingshotColor = RGB(242,242,242);
	//m_d.m_topcolor = RGB(230,210,255);

	m_d.m_topcolor = RGB(63,63,63);

	m_d.m_fDroppable = fFalse;
	m_d.m_fCastsShadow = fTrue;

	m_d.m_heightbottom = 0;
	m_d.m_heighttop = 50;

	m_d.m_fDisplayTexture = fFalse;

	m_d.m_slingshotforce = 80;

	m_d.m_elasticity = 0.3f;

	m_d.m_fVisible = fTrue;
	m_d.m_fSideVisible = fTrue;
	}

/*void Surface::GetRgVertex(Vector<RenderVertex> *pvv)
	{
	//int cpointCur;
	int i;
	int cpoint;

	cpoint = m_vdpoint.Size();

	//cpointCur = 0;

	for (i=0;i<cpoint;i++)
		{
		BOOL fNoSmooth = fTrue;
		CComObject<DragPoint> *pdp0;
		CComObject<DragPoint> *pdp3;
		CComObject<DragPoint> *pdp1 = m_vdpoint.ElementAt(i);
		CComObject<DragPoint> *pdp2 = m_vdpoint.ElementAt((i+1)%cpoint);

		if (pdp1->m_fSmooth)
			{
			fNoSmooth = fFalse;
			pdp0 = m_vdpoint.ElementAt((i+cpoint-1)%cpoint);
			}
		else
			{
			pdp0 = pdp1;
			}

		if (pdp2->m_fSmooth)
			{
			fNoSmooth = fFalse;
			pdp3 = m_vdpoint.ElementAt((i+2)%cpoint);
			}
		else
			{
			pdp3 = pdp2;
			}

		CatmullCurve cc;
		cc.SetCurve(&pdp0->m_v, &pdp1->m_v, &pdp2->m_v, &pdp3->m_v);

		RenderVertex rendv1, rendv2; // Create these to add the special properties

		rendv1.x = pdp1->m_v.x;
		rendv1.y = pdp1->m_v.y;
		rendv1.fSmooth = pdp1->m_fSmooth;
		rendv1.fSlingshot = pdp1->m_fSlingshot;

		// Properties of last point don't matter, because it won't be added to the list on this pass (it'll get added as the first point of the next curve)
		rendv2.x = pdp2->m_v.x;
		rendv2.y = pdp2->m_v.y;

		RecurseSmoothLine(&cc, 0, 1, &rendv1, &rendv2, pvv);
		}
	}*/

void Surface::PreRender(Sur *psur)
	{
	//m_rgvT = GetRgVertex(&m_cvertexT);

	int cvertex;

	Vector<RenderVertex> vvertex;
	GetRgVertex(&vvertex);

	m_cvertexT = vvertex.Size();
	m_rgvT = new Vertex[m_cvertexT + 6]; // Add points so inverted polygons can be drawn

	int i;
	for (i=0;i<vvertex.Size();i++)
		{
		m_rgvT[i] = *((Vertex *)vvertex.ElementAt(i));
		delete vvertex.ElementAt(i);
		}

	psur->SetFillColor(RGB(192,192,192));

	psur->SetObject(this);

	// Don't want border color to be over-ridden when selected - that will be drawn later
	psur->SetBorderColor(-1,fFalse,0);

	if (!m_d.m_fInner)
		{
		m_rgvT[m_cvertexT].x = m_ptable->m_left;
		m_rgvT[m_cvertexT].y = m_ptable->m_top;
		m_rgvT[m_cvertexT+1].x = m_ptable->m_left;
		m_rgvT[m_cvertexT+1].y = m_ptable->m_bottom;
		m_rgvT[m_cvertexT+2].x = m_ptable->m_right;
		m_rgvT[m_cvertexT+2].y = m_ptable->m_bottom;
		m_rgvT[m_cvertexT+3].x = m_ptable->m_right;
		m_rgvT[m_cvertexT+3].y = m_ptable->m_top;
		m_rgvT[m_cvertexT+4].x = m_ptable->m_left;
		m_rgvT[m_cvertexT+4].y = m_ptable->m_top;
		m_rgvT[m_cvertexT+5].x = m_rgvT[m_cvertexT-1].x;
		m_rgvT[m_cvertexT+5].y = m_rgvT[m_cvertexT-1].y;

		//psur->Polygon(m_rgvT, m_cvertexT+6);
		cvertex = m_cvertexT + 6;
		}
	else
		{
		//psur->Polygon(m_rgvT, m_cvertexT);
		cvertex = m_cvertexT;
		}

	PinImage *ppi;
	if (m_d.m_fDisplayTexture && (ppi = m_ptable->GetImage(m_d.m_szImage)))
		{
		ppi->EnsureHBitmap();
		if (ppi->m_hbmGDIVersion)
			{
			psur->PolygonImage(m_rgvT, cvertex, ppi->m_hbmGDIVersion, m_ptable->m_left, m_ptable->m_top, m_ptable->m_right, m_ptable->m_bottom, ppi->m_width, ppi->m_height);
			}
		else
			{
			// Do nothing for now to indicate to user that there is a problem
			}
		}
	else
		{
		psur->Polygon(m_rgvT, cvertex);
		}

	/*PinImage *ppi = m_ptable->GetImage(m_d.m_szImage);

	if (ppi)
		{
		HDC hdcScreen;
		HDC hdcNew;
		HBITMAP hbmOld;
		ppi->EnsureHBitmap();
		if (ppi->m_hbmGDIVersion)
			{
			hdcScreen = GetDC(NULL);
			hdcNew = CreateCompatibleDC(hdcScreen);
			hbmOld = (HBITMAP)SelectObject(hdcNew, ppi->m_hbmGDIVersion);

			psur->Image(m_ptable->m_left, m_ptable->m_top, m_ptable->m_right, m_ptable->m_bottom, hdcNew, ppi->m_width, ppi->m_height);

			SelectObject(hdcNew, hbmOld);
			DeleteDC(hdcNew);
			ReleaseDC(NULL, hdcScreen);
			}
		}*/
	}

void Surface::Render(Sur *psur)
	{
	int		i;
	BOOL	fDrawDragpoints;		//>>> added by chris

	psur->SetFillColor(-1);
	psur->SetBorderColor(RGB(0,0,0),fFalse,0);
	psur->SetObject(this); // For selected formatting
	psur->SetObject(NULL);

	// PreRender may not have been called - for export
	if (!m_rgvT)
		{
		Vector<RenderVertex> vvertex;
		GetRgVertex(&vvertex);

		m_cvertexT = vvertex.Size();
		m_rgvT = new Vertex[m_cvertexT];

		int i;
		for (i=0;i<vvertex.Size();i++)
			{
			m_rgvT[i] = *((Vertex *)vvertex.ElementAt(i));
			delete vvertex.ElementAt(i);
			}
		//m_rgvT = GetRgVertex(&m_cvertexT);
		}

	psur->Polygon(m_rgvT, m_cvertexT);
	delete m_rgvT;
	m_rgvT = NULL;

	// if the item is selected then draw the dragpoints (or if we are always to draw dragpoints)
	if ( (m_selectstate != eNotSelected) || (g_pvp->m_fAlwaysDrawDragPoints == fTrue) )
		{
		fDrawDragpoints = fTrue;
		}
	else
		{
		// if any of the dragpoints of this object are selected then draw all the dragpoints
		fDrawDragpoints = fFalse;
		for (i=0;i<m_vdpoint.Size();i++)
			{
			CComObject<DragPoint> *pdp;
			pdp = m_vdpoint.ElementAt(i);
			if (pdp->m_selectstate != eNotSelected)
				{
				fDrawDragpoints = fTrue;
				break;
				}
			}
		}

	for (i=0;i<m_vdpoint.Size();i++)
		{
		CComObject<DragPoint> *pdp;
		pdp = m_vdpoint.ElementAt(i);
		psur->SetFillColor(-1);
		psur->SetBorderColor(RGB(255,0,0),fFalse,0);

		if (pdp->m_fDragging)
			{
			//psur->SetFillColor(RGB(0,255,0));
			psur->SetBorderColor(RGB(0,255,0),fFalse,0);
			}

		if (fDrawDragpoints == fTrue)
			{
			psur->SetObject(pdp);
			psur->Ellipse2(pdp->m_v.x, pdp->m_v.y, 8);
			}

		if (pdp->m_fSlingshot)
			{
			CComObject<DragPoint> *pdp2;

			psur->SetObject(NULL);
			pdp2 = m_vdpoint.ElementAt((i+1) % m_vdpoint.Size());

			psur->SetLineColor(RGB(0,0,0),fFalse,3);
			psur->Line(pdp->m_v.x, pdp->m_v.y, pdp2->m_v.x, pdp2->m_v.y);
			}
		}

	}

void Surface::RenderBlueprint(Sur *psur)
	{
	// Don't render dragpoints for blueprint

	psur->SetFillColor(-1);
	psur->SetBorderColor(RGB(0,0,0),fFalse,0);
	psur->SetObject(this); // For selected formatting
	psur->SetObject(NULL);

	Vector<RenderVertex> vvertex;
	GetRgVertex(&vvertex);

	m_cvertexT = vvertex.Size();
	m_rgvT = new Vertex[m_cvertexT];

	int i;
	for (i=0;i<vvertex.Size();i++)
		{
		m_rgvT[i] = *((Vertex *)vvertex.ElementAt(i));
		delete vvertex.ElementAt(i);
		}

	//m_rgvT = GetRgVertex(&m_cvertexT);

	psur->Polygon(m_rgvT, m_cvertexT);
	delete m_rgvT;
	m_rgvT = NULL;
	}

void Surface::RenderShadow(ShadowSur *psur, float height)
	{
	if ( (m_d.m_fCastsShadow != fTrue) || (m_ptable->m_fRenderShadows == fFalse) )
		return;

	/*if (m_d.m_fDroppable)
		{
		return; // Don't want an orphan shadow when the wall drops
		}*/

	psur->SetFillColor(RGB(0,0,0));
	psur->SetBorderColor(-1,fFalse,0);
	psur->SetObject(this); // For selected formatting
	psur->SetObject(NULL);

	Vector<RenderVertex> vvertex;
	GetRgVertex(&vvertex);

	m_cvertexT = vvertex.Size();
	m_rgvT = new Vertex[m_cvertexT+6];

	int i;
	for (i=0;i<vvertex.Size();i++)
		{
		m_rgvT[i] = *((Vertex *)vvertex.ElementAt(i));

		//m_rgvT[i].x += m_d.m_heightbottom;
		//m_rgvT[i].y -= m_d.m_heightbottom;

		delete vvertex.ElementAt(i);
		}

	//m_rgvT = GetRgVertex(&m_cvertexT);

	if (!m_d.m_fInner)
		{
		m_rgvT[m_cvertexT].x = m_ptable->m_left;
		m_rgvT[m_cvertexT].y = m_ptable->m_top;
		m_rgvT[m_cvertexT+1].x = m_ptable->m_left;
		m_rgvT[m_cvertexT+1].y = m_ptable->m_bottom;
		m_rgvT[m_cvertexT+2].x = m_ptable->m_right;
		m_rgvT[m_cvertexT+2].y = m_ptable->m_bottom;
		m_rgvT[m_cvertexT+3].x = m_ptable->m_right;
		m_rgvT[m_cvertexT+3].y = m_ptable->m_top;
		m_rgvT[m_cvertexT+4].x = m_ptable->m_left;
		m_rgvT[m_cvertexT+4].y = m_ptable->m_top;
		m_rgvT[m_cvertexT+5].x = m_rgvT[m_cvertexT-1].x;
		m_rgvT[m_cvertexT+5].y = m_rgvT[m_cvertexT-1].y;

		psur->PolygonSkew(m_rgvT, m_cvertexT+6, NULL, m_d.m_heightbottom, m_d.m_heighttop, fFalse);
		}
	else
		{
		psur->PolygonSkew(m_rgvT, m_cvertexT, NULL, m_d.m_heightbottom, m_d.m_heighttop, fFalse);
		}

	delete m_rgvT;
	m_rgvT = NULL;
	}

void Surface::GetTimers(Vector<HitTimer> *pvht)
	{
	IEditable::BeginPlay();

	HitTimer *pht;
	pht = new HitTimer();
	pht->m_interval = m_d.m_tdr.m_TimerInterval;
	pht->m_nextfire = pht->m_interval;
	pht->m_pfe = (IFireEvents *)this;

	m_phittimer = pht;

	if (m_d.m_tdr.m_fTimerEnabled)
		{
		pvht->AddElement(pht);
		}
	}

void Surface::GetHitShapes(Vector<HitObject> *pvho)
	{
	CurvesToShapes(pvho);

	m_fIsDropped = fFalse;
	m_fDisabled = fFalse;
	}

void Surface::GetHitShapesDebug(Vector<HitObject> *pvho)
	{
	if (!m_d.m_fInner)
		{
		Vector<RenderVertex> vvertex;
		GetRgVertex(&vvertex);

		int cvertex;
		Vertex3D *rgv3d;

		cvertex = vvertex.Size();
		rgv3d = new Vertex3D[cvertex + 5];

		int i;
		for (i=0;i<cvertex;i++)
			{
			rgv3d[i].x = vvertex.ElementAt(i)->x;
			rgv3d[i].y = vvertex.ElementAt(i)->y;
			rgv3d[i].z = m_d.m_heighttop;
			delete vvertex.ElementAt(i);
			}

		rgv3d[cvertex].x = m_ptable->m_left;
		rgv3d[cvertex].y = m_ptable->m_top;
		rgv3d[cvertex].z = m_d.m_heighttop;
		rgv3d[cvertex+1].x = m_ptable->m_left;
		rgv3d[cvertex+1].y = m_ptable->m_bottom;
		rgv3d[cvertex+1].z = m_d.m_heighttop;
		rgv3d[cvertex+2].x = m_ptable->m_right;
		rgv3d[cvertex+2].y = m_ptable->m_bottom;
		rgv3d[cvertex+2].z = m_d.m_heighttop;
		rgv3d[cvertex+3].x = m_ptable->m_right;
		rgv3d[cvertex+3].y = m_ptable->m_top;
		rgv3d[cvertex+3].z = m_d.m_heighttop;
		rgv3d[cvertex+4].x = m_ptable->m_left;
		rgv3d[cvertex+4].y = m_ptable->m_top;
		rgv3d[cvertex+4].z = m_d.m_heighttop;


		Hit3DPoly *ph3dp = new Hit3DPoly(rgv3d, cvertex+5);
		pvho->AddElement(ph3dp);

		delete rgv3d;
		}
	}

void Surface::CurvesToShapes(Vector<HitObject> *pvho)
	{
	int i;
	int count;
	RenderVertex *pv1, *pv2, *pv3, *pv4;
	//Vector<Vertex> vvertex;

	int cpoint;
	int cpointCur;

	cpoint = m_vdpoint.Size();

	cpointCur = 0;

	RenderVertex *rgv;
	Vertex3D *rgv3D;
	//rgv = GetRgRenderVertex(&count);

	{
	Vector<RenderVertex> vvertex;
	GetRgVertex(&vvertex);

	count = vvertex.Size();
	rgv = new RenderVertex[count + 6]; // Add points so inverted polygons can be drawn
	rgv3D = new Vertex3D[count + 6];

	if (m_d.m_fInner)
		{
		for (i=0;i<count;i++)
			{
			rgv3D[i].x = vvertex.ElementAt(i)->x;
			rgv3D[i].y = vvertex.ElementAt(i)->y;
			rgv3D[i].z = m_d.m_heighttop;
			}
		}

	for (i=0;i<count;i++)
		{
		rgv[i] = *vvertex.ElementAt(i);
		delete vvertex.ElementAt(i);
		}
	}

	for (i=0;i<count;i++)
		{
		pv1 = &rgv[i];
		pv2 = &rgv[(i+1) % count];
		pv3 = &rgv[(i+2) % count];
		pv4 = &rgv[(i+3) % count];

		if (m_d.m_fInner)
			{
			AddLine(pvho, pv2, pv3, pv1, pv2->fSlingshot);
			}
		else
			{
			AddLine(pvho, pv3, pv2, pv4, pv2->fSlingshot);
			}
		}

	if (m_d.m_fInner)
		{
		if (m_d.m_fDroppable)
			{
			// Special hit object that will allow us to animate the surface
			m_phitdrop = new Hit3DPolyDrop(rgv3D,count);
			m_phitdrop->m_pfe = (IFireEvents *)this;

			m_phitdrop->m_fVisible = fTrue;

			m_phitdrop->m_polydropanim.m_iframedesire = 0;

			pvho->AddElement(m_phitdrop);

			m_vhoDrop.AddElement(m_phitdrop);
			}
		else
			{
			Hit3DPoly *ph3dpoly;

			ph3dpoly = new Hit3DPoly(rgv3D,count);
			ph3dpoly->m_pfe = (IFireEvents *)this;

			ph3dpoly->m_fVisible = fTrue;

			pvho->AddElement(ph3dpoly);
			}
		}

	delete rgv;
	delete rgv3D;
	}

void Surface::AddLine(Vector<HitObject> *pvho, RenderVertex *pv1, RenderVertex *pv2, RenderVertex *pv3, BOOL fSlingshot)
	{
	LineSeg *plineseg;
	LineSegSlingshot *plinesling;
	Joint *pjoint;
	float dot;
	Vertex vt1, vt2;
	float length;

	/*if (pv1->x == pv2->x && pv1->y == pv2->y)
		{
		// Special case - wall has two points which coincide
		return;
		}*/

	if (!fSlingshot)
		{
		plineseg = new LineSeg();

		if (m_d.m_fHitEvent)
			{
			plineseg->m_pfe = (IFireEvents *)this;
			plineseg->m_threshold = m_d.m_threshold;
			}
		else
			{
			plineseg->m_pfe = NULL;
			}
		}
	else
		{
		plinesling = new LineSegSlingshot();
		plineseg = (LineSeg *)plinesling;

		// Slingshots always have hit events
		plineseg->m_pfe = (IFireEvents *)this;
		plineseg->m_threshold = m_d.m_threshold;

		plinesling->m_force = m_d.m_slingshotforce;
		plinesling->m_psurface = this;

		m_vlinesling.AddElement(plinesling);
		}

	plineseg->m_rcHitRect.zlow = m_d.m_heightbottom;
	plineseg->m_rcHitRect.zhigh = m_d.m_heighttop;

	plineseg->v1.x = pv1->x;
	plineseg->v1.y = pv1->y;
	plineseg->v2.x = pv2->x;
	plineseg->v2.y = pv2->y;

	plineseg->m_elasticity = m_d.m_elasticity;
	/*if (plineseg->v1.x == plineseg->v2.x)
		{
		plineseg->v2.x += 0.0001f;
		}
	if (plineseg->v1.y == plineseg->v2.y)
		{
		plineseg->v2.y += 0.0001f;
		}*/

	pvho->AddElement(plineseg);
	if (m_d.m_fDroppable)
		{
		m_vhoDrop.AddElement(plineseg);
		}

	plineseg->CalcNormal();

	vt1.x = pv1->x - pv2->x;
	vt1.y = pv1->y - pv2->y;

	// Set up line normal
	/*length = (float)sqrt((vt1.x * vt1.x) + (vt1.y * vt1.y));
	plineseg->normal.x = vt1.y / length;
	plineseg->normal.y = -vt1.x / length;*/

	vt2.x = pv1->x - pv3->x;
	vt2.y = pv1->y - pv3->y;

	dot = vt1.x*vt2.y - vt1.y*vt2.x;

	if (dot < 0) // Inside edges don't need joint hit-testing (dot == 0 continuous segments should mathematically never hit)
		{
		pjoint = new Joint();

		if (m_d.m_fHitEvent)
			{
			pjoint->m_pfe = (IFireEvents *)this;
			pjoint->m_threshold = m_d.m_threshold;
			}
		else
			{
			pjoint->m_pfe = NULL;
			}

		pjoint->m_rcHitRect.zlow = m_d.m_heightbottom;
		pjoint->m_rcHitRect.zhigh = m_d.m_heighttop;

		pjoint->m_elasticity = m_d.m_elasticity;

		pjoint->center.x = pv1->x;
		pjoint->center.y = pv1->y;
		pvho->AddElement(pjoint);
		if (m_d.m_fDroppable)
			{
			m_vhoDrop.AddElement(pjoint);
			}

		Vertex normalT;

		// Set up line normal
		length = (float)sqrt((vt2.x * vt2.x) + (vt2.y * vt2.y));
		normalT.x = -vt2.y / length;
		normalT.y = vt2.x / length;

		pjoint->normal.x = normalT.x + plineseg->normal.x;
		pjoint->normal.y = normalT.y + plineseg->normal.y;

		// Set up line normal
		length = (float)sqrt((pjoint->normal.x * pjoint->normal.x) + (pjoint->normal.y * pjoint->normal.y));
		pjoint->normal.x = pjoint->normal.x / length;
		pjoint->normal.y = pjoint->normal.y / length;
		}

	return;
	}

void Surface::GetBoundingVertices(Vector<Vertex3D> *pvvertex3D)
	{
	Vertex3D *pv;
	int i;
	//float minx, maxx, miny, maxy;

	float top,bottom;

	top = m_d.m_heighttop;
	bottom = m_d.m_heightbottom;

	for (i=0;i<8;i++)
		{
		pv = new Vertex3D();
		pv->x = i&1 ? m_ptable->m_right : m_ptable->m_left;
		pv->y = i&2 ? m_ptable->m_bottom : m_ptable->m_top;
		pv->z = i&4 ? top : bottom;
		pvvertex3D->AddElement(pv);
		}
	}

void Surface::EndPlay()
	{
	IEditable::EndPlay();

	if (m_phitdrop) // Failed Player Case
		{
		if (m_d.m_fDroppable)
			{
			int i;
			for (i=0;i<2;i++)
				{
				delete m_phitdrop->m_polydropanim.m_pobjframe[i];
				}
			}

		m_phitdrop = NULL;
		}

	m_vlinesling.RemoveAllElements();
	m_vhoDrop.RemoveAllElements();
	}

void Surface::MoveOffset(float dx, float dy)
	{
	int i;

	for (i=0;i<m_vdpoint.Size();i++)
		{
		CComObject<DragPoint> *pdp;

		pdp = m_vdpoint.ElementAt(i);

		pdp->m_v.x += dx;
		pdp->m_v.y += dy;
		}

	m_ptable->SetDirtyDraw();
	}

/*void Surface::CheckIntersecting()
	{
	int i;
	int l;
	int cvertex;

	RenderVertex *rgv;

	rgv = GetRgRenderVertex(&cvertex);

	for (i=0;i<cvertex;i++)
		{
		//Vertex vPath;

		//int crosscount;

		for (l=0;l<cvertex;l++)
			{

			}
		}

	delete rgv;
	}*/

void Surface::RenderStatic(LPDIRECT3DDEVICE7 pd3dDevice)
	{
	if (!m_d.m_fDroppable || !m_d.m_fInner)
		{
		RenderWallsAtHeight(pd3dDevice, fFalse, fFalse);
		}
	/*Vertex3D rgv3D[4];
	WORD rgi[4];
	int i;

	Pin3D *ppin3d = &g_pplayer->m_pin3d;

	PinImage *pin = m_ptable->GetImage(m_d.m_szImage);
	float maxtu, maxtv;

	D3DMATERIAL7 mtrl;
	ZeroMemory( &mtrl, sizeof(mtrl) );

	float r = (m_d.m_sidecolor & 255) / 255.0f;
	float g = (m_d.m_sidecolor & 65280) / 65280.0f;
	float b = (m_d.m_sidecolor & 16711680) / 16711680.0f;

	mtrl.diffuse.r = mtrl.ambient.r = r;
	mtrl.diffuse.g = mtrl.ambient.g = g;
	mtrl.diffuse.b = mtrl.ambient.b = b;
	mtrl.diffuse.a = mtrl.ambient.a = 1;

	int cvertex;

	RenderVertex *rgv;
	Vertex *rgnormal;

		{
		Vector<RenderVertex> vvertex;
		GetRgVertex(&vvertex);

		cvertex = vvertex.Size();
		rgv = new RenderVertex[cvertex + 6]; // Add points so inverted polygons can be drawn

		int i;
		for (i=0;i<vvertex.Size();i++)
			{
			rgv[i] = *vvertex.ElementAt(i);
			delete vvertex.ElementAt(i);
			}
		}

	rgnormal = new Vertex[cvertex];

	pd3dDevice->SetMaterial(&mtrl);

	for (i=0;i<4;i++)
		{
		rgi[i] = i;
		}

	for (i=0;i<cvertex;i++)
		{
		RenderVertex *pv1 = &rgv[i];
		RenderVertex *pv2 = &rgv[(i+1) % cvertex];
		float dx = pv1->x - pv2->x;
		float dy = pv1->y - pv2->y;

		float len = (float)sqrt(dx*dx + dy*dy);

		rgnormal[i].x = dy/len;
		rgnormal[i].y = dx/len;
		}

	ppin3d->EnableLightMap(fTrue, m_d.m_heighttop);

	for (i=0;i<cvertex;i++)
		{
		RenderVertex *pv0 = &rgv[(i-1+cvertex) % cvertex];
		RenderVertex *pv1 = &rgv[i];
		RenderVertex *pv2 = &rgv[(i+1) % cvertex];
		RenderVertex *pv3 = &rgv[(i+2) % cvertex];

		rgv3D[0].Set(pv1->x,pv1->y,m_d.m_heightbottom);
		rgv3D[1].Set(pv1->x,pv1->y,m_d.m_heighttop);
		rgv3D[2].Set(pv2->x,pv2->y,m_d.m_heighttop);
		rgv3D[3].Set(pv2->x,pv2->y,m_d.m_heightbottom);

		ppin3d->m_lightproject.CalcCoordinates(&rgv3D[0]);
		ppin3d->m_lightproject.CalcCoordinates(&rgv3D[1]);
		ppin3d->m_lightproject.CalcCoordinates(&rgv3D[2]);
		ppin3d->m_lightproject.CalcCoordinates(&rgv3D[3]);

		Vertex vnormal[2];

		int a,b,c;;
		a = (i-1+cvertex) % cvertex;
		b = i;
		c = (i+1)%cvertex;

		if (pv1->fSmooth)
			{
			vnormal[0].x = (rgnormal[a].x + rgnormal[b].x)/2;
			vnormal[0].y = (rgnormal[a].y + rgnormal[b].y)/2;
			}
		else
			{
			vnormal[0].x = rgnormal[b].x;
			vnormal[0].y = rgnormal[b].y;
			}

		if (pv2->fSmooth)
			{
			vnormal[1].x = (rgnormal[b].x + rgnormal[c].x)/2;
			vnormal[1].y = (rgnormal[b].y + rgnormal[c].y)/2;
			}
		else
			{
			vnormal[1].x = rgnormal[b].x;
			vnormal[1].y = rgnormal[b].y;
			}

		float len;
		len = (vnormal[0].x * vnormal[0].x + vnormal[0].y * vnormal[0].y);
		vnormal[0].x /= len;
		vnormal[0].y /= len;
		len = (vnormal[1].x * vnormal[1].x + vnormal[1].y * vnormal[1].y);
		vnormal[1].x /= len;
		vnormal[1].y /= len;

		if (m_d.m_fInner)
			{
			rgi[1] = 1;
			rgi[3] = 3;

			int l;
			for (l=0;l<2;l++)
				{
				rgv3D[l].nx = -vnormal[0].x;
				rgv3D[l].ny = vnormal[0].y;
				rgv3D[l].nz = 0;

				rgv3D[l+2].nx = -vnormal[1].x;
				rgv3D[l+2].ny = vnormal[1].y;
				rgv3D[l+2].nz = 0;
				}
			}
		else
			{
			rgi[1] = 3;
			rgi[3] = 1;

			int l;
			for (l=0;l<2;l++)
				{
				rgv3D[l].nx = vnormal[0].x;
				rgv3D[l].ny = -vnormal[0].y;
				rgv3D[l].nz = 0;

				rgv3D[l+2].nx = vnormal[1].x;
				rgv3D[l+2].ny = -vnormal[1].y;
				rgv3D[l+2].nz = 0;
				}
			}

		pd3dDevice->DrawIndexedPrimitive(D3DPT_TRIANGLEFAN, MY_D3DFVF_VERTEX,
												  rgv3D, 4,
												  rgi, 4, NULL);

		}

	//if (m_fInner)
		{
		Vector<void> vpoly;
		Vector<Triangle> vtri;

		if (!m_d.m_fInner)
			{
			float miny = FLT_MAX;
			int minyindex;

			// Find smallest y point - use it to connect with surrounding border

			for (i=0;i<cvertex;i++)
				{
				if (rgv[i].y < miny)
					{
					miny = rgv[i].y;
					minyindex = i;
					}
				}

			rgv[cvertex].x = m_ptable->m_left;
			rgv[cvertex].y = m_ptable->m_top;
			rgv[cvertex+3].x = m_ptable->m_left;
			rgv[cvertex+3].y = m_ptable->m_bottom;
			rgv[cvertex+2].x = m_ptable->m_right;
			rgv[cvertex+2].y = m_ptable->m_bottom;
			rgv[cvertex+1].x = m_ptable->m_right;
			rgv[cvertex+1].y = m_ptable->m_top;
			rgv[cvertex+4].x = m_ptable->m_left-1; // put tiny gap in to avoid errors
			rgv[cvertex+4].y = m_ptable->m_top;
			rgv[cvertex+5].x = rgv[minyindex].x;
			rgv[cvertex+5].y = rgv[minyindex].y - 1; // put tiny gap in to avoid errors

			for (i=0;i<cvertex;i++)
				{
				vpoly.AddElement((void *)(cvertex-i-1));
				}

			for (i=0;i<6;i++)
				{
				vpoly.InsertElementAt((void *)(cvertex+i), (cvertex-minyindex-1));
				}
			}
		else
			{
			for (i=0;i<cvertex;i++)
				{
				vpoly.AddElement((void *)i);
				}
			}

		if (pin)
			{
			m_ptable->GetTVTU(pin, &maxtu, &maxtv);
			//pd3dDevice->SetTexture(ePictureTexture, pin->m_pdsBuffer);
			ppin3d->SetTexture(pin->m_pdsBuffer);

			mtrl.diffuse.r = mtrl.ambient.r = 1;
			mtrl.diffuse.g = mtrl.ambient.g = 1;
			mtrl.diffuse.b = mtrl.ambient.b = 1;
			mtrl.diffuse.a = mtrl.ambient.a = 0.5;
			}
		else
			{
			float r = (m_d.m_topcolor & 255) / 255.0f;
			float g = (m_d.m_topcolor & 65280) / 65280.0f;
			float b = (m_d.m_topcolor & 16711680) / 16711680.0f;

			mtrl.diffuse.r = mtrl.ambient.r = r;
			mtrl.diffuse.g = mtrl.ambient.g = g;
			mtrl.diffuse.b = mtrl.ambient.b = b;
			mtrl.diffuse.a = mtrl.ambient.a = 1;
			}

		PolygonToTriangles(rgv, &vpoly, &vtri);

		if (!m_d.m_fInner)
			{
			// Remove tiny gap
			rgv[cvertex+4].x += 1;
			rgv[cvertex+5].y += 1;
			}

		pd3dDevice->SetMaterial(&mtrl);

		for (i=0;i<vtri.Size();i++)
			{
			Triangle *ptri = vtri.ElementAt(i);

			RenderVertex *pv0 = &rgv[ptri->a];
			RenderVertex *pv1 = &rgv[ptri->b];
			RenderVertex *pv2 = &rgv[ptri->c];

			rgv3D[0].Set(pv0->x,pv0->y,m_d.m_heighttop);
			rgv3D[2].Set(pv1->x,pv1->y,m_d.m_heighttop);
			rgv3D[1].Set(pv2->x,pv2->y,m_d.m_heighttop);

			float tablewidth = m_ptable->m_right - m_ptable->m_left;
			float tableheight = m_ptable->m_bottom - m_ptable->m_top;

			rgv3D[0].tu = rgv3D[0].x / tablewidth * maxtu;
			rgv3D[0].tv = rgv3D[0].y / tableheight * maxtv;
			rgv3D[1].tu = rgv3D[1].x / tablewidth * maxtu;
			rgv3D[1].tv = rgv3D[1].y / tableheight* maxtv;
			rgv3D[2].tu = rgv3D[2].x / tablewidth * maxtu;
			rgv3D[2].tv = rgv3D[2].y / tableheight* maxtv;

			ppin3d->m_lightproject.CalcCoordinates(&rgv3D[0]);
			ppin3d->m_lightproject.CalcCoordinates(&rgv3D[1]);
			ppin3d->m_lightproject.CalcCoordinates(&rgv3D[2]);

			WORD rgi[3];
			int l;
			for (l=0;l<3;l++)
				{
				rgi[l] = l;
				rgv3D[l].nx = 0;
				rgv3D[l].ny = 0;
				rgv3D[l].nz = -1;
				}

			pd3dDevice->DrawIndexedPrimitive(D3DPT_TRIANGLEFAN, MY_D3DFVF_VERTEX,
													  rgv3D, 3,
													  rgi, 3, NULL);

			//pd3dDevice->SetRenderState(D3DRENDERSTATE_COLORKEYENABLE, FALSE);
			}

		for (i=0;i<vtri.Size();i++)
			{
			delete vtri.ElementAt(i);
			}

		//pd3dDevice->SetTexture(ePictureTexture, NULL);
		ppin3d->SetTexture(NULL);
		}

	ppin3d->EnableLightMap(fFalse, -1);

	delete [] rgv;
	delete [] rgnormal;*/
	}

void Surface::RenderSlingshots(LPDIRECT3DDEVICE7 pd3dDevice)
	{
	int i,l;
	WORD rgi[8];
	Vertex3D rgv3D[32];
	Pin3D *ppin3d = &g_pplayer->m_pin3d;
	ObjFrame *pof;

	float slingbottom = ((m_d.m_heighttop - m_d.m_heightbottom) * 0.2f) + m_d.m_heightbottom;
	float slingtop = ((m_d.m_heighttop - m_d.m_heightbottom) * 0.8f) + m_d.m_heightbottom;

	D3DMATERIAL7 mtrl;
	ZeroMemory( &mtrl, sizeof(mtrl) );

	//ppin3d->m_pddsBackBuffer->Blt(NULL, ppin3d->m_pddsStatic, NULL, 0, NULL);
	//ppin3d->m_pddsZBuffer->Blt(NULL, ppin3d->m_pddsStaticZ, NULL, 0, NULL);

	for (i=0;i<m_vlinesling.Size();i++)
		{
		LineSegSlingshot *plinesling = m_vlinesling.ElementAt(i);

		pd3dDevice->Clear( 0, NULL, D3DCLEAR_TARGET ,
					   0x00000000, 1.0f, 0L );

		ppin3d->m_pddsZBuffer->Blt(NULL, ppin3d->m_pddsStaticZ, NULL, 0, NULL);
		
		float r = (m_d.m_slingshotColor & 255) / 255.0f;
		float g = (m_d.m_slingshotColor & 65280) / 65280.0f;
		float b = (m_d.m_slingshotColor & 16711680) / 16711680.0f;

		mtrl.diffuse.r = mtrl.ambient.r = r;
		mtrl.diffuse.g = mtrl.ambient.g = g;
		mtrl.diffuse.b = mtrl.ambient.b = b;
		mtrl.diffuse.a = mtrl.ambient.a = 1;

		//mtrl.diffuse.r = mtrl.ambient.r = 0.95f;
		//mtrl.diffuse.g = mtrl.ambient.g = 0.95f;
		//mtrl.diffuse.b = mtrl.ambient.b = 0.95f;

		pd3dDevice->SetMaterial(&mtrl);

		pof = new ObjFrame();
		plinesling->m_slingshotanim.m_pobjframe = pof;

		ppin3d->ClearExtents(&plinesling->m_slingshotanim.m_rcBounds, &plinesling->m_slingshotanim.m_znear, &plinesling->m_slingshotanim.m_zfar);

		rgv3D[0].x = plinesling->v1.x;
		rgv3D[0].y = plinesling->v1.y;
		rgv3D[0].z = slingbottom;

		rgv3D[1].x = (plinesling->v1.x + plinesling->v2.x)/2 + plinesling->normal.x*(m_d.m_slingshotforce * 0.25f);//40;//20;
		rgv3D[1].y = (plinesling->v1.y + plinesling->v2.y)/2 + plinesling->normal.y*(m_d.m_slingshotforce * 0.25f);//20;
		rgv3D[1].z = slingbottom;

		rgv3D[2].x = plinesling->v2.x;
		rgv3D[2].y = plinesling->v2.y;
		rgv3D[2].z = slingbottom;

		for (l=0;l<3;l++)
			{
			rgv3D[l+3].x = rgv3D[l].x;
			rgv3D[l+3].y = rgv3D[l].y;
			rgv3D[l+3].z = slingtop;
			}

		for (l=0;l<6;l++)
			{
			rgv3D[l+6].x = rgv3D[l].x - plinesling->normal.x*5;
			rgv3D[l+6].y = rgv3D[l].y - plinesling->normal.y*5;
			rgv3D[l+6].z = rgv3D[l].z;
			}

		for (l=0;l<12;l++)
			{
			ppin3d->m_lightproject.CalcCoordinates(&rgv3D[l]);
			}

		ppin3d->ClearExtents(&pof->rc, NULL, NULL);
		ppin3d->ExpandExtents(&pof->rc, rgv3D, &plinesling->m_slingshotanim.m_znear, &plinesling->m_slingshotanim.m_zfar, 6, fFalse);

		pof->pdds = ppin3d->CreateOffscreen(pof->rc.right - pof->rc.left, pof->rc.bottom - pof->rc.top);
		pof->pddsZBuffer = ppin3d->CreateZBufferOffscreen(pof->rc.right - pof->rc.left, pof->rc.bottom - pof->rc.top);

		rgi[0] = 0;
		rgi[1] = 1;
		rgi[2] = 4;
		rgi[3] = 3;

		SetNormal(rgv3D, rgi, 4, NULL, NULL, NULL);

		pd3dDevice->DrawIndexedPrimitive(D3DPT_TRIANGLEFAN, MY_D3DFVF_VERTEX,
												  rgv3D, 12,
												  rgi, 4, NULL);

		rgi[0] = 1;
		rgi[1] = 2;
		rgi[2] = 5;
		rgi[3] = 4;

		SetNormal(rgv3D, rgi, 4, NULL, NULL, NULL);

		pd3dDevice->DrawIndexedPrimitive(D3DPT_TRIANGLEFAN, MY_D3DFVF_VERTEX,
												  rgv3D, 12,
												  rgi, 4, NULL);

		rgi[0] = 0;
		rgi[3] = 1;
		rgi[2] = 4;
		rgi[1] = 3;

		SetNormal(rgv3D, rgi, 4, NULL, NULL, NULL);

		pd3dDevice->DrawIndexedPrimitive(D3DPT_TRIANGLEFAN, MY_D3DFVF_VERTEX,
												  rgv3D, 12,
												  rgi, 4, NULL);

		rgi[0] = 1;
		rgi[3] = 2;
		rgi[2] = 5;
		rgi[1] = 4;

		SetNormal(rgv3D, rgi, 4, NULL, NULL, NULL);

		pd3dDevice->DrawIndexedPrimitive(D3DPT_TRIANGLEFAN, MY_D3DFVF_VERTEX,
												  rgv3D, 12,
												  rgi, 4, NULL);

		rgi[0] = 3;
		rgi[1] = 9;
		rgi[2] = 10;
		rgi[3] = 4;

		SetNormal(rgv3D, rgi, 4, NULL, NULL, NULL);

		pd3dDevice->DrawIndexedPrimitive(D3DPT_TRIANGLEFAN, MY_D3DFVF_VERTEX,
												  rgv3D, 12,
												  rgi, 4, NULL);

		rgi[0] = 4;
		rgi[1] = 10;
		rgi[2] = 11;
		rgi[3] = 5;

		SetNormal(rgv3D, rgi, 4, NULL, NULL, NULL);

		pd3dDevice->DrawIndexedPrimitive(D3DPT_TRIANGLEFAN, MY_D3DFVF_VERTEX,
												  rgv3D, 12,
												  rgi, 4, NULL);

		pof->pdds->Blt(NULL, ppin3d->m_pddsBackBuffer, &pof->rc, 0, NULL);
		pof->pddsZBuffer->BltFast(0, 0, ppin3d->m_pddsZBuffer, &pof->rc, DDBLTFAST_NOCOLORKEY);
		
		ppin3d->ExpandRectByRect(&plinesling->m_slingshotanim.m_rcBounds, &pof->rc);

		ppin3d->WriteAnimObjectToCacheFile(&plinesling->m_slingshotanim, &plinesling->m_slingshotanim.m_pobjframe, 1);

		// reset the portion of the z-buffer that we changed
		ppin3d->m_pddsZBuffer->BltFast(pof->rc.left, pof->rc.top, ppin3d->m_pddsStaticZ, &pof->rc, DDBLTFAST_NOCOLORKEY);
		// Reset color key in back buffer
		DDBLTFX ddbltfx;
		ddbltfx.dwSize = sizeof(DDBLTFX);
		ddbltfx.dwFillColor = 0;
		ppin3d->m_pddsBackBuffer->Blt(&pof->rc, NULL,
				&pof->rc, DDBLT_COLORFILL, &ddbltfx);
		}
	}

ObjFrame *Surface::RenderWallsAtHeight(LPDIRECT3DDEVICE7 pd3dDevice, BOOL fMover, BOOL fDrop)
	{
	ObjFrame *pof = NULL;

	Vertex3D rgv3D[4];
	WORD rgi[4];
	int i;

	Pin3D *ppin3d = &g_pplayer->m_pin3d;

	PinImage *pin = m_ptable->GetImage(m_d.m_szImage);
	float maxtu, maxtv;

	PinImage *pinSide = m_ptable->GetImage(m_d.m_szSideImage);
	float maxtuSide, maxtvSide;

	//ppin3d->SetTexture(NULL);

	if (fMover)
		{
		//pd3dDevice->Clear( 0, NULL, D3DCLEAR_TARGET ,
			//0x00000000, 1.0f, 0L );

		//ppin3d->m_pddsZBuffer->Blt(NULL, ppin3d->m_pddsStaticZ, NULL, 0, NULL);

		pof = new ObjFrame();

		ppin3d->ClearExtents(&m_phitdrop->m_polydropanim.m_rcBounds, &m_phitdrop->m_polydropanim.m_znear, &m_phitdrop->m_polydropanim.m_zfar);
		ppin3d->ClearExtents(&pof->rc, NULL, NULL);
		}

	D3DMATERIAL7 mtrl;
	ZeroMemory( &mtrl, sizeof(mtrl) );

	if (pinSide)
		{
		m_ptable->GetTVTU(pinSide, &maxtuSide, &maxtvSide);
		//ppin3d->SetTexture(pinSide->m_pdsBuffer);

		pinSide->EnsureColorKey();

		mtrl.diffuse.r = mtrl.ambient.r = 1;
		mtrl.diffuse.g = mtrl.ambient.g = 1;
		mtrl.diffuse.b = mtrl.ambient.b = 1;
		mtrl.diffuse.a = mtrl.ambient.a = 1;

		if (pinSide->m_fTransparent)
			{
			pd3dDevice->SetTexture(ePictureTexture, pinSide->m_pdsBufferColorKey);
			pd3dDevice->SetRenderState(D3DRENDERSTATE_ALPHABLENDENABLE, FALSE);
			pd3dDevice->SetRenderState(D3DRENDERSTATE_COLORKEYENABLE, TRUE);

			//pd3dDevice->SetRenderState(D3DRENDERSTATE_ALPHABLENDENABLE, TRUE);
			//pd3dDevice->SetRenderState(D3DRENDERSTATE_COLORKEYENABLE, FALSE);

			/*pd3dDevice->SetRenderState(D3DRENDERSTATE_ALPHAREF, (DWORD)0x0000002f);
			pd3dDevice->SetRenderState(D3DRENDERSTATE_ALPHATESTENABLE, TRUE); 
			pd3dDevice->SetRenderState(D3DRENDERSTATE_ALPHAFUNC, D3DCMP_GREATEREQUAL);*/

			pd3dDevice->SetRenderState(D3DRENDERSTATE_CULLMODE, D3DCULL_NONE);

			pd3dDevice->SetTextureStageState(ePictureTexture, D3DTSS_MAGFILTER, D3DTFG_POINT);
			pd3dDevice->SetTextureStageState(ePictureTexture, D3DTSS_MINFILTER, D3DTFN_POINT);
			pd3dDevice->SetTextureStageState(ePictureTexture, D3DTSS_MIPFILTER, D3DTFP_NONE);
			}
		else
			{
			pd3dDevice->SetTexture(ePictureTexture, pinSide->m_pdsBuffer);
			//pd3dDevice->SetRenderState(D3DRENDERSTATE_ALPHABLENDENABLE, TRUE);
			//pd3dDevice->SetRenderState(D3DRENDERSTATE_COLORKEYENABLE, FALSE);
			}
		}
	else
		{
		float r = (m_d.m_sidecolor & 255) / 255.0f;
		float g = (m_d.m_sidecolor & 65280) / 65280.0f;
		float b = (m_d.m_sidecolor & 16711680) / 16711680.0f;

		mtrl.diffuse.r = mtrl.ambient.r = r;
		mtrl.diffuse.g = mtrl.ambient.g = g;
		mtrl.diffuse.b = mtrl.ambient.b = b;
		mtrl.diffuse.a = mtrl.ambient.a = 1;
		}

	int cvertex;

	RenderVertex *rgv;
	Vertex *rgnormal;
	float *rgtexcoord = NULL;

		{
		Vector<RenderVertex> vvertex;
		GetRgVertex(&vvertex);

		if (pinSide)
			{
			GetTextureCoords(&vvertex, &rgtexcoord);
			}

		cvertex = vvertex.Size();
		rgv = new RenderVertex[cvertex + 6]; // Add points so inverted polygons can be drawn

		int i;
		for (i=0;i<vvertex.Size();i++)
			{
			rgv[i] = *vvertex.ElementAt(i);
			delete vvertex.ElementAt(i);
			}
		}

	rgnormal = new Vertex[cvertex];

	pd3dDevice->SetMaterial(&mtrl);

	for (i=0;i<4;i++)
		{
		rgi[i] = i;
		}

	for (i=0;i<cvertex;i++)
		{
		RenderVertex *pv1 = &rgv[i];
		RenderVertex *pv2 = &rgv[(i+1) % cvertex];
		float dx = pv1->x - pv2->x;
		float dy = pv1->y - pv2->y;

		float len = (float)sqrt(dx*dx + dy*dy);

		rgnormal[i].x = dy/len;
		rgnormal[i].y = dx/len;
		}

	//if (!pinSide)
		{
		ppin3d->EnableLightMap(fTrue, fDrop ? m_d.m_heightbottom : m_d.m_heighttop);
		}
	/*else
		{
		ppin3d->EnableLightMap(fFalse, -1);
		}*/

		// Render side
		{
		for (i=0;i<cvertex;i++)
			{
			RenderVertex *pv0 = &rgv[(i-1+cvertex) % cvertex];
			RenderVertex *pv1 = &rgv[i];
			RenderVertex *pv2 = &rgv[(i+1) % cvertex];
			RenderVertex *pv3 = &rgv[(i+2) % cvertex];

			rgv3D[0].Set(pv1->x,pv1->y,m_d.m_heightbottom);
			rgv3D[1].Set(pv1->x,pv1->y,m_d.m_heighttop);
			rgv3D[2].Set(pv2->x,pv2->y,m_d.m_heighttop);
			rgv3D[3].Set(pv2->x,pv2->y,m_d.m_heightbottom);

			if (pinSide)
				{
				rgv3D[0].tu = rgtexcoord[i] * maxtuSide;
				rgv3D[0].tv = maxtvSide;

				rgv3D[1].tu = rgtexcoord[i] * maxtuSide;
				rgv3D[1].tv = 0;

				rgv3D[2].tu = rgtexcoord[(i+1) % cvertex] * maxtuSide;
				rgv3D[2].tv = 0;

				rgv3D[3].tu = rgtexcoord[(i+1) % cvertex] * maxtuSide;
				rgv3D[3].tv = maxtvSide;
				}

			ppin3d->m_lightproject.CalcCoordinates(&rgv3D[0]);
			ppin3d->m_lightproject.CalcCoordinates(&rgv3D[1]);
			ppin3d->m_lightproject.CalcCoordinates(&rgv3D[2]);
			ppin3d->m_lightproject.CalcCoordinates(&rgv3D[3]);

			Vertex vnormal[2];

			int a,b,c;
			a = (i-1+cvertex) % cvertex;
			b = i;
			c = (i+1)%cvertex;

			if (pv1->fSmooth)
				{
				vnormal[0].x = (rgnormal[a].x + rgnormal[b].x)/2;
				vnormal[0].y = (rgnormal[a].y + rgnormal[b].y)/2;
				}
			else
				{
				vnormal[0].x = rgnormal[b].x;
				vnormal[0].y = rgnormal[b].y;
				}

			if (pv2->fSmooth)
				{
				vnormal[1].x = (rgnormal[b].x + rgnormal[c].x)/2;
				vnormal[1].y = (rgnormal[b].y + rgnormal[c].y)/2;
				}
			else
				{
				vnormal[1].x = rgnormal[b].x;
				vnormal[1].y = rgnormal[b].y;
				}

			float len;
			len = (vnormal[0].x * vnormal[0].x + vnormal[0].y * vnormal[0].y);
			vnormal[0].x /= len;
			vnormal[0].y /= len;
			len = (vnormal[1].x * vnormal[1].x + vnormal[1].y * vnormal[1].y);
			vnormal[1].x /= len;
			vnormal[1].y /= len;

			if (m_d.m_fInner)
				{
				rgi[1] = 1;
				rgi[3] = 3;

				int l;
				for (l=0;l<2;l++)
					{
					/*rgv3D[l].nx = -vnormal[0].x;
					rgv3D[l].ny = 0;
					rgv3D[l].nz = -vnormal[0].y;

					rgv3D[l+2].nx = -vnormal[1].x;
					rgv3D[l+2].ny = 0;
					rgv3D[l+2].nz = -vnormal[1].y;*/

					rgv3D[l].nx = -vnormal[0].x;
					rgv3D[l].ny = vnormal[0].y;
					rgv3D[l].nz = 0;

					rgv3D[l+2].nx = -vnormal[1].x;
					rgv3D[l+2].ny = vnormal[1].y;
					rgv3D[l+2].nz = 0;
					}
				}
			else
				{
				rgi[1] = 3;
				rgi[3] = 1;

				int l;
				for (l=0;l<2;l++)
					{
					/*rgv3D[l].nx = vnormal[0].x;
					rgv3D[l].ny = 0;
					rgv3D[l].nz = vnormal[0].y;

					rgv3D[l+2].nx = vnormal[1].x;
					rgv3D[l+2].ny = 0;
					rgv3D[l+2].nz = vnormal[1].y;*/

					rgv3D[l].nx = vnormal[0].x;
					rgv3D[l].ny = -vnormal[0].y;
					rgv3D[l].nz = 0;

					rgv3D[l+2].nx = vnormal[1].x;
					rgv3D[l+2].ny = -vnormal[1].y;
					rgv3D[l+2].nz = 0;
					}
				}

			if (!fDrop && m_d.m_fSideVisible) // Don't need to render walls if dropped, but we do need to extend the extrema
				{
				pd3dDevice->DrawIndexedPrimitive(D3DPT_TRIANGLEFAN, MY_D3DFVF_VERTEX,
														  rgv3D, 4,
														  rgi, 4, NULL);
				}

			if (fMover)
				{
				// Only do two points - each segment has two new points
				ppin3d->ExpandExtents(&pof->rc, rgv3D, &m_phitdrop->m_polydropanim.m_znear, &m_phitdrop->m_polydropanim.m_zfar, 2, fFalse);
				}
			}
		}

	SAFE_DELETE(rgtexcoord);

	if (m_d.m_fVisible)
		{
		Vector<void> vpoly;
		Vector<Triangle> vtri;

		if (!m_d.m_fInner)
			{
			float miny = FLT_MAX;
			int minyindex;

			// Find smallest y point - use it to connect with surrounding border

			for (i=0;i<cvertex;i++)
				{
				if (rgv[i].y < miny)
					{
					miny = rgv[i].y;
					minyindex = i;
					}
				}

			rgv[cvertex].x = m_ptable->m_left;
			rgv[cvertex].y = m_ptable->m_top;
			rgv[cvertex+3].x = m_ptable->m_left;
			rgv[cvertex+3].y = m_ptable->m_bottom;
			rgv[cvertex+2].x = m_ptable->m_right;
			rgv[cvertex+2].y = m_ptable->m_bottom;
			rgv[cvertex+1].x = m_ptable->m_right;
			rgv[cvertex+1].y = m_ptable->m_top;
			rgv[cvertex+4].x = m_ptable->m_left-1; // put tiny gap in to avoid errors
			rgv[cvertex+4].y = m_ptable->m_top;
			rgv[cvertex+5].x = rgv[minyindex].x;
			rgv[cvertex+5].y = rgv[minyindex].y - 1; // put tiny gap in to avoid errors

			for (i=0;i<cvertex;i++)
				{
				vpoly.AddElement((void *)(cvertex-i-1));
				}

			for (i=0;i<6;i++)
				{
				vpoly.InsertElementAt((void *)(cvertex+i), (cvertex-minyindex-1));
				}
			}
		else
			{
			for (i=0;i<cvertex;i++)
				{
				vpoly.AddElement((void *)i);
				}
			}

		pd3dDevice->SetRenderState(D3DRENDERSTATE_ALPHABLENDENABLE, FALSE);

		if (pinSide)
			{
			ppin3d->EnableLightMap(fTrue, fDrop ? m_d.m_heightbottom : m_d.m_heighttop);
			pd3dDevice->SetRenderState(D3DRENDERSTATE_ALPHATESTENABLE, FALSE); 
			if (pinSide->m_fTransparent)
				{
				pd3dDevice->SetTextureStageState(ePictureTexture, D3DTSS_MAGFILTER, D3DTFG_LINEAR);
				pd3dDevice->SetTextureStageState(ePictureTexture, D3DTSS_MINFILTER, D3DTFN_LINEAR);
				pd3dDevice->SetTextureStageState(ePictureTexture, D3DTSS_MIPFILTER, D3DTFP_LINEAR);
				pd3dDevice->SetRenderState(D3DRENDERSTATE_CULLMODE, D3DCULL_CCW);
				}
			}

		if (pin)
			{
			m_ptable->GetTVTU(pin, &maxtu, &maxtv);
			//pd3dDevice->SetTexture(ePictureTexture, pin->m_pdsBuffer);
			ppin3d->SetTexture(pin->m_pdsBuffer);

			mtrl.diffuse.r = mtrl.ambient.r = 1;
			mtrl.diffuse.g = mtrl.ambient.g = 1;
			mtrl.diffuse.b = mtrl.ambient.b = 1;
			mtrl.diffuse.a = mtrl.ambient.a = 1.0;//0.5;
			}
		else
			{
			ppin3d->SetTexture(NULL);
			float r = (m_d.m_topcolor & 255) / 255.0f;
			float g = (m_d.m_topcolor & 65280) / 65280.0f;
			float b = (m_d.m_topcolor & 16711680) / 16711680.0f;

			mtrl.diffuse.r = mtrl.ambient.r = r;
			mtrl.diffuse.g = mtrl.ambient.g = g;
			mtrl.diffuse.b = mtrl.ambient.b = b;
			mtrl.diffuse.a = mtrl.ambient.a = 1;
			
			maxtv = maxtu = 1;
			}

		PolygonToTriangles(rgv, &vpoly, &vtri);

		if (!m_d.m_fInner)
			{
			// Remove tiny gap
			rgv[cvertex+4].x += 1;
			rgv[cvertex+5].y += 1;
			}

		pd3dDevice->SetMaterial(&mtrl);

		float height;

		if (!fDrop)
			{
			height = m_d.m_heighttop;
			}
		else
			{
			height = m_d.m_heightbottom + 0.1f;
			}

		float tablewidth = m_ptable->m_right - m_ptable->m_left;
		float tableheight = m_ptable->m_bottom - m_ptable->m_top;

		for (i=0;i<vtri.Size();i++)
			{
			Triangle *ptri = vtri.ElementAt(i);

			RenderVertex *pv0 = &rgv[ptri->a];
			RenderVertex *pv1 = &rgv[ptri->b];
			RenderVertex *pv2 = &rgv[ptri->c];

			rgv3D[0].Set(pv0->x,pv0->y,height);
			rgv3D[2].Set(pv1->x,pv1->y,height);
			rgv3D[1].Set(pv2->x,pv2->y,height);

			rgv3D[0].tu = rgv3D[0].x / tablewidth * maxtu;
			rgv3D[0].tv = rgv3D[0].y / tableheight * maxtv;
			rgv3D[1].tu = rgv3D[1].x / tablewidth * maxtu;
			rgv3D[1].tv = rgv3D[1].y / tableheight* maxtv;
			rgv3D[2].tu = rgv3D[2].x / tablewidth * maxtu;
			rgv3D[2].tv = rgv3D[2].y / tableheight* maxtv;

			ppin3d->m_lightproject.CalcCoordinates(&rgv3D[0]);
			ppin3d->m_lightproject.CalcCoordinates(&rgv3D[1]);
			ppin3d->m_lightproject.CalcCoordinates(&rgv3D[2]);

			WORD rgi[3];
			int l;
			for (l=0;l<3;l++)
				{
				rgi[l] = l;
				rgv3D[l].nx = 0;
				rgv3D[l].ny = 0;
				rgv3D[l].nz = -1;
				}

			//SetNormal(rgv3D, rgi, 3, NULL, NULL, NULL);

			pd3dDevice->DrawIndexedPrimitive(D3DPT_TRIANGLEFAN, MY_D3DFVF_VERTEX,
													  rgv3D, 3,
													  rgi, 3, NULL);

			//pd3dDevice->SetRenderState(D3DRENDERSTATE_COLORKEYENABLE, FALSE);
			}

		for (i=0;i<vtri.Size();i++)
			{
			delete vtri.ElementAt(i);
			}

		//pd3dDevice->SetTexture(ePictureTexture, NULL);
		}

	ppin3d->SetTexture(NULL);

	ppin3d->EnableLightMap(fFalse, -1);

	delete [] rgv;
	delete [] rgnormal;

	if (fMover)
		{
		pof->pdds = ppin3d->CreateOffscreen(pof->rc.right - pof->rc.left, pof->rc.bottom - pof->rc.top);
		pof->pddsZBuffer = ppin3d->CreateZBufferOffscreen(pof->rc.right - pof->rc.left, pof->rc.bottom - pof->rc.top);

		pof->pdds->Blt(NULL, ppin3d->m_pddsBackBuffer, &pof->rc, 0, NULL);
		//HRESULT hr = pof->pddsZBuffer->Blt(NULL, ppin3d->m_pddsZBuffer, &pof->rc, 0, NULL);
		HRESULT hr = pof->pddsZBuffer->BltFast(0, 0, ppin3d->m_pddsZBuffer, &pof->rc, DDBLTFAST_NOCOLORKEY);

		//pdds->Blt(NULL, NULL, NULL, DDBLT_COLORFILL, &ddbfx);
		//m_pbumperhitcircle->m_pobjframe[i] = pof;

		ppin3d->ExpandRectByRect(&m_phitdrop->m_polydropanim.m_rcBounds, &pof->rc);


		// reset the portion of the z-buffer that we changed
		ppin3d->m_pddsZBuffer->BltFast(pof->rc.left, pof->rc.top, ppin3d->m_pddsStaticZ, &pof->rc, DDBLTFAST_NOCOLORKEY);
		// Reset color key in back buffer
		DDBLTFX ddbltfx;
		ddbltfx.dwSize = sizeof(DDBLTFX);
		ddbltfx.dwFillColor = 0;
		ppin3d->m_pddsBackBuffer->Blt(&pof->rc, NULL,
				&pof->rc, DDBLT_COLORFILL, &ddbltfx);
		}

	return pof;
	}
	
void Surface::RenderMoversFromCache(Pin3D *ppin3d)
	{
	int i;
	for (i=0;i<m_vlinesling.Size();i++)
		{
		LineSegSlingshot *plinesling = m_vlinesling.ElementAt(i);
		ppin3d->ReadAnimObjectFromCacheFile(&plinesling->m_slingshotanim, &plinesling->m_slingshotanim.m_pobjframe, 1);
		}
	
	if (m_d.m_fDroppable && m_d.m_fInner)
		{
		ppin3d->ReadAnimObjectFromCacheFile(&m_phitdrop->m_polydropanim, m_phitdrop->m_polydropanim.m_pobjframe, 2);
		}
	}

void Surface::RenderMovers(LPDIRECT3DDEVICE7 pd3dDevice)
	{
	RenderSlingshots(pd3dDevice);

	if (m_d.m_fDroppable && m_d.m_fInner)
		{
		ObjFrame *pof;

		pof = RenderWallsAtHeight(pd3dDevice, fTrue, fFalse);
		m_phitdrop->m_polydropanim.m_pobjframe[0] = pof;
		pof = RenderWallsAtHeight(pd3dDevice, fTrue, fTrue);
		m_phitdrop->m_polydropanim.m_pobjframe[1] = pof;
		
		Pin3D *ppin3d = &g_pplayer->m_pin3d;
		ppin3d->WriteAnimObjectToCacheFile(&m_phitdrop->m_polydropanim, m_phitdrop->m_polydropanim.m_pobjframe, 2);
		}
	}

void Surface::DoCommand(int icmd, int x, int y)
	{
	ISelect::DoCommand(icmd, x, y);

	switch (icmd)
		{
		case ID_WALLMENU_FLIP:
			{
			Vertex vCenter;
			GetPointCenter(&vCenter);
			FlipPointY(&vCenter);
			}
			break;

		case ID_WALLMENU_MIRROR:
			{
			Vertex vCenter;
			GetPointCenter(&vCenter);
			FlipPointX(&vCenter);
			}
			break;

		case ID_WALLMENU_ROTATE:
			RotateDialog();
			break;

		case ID_WALLMENU_SCALE:
			ScaleDialog();
			break;

		case ID_WALLMENU_TRANSLATE:
			TranslateDialog();
			break;

		case ID_WALLMENU_ADDPOINT:
			{
			STARTUNDO

			HitSur *phs;

			RECT rc;
			GetClientRect(m_ptable->m_hwnd, &rc);
			Vertex v, vOut;
			int iSeg;

			phs = new HitSur(NULL, m_ptable->m_zoom, m_ptable->m_offsetx, m_ptable->m_offsety, rc.right - rc.left, rc.bottom - rc.top, 0, 0, NULL);

			phs->ScreenToSurface(x, y, &v.x, &v.y);
			delete phs;

			Vector<RenderVertex> vvertex;
			GetRgVertex(&vvertex);

			m_cvertexT = vvertex.Size();
			m_rgvT = new Vertex[m_cvertexT];

			int i;
			for (i=0;i<vvertex.Size();i++)
				{
				m_rgvT[i] = *((Vertex *)vvertex.ElementAt(i));
				}

			ClosestPointOnPolygon(m_rgvT, m_cvertexT, &v, &vOut, &iSeg, fTrue);

			// Go through vertices (including iSeg itself) counting control points until iSeg
			int icp = 0;
			for (i=0;i<(iSeg+1);i++)
				{
				if (vvertex.ElementAt(i)->fControlPoint)
					{
					icp++;
					}
				}

			//if (icp == 0) // need to add point after the last point
				//icp = m_vdpoint.Size();

			CComObject<DragPoint> *pdp;

			CComObject<DragPoint>::CreateInstance(&pdp);
			if (pdp)
				{
				pdp->AddRef();
				pdp->Init(this, vOut.x, vOut.y);
				m_vdpoint.InsertElementAt(pdp, icp); // push the second point forward, and replace it with this one.  Should work when index2 wraps.
				}

			for (i=0;i<vvertex.Size();i++)
				{
				delete vvertex.ElementAt(i);
				}

			delete m_rgvT;
			m_rgvT = NULL;

			SetDirtyDraw();

			STOPUNDO
			}
			break;
		}
	}

void Surface::FlipY(Vertex *pvCenter)
	{
	IHaveDragPoints::FlipPointY(pvCenter);
	}

void Surface::FlipX(Vertex *pvCenter)
	{
	IHaveDragPoints::FlipPointX(pvCenter);
	}

void Surface::Rotate(float ang, Vertex *pvCenter)
	{
	IHaveDragPoints::RotatePoints(ang, pvCenter);
	}

void Surface::Scale(float scalex, float scaley, Vertex *pvCenter)
	{
	IHaveDragPoints::ScalePoints(scalex, scaley, pvCenter);
	}

void Surface::Translate(Vertex *pvOffset)
	{
	IHaveDragPoints::TranslatePoints(pvOffset);
	}

HRESULT Surface::SaveData(IStream *pstm, HCRYPTHASH hcrypthash, HCRYPTKEY hcryptkey)
	{
	HRESULT hr;
	BiffWriter bw(pstm, hcrypthash, hcryptkey);

#ifdef VBA
	bw.WriteInt(FID(PIID), ApcProjectItem.ID());
#endif
	//bw.WriteStruct(FID(VCEN), &m_d.m_Center, sizeof(Vertex));
	bw.WriteBool(FID(HTEV), m_d.m_fHitEvent);
	bw.WriteBool(FID(DROP), m_d.m_fDroppable);
	bw.WriteBool(FID(TMON), m_d.m_tdr.m_fTimerEnabled);
	bw.WriteInt(FID(TMIN), m_d.m_tdr.m_TimerInterval);
	bw.WriteFloat(FID(THRS), m_d.m_threshold);
	bw.WriteString(FID(IMAG), m_d.m_szImage);
	bw.WriteString(FID(SIMG), m_d.m_szSideImage);
	bw.WriteInt(FID(COLR), m_d.m_sidecolor);
	bw.WriteInt(FID(TCLR), m_d.m_topcolor);
	bw.WriteInt(FID(SCLR), m_d.m_slingshotColor);
	bw.WriteInt(FID(ALGN), m_d.m_ia);
	bw.WriteFloat(FID(HTBT), m_d.m_heightbottom);
	bw.WriteFloat(FID(HTTP), m_d.m_heighttop);
	bw.WriteBool(FID(INNR), m_d.m_fInner);
	bw.WriteWideString(FID(NAME), (WCHAR *)m_wzName);
	bw.WriteBool(FID(DSPT), m_d.m_fDisplayTexture);
	bw.WriteFloat(FID(SLGF), m_d.m_slingshotforce);
	bw.WriteFloat(FID(ELAS), m_d.m_elasticity);
	bw.WriteBool(FID(CSHD), m_d.m_fCastsShadow);
	bw.WriteBool(FID(VSBL), m_d.m_fVisible);
	bw.WriteBool(FID(SVBL), m_d.m_fSideVisible);

	ISelect::SaveData(pstm, hcrypthash, hcryptkey);

	bw.WriteTag(FID(PNTS));
	if(FAILED(hr = SavePointData(pstm, hcrypthash, hcryptkey)))
		return hr;

	bw.WriteTag(FID(ENDB));

	return S_OK;

	/*ULONG writ = 0;
	HRESULT hr = S_OK;
	int i,temp;

	DWORD dwID = ApcProjectItem.ID();
	if(FAILED(hr = pstm->Write(&dwID, sizeof dwID, &writ)))
		return hr;

	temp = m_vdpoint.Size();
	if(FAILED(hr = pstm->Write(&temp, sizeof(int), &writ)))
			return hr;

	for (i=0;i<m_vdpoint.Size();i++)
		{
		CComObject<DragPoint> *pdp = m_vdpoint.ElementAt(i);
		if(FAILED(hr = pstm->Write(&(pdp->m_v), sizeof(Vertex), &writ)))
			return hr;
		if(FAILED(hr = pstm->Write(&(pdp->m_fSmooth), sizeof(BOOL), &writ)))
			return hr;
		if(FAILED(hr = pstm->Write(&(pdp->m_fSlingshot), sizeof(BOOL), &writ)))
			return hr;
		}

	//if(FAILED(hr = pstm->Write(&m_fInner, sizeof(BOOL), &writ)))
			//return hr;

	if(FAILED(hr = pstm->Write(&m_d, sizeof(SurfaceData), &writ)))
		return hr;

	return hr;*/
	}

void Surface::ClearForOverwrite()
	{
	/*int i;

	for (i=0;i<m_vdpoint.Size();i++)
		{
		if (m_ptable->m_pselcur == m_vdpoint.ElementAt(i))
			{
			m_ptable->SetSel(m_ptable);
			}

		m_vdpoint.ElementAt(i)->Release();
		}

	m_vdpoint.RemoveAllElements();*/
	ClearPointsForOverwrite();
	}

HRESULT Surface::InitLoad(IStream *pstm, PinTable *ptable, int *pid, int version, HCRYPTHASH hcrypthash, HCRYPTKEY hcryptkey)
	{
	SetDefaults();
#ifndef OLDLOAD
	BiffReader br(pstm, this, pid, version, hcrypthash, hcryptkey);

	m_ptable = ptable;

	br.Load();
	return S_OK;
#else
	ULONG read = 0;
	HRESULT hr = S_OK;
	int i,temp;

	m_ptable = ptable;

	DWORD dwID;
	if(FAILED(hr = pstm->Read(&dwID, sizeof dwID, &read)))
		return hr;

	if(FAILED(hr = pstm->Read(&temp, sizeof(int), &read)))
		return hr;

	for (i=0;i<temp;i++)
		{
		Vertex v;
		BOOL fSmooth;
		BOOL fSlingshot;
		CComObject<DragPoint> *pdp;

		if(FAILED(hr = pstm->Read(&v, sizeof(Vertex), &read)))
			return hr;
		if(FAILED(hr = pstm->Read(&fSmooth, sizeof(BOOL), &read)))
			return hr;
		if(FAILED(hr = pstm->Read(&fSlingshot, sizeof(BOOL), &read)))
			return hr;

		CComObject<DragPoint>::CreateInstance(&pdp);
		if (pdp)
			{
			pdp->AddRef();
			pdp->Init(this, v.x, v.y);
			pdp->m_fSmooth = fSmooth;
			pdp->m_fSlingshot = fSlingshot;
			m_vdpoint.AddElement(pdp);
			}
		}

	//if(FAILED(hr = pstm->Read(&m_fInner, sizeof(BOOL), &read)))
			//return hr;

	if(FAILED(hr = pstm->Read(&m_d, sizeof(SurfaceData), &read)))
			return hr;

	*pid = dwID;

	return hr;
#endif
	}

BOOL Surface::LoadToken(int id, BiffReader *pbr)
	{
	if (id == FID(PIID))
		{
		pbr->GetInt((int *)pbr->m_pdata);
		}
	else if (id == FID(HTEV))
		{
		pbr->GetBool(&m_d.m_fHitEvent);
		}
	else if (id == FID(DROP))
		{
		pbr->GetBool(&m_d.m_fDroppable);
		}
	else if (id == FID(TMON))
		{
		pbr->GetBool(&m_d.m_tdr.m_fTimerEnabled);
		}
	else if (id == FID(TMIN))
		{
		pbr->GetInt(&m_d.m_tdr.m_TimerInterval);
		}
	else if (id == FID(THRS))
		{
		pbr->GetFloat(&m_d.m_threshold);
		}
	else if (id == FID(IMAG))
		{
		pbr->GetString(m_d.m_szImage);
		}
	else if (id == FID(SIMG))
		{
		pbr->GetString(m_d.m_szSideImage);
		}
	else if (id == FID(COLR))
		{
		pbr->GetInt(&m_d.m_sidecolor);
		}
	else if (id == FID(TCLR))
		{
		pbr->GetInt(&m_d.m_topcolor);
		}
	else if (id == FID(SCLR))
		{
		pbr->GetInt(&m_d.m_slingshotColor);
		}
	else if (id == FID(ALGN))
		{
		pbr->GetInt(&m_d.m_ia);
		}
	else if (id == FID(HTBT))
		{
		pbr->GetFloat(&m_d.m_heightbottom);
		}
	else if (id == FID(HTTP))
		{
		pbr->GetFloat(&m_d.m_heighttop);
		}
	else if (id == FID(INNR))
		{
		pbr->GetBool(&m_d.m_fInner);
		}
	else if (id == FID(NAME))
		{
		pbr->GetWideString((WCHAR *)m_wzName);
		}
	else if (id == FID(DSPT))
		{
		pbr->GetBool(&m_d.m_fDisplayTexture);
		}
	else if (id == FID(SLGF))
		{
		pbr->GetFloat(&m_d.m_slingshotforce);
		}
	else if (id == FID(ELAS))
		{
		pbr->GetFloat(&m_d.m_elasticity);
		}
	else if (id == FID(CSHD))
		{
		pbr->GetBool(&m_d.m_fCastsShadow);
		}
	else if (id == FID(VSBL))
		{
		pbr->GetBool(&m_d.m_fVisible);
		}
	else if (id == FID(SVBL))
		{
		pbr->GetBool(&m_d.m_fSideVisible);
		}
	else
		{
		LoadPointToken(id, pbr, pbr->m_version);
		ISelect::LoadToken(id, pbr);
		}
	return fTrue;
	}

HRESULT Surface::InitPostLoad()
	{
	return S_OK;
	}

STDMETHODIMP Surface::get_HasHitEvent(VARIANT_BOOL *pVal)
{
	*pVal = FTOVB(m_d.m_fHitEvent);

	return S_OK;
}

STDMETHODIMP Surface::put_HasHitEvent(VARIANT_BOOL newVal)
{
	STARTUNDO

	m_d.m_fHitEvent = VBTOF(newVal);

	STOPUNDO

	return S_OK;
}

STDMETHODIMP Surface::get_Threshold(float *pVal)
{
	*pVal = m_d.m_threshold;

	return S_OK;
}

STDMETHODIMP Surface::put_Threshold(float newVal)
{
	STARTUNDO

	m_d.m_threshold = newVal;

	STOPUNDO

	return S_OK;
}

STDMETHODIMP Surface::get_Image(BSTR *pVal)
{
	OLECHAR wz[512];

	MultiByteToWideChar(CP_ACP, 0, m_d.m_szImage, -1, wz, 32);
	*pVal = SysAllocString(wz);

	return S_OK;
}

STDMETHODIMP Surface::put_Image(BSTR newVal)
{
	STARTUNDO

	WideCharToMultiByte(CP_ACP, 0, newVal, -1, m_d.m_szImage, 32, NULL, NULL);

	STOPUNDO

	return S_OK;
}

STDMETHODIMP Surface::get_SideColor(OLE_COLOR *pVal)
{
	*pVal = m_d.m_sidecolor;

	return S_OK;
}

STDMETHODIMP Surface::put_SideColor(OLE_COLOR newVal)
{
	STARTUNDO

	m_d.m_sidecolor = newVal;

	STOPUNDO

	return S_OK;
}

STDMETHODIMP Surface::get_SlingshotColor(OLE_COLOR *pVal)
{
	*pVal = m_d.m_slingshotColor;

	return S_OK;
}

STDMETHODIMP Surface::put_SlingshotColor(OLE_COLOR newVal)
{
	STARTUNDO

	m_d.m_slingshotColor = newVal;

	STOPUNDO

	return S_OK;
}

STDMETHODIMP Surface::get_ImageAlignment(ImageAlignment *pVal)
{
	*pVal = m_d.m_ia;

	return S_OK;
}

STDMETHODIMP Surface::put_ImageAlignment(ImageAlignment newVal)
{
	STARTUNDO

	m_d.m_ia = newVal;

	STOPUNDO

	return S_OK;
}

STDMETHODIMP Surface::get_HeightBottom(float *pVal)
{
	*pVal = m_d.m_heightbottom;

	return S_OK;
}

STDMETHODIMP Surface::put_HeightBottom(float newVal)
{
	STARTUNDO

	m_d.m_heightbottom = newVal;

	STOPUNDO

	return S_OK;
}

STDMETHODIMP Surface::get_HeightTop(float *pVal)
{
	*pVal = m_d.m_heighttop;

	return S_OK;
}

STDMETHODIMP Surface::put_HeightTop(float newVal)
{
	STARTUNDO

	m_d.m_heighttop = newVal;

	STOPUNDO

	return S_OK;
}

STDMETHODIMP Surface::get_FaceColor(OLE_COLOR *pVal)
{
	*pVal = m_d.m_topcolor;

	return S_OK;
}

STDMETHODIMP Surface::put_FaceColor(OLE_COLOR newVal)
{
	STARTUNDO

	m_d.m_topcolor = newVal;

	STOPUNDO

	return S_OK;
}

/*HRESULT Surface::GetTypeName(BSTR *pVal)
	{
	*pVal = SysAllocString(L"Wall");

	return S_OK;
	}*/

/*int Surface::GetDialogID()
	{
	return IDD_PROPWALL;
	}*/

void Surface::GetDialogPanes(Vector<PropertyPane> *pvproppane)
	{
	PropertyPane *pproppane;

	pproppane = new PropertyPane(IDD_PROP_NAME, NULL);
	pvproppane->AddElement(pproppane);

	pproppane = new PropertyPane(IDD_PROPWALL_VISUALS, IDS_VISUALS);
	pvproppane->AddElement(pproppane);

	pproppane = new PropertyPane(IDD_PROPWALL_POSITION, IDS_POSITION);
	pvproppane->AddElement(pproppane);

	pproppane = new PropertyPane(IDD_PROPWALL_PHYSICS, IDS_STATE);
	pvproppane->AddElement(pproppane);

	pproppane = new PropertyPane(IDD_PROP_TIMER, IDS_MISC);
	pvproppane->AddElement(pproppane);
	}

void Surface::GetPointDialogPanes(Vector<PropertyPane> *pvproppane)
	{
	PropertyPane *pproppane;

	pproppane = new PropertyPane(IDD_PROPPOINT_VISUALSWTEX, IDS_VISUALS);
	pvproppane->AddElement(pproppane);

	pproppane = new PropertyPane(IDD_PROPPOINT_POSITION, IDS_POSITION);
	pvproppane->AddElement(pproppane);
	}

STDMETHODIMP Surface::get_CanDrop(VARIANT_BOOL *pVal)
{
	*pVal = FTOVB(m_d.m_fDroppable);

	return S_OK;
}

STDMETHODIMP Surface::put_CanDrop(VARIANT_BOOL newVal)
{
	STARTUNDO

	m_d.m_fDroppable = VBTOF(newVal);

	STOPUNDO

	return S_OK;
}

STDMETHODIMP Surface::get_IsDropped(VARIANT_BOOL *pVal)
{
	if (!g_pplayer)
		{
		return E_FAIL;
		}

	*pVal = FTOVB(m_fIsDropped);

	return S_OK;
}

STDMETHODIMP Surface::put_IsDropped(VARIANT_BOOL newVal)
{
	if (!g_pplayer || !m_d.m_fDroppable || !m_d.m_fInner)
		{
		return E_FAIL;
		}

	BOOL fNewVal = VBTOF(newVal);

	if (m_fIsDropped != fNewVal)
		{
		int i;

		m_fIsDropped = fNewVal;

		m_phitdrop->m_polydropanim.m_iframedesire = m_fIsDropped ? 1 : 0;

		for (i=0;i<m_vhoDrop.Size();i++)
			{
			m_vhoDrop.ElementAt(i)->m_fEnabled = !m_fIsDropped;
			}
		}

	return S_OK;
}

STDMETHODIMP Surface::get_DisplayTexture(VARIANT_BOOL *pVal)
{
	*pVal = FTOVB(m_d.m_fDisplayTexture);

	return S_OK;
}

STDMETHODIMP Surface::put_DisplayTexture(VARIANT_BOOL newVal)
{
	STARTUNDO

	m_d.m_fDisplayTexture = VBTOF(newVal);

	STOPUNDO

	return S_OK;
}

STDMETHODIMP Surface::get_SlingshotStrength(float *pVal)
{
	*pVal = m_d.m_slingshotforce/10;

	// Force value divided by 10 to make it look more like flipper strength value

	return S_OK;
}

STDMETHODIMP Surface::put_SlingshotStrength(float newVal)
{
	STARTUNDO

	m_d.m_slingshotforce = newVal*10;

	STOPUNDO

	return S_OK;
}

STDMETHODIMP Surface::get_Elasticity(float *pVal)
{
	*pVal = m_d.m_elasticity;

	return S_OK;
}

STDMETHODIMP Surface::put_Elasticity(float newVal)
{
	STARTUNDO

	m_d.m_elasticity = newVal;

	STOPUNDO

	return S_OK;
}

STDMETHODIMP Surface::get_CastsShadow(VARIANT_BOOL *pVal)
{
	*pVal = FTOVB(m_d.m_fCastsShadow);

	return S_OK;
}

STDMETHODIMP Surface::put_CastsShadow(VARIANT_BOOL newVal)
{
	STARTUNDO
	m_d.m_fCastsShadow = VBTOF(newVal);
	STOPUNDO

	return S_OK;
}

STDMETHODIMP Surface::get_Visible(VARIANT_BOOL *pVal)
{
	*pVal = FTOVB(m_d.m_fVisible);

	return S_OK;
}

STDMETHODIMP Surface::put_Visible(VARIANT_BOOL newVal)
{
	STARTUNDO

	m_d.m_fVisible = VBTOF(newVal);

	STOPUNDO

	return S_OK;
}

STDMETHODIMP Surface::get_SideImage(BSTR *pVal)
{
	OLECHAR wz[512];

	MultiByteToWideChar(CP_ACP, 0, m_d.m_szSideImage, -1, wz, 32);
	*pVal = SysAllocString(wz);

	return S_OK;
}

STDMETHODIMP Surface::put_SideImage(BSTR newVal)
{
	STARTUNDO

	WideCharToMultiByte(CP_ACP, 0, newVal, -1, m_d.m_szSideImage, 32, NULL, NULL);

	STOPUNDO

	return S_OK;
}

STDMETHODIMP Surface::get_Disabled(VARIANT_BOOL *pVal)
{
	*pVal = FTOVB(m_fDisabled);

	return S_OK;
}

STDMETHODIMP Surface::put_Disabled(VARIANT_BOOL newVal)
{
	STARTUNDO
	m_fDisabled = VBTOF(newVal);
	STOPUNDO

	return S_OK;
}

STDMETHODIMP Surface::get_SideVisible(VARIANT_BOOL *pVal)
{
	*pVal = FTOVB(m_d.m_fSideVisible);

	return S_OK;
}

STDMETHODIMP Surface::put_SideVisible(VARIANT_BOOL newVal)
{
	STARTUNDO

	m_d.m_fSideVisible = VBTOF(newVal);

	STOPUNDO

	return S_OK;
}
