// CEditMode.cpp: 구현 파일
//

#include "stdafx.h"
#include "MFC_Team_Dlg01.h"
#include "CEditMode.h"
#include "afxdialogex.h"
#include "CTitle.h"

#include <locale.h>
#include <math.h>

#pragma comment(lib, "Strmiids.lib")

// CEditMode 대화 상자

IMPLEMENT_DYNAMIC(CEditMode, CDialog)

CEditMode::CEditMode(CWnd* pParent /*=nullptr*/)
	: CDialog(IDD_DIALOG2, pParent)
	, m_title(_T(""))
	, m_all_time(_T(""))
{

}

CEditMode::~CEditMode()
{
}

void CEditMode::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_SLIDER1, m_slide);
	DDX_Control(pDX, IDC_STATIC1, m_control);
	DDX_Control(pDX, IDC_STATIC2, m_screen);
	DDX_Control(pDX, IDC_SLIDER2, m_cur_pos);
	DDX_Text(pDX, IDC_EDIT1, m_title);
	DDX_Text(pDX, IDC_EDIT2, m_all_time);
	DDX_Control(pDX, IDC_BUTTON1, b_select);
	DDX_Control(pDX, IDC_BUTTON2, b_play);
	DDX_Control(pDX, IDC_BUTTON3, b_pause);
	DDX_Control(pDX, IDOK, b_save);
}


BEGIN_MESSAGE_MAP(CEditMode, CDialog)
	ON_WM_PAINT()
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLIDER1, &CEditMode::OnNMCustomdrawSlider1)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLIDER2, &CEditMode::OnNMCustomdrawSlider2)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BUTTON1, &CEditMode::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &CEditMode::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON3, &CEditMode::OnBnClickedButton3)
	ON_STN_CLICKED(IDC_STATIC1, &CEditMode::OnStnClickedStatic1)
END_MESSAGE_MAP()


// CEditMode 메시지 처리기


BOOL CEditMode::OnInitDialog()
{
	CDialog::OnInitDialog();

	GetParent()->ShowWindow(SW_HIDE);

	m_slide.SetRange(0, 0); // 입력받은 시간 혹은 음악길이 X 100 -> 0.01 단위
	m_slide.SetPos(0); //첫 위치는 0
	m_slide.SetTicFreq(1000); //눈금 빈도 10초마다

	m_cur_pos.SetRange(0, 50);
	m_cur_pos.SetPos(0);
	m_cur_pos.SetTicFreq(5);

	m_mp3.SetVolume(5000);
	m_mp3.LoadDLL(L"LibMP3DLL.dll");

	bmp_size = 0;

	m_check = false;
	playing = false;

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


void CEditMode::OnOK()
{
	CTitle dlg;
	if (dlg.DoModal() == IDOK)
	{
		CStdioFile file;
		CFileException e;
		
		CString t_title = dlg.t_title + ".txt";

		setlocale(LC_ALL, "korean");
		file.Open(t_title, CStdioFile::modeCreate | CStdioFile::modeWrite | CStdioFile::modeNoTruncate, &e);
		file.WriteString(m_title);
		CString ttt;
		ttt.Format(_T("%d"), duration * 100);
		file.WriteString(_T("\n"));
		file.WriteString(ttt);

		for (int i = 0; i < arrow.GetSize(); i++) { //내용 입력
			file.WriteString(_T("\n"));

			CString str;
			str.Format(_T("%d %d %d"), arrow[i].m_pos, arrow[i].m_arrow, arrow[i].m_delay);
			file.WriteString(str);
		}
		file.Close();

		m_mp3.Cleanup();
		CDialog::OnOK();
	}
}


void CEditMode::OnCancel()
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	m_mp3.Cleanup();
	CDialog::OnCancel();
}


