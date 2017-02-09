
// MFCApplication1Dlg.h : 헤더 파일
//

#pragma once
#include "afxwin.h"
#include "afxcmn.h"


// CMFCApplication1Dlg 대화 상자
class CMFCApplication1Dlg : public CDialogEx
{
// 생성입니다.
public:
	CMFCApplication1Dlg(CWnd* pParent = NULL);	// 표준 생성자입니다.
	static UINT ThreadFirst(LPVOID _mothod);
// 대화 상자 데이터입니다.
	enum { IDD = IDD_MFCAPPLICATION1_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 지원입니다.


// 구현입니다.
protected:
	HICON m_hIcon;

	// 생성된 메시지 맵 함수
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOpen();
	afx_msg void OnBnClickedPrevious();
	afx_msg void OnBnClickedNext();
	int pid_1;
	int pid_2;
	int pid_3;
	int pid_4;
	int pid_5;

	int sit;
	int stand;
	int sleep;
	int fall;
	int walk;
	int run;
	int box;
	int kick;
	int ask;
	int twave;
	int ambiguous;

	/*BOOL pid_1;*/

	int EDText1;
	afx_msg void OnBnClickedAssign();
	afx_msg void OnBnClickedSit();
	afx_msg void OnBnClickedStand();
	afx_msg void OnBnClickedSleep();
	afx_msg void OnBnClickedFall();
	afx_msg void OnBnClickedWalk();
	afx_msg void OnBnClickedRun();
	afx_msg void OnBnClickedBox();
	afx_msg void OnBnClickedKick();
	afx_msg void OnBnClickedAsk();
	afx_msg void OnBnClickedTwave();
	afx_msg void OnBnClickedAmbiguous();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedDelete();
	afx_msg void OnBnClickedAutoAssign();
	afx_msg void OnBnClickedMove();
	afx_msg void OnBnClickedContinue();
	afx_msg void OnBnClickedStop();

	CButton pCheck1;
	CButton pCheck2;
	CButton pCheck3;
	CButton pCheck4;
	CButton pCheck5;
	CButton pCheck6;
	CButton pCheck7;
	CButton pCheck8;
	CButton pCheck9;
	CButton pCheck10;

	CSliderCtrl SliderCtrl;
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
};
