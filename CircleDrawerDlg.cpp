
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
	ON_WM_DESTROY()
	ON_WM_TIMER()
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

	m_image.Create(m_rect.Width(), m_rect.Height(), 32, 0);
	m_image_dc.Attach(m_image.GetDC()); // HDC -> CDC Type으로 캐스팅

	SetBackgroundColor(RGB(0, 0, 0));

	m_grid_pen.CreatePen(PS_DOT, 1, RGB(168, 168, 168));
	m_sine_pen.CreatePen(PS_SOLID, 2, RGB(0, 200, 255));
	m_cos_pen.CreatePen(PS_SOLID, 2, RGB(100, 255, 100));
	
	m_red_brush.CreateSolidBrush(RGB(255, 0, 0));
	m_green_brush.CreateSolidBrush(RGB(0, 200, 0));
	m_image_dc.SelectObject(&m_red_brush);

	SetTimer(1, 10, NULL);
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
		
		m_image.Draw(dc, 0, 0);

		// CDialogEx::OnPaint();
	}
}

// 사용자가 최소화된 창을 끄는 동안에 커서가 표시되도록 시스템에서
//  이 함수를 호출합니다.
HCURSOR CCircleDrawerDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CCircleDrawerDlg::OnDestroy()
{
	CDialogEx::OnDestroy();

	KillTimer(1);

	m_grid_pen.DeleteObject();
	m_sine_pen.DeleteObject();
	m_red_brush.DeleteObject();

	m_image_dc.Detach();
	m_image.ReleaseDC();
}

void CCircleDrawerDlg::ShowGrid()
{
	m_image_dc.SelectObject(&m_grid_pen);

	// m_image_dc.SetBkMode(TRANSPARENT);  // FillSolidRect에서 처리하기떄문에 생략 가능

	// 수평과 수직선을 그린다
	m_image_dc.MoveTo(m_center_pos.x, 0);
	m_image_dc.LineTo(m_center_pos.x, m_rect.bottom);

	m_image_dc.MoveTo(0, m_center_pos.y);
	m_image_dc.LineTo(m_rect.right, m_center_pos.y);
}

void CCircleDrawerDlg::ShowSine()
{
	m_image_dc.SelectObject(&m_sine_pen);

	int degree, x, y;
	double radian;

	/*for (x = 0; x < m_rect.right; x++)*/
	for (x = 0; x < m_step; x++)
	{
		degree = x - m_center_pos.x;
		radian = degree * PI / 180;
		y = (int)(sin(radian) * -100) + m_center_pos.y; // sin값은 -1 0 1 안에서 움직이니 100을 곱해서 섬세하게 값 처리
														// -100으로 곱한 이유는 좌표계가 달라 sin 그래프를 reversing
														// 좌표계 체계가 MFC에서는 다르니 m_center_pos.y를 따로 더함
		if (x) m_image_dc.LineTo(x, y);
		else m_image_dc.MoveTo(x, y);
	}
	m_image_dc.SelectObject(&m_red_brush);
	m_image_dc.Ellipse(x - 20, y - 20, x + 20, y + 20);
}

void CCircleDrawerDlg::ShowCos()
{
	m_image_dc.SelectObject(&m_cos_pen);

	int degree, x, y;
	double radian;

	/*for (x = 0; x < m_rect.right; x++)*/
	for (x = 0; x < m_step; x++)
	{
		degree = x - m_center_pos.x;
		radian = degree * PI / 180;
		y = (int)(cos(radian) * -100) + m_center_pos.y; 
		if (x) m_image_dc.LineTo(x, y);
		else m_image_dc.MoveTo(x, y);
	}
	m_image_dc.SelectObject(&m_green_brush);
	m_image_dc.Ellipse(x - 20, y - 20, x + 20, y + 20);
}

void CCircleDrawerDlg::OnTimer(UINT_PTR nIDEvent)
{
	if (nIDEvent == 1)
	{
		if (m_step < m_rect.right) m_step++;
		else m_step = 1;
		
		m_image_dc.FillSolidRect(m_rect, RGB(0, 0, 0));

		ShowGrid();
		ShowSine();
		ShowCos();

		Invalidate(FALSE);
	}
	else CDialogEx::OnTimer(nIDEvent);
}
