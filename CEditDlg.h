#pragma once
#include "CMyBitmapButton.h"

// CEditDlg 대화 상자

class CEditDlg : public CDialog
{
	DECLARE_DYNAMIC(CEditDlg)

public:
	CEditDlg(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~CEditDlg();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_EDITDLG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	CStatic m_editMain;
	virtual BOOL OnInitDialog();
	CMyBitmapButton m_esBtn;
	CMyBitmapButton m_meBtn;
	afx_msg void OnBnClickedEsbtn();
	afx_msg void OnBnClickedMebtn();
};