void CEditMode::OnPaint()
{
	CPaintDC dc(this); // device context for painting
					   // TODO: 여기에 메시지 처리기 코드를 추가합니다.
					   // 그리기 메시지에 대해서는 CDialog::OnPaint()을(를) 호출하지 마십시오.
	CClientDC dc_1(&m_control); //조작버튼
	CClientDC dc_2(&m_screen); //실제 이미지버튼

	if (!m_check && !(bmp_size<1)) {
		m_control.GetClientRect(&size_1); //이미지를 그릴 공간의 크기정보를 받아옴
		m_screen.GetClientRect(&size_2);

		img_1.CreateCompatibleBitmap(&dc_1, size_1.Width(), size_1.Height()); //비트맵 생성
		img_2.CreateCompatibleBitmap(&dc_2, bmp_size *(size_2.Width() / 50), size_2.Height());

		memdc[0].CreateCompatibleDC(&dc_1);
		memdc[1].CreateCompatibleDC(&dc_2); //공간생성
		memdc[0].SelectObject(&img_1);
		memdc[1].SelectObject(&img_2); //비트맵 쥐여줌
		m_check = true;
	}
	if (m_check) {
		{
			CDC sub_dc;
			sub_dc.CreateCompatibleDC(&memdc[0]); //버튼 그릴 때 사용할 부dc

			for (int i = 0; i < 4; i++) //원래는 비트맵으로 그려준다
			{
				CBitmap sub_bmp;
				BITMAP sub_info;

				sub_bmp.LoadBitmapW(IDB_BITMAP1 + i*10); //로딩하고
				sub_bmp.GetBitmap(&sub_info); //크기 정보를 얻오오고
				sub_dc.SelectObject(sub_bmp); //쥐여주고
				memdc[0].StretchBlt((size_1.Width() / 4) *i + (size_1.Width() / 4 - 69) / 2, (size_1.Height() - 69) / 2, 69, 69,
					&sub_dc, 0, 0, sub_info.bmWidth, sub_info.bmHeight, SRCCOPY);
				//그린다
			}
			dc_1.BitBlt(0, 0, size_1.Width(), size_1.Height(), &memdc[0], 0, 0, SRCCOPY); //합친 그림을 그린다.
		}
		{

			CDC dc3;
			CBitmap sub_bmp;
			BITMAP sub_info;

			dc3.CreateCompatibleDC(&dc_2);


			CDC sub_dc;
			sub_dc.CreateCompatibleDC(&memdc[1]); //아래 라인을그릴 때 사용할 부dc

			sub_bmp.LoadBitmapW(IDB_BITMAP57);
			sub_bmp.GetBitmap(&sub_info);
			sub_dc.SelectObject(sub_bmp); //쥐여주고


										  // 비트맵을 공용으로 바꾸고 매번 갱신x 
										  // 이미지출력이안없어짐.

			dc_2.BitBlt(0, 0, size_2.Width(), size_2.Height(), &memdc[1], pos_all*(size_2.Width() / 50), 0, SRCCOPY);

			dc_2.TransparentBlt((size_2.Width() / 50) *pos_cur, 0, size_2.Width() / 100, 15,
				&sub_dc, 0, 0, sub_info.bmWidth, sub_info.bmHeight, RGB(0, 0, 0));
		}
	}
}


void CEditMode::OnNMCustomdrawSlider1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	
	if (!playing) {
		__int64 duration = m_mp3.GetDuration();
		__int64 pos = m_slide.GetPos() * 100000;
		m_mp3.SetPositions(&pos, &duration, true);

		if (pos_all != m_slide.GetPos())
		{
			pos_all = m_slide.GetPos();
			Invalidate(false);
		}
	}
	//현재 슬라이더 위치를 받아옴
	*pResult = 0;
}


void CEditMode::OnNMCustomdrawSlider2(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if (pos_cur != m_cur_pos.GetPos()) {
		pos_cur = m_cur_pos.GetPos();
		Invalidate(false);
	}
	*pResult = 0;
}


void CEditMode::OnTimer(UINT_PTR nIDEvent)
{
	int duration = m_mp3.GetDuration() / 10000000;
	__int64 curPos = m_mp3.GetCurrentPosition();
	__int64 pos = (curPos / 100000);
	m_slide.SetPos((int)(pos));

	pos_all = m_slide.GetPos();

	if (pos_all > bmp_size - 51) {
		KillTimer(101);
		playing = false;
	}
	Invalidate(false);

	CDialog::OnTimer(nIDEvent);
}


void CEditMode::OnBnClickedButton1()
{
	CFileDialog m_dlg(true, _T("wav file(*.wav)"), _T("*.wav"), OFN_READONLY, _T("wav file(*.wav)")); // true가 파일 열기, , 읽기 전용,
	if (m_dlg.DoModal() == IDOK) {
		m_mp3.Cleanup();

		CString filename = m_dlg.GetPathName(); // 경로 정보 가져오기

		m_mp3.Load(filename);

		CString time;

		duration = m_mp3.GetDuration() / 10000000; //전체시간

		time.Format(_T("%d:%02d"), duration / 60, duration % 60);

		m_title = filename;
		m_all_time = time;
		// 프로그래스바의 길이를 지정한다.(0 ~ 재생시간길이)
		bmp_size = duration * 100; //1분 23초 -> 8300,8250 4분 곡 = 240 00
		m_slide.SetRange(0, bmp_size - 50); //0,01초 단위로 슬라이더 생성
		m_slide.SetPos(0);

		UpdateData(false);

		b_select.EnableWindow(false);
		b_play.EnableWindow(true);
		b_pause.EnableWindow(true);
		b_save.EnableWindow(true);

		Invalidate(false);
	}
}


