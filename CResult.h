#pragma once


// CResult 대화 상자

class CResult : public CDialog
{
	DECLARE_DYNAMIC(CResult)

public:
	CResult(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~CResult();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG3 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	int r_perfect;
	int r_great;
	int r_miss;
	int r_mini;

	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	CRect m_client;
};
