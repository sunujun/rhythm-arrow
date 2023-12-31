// CMyBitmapButton.cpp: 구현 파일
//

#include "stdafx.h"
#include "MFC_Team_Dlg01.h"
#include "CMyBitmapButton.h"
#include "CLayoutMgr.h"

// CMyBitmapButton

IMPLEMENT_DYNAMIC(CMyBitmapButton, CBitmapButton)

CMyBitmapButton::CMyBitmapButton()
{
	m_clrTrans = RGB(0, 0, 0);
	m_bMouseHover = false;
	m_nIDBitmapResourceHover = 0;
	ZeroMemory(&m_stTrackMouse, sizeof(m_stTrackMouse));
}

CMyBitmapButton::~CMyBitmapButton()
{
}


BEGIN_MESSAGE_MAP(CMyBitmapButton, CBitmapButton)
	ON_WM_ERASEBKGND()
	ON_WM_MOUSEMOVE()
	ON_WM_MOUSELEAVE()
	ON_WM_CREATE()
END_MESSAGE_MAP()



// CMyBitmapButton 메시지 처리기




void CMyBitmapButton::DrawItem(LPDRAWITEMSTRUCT lpDIS)
{

	// TODO:  지정된 항목을 그리는 코드를 추가합니다.
	ASSERT(lpDIS != NULL);
	// must have at least the first bitmap loaded before calling DrawItem  
	ASSERT(m_bitmap.m_hObject != NULL);     // required  

											// use the main bitmap for up, the selected bitmap for down  
	CBitmap* pBitmap = &m_bitmap;
	UINT state = lpDIS->itemState;
	if (m_bMouseHover == true && NULL != m_cBitmapHover.GetSafeHandle())
	{
		//pBitmap = &m_bitmap;
		pBitmap = &m_cBitmapHover;
		if ((state & ODS_SELECTED) && m_bitmapSel.m_hObject != NULL) {
			pBitmap = &m_bitmapSel;
		}
		else if ((state & ODS_FOCUS) && m_bitmapFocus.m_hObject != NULL) {
			pBitmap = &m_bitmapFocus;   // third image for focused  

		}
	//	//Invalidate(true);
	}
	if ((state & ODS_SELECTED) && m_bitmapSel.m_hObject != NULL) {
		pBitmap = &m_bitmapSel;
	}
	else if ((state & ODS_FOCUS) && m_bitmapFocus.m_hObject != NULL) {
		pBitmap = &m_bitmapFocus;   // third image for focused  

	}
	else if ((state & ODS_DISABLED) && m_bitmapDisabled.m_hObject != NULL) {
		pBitmap = &m_bitmapDisabled;   // last image for disabled  
	}
	else {
		pBitmap = &m_bitmap;

	}

	// draw the whole button  
	CDC* pDC = CDC::FromHandle(lpDIS->hDC);
	CDC memDC;
	memDC.CreateCompatibleDC(pDC);
	CBitmap* pOld = memDC.SelectObject(pBitmap);
	if (pOld == NULL)
		return;
	CRect rect;
	rect.CopyRect(&lpDIS->rcItem);
	pDC->TransparentBlt(rect.left, rect.top, 414, 76,
		&memDC, 0, 0, 414, 76, RGB(91,155,213));
	memDC.SelectObject(pOld);
	//CLayoutMgr::DrawBitmapTrans(pDC, pBitmap, rect.left, rect.top, m_clrTrans);
	//Invalidate(true);


}

BOOL CMyBitmapButton::OnEraseBkgnd(CDC* pDC)
{
	return false;
}

void CMyBitmapButton::OnMouseMove(UINT nFlags, CPoint point)
{
	if (false == m_bMouseHover)
	{
		::_TrackMouseEvent(&m_stTrackMouse);
		m_bMouseHover = true;
		Invalidate(true);
	}
	CBitmapButton::OnMouseMove(nFlags, point);
}

void CMyBitmapButton::OnMouseLeave()
{

	m_bMouseHover = false;
	Invalidate(true);

	CBitmapButton::OnMouseLeave();
}

VOID CMyBitmapButton::SetHoverBitmapID(IN UINT nIDBitmapResourceHover)
{
	m_nIDBitmapResourceHover = nIDBitmapResourceHover;
	if (0 != m_nIDBitmapResourceHover)
	{
		m_cBitmapHover.DeleteObject();
		m_cBitmapHover.LoadBitmap(m_nIDBitmapResourceHover);
	}
	
}
int CMyBitmapButton::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CBitmapButton::OnCreate(lpCreateStruct) == -1)
		return -1;

	m_stTrackMouse.cbSize = sizeof(TRACKMOUSEEVENT);
	m_stTrackMouse.dwFlags = TME_LEAVE;
	m_stTrackMouse.dwHoverTime = HOVER_DEFAULT;
	m_stTrackMouse.hwndTrack = GetSafeHwnd();

	return 0;
}
