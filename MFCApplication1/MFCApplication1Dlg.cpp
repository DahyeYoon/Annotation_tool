
// MFCApplication1Dlg.cpp : 구현 파일
//

#include "stdafx.h"
#include "MFCApplication1.h"
#include "MFCApplication1Dlg.h"
#include "afxdialogex.h"
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include "opencv\cv.h"
#include "opencv\highgui.h"
#include "opencv\CvvImage.h"
#include <io.h>       //access
//#include <unistd.h>



#ifdef _DEBUG
#define new DEBUG_NEW
#endif

using namespace cv;
using namespace std;

CDC	*pDCRGB;
CWnd *pWndRGB;
CvvImage cImageRGB;
CvCapture* pCaptureRGB;//cv capture point

IplImage* RGBImage;
IplImage* Depth1;
CString path_rgb;
CString path_roi;
CString path_gt;
CString path_gt_2;
//CString path_person_1;
CString path_revisedLabel;

IplImage* RGB;
//IplImage *frame;

CvCapture* m_Capture;
IplImage* frame_;
CvvImage c_ImageRGB;

char roipath[100], GTpath[100], GTpath_2[100];
char person1[100], revisedLabel[100];
FILE *fp;
FILE *label;
FILE *label_2;
FILE *re_label;
//FILE *testfp;
CString pre_label = _T("");
CString cur_label = _T("");
int numAction;
int p_label[11], c_label[11];
int total;
string toStrAction[11];

// Variables for ROI txt file
vector<int> nPeople;
vector<int> nHuman;
vector<int> nFrame[30];
vector<int> roi[30];
vector<int> roi_1;
vector<int> pt1x[30], pt1y[30], pt2x[30], pt2y[30], pt3x[30], pt3y[30];
vector<int> wPID;
vector<string> wLabel;
vector<int> scrTrack;



int x_1;//left-top-x
int y_1;//left-top-y
int x_2;//rigth-bottom-x
int y_2;//right-bottom-y
int nTemp;
int roi_idx[30];
int cnt = 0;
int fcnt_0, fcnt_1, fcnt_2, fcnt_3, fcnt_4, fcnt_5, fcnt_6, fcnt_7, fcnt_8, fcnt_9, fcnt_10, fcnt_11, fcnt_12, fcnt_13, fcnt_14, fcnt_15, fcnt_16, fcnt_17, fcnt_18, fcnt_19, fcnt_20;
CString strAction = _T("");
int cbList[11] = { 1034, 1035, 1036, 1037, 1038, 1039, 1040, 1041, 1042, 1043, 1044 };
BOOL bcheck[11];
int cur_check = 100;
int pre_check;
int PersonID[10] = { 1012, 1013, 1014, 1015, 1016, 1017, 1018, 1019, 1020, 1021 };
BOOL bradio[10];
int PID;
int txtChangePos;
char *posFind = NULL;
int strLabel;
char strTemp[30];
CButton* pCheck[11];
CButton* pCheck_2;
int rID;
BOOL scrCheck = FALSE;
BOOL preBtnCheck = FALSE;
BOOL deleteCheck = FALSE;
BOOL moveCheck = FALSE;
BOOL StopCheck = TRUE;
BOOL auto_assignCheck = FALSE;
BOOL Cbox_check = FALSE;
BOOL alertCheck = FALSE;
int init_cnt;
int scroll_pos;
int iter;
int lastAssignPos;
int wPIDsize;
int wLabelsize;
CString strName;

int pre_pid;
char pre_gt[100];
int gt_check;
int saveFrame_cnt = -1;


//-------------------------- Regarding to visualization -- for color of ROI, text, etc.. -----------------------------------
CvFont font;
int R[6] = { 255, 51, 4, 218, 150, 60 };
int G[6] = { 255, 51, 73, 150, 60, 240 };
int B[6] = { 255, 218, 240, 51, 60, 54 };
int r, g, b;
int pre_RV[30], cur_RV[30];
int rand_value;
double hScale = 0.5;
double vScale = 0.5;
int lineWidth = 2;
char output_text[30];
char output_text_2[30];
//--------------------------------------------------------------------------------------------------------------------------
void action_label_out(int ret, string &out){

	switch (ret) {
	case 1:
		out = "SITTING";
		break;
	case 2:
		out = "STANDING";
		break;
	case 3:
		out = "SLEEPING";
		break;
	case 4:
		out = "FALLING";
		break;
	case 5:
		out = "WALKING";
		break;
	case 6:
		out = "RUNNING";
		break;
	case 7:
		out = "BOXING";
		break;
	case 8:
		out = "KICKING";
		break;
	case 9:
		out = "ASKING";
		break;
	case 10:
		out = "HAND_WAVING";
		break;
	case 11:
		out = "AMBIGUOUS";
		break;
	}

}
// 응용 프로그램 정보에 사용되는 CAboutDlg 대화 상자입니다.

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

	// 대화 상자 데이터입니다.
	enum { IDD = IDD_ABOUTBOX };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	// 구현입니다.
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CMFCApplication1Dlg 대화 상자



CMFCApplication1Dlg::CMFCApplication1Dlg(CWnd* pParent /*=NULL*/)
: CDialogEx(CMFCApplication1Dlg::IDD, pParent)
, EDText1(0)
, pid_1(0)
, sit(0)

{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CMFCApplication1Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, EDText1);
	DDX_Radio(pDX, IDC_RADIO1, pid_1);
	DDX_Check(pDX, IDC_SIT, sit);
	DDX_Check(pDX, IDC_STAND, stand);
	DDX_Check(pDX, IDC_SLEEP, sleep);
	DDX_Check(pDX, IDC_FALL, fall);
	DDX_Check(pDX, IDC_WALK, walk);
	DDX_Check(pDX, IDC_RUN, run);
	DDX_Check(pDX, IDC_BOX, box);
	DDX_Check(pDX, IDC_KICK, kick);
	DDX_Check(pDX, IDC_ASK, ask);
	DDX_Check(pDX, IDC_TWAVE, twave);
	DDX_Check(pDX, IDC_AMB, ambiguous);
	DDX_Control(pDX, IDC_RADIO1, pCheck1);
	DDX_Control(pDX, IDC_RADIO2, pCheck2);
	DDX_Control(pDX, IDC_RADIO3, pCheck3);
	DDX_Control(pDX, IDC_RADIO4, pCheck4);
	DDX_Control(pDX, IDC_RADIO5, pCheck5);
	DDX_Control(pDX, IDC_RADIO6, pCheck6);
	DDX_Control(pDX, IDC_RADIO7, pCheck7);
	DDX_Control(pDX, IDC_RADIO8, pCheck8);
	DDX_Control(pDX, IDC_RADIO9, pCheck9);
	DDX_Control(pDX, IDC_RADIO10, pCheck10);
	DDX_Control(pDX, IDC_SLIDER1, SliderCtrl);
}

BEGIN_MESSAGE_MAP(CMFCApplication1Dlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_OPEN, &CMFCApplication1Dlg::OnBnClickedOpen)
	ON_BN_CLICKED(IDC_PREVIOUSS, &CMFCApplication1Dlg::OnBnClickedPrevious)
	ON_BN_CLICKED(IDC_NEXT, &CMFCApplication1Dlg::OnBnClickedNext)
	ON_BN_CLICKED(IDC_ASSIGN, &CMFCApplication1Dlg::OnBnClickedAssign)
	ON_BN_CLICKED(IDC_SIT, &CMFCApplication1Dlg::OnBnClickedSit)
	ON_BN_CLICKED(IDC_STAND, &CMFCApplication1Dlg::OnBnClickedStand)
	ON_BN_CLICKED(IDC_SLEEP, &CMFCApplication1Dlg::OnBnClickedSleep)
	ON_BN_CLICKED(IDC_FALL, &CMFCApplication1Dlg::OnBnClickedFall)
	ON_BN_CLICKED(IDC_WALK, &CMFCApplication1Dlg::OnBnClickedWalk)
	ON_BN_CLICKED(IDC_RUN, &CMFCApplication1Dlg::OnBnClickedRun)
	ON_BN_CLICKED(IDC_BOX, &CMFCApplication1Dlg::OnBnClickedBox)
	ON_BN_CLICKED(IDC_KICK, &CMFCApplication1Dlg::OnBnClickedKick)
	ON_BN_CLICKED(IDC_ASK, &CMFCApplication1Dlg::OnBnClickedAsk)
	ON_BN_CLICKED(IDC_TWAVE, &CMFCApplication1Dlg::OnBnClickedTwave)
	ON_BN_CLICKED(IDC_AMB, &CMFCApplication1Dlg::OnBnClickedAmbiguous)
	ON_BN_CLICKED(IDOK, &CMFCApplication1Dlg::OnBnClickedOk)
	ON_BN_CLICKED(IDC_DELETE, &CMFCApplication1Dlg::OnBnClickedDelete)
	ON_BN_CLICKED(IDC_Auto_ASSIGN, &CMFCApplication1Dlg::OnBnClickedAutoAssign)
	ON_BN_CLICKED(IDC_MOVE, &CMFCApplication1Dlg::OnBnClickedMove)
	ON_BN_CLICKED(IDC_CONT, &CMFCApplication1Dlg::OnBnClickedContinue)
	ON_BN_CLICKED(IDC_STOP, &CMFCApplication1Dlg::OnBnClickedStop)

	ON_WM_HSCROLL()
END_MESSAGE_MAP()


// CMFCApplication1Dlg 메시지 처리기

