
// CircleDrawerDlg.h: 헤더 파일
//

#pragma once


// CCircleDrawerDlg 대화 상자
class CCircleDrawerDlg : public CDialogEx
{
private:
	CImage m_image;
	CDC m_image_dc;

	CRect m_rect;
	CPoint m_center_pos;
	CPen m_grid_pen, m_sine_pen;
	CBrush m_red_brush;
	int m_step = 1;

// 생성입니다.
public:
	CCircleDrawerDlg(CWnd* pParent = nullptr);	// 표준 생성자입니다.

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CIRCLEDRAWER_DIALOG };
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
	afx_msg void OnDestroy();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
};