void CEditMode::OnBnClickedButton2()
{
	m_mp3.Play();
	SetTimer(101, 10, NULL);
	playing = true;
}


void CEditMode::OnBnClickedButton3()
{
	m_mp3.Pause();
	KillTimer(101);
	playing = false;
}


void CEditMode::OnStnClickedStatic1()
{
	if (size_1.Width() != 0) {
		CRect m_size;
		m_control.GetClientRect(m_size);

		CPoint pos;
		GetCursorPos(&pos);
		GetDlgItem(IDC_STATIC1)->ScreenToClient(&pos);

		int note_pos = m_slide.GetPos() + pos_cur; // 현재 슬라이더위치 + 커서위치
		int dir = pos.x / (size_1.Width() / 4);

		Note_s new_n(note_pos, dir, 0);

		int first = 0;
		int last = arrow.GetSize() - 1;
		int mid = 0;

		int check = true;

		while (first <= last) {
			mid = (first + last) / 2;
			if (arrow[mid].m_pos == note_pos) {
				arrow.RemoveAt(mid);
				break;
			}
			else {
				if (arrow[mid].m_pos > note_pos) {
					last = mid - 1;
					check = true;
				}
				else {
					first = mid + 1;
					check = false;
				}
			}
		}
		arrow.InsertAt(first, new_n);

		for (int i = mid; i < arrow.GetSize(); i++) {
			CDC sub_dc;
			CBitmap sub_bmp;
			BITMAP sub_info;

			sub_dc.CreateCompatibleDC(&memdc[1]);
			sub_bmp.LoadBitmapW(IDB_BITMAP2 + arrow[i].m_arrow); //로딩하고
			sub_bmp.GetBitmap(&sub_info); //크기 정보를 얻오오고
			sub_dc.SelectObject(&sub_bmp); //쥐여주고

			memdc[1].TransparentBlt(arrow[i].m_pos*(size_2.Width() / 50) - (size_2.Width() / 50), (size_2.Height() - 69) / 2, 69, 69,
				&sub_dc, 0, 0, sub_info.bmWidth, sub_info.bmHeight, RGB(0, 0, 0));
		}
		Invalidate(false);
	}
}


BOOL CEditMode::PreTranslateMessage(MSG* pMsg)
{
	if (pMsg->message == WM_KEYDOWN)
	{
		bool input = true;
		int dir;
		switch (pMsg->wParam)
		{
		case VK_DOWN:
			dir = 0;
			break;
		case VK_LEFT:
			dir = 1;
			break;
		case VK_RIGHT:
			dir = 2;
			break;
		case VK_UP:
			dir = 3;
			break;
		default:
			input = false;
			break;
		}

		if (size_1.Width() != 0 && input) {
			int note_pos = m_slide.GetPos() + pos_cur; // 현재 슬라이더위치 + 커서위치		

			Note_s new_n(note_pos, dir, 0);

			int first = 0;
			int last = arrow.GetSize() - 1;
			int mid = 0;

			int check = true;

			while (first <= last) {
				mid = (first + last) / 2;
				if (arrow[mid].m_pos == note_pos) {
					arrow.RemoveAt(mid);
					break;
				}
				else {
					if (arrow[mid].m_pos > note_pos) {
						last = mid - 1;
						check = true;
					}
					else {
						first = mid + 1;
						check = false;
					}
				}
			}
			arrow.InsertAt(first, new_n);

			for (int i = mid; i < arrow.GetSize(); i++) {
				CDC sub_dc;
				CBitmap sub_bmp;
				BITMAP sub_info;

				sub_dc.CreateCompatibleDC(&memdc[1]);
				sub_bmp.LoadBitmapW(IDB_BITMAP1 + arrow[i].m_arrow * 10); //로딩하고
				sub_bmp.GetBitmap(&sub_info); //크기 정보를 얻오오고
				sub_dc.SelectObject(&sub_bmp); //쥐여주고

				memdc[1].TransparentBlt(arrow[i].m_pos*(size_2.Width() / 50) - (size_2.Width() / 50), (size_2.Height() - 69) / 2, 69, 69,
					&sub_dc, 0, 0, sub_info.bmWidth, sub_info.bmHeight, RGB(0, 0, 0));
			}
			Invalidate(false);
			return TRUE;
		}
	}
	return CDialog::PreTranslateMessage(pMsg);
}