BOOL CMFCApplication1Dlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 시스템 메뉴에 "정보..." 메뉴 항목을 추가합니다.

	// IDM_ABOUTBOX는 시스템 명령 범위에 있어야 합니다.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 이 대화 상자의 아이콘을 설정합니다.  응용 프로그램의 주 창이 대화 상자가 아닐 경우에는
	//  프레임워크가 이 작업을 자동으로 수행합니다.
	SetIcon(m_hIcon, TRUE);			// 큰 아이콘을 설정합니다.
	SetIcon(m_hIcon, FALSE);		// 작은 아이콘을 설정합니다.

	// TODO: 여기에 추가 초기화 작업을 추가합니다.

	pWndRGB = this->GetDlgItem(IDC_STATIC1);
	pDCRGB = pWndRGB->GetDC();
	pDCRGB->GetSafeHdc();


	return TRUE;  // 포커스를 컨트롤에 설정하지 않으면 TRUE를 반환합니다.
}

void CMFCApplication1Dlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 대화 상자에 최소화 단추를 추가할 경우 아이콘을 그리려면
//  아래 코드가 필요합니다.  문서/뷰 모델을 사용하는 MFC 응용 프로그램의 경우에는
//  프레임워크에서 이 작업을 자동으로 수행합니다.

void CMFCApplication1Dlg::OnPaint()
{


	if (IsIconic())
	{
		CPaintDC dc(this); // 그리기를 위한 디바이스 컨텍스트입니다.

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
		CPaintDC dc(this);
		// Init

		if (init_cnt == 1){

			for (int i = 0; i < 30; i++){
				if (i == 0){
					pre_RV[i] = cur_RV[i];
				}
				else{
					pre_RV[i] = cur_RV[i - 1];
				}
				rand_value = rand() % 6;
				if (pre_RV[i] == rand_value){
					while (1){
						rand_value = rand() % 6;
						if (pre_RV[i] != rand_value){
							break;
						}
					}
				}
				cur_RV[i] = rand_value;
			}
			for (int i = 0; i < nHuman[0]; i++){
				pt1x[i].push_back(roi[i][roi_idx[i]]);
				pt1y[i].push_back(roi[i][roi_idx[i] + 1]);
				pt2x[i].push_back(roi[i][roi_idx[i] + 2]);
				pt2y[i].push_back(roi[i][roi_idx[i] + 3]);
				pt3x[i].push_back(pt1x[i][0]);
				pt3y[i].push_back(pt1y[i][0] - 10);
			}

			// Draw ROI
			for (int j = 0; j < nHuman[0]; j++){

				switch (j){
				case 0:
					r = R[0];
					g = G[1];
					b = B[1];
					break;
				case 1:
					r = R[1];
					g = G[0];
					b = B[2];
					break;
				case 2:
					r = R[0];
					g = G[1];
					b = B[3];
					break;
				case 3:
					r = R[1];
					g = G[0];
					b = B[4];
					break;
				case 4:
					r = R[1];
					g = G[2];
					b = B[0];
					break;
				case 5:
					r = R[2];
					g = G[3];
					b = B[5];
					break;
				default:
					/*rand_value = rand() % 6;*/
					r = R[cur_RV[j]];
					g = G[cur_RV[j]];
					b = B[cur_RV[j]];
					break;
				}

				cvRectangle(RGBImage, { pt1x[j][0], pt1y[j][0] }, { pt2x[j][0], pt2y[j][0] }, CV_RGB(b, g, r), 2, 8, 0);
				cout << "{pt1x, pt1y}: {" << pt1x[j][0] << "," << pt1y[j][0] << "}" << endl;
				cout << "{pt2x, pt2y}: {" << pt2x[j][0] << "," << pt2y[j][0] << "}" << endl;
				cvInitFont(&font, CV_FONT_HERSHEY_SIMPLEX | CV_FONT_ITALIC, hScale, vScale, 0, lineWidth);
				sprintf(output_text, "[PID: %d]", j + 1);
				cvPutText(RGBImage, output_text, { pt3x[j][0], pt3y[j][0] }, &font, CV_RGB(b, g, r));
				pt1x[j].clear();
				pt1y[j].clear();
				pt2x[j].clear();
				pt2y[j].clear();
				pt3x[j].clear();
				pt3y[j].clear();
			}

		}

		cImageRGB.CopyOf(RGBImage);
		cImageRGB.Show(pDCRGB->GetSafeHdc(), 0, 0, 1000, 480);

		CDialogEx::OnPaint();
	}
	init_cnt++;
}

