// CTitle.cpp: 구현 파일
//

#include "stdafx.h"
#include "MFC_Team_Dlg01.h"
#include "CTitle.h"
#include "afxdialogex.h"


// CTitle 대화 상자

IMPLEMENT_DYNAMIC(CTitle, CDialog)

CTitle::CTitle(CWnd* pParent /*=nullptr*/)
	: CDialog(IDD_DIALOG5, pParent)
	, t_title(_T(""))
{

}

CTitle::~CTitle()
{
}

void CTitle::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, t_title);
}


BEGIN_MESSAGE_MAP(CTitle, CDialog)
END_MESSAGE_MAP()


// CTitle 메시지 처리기
