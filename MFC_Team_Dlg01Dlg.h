
// MFC_Team_Dlg01Dlg.h: 헤더 파일
//

#pragma once
#include "CMyBitmapButton.h"

// CMFCTeamDlg01Dlg 대화 상자
class CMFCTeamDlg01Dlg : public CDialogEx
{
// 생성입니다.
public:
	CMFCTeamDlg01Dlg(CWnd* pParent = nullptr);	// 표준 생성자입니다.

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MFC_TEAM_DLG01_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 지원입니다.


// 구현입니다.
protected:
	HICON m_hIcon;

	// 생성된 메시지 맵 함수
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButton1();
	virtual void OnCancel();
	CStatic m_mainPic;
	CMyBitmapButton m_startBtn;
	CMyBitmapButton m_editBtn;
	CMyBitmapButton m_exitBtn;
	afx_msg void OnBnClickedEditbtn();
};