// 사용자가 최소화된 창을 끄는 동안에 커서가 표시되도록 시스템에서
//  이 함수를 호출합니다.
HCURSOR CMFCApplication1Dlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CMFCApplication1Dlg::OnBnClickedOpen()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	//AfxMessageBox("Load RGB avi");
	char title[] = { "Open Image" };//set title name
	/*CString path_rgb;*/
	CString path_depth;
	CString strPathName;
	double RGBFrame;
	double DepthFrame;
	int nIndex;
	int pre_pid;
	char pre_gt[100];
	int init_frame;
	//CString pre_gt;
	//double slingFrame;
	CFileDialog dlg(TRUE, _T("*.bmp"), "", OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST | OFN_ALLOWMULTISELECT, "All Files (*.*)|*.*| AVI files (*.avi) |*.avi||", NULL);//open image
	dlg.m_ofn.lpstrTitle = title;//title name
	dlg.m_ofn.lpstrInitialDir = _T("E:/");

	if (dlg.DoModal() == IDOK)
	{
		path_rgb = dlg.GetPathName(); // contain the selected filename  

	}//get path_rgb
	path_roi = path_rgb;
	path_gt = path_rgb;
	path_gt_2 = path_rgb;
	path_revisedLabel = path_rgb;
	path_roi.Replace("avi", "txt");
	path_roi.Replace("rgb", "roi");
	path_gt.Replace("avi", "txt");
	path_gt.Replace("rgb", "groundTruth_1");
	path_gt_2.Replace("avi", "txt");
	path_gt_2.Replace("rgb", "groundTruth_2");
	path_revisedLabel.Replace("avi", "txt");
	path_revisedLabel.Replace("rgb", "Final_Ground_Truth");



	// Initialization of ROI information
	rID = 1012;
	for (int i = 0; i < 10; i++){
		pCheck[i] = (CButton*)GetDlgItem(rID);
		rID++;
	}
	pCheck[0]->SetCheck(TRUE);
	pCheck_2 = (CButton*)GetDlgItem(IDC_RADIO1);
	pCheck_2->SetCheck(TRUE);
	// Read and store ROI information

	//sprintf(roipath, "E:\\Eval_repository\\psn1\\roi_train_1000.txt"); // roi
	//sprintf(GTpath, "E:\\ground_truth.txt"); // roi
	//sprintf(GTpath_2, "E:\\ground_truth_2.txt"); // roi
	//sprintf(person1, "E:\\person1.txt");
	//sprintf(revisedLabel, "E:\\revised_ground_truth.txt");
	//fp = fopen(roipath, "r");
	fp = fopen((char*)(LPCTSTR)(path_roi), "r");
	label = fopen((char*)(LPCTSTR)(path_gt), "w");
	label_2 = fopen((char*)(LPCTSTR)(path_gt_2), "w");
	//label = fopen(GTpath, "w");
	//label_2 = fopen(GTpath_2, "w");
	//testfp = fopen(person1, "w");

	fprintf(label, "%s", "Frame#:");
	fprintf(label, "%d\n", 0);

	while (!feof(fp)){
		fscanf(fp, "%d\n", &total);
		nPeople.push_back(total);
		nTemp = nPeople[cnt];
		if (nTemp == 0){
			nFrame[0].push_back(fcnt_0);
			nFrame[1].push_back(0);
			nFrame[2].push_back(0);
			nFrame[3].push_back(0);
			nFrame[4].push_back(0);
			nFrame[5].push_back(0);
			nFrame[6].push_back(0);
			nFrame[7].push_back(0);
			nFrame[8].push_back(0);
			nFrame[9].push_back(0);
			nFrame[10].push_back(0);
			nFrame[11].push_back(0);
			nFrame[12].push_back(0);
			nFrame[13].push_back(0);
			nFrame[14].push_back(0);
			nFrame[15].push_back(0);
			nFrame[16].push_back(0);
			nFrame[17].push_back(0);
			nFrame[18].push_back(0);
			nFrame[19].push_back(0);
			fcnt_0++;
		}
		else if (nTemp == 1){
			nFrame[0].push_back(fcnt_0);
			nFrame[1].push_back(fcnt_1);
			nFrame[2].push_back(0);
			nFrame[3].push_back(0);
			nFrame[4].push_back(0);
			nFrame[5].push_back(0);
			nFrame[6].push_back(0);
			nFrame[7].push_back(0);
			nFrame[8].push_back(0);
			nFrame[9].push_back(0);
			nFrame[10].push_back(0);
			nFrame[11].push_back(0);
			nFrame[12].push_back(0);
			nFrame[13].push_back(0);
			nFrame[14].push_back(0);
			nFrame[15].push_back(0);
			nFrame[16].push_back(0);
			nFrame[17].push_back(0);
			nFrame[18].push_back(0);
			nFrame[19].push_back(0);
			nFrame[20].push_back(0);
			fcnt_0++;
			fcnt_1++;
		}
		else if (nTemp == 2){
			nFrame[0].push_back(fcnt_0);
			nFrame[1].push_back(fcnt_1);
			nFrame[2].push_back(fcnt_2);
			nFrame[3].push_back(0);
			nFrame[4].push_back(0);
			nFrame[5].push_back(0);
			nFrame[6].push_back(0);
			nFrame[7].push_back(0);
			nFrame[8].push_back(0);
			nFrame[9].push_back(0);
			nFrame[10].push_back(0);
			nFrame[11].push_back(0);
			nFrame[12].push_back(0);
			nFrame[13].push_back(0);
			nFrame[14].push_back(0);
			nFrame[15].push_back(0);
			nFrame[16].push_back(0);
			nFrame[17].push_back(0);
			nFrame[18].push_back(0);
			nFrame[19].push_back(0);
			nFrame[20].push_back(0);
			fcnt_0++;
			fcnt_1++;
			fcnt_2++;
		}
		else if (nTemp == 3){
			nFrame[0].push_back(fcnt_0);
			nFrame[1].push_back(fcnt_1);
			nFrame[2].push_back(fcnt_2);
			nFrame[3].push_back(fcnt_3);
			nFrame[4].push_back(0);
			nFrame[5].push_back(0);
			nFrame[6].push_back(0);
			nFrame[7].push_back(0);
			nFrame[8].push_back(0);
			nFrame[9].push_back(0);
			nFrame[10].push_back(0);
			nFrame[11].push_back(0);
			nFrame[12].push_back(0);
			nFrame[13].push_back(0);
			nFrame[14].push_back(0);
			nFrame[15].push_back(0);
			nFrame[16].push_back(0);
			nFrame[17].push_back(0);
			nFrame[18].push_back(0);
			nFrame[19].push_back(0);
			nFrame[20].push_back(0);
			fcnt_0++;
			fcnt_1++;
			fcnt_2++;
			fcnt_3++;
		}
		else if (nTemp == 4){
			nFrame[0].push_back(fcnt_0);
			nFrame[1].push_back(fcnt_1);
			nFrame[2].push_back(fcnt_2);
			nFrame[3].push_back(fcnt_3);
			nFrame[4].push_back(fcnt_4);
			nFrame[5].push_back(0);
			nFrame[6].push_back(0);
			nFrame[7].push_back(0);
			nFrame[8].push_back(0);
			nFrame[9].push_back(0);
			nFrame[10].push_back(0);
			nFrame[11].push_back(0);
			nFrame[12].push_back(0);
			nFrame[13].push_back(0);
			nFrame[14].push_back(0);
			nFrame[15].push_back(0);
			nFrame[16].push_back(0);
			nFrame[17].push_back(0);
			nFrame[18].push_back(0);
			nFrame[19].push_back(0);
			nFrame[20].push_back(0);
			fcnt_0++;
			fcnt_1++;
			fcnt_2++;
			fcnt_3++;
			fcnt_4++;

		}
		else if (nTemp == 5){
			nFrame[0].push_back(fcnt_0);
			nFrame[1].push_back(fcnt_1);
			nFrame[2].push_back(fcnt_2);
			nFrame[3].push_back(fcnt_3);
			nFrame[4].push_back(fcnt_4);
			nFrame[5].push_back(fcnt_5);
			nFrame[6].push_back(0);
			nFrame[7].push_back(0);
			nFrame[8].push_back(0);
			nFrame[9].push_back(0);
			nFrame[10].push_back(0);
			nFrame[11].push_back(0);
			nFrame[12].push_back(0);
			nFrame[13].push_back(0);
			nFrame[14].push_back(0);
			nFrame[15].push_back(0);
			nFrame[16].push_back(0);
			nFrame[17].push_back(0);
			nFrame[18].push_back(0);
			nFrame[19].push_back(0);
			nFrame[20].push_back(0);
			fcnt_0++;
			fcnt_1++;
			fcnt_2++;
			fcnt_3++;
			fcnt_4++;
			fcnt_5++;

		}
		else if (nTemp == 6){
			nFrame[0].push_back(fcnt_0);
			nFrame[1].push_back(fcnt_1);
			nFrame[2].push_back(fcnt_2);
			nFrame[3].push_back(fcnt_3);
			nFrame[4].push_back(fcnt_4);
			nFrame[5].push_back(fcnt_5);
			nFrame[6].push_back(fcnt_6);
			nFrame[7].push_back(0);
			nFrame[8].push_back(0);
			nFrame[9].push_back(0);
			nFrame[10].push_back(0);
			nFrame[11].push_back(0);
			nFrame[12].push_back(0);
			nFrame[13].push_back(0);
			nFrame[14].push_back(0);
			nFrame[15].push_back(0);
			nFrame[16].push_back(0);
			nFrame[17].push_back(0);
			nFrame[18].push_back(0);
			nFrame[19].push_back(0);
			nFrame[20].push_back(0);
			fcnt_0++;
			fcnt_1++;
			fcnt_2++;
			fcnt_3++;
			fcnt_4++;
			fcnt_5++;
			fcnt_6++;
		}
		else if (nTemp == 7){
			nFrame[0].push_back(fcnt_0);
			nFrame[1].push_back(fcnt_1);
			nFrame[2].push_back(fcnt_2);
			nFrame[3].push_back(fcnt_3);
			nFrame[4].push_back(fcnt_4);
			nFrame[5].push_back(fcnt_5);
			nFrame[6].push_back(fcnt_6);
			nFrame[7].push_back(fcnt_7);
			nFrame[8].push_back(0);
			nFrame[9].push_back(0);
			nFrame[10].push_back(0);
			nFrame[11].push_back(0);
			nFrame[12].push_back(0);
			nFrame[13].push_back(0);
			nFrame[14].push_back(0);
			nFrame[15].push_back(0);
			nFrame[16].push_back(0);
			nFrame[17].push_back(0);
			nFrame[18].push_back(0);
			nFrame[19].push_back(0);
			nFrame[20].push_back(0);
			fcnt_0++;
			fcnt_1++;
			fcnt_2++;
			fcnt_3++;
			fcnt_4++;
			fcnt_5++;
			fcnt_6++;
			fcnt_7++;
		}
		else if (nTemp == 8){
			nFrame[0].push_back(fcnt_0);
			nFrame[1].push_back(fcnt_1);
			nFrame[2].push_back(fcnt_2);
			nFrame[3].push_back(fcnt_3);
			nFrame[4].push_back(fcnt_4);
			nFrame[5].push_back(fcnt_5);
			nFrame[6].push_back(fcnt_6);
			nFrame[7].push_back(fcnt_7);
			nFrame[8].push_back(fcnt_8);
			nFrame[9].push_back(0);
			nFrame[10].push_back(0);
			nFrame[11].push_back(0);
			nFrame[12].push_back(0);
			nFrame[13].push_back(0);
			nFrame[14].push_back(0);
			nFrame[15].push_back(0);
			nFrame[16].push_back(0);
			nFrame[17].push_back(0);
			nFrame[18].push_back(0);
			nFrame[19].push_back(0);
			nFrame[20].push_back(0);
			fcnt_0++;
			fcnt_1++;
			fcnt_2++;
			fcnt_3++;
			fcnt_4++;
			fcnt_5++;
			fcnt_6++;
			fcnt_7++;
			fcnt_8++;
		}
		else if (nTemp == 9){
			nFrame[0].push_back(fcnt_0);
			nFrame[1].push_back(fcnt_1);
			nFrame[2].push_back(fcnt_2);
			nFrame[3].push_back(fcnt_3);
			nFrame[4].push_back(fcnt_4);
			nFrame[5].push_back(fcnt_5);
			nFrame[6].push_back(fcnt_6);
			nFrame[7].push_back(fcnt_7);
			nFrame[8].push_back(fcnt_8);
			nFrame[9].push_back(fcnt_9);
			nFrame[10].push_back(0);
			nFrame[11].push_back(0);
			nFrame[12].push_back(0);
			nFrame[13].push_back(0);
			nFrame[14].push_back(0);
			nFrame[15].push_back(0);
			nFrame[16].push_back(0);
			nFrame[17].push_back(0);
			nFrame[18].push_back(0);
			nFrame[19].push_back(0);
			nFrame[20].push_back(0);
			fcnt_0++;
			fcnt_1++;
			fcnt_2++;
			fcnt_3++;
			fcnt_4++;
			fcnt_5++;
			fcnt_6++;
			fcnt_7++;
			fcnt_8++;
			fcnt_9++;
		}
		else if (nTemp == 10){
			nFrame[0].push_back(fcnt_0);
			nFrame[1].push_back(fcnt_1);
			nFrame[2].push_back(fcnt_2);
			nFrame[3].push_back(fcnt_3);
			nFrame[4].push_back(fcnt_4);
			nFrame[5].push_back(fcnt_5);
			nFrame[6].push_back(fcnt_6);
			nFrame[7].push_back(fcnt_7);
			nFrame[8].push_back(fcnt_8);
			nFrame[9].push_back(fcnt_9);
			nFrame[10].push_back(fcnt_10);
			nFrame[11].push_back(0);
			nFrame[12].push_back(0);
			nFrame[13].push_back(0);
			nFrame[14].push_back(0);
			nFrame[15].push_back(0);
			nFrame[16].push_back(0);
			nFrame[17].push_back(0);
			nFrame[18].push_back(0);
			nFrame[19].push_back(0);
			nFrame[20].push_back(0);
			fcnt_0++;
			fcnt_1++;
			fcnt_2++;
			fcnt_3++;
			fcnt_4++;
			fcnt_5++;
			fcnt_6++;
			fcnt_7++;
			fcnt_8++;
			fcnt_9++;
			fcnt_10++;
		}
		else if (nTemp == 11){
			nFrame[0].push_back(fcnt_0);
			nFrame[1].push_back(fcnt_1);
			nFrame[2].push_back(fcnt_2);
			nFrame[3].push_back(fcnt_3);
			nFrame[4].push_back(fcnt_4);
			nFrame[5].push_back(fcnt_5);
			nFrame[6].push_back(fcnt_6);
			nFrame[7].push_back(fcnt_7);
			nFrame[8].push_back(fcnt_8);
			nFrame[9].push_back(fcnt_9);
			nFrame[10].push_back(fcnt_10);
			nFrame[11].push_back(fcnt_11);
			nFrame[12].push_back(0);
			nFrame[13].push_back(0);
			nFrame[14].push_back(0);
			nFrame[15].push_back(0);
			nFrame[16].push_back(0);
			nFrame[17].push_back(0);
			nFrame[18].push_back(0);
			nFrame[19].push_back(0);
			nFrame[20].push_back(0);
			fcnt_0++;
			fcnt_1++;
			fcnt_2++;
			fcnt_3++;
			fcnt_4++;
			fcnt_5++;
			fcnt_6++;
			fcnt_7++;
			fcnt_8++;
			fcnt_9++;
			fcnt_10++;
			fcnt_11++;
		}
		else if (nTemp == 12){
			nFrame[0].push_back(fcnt_0);
			nFrame[1].push_back(fcnt_1);
			nFrame[2].push_back(fcnt_2);
			nFrame[3].push_back(fcnt_3);
			nFrame[4].push_back(fcnt_4);
			nFrame[5].push_back(fcnt_5);
			nFrame[6].push_back(fcnt_6);
			nFrame[7].push_back(fcnt_7);
			nFrame[8].push_back(fcnt_8);
			nFrame[9].push_back(fcnt_9);
			nFrame[10].push_back(fcnt_10);
			nFrame[11].push_back(fcnt_11);
			nFrame[12].push_back(fcnt_12);
			nFrame[13].push_back(0);
			nFrame[14].push_back(0);
			nFrame[15].push_back(0);
			nFrame[16].push_back(0);
			nFrame[17].push_back(0);
			nFrame[18].push_back(0);
			nFrame[19].push_back(0);
			nFrame[20].push_back(0);
			fcnt_0++;
			fcnt_1++;
			fcnt_2++;
			fcnt_3++;
			fcnt_4++;
			fcnt_5++;
			fcnt_6++;
			fcnt_7++;
			fcnt_8++;
			fcnt_9++;
			fcnt_10++;
			fcnt_11++;
			fcnt_12++;
		}
		else if (nTemp == 13){
			nFrame[0].push_back(fcnt_0);
			nFrame[1].push_back(fcnt_1);
			nFrame[2].push_back(fcnt_2);
			nFrame[3].push_back(fcnt_3);
			nFrame[4].push_back(fcnt_4);
			nFrame[5].push_back(fcnt_5);
			nFrame[6].push_back(fcnt_6);
			nFrame[7].push_back(fcnt_7);
			nFrame[8].push_back(fcnt_8);
			nFrame[9].push_back(fcnt_9);
			nFrame[10].push_back(fcnt_10);
			nFrame[11].push_back(fcnt_11);
			nFrame[12].push_back(fcnt_12);
			nFrame[13].push_back(fcnt_13);
			nFrame[14].push_back(0);
			nFrame[15].push_back(0);
			nFrame[16].push_back(0);
			nFrame[17].push_back(0);
			nFrame[18].push_back(0);
			nFrame[19].push_back(0);
			nFrame[20].push_back(0);

			fcnt_0++;
			fcnt_1++;
			fcnt_2++;
			fcnt_3++;
			fcnt_4++;
			fcnt_5++;
			fcnt_6++;
			fcnt_7++;
			fcnt_8++;
			fcnt_9++;
			fcnt_10++;
			fcnt_11++;
			fcnt_12++;
			fcnt_13++;
		}
		else if (nTemp == 14){
			nFrame[0].push_back(fcnt_0);
			nFrame[1].push_back(fcnt_1);
			nFrame[2].push_back(fcnt_2);
			nFrame[3].push_back(fcnt_3);
			nFrame[4].push_back(fcnt_4);
			nFrame[5].push_back(fcnt_5);
			nFrame[6].push_back(fcnt_6);
			nFrame[7].push_back(fcnt_7);
			nFrame[8].push_back(fcnt_8);
			nFrame[9].push_back(fcnt_9);
			nFrame[10].push_back(fcnt_10);
			nFrame[11].push_back(fcnt_11);
			nFrame[12].push_back(fcnt_12);
			nFrame[13].push_back(fcnt_13);
			nFrame[14].push_back(fcnt_14);
			nFrame[15].push_back(0);
			nFrame[16].push_back(0);
			nFrame[17].push_back(0);
			nFrame[18].push_back(0);
			nFrame[19].push_back(0);
			nFrame[20].push_back(0);

			fcnt_0++;
			fcnt_1++;
			fcnt_2++;
			fcnt_3++;
			fcnt_4++;
			fcnt_5++;
			fcnt_6++;
			fcnt_7++;
			fcnt_8++;
			fcnt_9++;
			fcnt_10++;
			fcnt_11++;
			fcnt_12++;
			fcnt_13++;
			fcnt_14++;
		}
		else if (nTemp == 15){
			nFrame[0].push_back(fcnt_0);
			nFrame[1].push_back(fcnt_1);
			nFrame[2].push_back(fcnt_2);
			nFrame[3].push_back(fcnt_3);
			nFrame[4].push_back(fcnt_4);
			nFrame[5].push_back(fcnt_5);
			nFrame[6].push_back(fcnt_6);
			nFrame[7].push_back(fcnt_7);
			nFrame[8].push_back(fcnt_8);
			nFrame[9].push_back(fcnt_9);
			nFrame[10].push_back(fcnt_10);
			nFrame[11].push_back(fcnt_11);
			nFrame[12].push_back(fcnt_12);
			nFrame[13].push_back(fcnt_13);
			nFrame[14].push_back(fcnt_14);
			nFrame[15].push_back(fcnt_15);
			nFrame[16].push_back(0);
			nFrame[17].push_back(0);
			nFrame[18].push_back(0);
			nFrame[19].push_back(0);
			nFrame[20].push_back(0);

			fcnt_0++;
			fcnt_1++;
			fcnt_2++;
			fcnt_3++;
			fcnt_4++;
			fcnt_5++;
			fcnt_6++;
			fcnt_7++;
			fcnt_8++;
			fcnt_9++;
			fcnt_10++;
			fcnt_11++;
			fcnt_12++;
			fcnt_13++;
			fcnt_14++;
			fcnt_15++;
		}
		else if (nTemp == 16){
			nFrame[0].push_back(fcnt_0);
			nFrame[1].push_back(fcnt_1);
			nFrame[2].push_back(fcnt_2);
			nFrame[3].push_back(fcnt_3);
			nFrame[4].push_back(fcnt_4);
			nFrame[5].push_back(fcnt_5);
			nFrame[6].push_back(fcnt_6);
			nFrame[7].push_back(fcnt_7);
			nFrame[8].push_back(fcnt_8);
			nFrame[9].push_back(fcnt_9);
			nFrame[10].push_back(fcnt_10);
			nFrame[11].push_back(fcnt_11);
			nFrame[12].push_back(fcnt_12);
			nFrame[13].push_back(fcnt_13);
			nFrame[14].push_back(fcnt_14);
			nFrame[15].push_back(fcnt_15);
			nFrame[16].push_back(fcnt_16);
			nFrame[17].push_back(0);
			nFrame[18].push_back(0);
			nFrame[19].push_back(0);
			nFrame[20].push_back(0);

			fcnt_0++;
			fcnt_1++;
			fcnt_2++;
			fcnt_3++;
			fcnt_4++;
			fcnt_5++;
			fcnt_6++;
			fcnt_7++;
			fcnt_8++;
			fcnt_9++;
			fcnt_10++;
			fcnt_11++;
			fcnt_12++;
			fcnt_13++;
			fcnt_14++;
			fcnt_15++;
			fcnt_16++;
		}
		else if (nTemp == 17){
			nFrame[0].push_back(fcnt_0);
			nFrame[1].push_back(fcnt_1);
			nFrame[2].push_back(fcnt_2);
			nFrame[3].push_back(fcnt_3);
			nFrame[4].push_back(fcnt_4);
			nFrame[5].push_back(fcnt_5);
			nFrame[6].push_back(fcnt_6);
			nFrame[7].push_back(fcnt_7);
			nFrame[8].push_back(fcnt_8);
			nFrame[9].push_back(fcnt_9);
			nFrame[10].push_back(fcnt_10);
			nFrame[11].push_back(fcnt_11);
			nFrame[12].push_back(fcnt_12);
			nFrame[13].push_back(fcnt_13);
			nFrame[14].push_back(fcnt_14);
			nFrame[15].push_back(fcnt_15);
			nFrame[16].push_back(fcnt_16);
			nFrame[17].push_back(fcnt_17);
			nFrame[18].push_back(0);
			nFrame[19].push_back(0);
			nFrame[20].push_back(0);
			fcnt_0++;
			fcnt_1++;
			fcnt_2++;
			fcnt_3++;
			fcnt_4++;
			fcnt_5++;
			fcnt_6++;
			fcnt_7++;
			fcnt_8++;
			fcnt_9++;
			fcnt_10++;
			fcnt_11++;
			fcnt_12++;
			fcnt_13++;
			fcnt_14++;
			fcnt_15++;
			fcnt_16++;
			fcnt_17++;
		}
		else if (nTemp == 18){
			nFrame[0].push_back(fcnt_0);
			nFrame[1].push_back(fcnt_1);
			nFrame[2].push_back(fcnt_2);
			nFrame[3].push_back(fcnt_3);
			nFrame[4].push_back(fcnt_4);
			nFrame[5].push_back(fcnt_5);
			nFrame[6].push_back(fcnt_6);
			nFrame[7].push_back(fcnt_7);
			nFrame[8].push_back(fcnt_8);
			nFrame[9].push_back(fcnt_9);
			nFrame[10].push_back(fcnt_10);
			nFrame[11].push_back(fcnt_11);
			nFrame[12].push_back(fcnt_12);
			nFrame[13].push_back(fcnt_13);
			nFrame[14].push_back(fcnt_14);
			nFrame[15].push_back(fcnt_15);
			nFrame[16].push_back(fcnt_16);
			nFrame[17].push_back(fcnt_17);
			nFrame[18].push_back(fcnt_18);
			nFrame[19].push_back(0);
			nFrame[20].push_back(0);
			fcnt_0++;
			fcnt_1++;
			fcnt_2++;
			fcnt_3++;
			fcnt_4++;
			fcnt_5++;
			fcnt_6++;
			fcnt_7++;
			fcnt_8++;
			fcnt_9++;
			fcnt_10++;
			fcnt_11++;
			fcnt_12++;
			fcnt_13++;
			fcnt_14++;
			fcnt_15++;
			fcnt_16++;
			fcnt_17++;
			fcnt_18++;
		}
		else if (nTemp == 19){
			nFrame[0].push_back(fcnt_0);
			nFrame[1].push_back(fcnt_1);
			nFrame[2].push_back(fcnt_2);
			nFrame[3].push_back(fcnt_3);
			nFrame[4].push_back(fcnt_4);
			nFrame[5].push_back(fcnt_5);
			nFrame[6].push_back(fcnt_6);
			nFrame[7].push_back(fcnt_7);
			nFrame[8].push_back(fcnt_8);
			nFrame[9].push_back(fcnt_9);
			nFrame[10].push_back(fcnt_10);
			nFrame[11].push_back(fcnt_11);
			nFrame[12].push_back(fcnt_12);
			nFrame[13].push_back(fcnt_13);
			nFrame[14].push_back(fcnt_14);
			nFrame[15].push_back(fcnt_15);
			nFrame[16].push_back(fcnt_16);
			nFrame[17].push_back(fcnt_17);
			nFrame[18].push_back(fcnt_18);
			nFrame[19].push_back(fcnt_19);
			nFrame[20].push_back(0);

			fcnt_0++;
			fcnt_1++;
			fcnt_2++;
			fcnt_3++;
			fcnt_4++;
			fcnt_5++;
			fcnt_6++;
			fcnt_7++;
			fcnt_8++;
			fcnt_9++;
			fcnt_10++;
			fcnt_11++;
			fcnt_12++;
			fcnt_13++;
			fcnt_14++;
			fcnt_15++;
			fcnt_16++;
			fcnt_17++;
			fcnt_18++;
			fcnt_19++;
		}
		else if (nTemp == 20){
			nFrame[0].push_back(fcnt_0);
			nFrame[1].push_back(fcnt_1);
			nFrame[2].push_back(fcnt_2);
			nFrame[3].push_back(fcnt_3);
			nFrame[4].push_back(fcnt_4);
			nFrame[5].push_back(fcnt_5);
			nFrame[6].push_back(fcnt_6);
			nFrame[7].push_back(fcnt_7);
			nFrame[8].push_back(fcnt_8);
			nFrame[9].push_back(fcnt_9);
			nFrame[10].push_back(fcnt_10);
			nFrame[11].push_back(fcnt_11);
			nFrame[12].push_back(fcnt_12);
			nFrame[13].push_back(fcnt_13);
			nFrame[14].push_back(fcnt_14);
			nFrame[15].push_back(fcnt_15);
			nFrame[16].push_back(fcnt_16);
			nFrame[17].push_back(fcnt_17);
			nFrame[18].push_back(fcnt_18);
			nFrame[19].push_back(fcnt_19);
			nFrame[20].push_back(fcnt_20);

			fcnt_0++;
			fcnt_1++;
			fcnt_2++;
			fcnt_3++;
			fcnt_4++;
			fcnt_5++;
			fcnt_6++;
			fcnt_7++;
			fcnt_8++;
			fcnt_9++;
			fcnt_10++;
			fcnt_11++;
			fcnt_12++;
			fcnt_13++;
			fcnt_14++;
			fcnt_15++;
			fcnt_16++;
			fcnt_17++;
			fcnt_18++;
			fcnt_19++;
			fcnt_20++;

		}
		nHuman.push_back(nTemp);
		cnt++;
		for (int i = 0; i < nTemp; i++){
			fscanf(fp, "%d\n", &x_1);
			fscanf(fp, "%d\n", &y_1);
			fscanf(fp, "%d\n", &x_2);
			fscanf(fp, "%d\n", &y_2);
			roi[i].push_back(x_1);
			roi[i].push_back(y_1);
			roi[i].push_back(x_2);
			roi[i].push_back(y_2);
		}
	}

	fprintf(label_2, "%d\n", nHuman[0]);
	SliderCtrl.SetRange(0, nHuman.size(), TRUE);

	//gt_check = _access(revisedLabel, 0);
	gt_check = _access((char*)(LPCTSTR)(path_revisedLabel), 0);

	if (gt_check == 0){ //existing
		//re_label = fopen(revisedLabel, "r");
		re_label = fopen((char*)(LPCTSTR)(path_revisedLabel), "r");
		while (!feof(re_label)){
			fscanf(re_label, "%d\n", &pre_pid);
			fscanf(re_label, "%s\n", &pre_gt);
			wPID.push_back(pre_pid);
			wLabel.push_back(pre_gt);
			if (pre_pid == 1){
				saveFrame_cnt++;
			}
		}
		if (saveFrame_cnt < 0){
			init_frame = 0;
			EDText1 = 0;
		}
		else{
			init_frame = saveFrame_cnt;
			EDText1 = saveFrame_cnt;
		}
		SliderCtrl.SetPos(EDText1);
	}
	else{
		//re_label = fopen(revisedLabel, "w");
		re_label = fopen((char*)(LPCTSTR)(path_revisedLabel), "w");
		init_frame = 0;
		EDText1 = 0;
		SliderCtrl.SetPos(0);
	}

	//re_label = fopen(revisedLabel, "a");
	re_label = fopen((char*)(LPCTSTR)(path_revisedLabel), "a");

	/*SliderCtrl.SetRange(0, nHuman.size(), TRUE);
	SliderCtrl.SetPos(0);*/

	// Initialization
	switch (nHuman[0]){
	case 1:
		GetDlgItem(IDC_RADIO2)->ShowWindow(FALSE);
		GetDlgItem(IDC_RADIO3)->ShowWindow(FALSE);
		GetDlgItem(IDC_RADIO4)->ShowWindow(FALSE);
		GetDlgItem(IDC_RADIO5)->ShowWindow(FALSE);
		GetDlgItem(IDC_RADIO6)->ShowWindow(FALSE);
		GetDlgItem(IDC_RADIO7)->ShowWindow(FALSE);
		GetDlgItem(IDC_RADIO8)->ShowWindow(FALSE);
		GetDlgItem(IDC_RADIO9)->ShowWindow(FALSE);
		GetDlgItem(IDC_RADIO10)->ShowWindow(FALSE);
		break;
	case 2:
		GetDlgItem(IDC_RADIO3)->ShowWindow(FALSE);
		GetDlgItem(IDC_RADIO4)->ShowWindow(FALSE);
		GetDlgItem(IDC_RADIO5)->ShowWindow(FALSE);
		GetDlgItem(IDC_RADIO6)->ShowWindow(FALSE);
		GetDlgItem(IDC_RADIO7)->ShowWindow(FALSE);
		GetDlgItem(IDC_RADIO8)->ShowWindow(FALSE);
		GetDlgItem(IDC_RADIO9)->ShowWindow(FALSE);
		GetDlgItem(IDC_RADIO10)->ShowWindow(FALSE);
		break;
	case 3:
		GetDlgItem(IDC_RADIO4)->ShowWindow(FALSE);
		GetDlgItem(IDC_RADIO5)->ShowWindow(FALSE);
		GetDlgItem(IDC_RADIO6)->ShowWindow(FALSE);
		GetDlgItem(IDC_RADIO7)->ShowWindow(FALSE);
		GetDlgItem(IDC_RADIO8)->ShowWindow(FALSE);
		GetDlgItem(IDC_RADIO9)->ShowWindow(FALSE);
		GetDlgItem(IDC_RADIO10)->ShowWindow(FALSE);
		break;
	case 4:
		GetDlgItem(IDC_RADIO5)->ShowWindow(FALSE);
		GetDlgItem(IDC_RADIO6)->ShowWindow(FALSE);
		GetDlgItem(IDC_RADIO7)->ShowWindow(FALSE);
		GetDlgItem(IDC_RADIO8)->ShowWindow(FALSE);
		GetDlgItem(IDC_RADIO9)->ShowWindow(FALSE);
		GetDlgItem(IDC_RADIO10)->ShowWindow(FALSE);
		break;
	case 5:
		GetDlgItem(IDC_RADIO6)->ShowWindow(FALSE);
		GetDlgItem(IDC_RADIO7)->ShowWindow(FALSE);
		GetDlgItem(IDC_RADIO8)->ShowWindow(FALSE);
		GetDlgItem(IDC_RADIO9)->ShowWindow(FALSE);
		GetDlgItem(IDC_RADIO10)->ShowWindow(FALSE);
		break;
	case 6:
		GetDlgItem(IDC_RADIO7)->ShowWindow(FALSE);
		GetDlgItem(IDC_RADIO8)->ShowWindow(FALSE);
		GetDlgItem(IDC_RADIO9)->ShowWindow(FALSE);
		GetDlgItem(IDC_RADIO10)->ShowWindow(FALSE);
		break;
	case 7:
		GetDlgItem(IDC_RADIO8)->ShowWindow(FALSE);
		GetDlgItem(IDC_RADIO9)->ShowWindow(FALSE);
		GetDlgItem(IDC_RADIO10)->ShowWindow(FALSE);
		break;
	case 8:
		GetDlgItem(IDC_RADIO9)->ShowWindow(FALSE);
		GetDlgItem(IDC_RADIO10)->ShowWindow(FALSE);
		break;
	case 9:
		GetDlgItem(IDC_RADIO10)->ShowWindow(FALSE);
		break;
	default:
		break;
	}
	UpdateData(FALSE);

	m_Capture = cvCaptureFromFile(path_rgb);
	//12451
	cvSetCaptureProperty(m_Capture, CV_CAP_PROP_POS_FRAMES, EDText1);

	cvGrabFrame(m_Capture);

	frame_ = cvRetrieveFrame(m_Capture);

	RGBImage = frame_;
	c_ImageRGB.CopyOf(frame_);
	c_ImageRGB.Show(pDCRGB->GetSafeHdc(), 0, 0, 1000, 480);
	c_ImageRGB.Destroy();

	CPaintDC dc(this);
	Invalidate();
	AfxMessageBox("Start!");
}

