
// CircleDrawerDlg.cpp: 구현 파일
//

#include "pch.h"
#include "framework.h"
#include "CircleDrawer.h"
#include "CircleDrawerDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CCircleDrawerDlg 대화 상자



CCircleDrawerDlg::CCircleDrawerDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_CIRCLEDRAWER_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CCircleDrawerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CCircleDrawerDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
END_MESSAGE_MAP()


// CCircleDrawerDlg 메시지 처리기

BOOL CCircleDrawerDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 이 대화 상자의 아이콘을 설정합니다.  응용 프로그램의 주 창이 대화 상자가 아닐 경우에는
	//  프레임워크가 이 작업을 자동으로 수행합니다.
	SetIcon(m_hIcon, TRUE);			// 큰 아이콘을 설정합니다.
	SetIcon(m_hIcon, FALSE);		// 작은 아이콘을 설정합니다.

	GetClientRect(m_rect);
	m_center_pos = m_rect.CenterPoint();
	// TODO: 여기에 추가 초기화 작업을 추가합니다.

	return TRUE;  // 포커스를 컨트롤에 설정하지 않으면 TRUE를 반환합니다.
}

// 대화 상자에 최소화 단추를 추가할 경우 아이콘을 그리려면
//  아래 코드가 필요합니다.  문서/뷰 모델을 사용하는 MFC 애플리케이션의 경우에는
//  프레임워크에서 이 작업을 자동으로 수행합니다.

#include<math.h>
#define PI		3.141592
void CCircleDrawerDlg::OnPaint()
{
	CPaintDC dc(this); // 그리기를 위한 디바이스 컨텍스트입니다.
	if (IsIconic())
	{
		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 클라이언트 사각형에서 아이콘을 가운데에 맞춥니다.
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 아이콘을 그립니다.
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		// 수평과 수직선을 그린다
		dc.MoveTo(m_center_pos.x, 0);
		dc.LineTo(m_center_pos.x, m_rect.bottom);

		dc.MoveTo(0, m_center_pos.y);
		dc.LineTo(m_rect.right, m_center_pos.y);

		int degree, x, y;
		double radian;

		for (x = 0; x < m_rect.right; x++)
		{
			degree = x - m_center_pos.x;
			radian = degree * PI / 180;
			y = (int)(sin(radian) *-100) + m_center_pos.y; // sin값은 -1 0 1 안에서 움직이니 100을 곱해서 섬세하게 값 처리
															// -100으로 곱한 이유는 좌표계가 달라 sin 그래프를 reversing
															// 좌표계 체계가 MFC에서는 다르니 m_center_pos.y를 따로 더함
			if (x) dc.LineTo(x, y);
			else dc.MoveTo(x, y);


		}


		// CDialogEx::OnPaint();
	}
}

// 사용자가 최소화된 창을 끄는 동안에 커서가 표시되도록 시스템에서
//  이 함수를 호출합니다.
HCURSOR CCircleDrawerDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