void CMFCApplication1Dlg::OnBnClickedPrevious()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	if (path_rgb.GetLength() == 0){
		AfxMessageBox("Load a video for annotation");
	}
	else{
		preBtnCheck = TRUE;
		if (EDText1 <= 0){
			EDText1 = 0;
		}
		else{
			EDText1 -= 1;
		}
		UpdateData(FALSE);
		OnBnClickedNext();
	}
	preBtnCheck = FALSE;
}

void CMFCApplication1Dlg::OnBnClickedNext()
{


	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if (EDText1 < nHuman.size()){
		for (int i = 1; i < nHuman[EDText1]; i++){
			pCheck[i]->SetCheck(FALSE);
		}
		pCheck[0]->SetCheck(TRUE);
	}
	UpdateData();
	if (path_rgb.GetLength() == 0){
		EDText1 = 0;
		SliderCtrl.SetPos(EDText1);
		AfxMessageBox("Load a video for annotation");
	}
	else{
		if (scrCheck || preBtnCheck){
			EDText1 = EDText1;
		}
		else{
			if (moveCheck){
				EDText1 = EDText1;

			}
			else{
				EDText1 += 1;
			}
			SliderCtrl.SetPos(EDText1);
		}

		if (EDText1 <= 0){
			EDText1 = 0;
			if (!scrCheck){
				AfxMessageBox("This frame is the first frame!!");
			}
		}
		else if (EDText1 != 0 && EDText1 >= nHuman.size()){
			EDText1 = nHuman.size() - 1;
			if (!scrCheck && !alertCheck){
				AfxMessageBox("This frame is the last frame!!");
			}
		}
		for (int i = 0; i < nHuman[EDText1]; i++){
			roi_idx[i] = nFrame[i + 1][EDText1];
			if (roi_idx[i] == 0 || roi_idx[i] < 0){
				roi_idx[i] = 0;
			}
			else{
				roi_idx[i] = (roi_idx[i] * 4);
			}
			cout << "ROI index ?" << roi_idx[i] << endl;

		}
		fprintf(label, "%s", "Frame#:");
		fprintf(label, "%d\n", EDText1);
		cout << "Current Frame#:" << EDText1 << endl;

		for (int i = 0; i < nHuman[EDText1]; i++){

			pt1x[i].push_back(roi[i][roi_idx[i]]);
			pt1y[i].push_back(roi[i][roi_idx[i] + 1]);
			pt2x[i].push_back(roi[i][roi_idx[i] + 2]);
			pt2y[i].push_back(roi[i][roi_idx[i] + 3]);
			pt3x[i].push_back(pt1x[i][0]);
			pt3y[i].push_back(pt1y[i][0] - 10);
		}


		for (int i = 0; i < nHuman[EDText1]; i++){
			//cout << pt1x[i][0] << endl;
			//cout << pt1y[i][0] << endl;
			//cout << pt2x[i][0] << endl;
			//cout << pt2y[i][0] << endl;
			cout << roi[i][roi_idx[i]] << endl;
			cout << roi[i][roi_idx[i] + 1] << endl;
			cout << roi[i][roi_idx[i] + 2] << endl;
			cout << roi[i][roi_idx[i] + 3] << endl;
		}

		switch (nHuman[EDText1]){
		case 1:
			GetDlgItem(IDC_RADIO1)->ShowWindow(TRUE);
			GetDlgItem(IDC_RADIO2)->ShowWindow(FALSE);
			GetDlgItem(IDC_RADIO3)->ShowWindow(FALSE);
			GetDlgItem(IDC_RADIO4)->ShowWindow(FALSE);
			GetDlgItem(IDC_RADIO5)->ShowWindow(FALSE);
			GetDlgItem(IDC_RADIO6)->ShowWindow(FALSE);
			GetDlgItem(IDC_RADIO7)->ShowWindow(FALSE);
			GetDlgItem(IDC_RADIO8)->ShowWindow(FALSE);
			GetDlgItem(IDC_RADIO9)->ShowWindow(FALSE);
			GetDlgItem(IDC_RADIO10)->ShowWindow(FALSE);
			break;
		case 2:
			GetDlgItem(IDC_RADIO1)->ShowWindow(TRUE);
			GetDlgItem(IDC_RADIO2)->ShowWindow(TRUE);
			GetDlgItem(IDC_RADIO3)->ShowWindow(FALSE);
			GetDlgItem(IDC_RADIO4)->ShowWindow(FALSE);
			GetDlgItem(IDC_RADIO5)->ShowWindow(FALSE);
			GetDlgItem(IDC_RADIO6)->ShowWindow(FALSE);
			GetDlgItem(IDC_RADIO7)->ShowWindow(FALSE);
			GetDlgItem(IDC_RADIO8)->ShowWindow(FALSE);
			GetDlgItem(IDC_RADIO9)->ShowWindow(FALSE);
			GetDlgItem(IDC_RADIO10)->ShowWindow(FALSE);
			break;
		case 3:
			GetDlgItem(IDC_RADIO1)->ShowWindow(TRUE);
			GetDlgItem(IDC_RADIO2)->ShowWindow(TRUE);
			GetDlgItem(IDC_RADIO3)->ShowWindow(TRUE);
			GetDlgItem(IDC_RADIO4)->ShowWindow(FALSE);
			GetDlgItem(IDC_RADIO5)->ShowWindow(FALSE);
			GetDlgItem(IDC_RADIO6)->ShowWindow(FALSE);
			GetDlgItem(IDC_RADIO7)->ShowWindow(FALSE);
			GetDlgItem(IDC_RADIO8)->ShowWindow(FALSE);
			GetDlgItem(IDC_RADIO9)->ShowWindow(FALSE);
			GetDlgItem(IDC_RADIO10)->ShowWindow(FALSE);
			break;
		case 4:
			GetDlgItem(IDC_RADIO1)->ShowWindow(TRUE);
			GetDlgItem(IDC_RADIO2)->ShowWindow(TRUE);
			GetDlgItem(IDC_RADIO3)->ShowWindow(TRUE);
			GetDlgItem(IDC_RADIO4)->ShowWindow(TRUE);
			GetDlgItem(IDC_RADIO5)->ShowWindow(FALSE);
			GetDlgItem(IDC_RADIO6)->ShowWindow(FALSE);
			GetDlgItem(IDC_RADIO7)->ShowWindow(FALSE);
			GetDlgItem(IDC_RADIO8)->ShowWindow(FALSE);
			GetDlgItem(IDC_RADIO9)->ShowWindow(FALSE);
			GetDlgItem(IDC_RADIO10)->ShowWindow(FALSE);
			break;
		case 5:
			GetDlgItem(IDC_RADIO1)->ShowWindow(TRUE);
			GetDlgItem(IDC_RADIO2)->ShowWindow(TRUE);
			GetDlgItem(IDC_RADIO3)->ShowWindow(TRUE);
			GetDlgItem(IDC_RADIO4)->ShowWindow(TRUE);
			GetDlgItem(IDC_RADIO5)->ShowWindow(TRUE);
			GetDlgItem(IDC_RADIO6)->ShowWindow(FALSE);
			GetDlgItem(IDC_RADIO7)->ShowWindow(FALSE);
			GetDlgItem(IDC_RADIO8)->ShowWindow(FALSE);
			GetDlgItem(IDC_RADIO9)->ShowWindow(FALSE);
			GetDlgItem(IDC_RADIO10)->ShowWindow(FALSE);
			break;
		case 6:
			GetDlgItem(IDC_RADIO1)->ShowWindow(TRUE);
			GetDlgItem(IDC_RADIO2)->ShowWindow(TRUE);
			GetDlgItem(IDC_RADIO3)->ShowWindow(TRUE);
			GetDlgItem(IDC_RADIO4)->ShowWindow(TRUE);
			GetDlgItem(IDC_RADIO5)->ShowWindow(TRUE);
			GetDlgItem(IDC_RADIO6)->ShowWindow(TRUE);
			GetDlgItem(IDC_RADIO7)->ShowWindow(FALSE);
			GetDlgItem(IDC_RADIO8)->ShowWindow(FALSE);
			GetDlgItem(IDC_RADIO9)->ShowWindow(FALSE);
			GetDlgItem(IDC_RADIO10)->ShowWindow(FALSE);
			break;
		case 7:
			GetDlgItem(IDC_RADIO1)->ShowWindow(TRUE);
			GetDlgItem(IDC_RADIO2)->ShowWindow(TRUE);
			GetDlgItem(IDC_RADIO3)->ShowWindow(TRUE);
			GetDlgItem(IDC_RADIO4)->ShowWindow(TRUE);
			GetDlgItem(IDC_RADIO5)->ShowWindow(TRUE);
			GetDlgItem(IDC_RADIO6)->ShowWindow(TRUE);
			GetDlgItem(IDC_RADIO7)->ShowWindow(TRUE);
			GetDlgItem(IDC_RADIO8)->ShowWindow(FALSE);
			GetDlgItem(IDC_RADIO9)->ShowWindow(FALSE);
			GetDlgItem(IDC_RADIO10)->ShowWindow(FALSE);
			break;
		case 8:
			GetDlgItem(IDC_RADIO1)->ShowWindow(TRUE);
			GetDlgItem(IDC_RADIO2)->ShowWindow(TRUE);
			GetDlgItem(IDC_RADIO3)->ShowWindow(TRUE);
			GetDlgItem(IDC_RADIO4)->ShowWindow(TRUE);
			GetDlgItem(IDC_RADIO5)->ShowWindow(TRUE);
			GetDlgItem(IDC_RADIO6)->ShowWindow(TRUE);
			GetDlgItem(IDC_RADIO7)->ShowWindow(TRUE);
			GetDlgItem(IDC_RADIO8)->ShowWindow(TRUE);
			GetDlgItem(IDC_RADIO9)->ShowWindow(FALSE);
			GetDlgItem(IDC_RADIO10)->ShowWindow(FALSE);
			break;
		case 9:
			GetDlgItem(IDC_RADIO1)->ShowWindow(TRUE);
			GetDlgItem(IDC_RADIO2)->ShowWindow(TRUE);
			GetDlgItem(IDC_RADIO3)->ShowWindow(TRUE);
			GetDlgItem(IDC_RADIO4)->ShowWindow(TRUE);
			GetDlgItem(IDC_RADIO5)->ShowWindow(TRUE);
			GetDlgItem(IDC_RADIO6)->ShowWindow(TRUE);
			GetDlgItem(IDC_RADIO7)->ShowWindow(TRUE);
			GetDlgItem(IDC_RADIO8)->ShowWindow(TRUE);
			GetDlgItem(IDC_RADIO9)->ShowWindow(TRUE);
			GetDlgItem(IDC_RADIO10)->ShowWindow(FALSE);
			break;
		default:
			GetDlgItem(IDC_RADIO1)->ShowWindow(TRUE);
			GetDlgItem(IDC_RADIO2)->ShowWindow(TRUE);
			GetDlgItem(IDC_RADIO3)->ShowWindow(TRUE);
			GetDlgItem(IDC_RADIO4)->ShowWindow(TRUE);
			GetDlgItem(IDC_RADIO5)->ShowWindow(TRUE);
			GetDlgItem(IDC_RADIO6)->ShowWindow(TRUE);
			GetDlgItem(IDC_RADIO7)->ShowWindow(TRUE);
			GetDlgItem(IDC_RADIO8)->ShowWindow(TRUE);
			GetDlgItem(IDC_RADIO9)->ShowWindow(TRUE);
			GetDlgItem(IDC_RADIO10)->ShowWindow(TRUE);
			break;
		}
		fprintf(label_2, "%d\n", nHuman[EDText1]);
		UpdateData(FALSE);
		m_Capture = cvCaptureFromFile(path_rgb);

		cvSetCaptureProperty(m_Capture, CV_CAP_PROP_POS_FRAMES, EDText1);

		cvGrabFrame(m_Capture);

		frame_ = cvRetrieveFrame(m_Capture);
		//RGBImage = cvRetrieveFrame(m_Capture);
		RGBImage = frame_;

		// Draw ROI
		for (int j = 0; j < nHuman[EDText1]; j++){

			switch (j){
			case 0:
				r = R[0];
				g = G[1];
				b = B[1];
				break;
			case 1:
				r = R[1];
				g = G[0];
				b = B[2];
				break;
			case 2:
				r = R[0];
				g = G[1];
				b = B[3];
				break;
			case 3:
				r = R[1];
				g = G[0];
				b = B[4];
				break;
			case 4:
				r = R[1];
				g = G[2];
				b = B[0];
				break;
			case 5:
				r = R[2];
				g = G[3];
				b = B[5];
				break;
			default:
				//rand_value = rand() % 6;
				r = R[cur_RV[j]];
				g = G[cur_RV[j]];
				b = B[cur_RV[j]];
				break;
			}

			cvRectangle(frame_, { pt1x[j][0], pt1y[j][0] }, { pt2x[j][0], pt2y[j][0] }, CV_RGB(b, g, r), 2, 8, 0);
			cout << "{pt1x, pt1y}: {" << pt1x[j][0] << "," << pt1y[j][0] << "}" << endl;
			cout << "{pt2x, pt2y}: {" << pt2x[j][0] << "," << pt2y[j][0] << "}" << endl;
			//cvRectangle(RGBImage, { pt1x[j].get(0), pt1y[j].get(0) }, { pt2x[j].get(0), pt2y[j].get(0) }, CV_RGB(b, g, r), 2, 8, 0);
			cvInitFont(&font, CV_FONT_HERSHEY_SIMPLEX | CV_FONT_ITALIC, hScale, vScale, 0, lineWidth);
			sprintf(output_text, "[PID: %d]", j + 1);
			cvPutText(frame_, output_text, { pt3x[j][0], pt3y[j][0] }, &font, CV_RGB(b, g, r));
			//cvPutText(RGBImage, output_text, { pt3x[j].get(0), pt3y[j].get(0) }, &font, CV_RGB(b, g, r));
			sprintf(output_text_2, "%s", toStrAction[j].c_str());
			cvPutText(frame_, output_text_2, { pt3x[j][0] + 70, pt3y[j][0] }, &font, CV_RGB(b, g, r));
			//cvPutText(RGBImage, output_text_2, { pt3x[j].get(0) + 70, pt3y[j].get(0) }, &font, CV_RGB(b, g, r));
			pt1x[j].clear();
			pt1y[j].clear();
			pt2x[j].clear();
			pt2y[j].clear();
			pt3x[j].clear();
			pt3y[j].clear();
		}

		c_ImageRGB.CopyOf(frame_);
		c_ImageRGB.Show(pDCRGB->GetSafeHdc(), 0, 0, 1000, 480);
		c_ImageRGB.Destroy();
	}
}

void CMFCApplication1Dlg::OnBnClickedAssign()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData();
	for (int i = 0; i < 10; i++){
		bradio[i] = IsDlgButtonChecked(PersonID[i]);
		if (bradio[i]){
			PID = i + 1;
		}
	}
	int sum = 0;
	for (int i = 0; i < 11; i++){
		bcheck[i] = IsDlgButtonChecked(cbList[i]);
		sum = sum + bcheck[i];
	}
	if (sum == 0){
		strAction.Empty();
	}
	else{
		strAction = _T("Init");
	}
	if (!strAction.IsEmpty()){
		if (sit){
			strAction = _T("SITTING");
			numAction = 1;
		}
		else if (stand){
			strAction = _T("STANDING");
			numAction = 2;
		}
		else if (sleep){
			strAction = _T("SLEEPING");
			numAction = 3;
		}
		else if (fall){
			strAction = _T("FALLING");
			numAction = 4;
		}
		else if (walk){
			strAction = _T("WALKING");
			numAction = 5;
		}
		else if (run){
			strAction = _T("RUNNING");
			numAction = 6;
		}
		else if (box){
			strAction = _T("BOXING");
			numAction = 7;
		}
		else if (kick){
			strAction = _T("KICKING");
			numAction = 8;
		}
		else if (ask){
			strAction = _T("ASKING");
			numAction = 9;
		}
		else if (twave){
			strAction = _T("HAND_WAVING");
			numAction = 10;
		}
		else if (ambiguous){
			strAction = _T("ambiguous");
			numAction = 11;
		}
		fprintf(label, "PID:");
		fprintf(label, "%d\n", PID);
		fprintf(label, "%s\n", strAction);
		fprintf(label_2, "%s\n", strAction);
		wPID.push_back(PID);
		wLabel.push_back((LPCTSTR)strAction);

		lastAssignPos = EDText1;
		//scrTrack.push_back(EDText1);
		if (EDText1 == 0){
			cur_label = strAction;
			c_label[PID - 1] = numAction;
			pre_label = cur_label;
			p_label[PID - 1] = c_label[PID - 1];
		}
		else{
			pre_label = cur_label;
			p_label[PID - 1] = c_label[PID - 1];
			cur_label = strAction;
			c_label[PID - 1] = numAction;
		}
		action_label_out(c_label[PID - 1], toStrAction[PID - 1]);
		AfxMessageBox(strAction);
		cout << "Assign EDText1:" << EDText1 << endl;
		if (PID != nHuman[EDText1]){
			pCheck[PID - 1]->SetCheck(FALSE);
			pCheck[PID]->SetCheck(TRUE);
			auto_assignCheck = FALSE;
		}
		else{
			OnBnClickedNext();
			auto_assignCheck = TRUE;

		}
	}
	else{
		AfxMessageBox("Check the action label");
	}
}

void CMFCApplication1Dlg::OnBnClickedSit()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	for (int i = 0; i < 11; i++){
		bcheck[i] = IsDlgButtonChecked(cbList[i]);
		if (bcheck[i] != 0){
			pre_check = cur_check;
			cur_check = i;
		}
	}
	if (bcheck[0]){
		CheckDlgButton(cbList[0], TRUE);
	}
	else{
		CheckDlgButton(cbList[0], FALSE);
	}
	for (int i = 0; i < 11; i++){
		if (bcheck[i] && i != 0){
			CheckDlgButton(cbList[i], FALSE);
		}
	}
}


void CMFCApplication1Dlg::OnBnClickedStand()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	for (int i = 0; i < 11; i++){
		bcheck[i] = IsDlgButtonChecked(cbList[i]);
		if (bcheck[i] != 0){
			pre_check = cur_check;
			cur_check = i;
		}
	}
	if (bcheck[1]){
		CheckDlgButton(cbList[1], TRUE);
	}
	else{
		CheckDlgButton(cbList[1], FALSE);
	}
	for (int i = 0; i < 11; i++){
		if (bcheck[i] && i != 1){
			CheckDlgButton(cbList[i], FALSE);
		}
	}
}


void CMFCApplication1Dlg::OnBnClickedSleep()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	for (int i = 0; i < 11; i++){
		bcheck[i] = IsDlgButtonChecked(cbList[i]);
		if (bcheck[i] != 0){
			pre_check = cur_check;
			cur_check = i;
		}
	}
	if (bcheck[2]){
		CheckDlgButton(cbList[2], TRUE);
	}
	else{
		CheckDlgButton(cbList[2], FALSE);
	}
	for (int i = 0; i < 11; i++){
		if (bcheck[i] && i != 2){
			CheckDlgButton(cbList[i], FALSE);
		}
	}
}


void CMFCApplication1Dlg::OnBnClickedFall()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	for (int i = 0; i < 11; i++){
		bcheck[i] = IsDlgButtonChecked(cbList[i]);
		if (bcheck[i] != 0){
			pre_check = cur_check;
			cur_check = i;
		}
	}
	if (bcheck[3]){
		CheckDlgButton(cbList[3], TRUE);
	}
	else{
		CheckDlgButton(cbList[3], FALSE);
	}
	for (int i = 0; i < 11; i++){
		if (bcheck[i] && i != 3){
			CheckDlgButton(cbList[i], FALSE);
		}
	}
}


void CMFCApplication1Dlg::OnBnClickedWalk()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	for (int i = 0; i < 11; i++){
		bcheck[i] = IsDlgButtonChecked(cbList[i]);
		if (bcheck[i] != 0){
			pre_check = cur_check;
			cur_check = i;
		}
	}
	if (bcheck[4]){
		CheckDlgButton(cbList[4], TRUE);
	}
	else{
		CheckDlgButton(cbList[4], FALSE);
	}
	for (int i = 0; i < 11; i++){
		if (bcheck[i] && i != 4){
			CheckDlgButton(cbList[i], FALSE);
		}
	}
}


void CMFCApplication1Dlg::OnBnClickedRun()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	for (int i = 0; i < 11; i++){
		bcheck[i] = IsDlgButtonChecked(cbList[i]);
		if (bcheck[i] != 0){
			pre_check = cur_check;
			cur_check = i;
		}
	}
	if (bcheck[5]){
		CheckDlgButton(cbList[5], TRUE);
	}
	else{
		CheckDlgButton(cbList[5], FALSE);
	}
	for (int i = 0; i < 11; i++){
		if (bcheck[i] && i != 5){
			CheckDlgButton(cbList[i], FALSE);
		}
	}
}


void CMFCApplication1Dlg::OnBnClickedBox()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	for (int i = 0; i < 11; i++){
		bcheck[i] = IsDlgButtonChecked(cbList[i]);
		if (bcheck[i] != 0){
			pre_check = cur_check;
			cur_check = i;
		}
	}
	if (bcheck[6]){
		CheckDlgButton(cbList[6], TRUE);
	}
	else{
		CheckDlgButton(cbList[6], FALSE);
	}
	for (int i = 0; i < 11; i++){
		if (bcheck[i] && i != 6){
			CheckDlgButton(cbList[i], FALSE);
		}
	}
}


void CMFCApplication1Dlg::OnBnClickedKick()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	for (int i = 0; i < 11; i++){
		bcheck[i] = IsDlgButtonChecked(cbList[i]);
		if (bcheck[i] != 0){
			pre_check = cur_check;
			cur_check = i;
		}
	}
	if (bcheck[7]){
		CheckDlgButton(cbList[7], TRUE);
	}
	else{
		CheckDlgButton(cbList[7], FALSE);
	}
	for (int i = 0; i < 11; i++){
		if (bcheck[i] && i != 7){
			CheckDlgButton(cbList[i], FALSE);
		}
	}
}


void CMFCApplication1Dlg::OnBnClickedAsk()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	for (int i = 0; i < 11; i++){
		bcheck[i] = IsDlgButtonChecked(cbList[i]);
		if (bcheck[i] != 0){
			pre_check = cur_check;
			cur_check = i;
		}
	}
	if (bcheck[8]){
		CheckDlgButton(cbList[8], TRUE);
	}
	else{
		CheckDlgButton(cbList[8], FALSE);
	}
	for (int i = 0; i < 11; i++){
		if (bcheck[i] && i != 8){
			CheckDlgButton(cbList[i], FALSE);
		}
	}
}


void CMFCApplication1Dlg::OnBnClickedTwave()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	for (int i = 0; i < 11; i++){
		bcheck[i] = IsDlgButtonChecked(cbList[i]);
		if (bcheck[i] != 0){
			pre_check = cur_check;
			cur_check = i;
		}
	}
	if (bcheck[9]){
		CheckDlgButton(cbList[9], TRUE);
	}
	else{
		CheckDlgButton(cbList[9], FALSE);
	}
	for (int i = 0; i < 11; i++){
		if (bcheck[i] && i != 9){
			CheckDlgButton(cbList[i], FALSE);
		}
	}
}
void CMFCApplication1Dlg::OnBnClickedAmbiguous(){
	for (int i = 0; i < 11; i++){
		bcheck[i] = IsDlgButtonChecked(cbList[i]);
		if (bcheck[i] != 0){
			pre_check = cur_check;
			cur_check = i;
		}
	}
	if (bcheck[9]){
		CheckDlgButton(cbList[10], TRUE);
	}
	else{
		CheckDlgButton(cbList[10], FALSE);
	}
	for (int i = 0; i < 11; i++){
		if (bcheck[i] && i != 10){
			CheckDlgButton(cbList[i], FALSE);
		}
	}
}


void CMFCApplication1Dlg::OnBnClickedOk()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	fclose(label);
	fclose(label_2);
	for (int i = 0; i < wPID.size(); i++){
		fprintf(re_label, "%d\n", wPID[i]);
		fprintf(re_label, "%s\n", wLabel[i].c_str());
	}
	//fprintf(re_label, "SAVED_FRAME#: %d\n", EDText1);
	fclose(re_label);
	CDialogEx::OnOK();
}


void CMFCApplication1Dlg::OnBnClickedDelete()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	deleteCheck = TRUE;
	/*if (EDText1 > 0){*/
	wPID.erase(wPID.end() - 1);
	wLabel.erase(wLabel.end() - 1);
	for (int i = 0; i < 10; i++){
		bradio[i] = IsDlgButtonChecked(PersonID[i]);
		if (bradio[i]){
			PID = i + 1;
		}
	}
	if (PID!=1 && PID != nHuman[EDText1]+1){
		pCheck[PID - 1]->SetCheck(FALSE);
		pCheck[PID - 2]->SetCheck(TRUE);
	}
	else if(PID==1){
		if (EDText1 > 0){
			OnBnClickedPrevious();
		}
		pCheck[nHuman[EDText1]-1]->SetCheck(TRUE);
		for (int i = 0; i < nHuman[EDText1]-1; i++){
			pCheck[i]->SetCheck(FALSE);
		}
	}
	AfxMessageBox("Deleted");
	//}
}

void CMFCApplication1Dlg::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	if (path_rgb.GetLength() == 0){
		AfxMessageBox("Load a video for annotation");
		SliderCtrl.SetPos(0);
	}
	else{
		if (pScrollBar == (CScrollBar *)&SliderCtrl) {
			int value = SliderCtrl.GetPos();
			cout << "Slider position:" << value << endl;
			EDText1 = value;
			strName.Format(_T("%d"), EDText1);
			SetDlgItemText(IDC_EDIT1, strName);
			UpdateData(TRUE);
			if (value >= 0){
				//EDText1 = value - 1;
				scrCheck = TRUE;

				OnBnClickedNext();
			}
			scrCheck = FALSE;

			UpdateData(FALSE);
		}
		else {
			CDialogEx::OnHScroll(nSBCode, nPos, pScrollBar);
		}
	}

}
void CMFCApplication1Dlg::OnBnClickedAutoAssign(){
	if (auto_assignCheck){
		scroll_pos = SliderCtrl.GetPos();
		iter = scroll_pos - lastAssignPos;
		wPIDsize = wPID.size();
		wLabelsize = wLabel.size();
		if (wPIDsize >= nHuman[EDText1]){
		for (int i = 0; i < iter; i++){
			for (int j = nHuman[EDText1]; j > 0; j--){
				wPID.push_back(wPID[wPIDsize - j]);
				wLabel.push_back(wLabel[wLabelsize - j]);
			}
		}
		lastAssignPos = scroll_pos;
		alertCheck = TRUE;
		OnBnClickedNext();
		AfxMessageBox("Assigned!");
		}
		else{
			AfxMessageBox("Exceed index");
		}
	}
	else{
		AfxMessageBox("Cannot assign label!");
	}
}

void CMFCApplication1Dlg::OnBnClickedMove(){
	UpdateData(TRUE);
	GetDlgItemText(IDC_EDIT1, strName);
	SetDlgItemText(IDC_EDIT1, strName);
	int x = _ttoi(strName);
	if (x == 0){
		x = 0;
		EDText1 = x;
	}
	else if (x != 0 && x >= nHuman.size()){
		x = nHuman.size() - 1;
		EDText1 = x - 1;
		AfxMessageBox("Exceed the number of frame!");
	}
	else{
		EDText1 = x;
		moveCheck = TRUE;
	}
	OnBnClickedNext();
	moveCheck = FALSE;
	UpdateData(FALSE);
}
void CMFCApplication1Dlg::OnBnClickedContinue(){
	//GetDlgItem(IDC_STOP)->EnableWindow(TRUE);

	while (StopCheck){
		OnBnClickedNext();

	}
}
void CMFCApplication1Dlg::OnBnClickedStop(){
	StopCheck = FALSE;
}

UINT CMFCApplication1Dlg::ThreadFirst(LPVOID _mothod)
{
	CMFCApplication1Dlg *fir = (CMFCApplication1Dlg*)_mothod;

	while (1)
	{
		// ..이 곳에 루프 시 수행할 동작을 선언하세요.
		Sleep(1000);
	}
	return 0;
}
