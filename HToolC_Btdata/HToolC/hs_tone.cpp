// hs_tone.cpp : implementation file
//

#include "stdafx.h"
#include "HToolC.h"
#include "hs_tone.h"
#include "afxdialogex.h"


// hs_tone dialog

IMPLEMENT_DYNAMIC(hs_tone, CDialogEx)

hs_tone::hs_tone(CWnd* pParent /*=NULL*/)
	: CDialogEx(hs_tone::IDD, pParent)
{

}

hs_tone::~hs_tone()
{
	if(p_option.ptr)
	{
		free(p_option.ptr);
		p_option.ptr = NULL;
	}
}

void hs_tone::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CHK_TONE_EN, hs_chkToneEn);
}


BEGIN_MESSAGE_MAP(hs_tone, CDialogEx)
	ON_BN_CLICKED(IDC_TONE_BUTTON_OPEN + 0, &hs_tone::OnBnClickedTone0)
	ON_BN_CLICKED(IDC_TONE_BUTTON_OPEN + 1, &hs_tone::OnBnClickedTone1)
	ON_BN_CLICKED(IDC_TONE_BUTTON_OPEN + 2, &hs_tone::OnBnClickedTone2)
	ON_BN_CLICKED(IDC_TONE_BUTTON_OPEN + 3, &hs_tone::OnBnClickedTone3)
	ON_BN_CLICKED(IDC_TONE_BUTTON_OPEN + 4, &hs_tone::OnBnClickedTone4)
	ON_BN_CLICKED(IDC_TONE_BUTTON_OPEN + 5, &hs_tone::OnBnClickedTone5)
	ON_BN_CLICKED(IDC_TONE_BUTTON_OPEN + 6, &hs_tone::OnBnClickedTone6)
	ON_BN_CLICKED(IDC_TONE_BUTTON_OPEN + 7, &hs_tone::OnBnClickedTone7)
	ON_BN_CLICKED(IDC_TONE_BUTTON_OPEN + 8, &hs_tone::OnBnClickedTone8)
	ON_BN_CLICKED(IDC_TONE_BUTTON_OPEN + 9, &hs_tone::OnBnClickedTone9)

	ON_BN_CLICKED(IDC_TONE_BUTTON_OPEN + 10, &hs_tone::OnBnClickedTone10)
	ON_BN_CLICKED(IDC_TONE_BUTTON_OPEN + 11, &hs_tone::OnBnClickedTone11)
	ON_BN_CLICKED(IDC_TONE_BUTTON_OPEN + 12, &hs_tone::OnBnClickedTone12)
	ON_BN_CLICKED(IDC_TONE_BUTTON_OPEN + 13, &hs_tone::OnBnClickedTone13)
	ON_BN_CLICKED(IDC_TONE_BUTTON_OPEN + 14, &hs_tone::OnBnClickedTone14)
	ON_BN_CLICKED(IDC_TONE_BUTTON_OPEN + 15, &hs_tone::OnBnClickedTone15)
	ON_BN_CLICKED(IDC_TONE_BUTTON_OPEN + 16, &hs_tone::OnBnClickedTone16)
	ON_BN_CLICKED(IDC_TONE_BUTTON_OPEN + 17, &hs_tone::OnBnClickedTone17)
	ON_BN_CLICKED(IDC_TONE_BUTTON_OPEN + 18, &hs_tone::OnBnClickedTone18)
	ON_BN_CLICKED(IDC_TONE_BUTTON_OPEN + 19, &hs_tone::OnBnClickedTone19)

	ON_BN_CLICKED(IDC_TONE_BUTTON_OPEN + 20, &hs_tone::OnBnClickedTone20)
	ON_BN_CLICKED(IDC_TONE_BUTTON_OPEN + 21, &hs_tone::OnBnClickedTone21)
	ON_BN_CLICKED(IDC_TONE_BUTTON_OPEN + 22, &hs_tone::OnBnClickedTone22)
	ON_BN_CLICKED(IDC_TONE_BUTTON_OPEN + 23, &hs_tone::OnBnClickedTone23)
	ON_BN_CLICKED(IDC_TONE_BUTTON_OPEN + 24, &hs_tone::OnBnClickedTone24)
	ON_BN_CLICKED(IDC_TONE_BUTTON_OPEN + 25, &hs_tone::OnBnClickedTone25)
	ON_BN_CLICKED(IDC_TONE_BUTTON_OPEN + 26, &hs_tone::OnBnClickedTone26)
	ON_BN_CLICKED(IDC_TONE_BUTTON_OPEN + 27, &hs_tone::OnBnClickedTone27)
	ON_BN_CLICKED(IDC_TONE_BUTTON_OPEN + 28, &hs_tone::OnBnClickedTone28)
	ON_BN_CLICKED(IDC_TONE_BUTTON_OPEN + 29, &hs_tone::OnBnClickedTone29)

	ON_BN_CLICKED(IDC_TONE_BUTTON_OPEN + 30, &hs_tone::OnBnClickedTone30)
	ON_BN_CLICKED(IDC_TONE_BUTTON_OPEN + 31, &hs_tone::OnBnClickedTone31)
	ON_BN_CLICKED(IDC_TONE_BUTTON_OPEN + 32, &hs_tone::OnBnClickedTone32)
	ON_BN_CLICKED(IDC_TONE_BUTTON_OPEN + 33, &hs_tone::OnBnClickedTone33)
	ON_BN_CLICKED(IDC_TONE_BUTTON_OPEN + 34, &hs_tone::OnBnClickedTone34)
	ON_BN_CLICKED(IDC_TONE_BUTTON_OPEN + 35, &hs_tone::OnBnClickedTone35)
	ON_BN_CLICKED(IDC_TONE_BUTTON_OPEN + 36, &hs_tone::OnBnClickedTone36)
	ON_BN_CLICKED(IDC_TONE_BUTTON_OPEN + 37, &hs_tone::OnBnClickedTone37)
	ON_BN_CLICKED(IDC_TONE_BUTTON_OPEN + 38, &hs_tone::OnBnClickedTone38)
	ON_BN_CLICKED(IDC_TONE_BUTTON_OPEN + 39, &hs_tone::OnBnClickedTone39)

	ON_BN_CLICKED(IDC_TONE_BUTTON_OPEN + 40, &hs_tone::OnBnClickedTone40)
	ON_BN_CLICKED(IDC_TONE_BUTTON_OPEN + 41, &hs_tone::OnBnClickedTone41)
	ON_BN_CLICKED(IDC_TONE_BUTTON_OPEN + 42, &hs_tone::OnBnClickedTone42)
	ON_BN_CLICKED(IDC_TONE_BUTTON_OPEN + 43, &hs_tone::OnBnClickedTone43)
	ON_BN_CLICKED(IDC_TONE_BUTTON_OPEN + 44, &hs_tone::OnBnClickedTone44)
	ON_BN_CLICKED(IDC_TONE_BUTTON_OPEN + 45, &hs_tone::OnBnClickedTone45)
	ON_BN_CLICKED(IDC_TONE_BUTTON_OPEN + 46, &hs_tone::OnBnClickedTone46)
	ON_BN_CLICKED(IDC_TONE_BUTTON_OPEN + 47, &hs_tone::OnBnClickedTone47)
	ON_BN_CLICKED(IDC_TONE_BUTTON_OPEN + 48, &hs_tone::OnBnClickedTone48)
	
	ON_BN_CLICKED(IDC_CHK_TONE_EN, &hs_tone::OnBnClickedChkToneEn)
END_MESSAGE_MAP()


// hs_tone message handlers
BOOL hs_tone::OnInitDialog(void)
{
	int i, tmp;
	int p_left = 25, p_top = 55, p_right = 85, p_button = 75;
	char str[255];

	CDialogEx::OnInitDialog();
	
	p_top += 30;
	p_button += 30;

	i = CFG_TONE_4K_MAXNUM;
	if(i > 0)
	{

		for(i=0; i<CFG_TONE_4K_MAXNUM; i++)
		{
			if((i%4) == 0)
			{
				p_top += 30;
				p_button += 30;

				p_left = 15;
				p_right = 75;
			}
			else
			{
				p_left += 70;
				p_right += 70;
			}

			sprintf_s(str,"sound %d", i);
			bToneOpen[i].Create( str, WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, CRect(p_left, p_top-61, p_right+10, p_button-54), this, IDC_TONE_BUTTON_OPEN+i); 
		}	

		p_top += 40;
		p_button += 40;
	}

	for(i=0; i<CFG_TONE_8K_MAXNUM; i++)
	{
		if((i%4) == 0)
		{
			p_top += 30;
			p_button += 30;

			p_left = 15;
			p_right = 75;
		}
		else
		{
			p_left += 70;
			p_right += 70;
		}

		tmp = i + CFG_TONE_4K_MAXNUM;
		sprintf_s(str,"sound %d", tmp);
		bToneOpen[tmp].Create( str, WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, CRect(p_left, p_top-61, p_right+10, p_button-54), this, IDC_TONE_BUTTON_OPEN + tmp); 
	}

	p_top += 40;
	p_button += 40;
	for(i=0; i<CFG_TONE_16K_MAXNUM; i++)
	{
		if((i%4) == 0)
		{
			p_top += 30;
			p_button += 30;

			p_left = 15;
			p_right = 75;
		}
		else
		{
			p_left += 70;
			p_right += 70;
		}

		tmp = i + CFG_TONE_4K_MAXNUM + CFG_TONE_8K_MAXNUM;

		sprintf_s(str,"Tone %d", tmp);
		bToneOpen[tmp].Create( str, WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, CRect(p_left, p_top-61, p_right+10, p_button-54), this, IDC_TONE_BUTTON_OPEN+tmp); 
	}

	//memset(p_option.w_toneInfo, 0, sizeof(hs_sound_info_t)* CFG_TONE_MAXNUM);

	return true;
}


static int getWinIndex(HWND hWnd, hs_tone *pThis)
{
	int i;

	if(pThis == NULL)
		return -1;

	for(i=0; i<CFG_TONE_MAXNUM; i++)
	{
		if(pThis->hWnd[i] == hWnd)
		{
			return i;
		}
	}

	return -1;
}

static void OnSaveToneInfo(int index, hs_tone *pThis)
{
	char str[20];
	int val;

	pThis->eEditVolume[index].GetWindowTextA(str, 9);
	sscanf_s(str,"%d", &val);
	pThis->p_option.w_toneInfo[index].u16ToneVolume = val;

	pThis->eEditBitpool[index].GetWindowTextA(str, 9);
	sscanf_s(str,"%d",&val);
	pThis->p_option.w_toneInfo[index].u8BitPool = val;

	pThis->eEditLoopNum[index].GetWindowTextA(str, 9);
	sscanf_s(str,"%d",&val);
	pThis->p_option.w_toneInfo[index].u8LoopNum = val;

	pThis->eEditDelay[index].GetWindowTextA(str, 9);
	sscanf_s(str,"%d",&val);
	pThis->p_option.w_toneInfo[index].u8Delay = val;

	pThis->cListBitrate[index].GetWindowTextA(str, 9);
	sscanf_s(str,"%d",&val);
	pThis->p_option.w_toneInfo[index].u32BitRate = val;

	pThis->cListBlocks[index].GetWindowTextA(str, 9);
	sscanf_s(str,"%d",&val);
	pThis->p_option.w_toneInfo[index].u8Blocks = val;

	pThis->cListSubband[index].GetWindowTextA(str, 9);
	sscanf_s(str,"%d",&val);
	pThis->p_option.w_toneInfo[index].u8SubBands = val;

	pThis->p_option.w_toneInfo[index].u8ChnMode = pThis->cListChnmode[index].GetCurSel();
}

static void _hex2ascii(char data, char *AscBuf)
{
	char tmp;

	tmp = (data >> 4) & 0xf;
	if((tmp>=0) && (tmp<=9))
		AscBuf[0] = '0' + tmp;
	else if((tmp>=0xa) && (tmp<=0xf))
		AscBuf[0] = 'A' + tmp - 0xa;

	tmp = data & 0xf;
	if((tmp>=0) && (tmp<=9))
		AscBuf[1] = '0' + tmp;
	else if((tmp>=0xa) && (tmp<=0xf))
		AscBuf[1] = 'A' + tmp - 0xa;

	AscBuf[2] = 0x20;
	AscBuf[3] = 0;
}

static void hex2ascii(char *inBuf, int len, char *AscBuf)
{
	int i;
	char tmp[16];

	AscBuf[0] = 0;
	for(i=0; i<len; i++)
	{
		if((i%16) == 0)
		{
			if(i != 0)
				strcat(AscBuf, "\r\n");

			sprintf(tmp, "%08X: ", i);
			strcat(AscBuf, tmp);
		}

		if(((i%16) != 0) && ((i%8) == 0))
			strcat(AscBuf, "  ");

		_hex2ascii(inBuf[i], tmp);
		strcat(AscBuf, tmp);
	}
}

static void OnGetToneFile(int index, hs_tone *pThis)
{
	char str[255];
	char filePath[1024] = {0};
	char *hex;
	int tmp, max_size;

	CFileDialog dlg(FALSE, _T("*.*"), NULL, 0, _T("pcm or wav Files (*.pcm; *.wav)|*.pcm; *.wav|All Files (*.*)|*.*|"));
	dlg.m_ofn.lpstrTitle = _T("get patch bin file");
	GetCurrentDirectory(255, str);

	if(strcmp(pThis->p_option.w_curdir, "") == 0)
		dlg.m_ofn.lpstrInitialDir = str;
	else
		dlg.m_ofn.lpstrInitialDir = pThis->p_option.w_curdir;

	if( dlg.DoModal() == IDOK )
	{
		hex = (char *)malloc(1024 * 1024);

		memcpy(filePath ,(LPSTR)(LPCTSTR) dlg.GetPathName(), dlg.GetPathName().GetLength());
		pThis->UpdateData(FALSE);		
		
		tmp = dlg.GetPathName().ReverseFind('\\');
		memcpy(pThis->p_option.w_curdir ,(LPSTR)(LPCTSTR) dlg.GetPathName(), tmp+1);

		OnSaveToneInfo(index, pThis);

		memset(pThis->p_option.w_toneData[index], 0, CFG_TONE_MAX_SIZE);
		tmp = encode(pThis->p_option.w_toneData[index], filePath, pThis->p_option.w_toneInfo[index].u8SubBands, 
													      pThis->p_option.w_toneInfo[index].u8BitPool, 
														  pThis->p_option.w_toneInfo[index].u8ChnMode,
														  pThis->p_option.w_toneInfo[index].u8Blocks,
														  pThis->p_option.w_toneInfo[index].u32BitRate);
		if(tmp >= 0)
		{
			if(index < CFG_TONE_4K_MAXNUM)
				max_size = 4*1024;
			else if(index < (CFG_TONE_8K_MAXNUM + CFG_TONE_4K_MAXNUM))
				max_size = 8*1024;
			else
				max_size = 16*1024;

			if(tmp > max_size)
			{
				sprintf(str, "The encoded data length of Tone %d size:%d, \n which is larger than %dKBytes! The excessive data will be losed!", index, tmp, (max_size/1024));
				pThis->MessageBox(str, "Tone encode Warning", MB_ICONEXCLAMATION|MB_OK);

				tmp = max_size;
			}

			pThis->p_option.w_toneInfo[index].u16DataLen = tmp;
			hex2ascii(pThis->p_option.w_toneData[index], tmp, hex);
			pThis->eEditFile[index].SetWindowText(hex);
		}
		else
		{
			pThis->p_option.w_toneInfo[index].u16DataLen = 0;
			sprintf(str, "The wave file of tone %d info is different from input!", index);
			pThis->MessageBox(str,"Tone -- infoPacking Warning",MB_ICONEXCLAMATION|MB_OK);
			pThis->eEditFile[index].SetWindowText("");
		}

		free(hex);
	}
	else
	{
		memset(pThis->p_option.w_toneData[index], 0, CFG_TONE_MAX_SIZE);
		pThis->UpdateData(FALSE);
		pThis->eEditFile[index].SetWindowText("");
	}
}

static int patch_GetFileLen(CString file)
{
	int temp,len;
	FILE *fp;
	errno_t fres;	

	fres = fopen_s(&fp, file, "rb" );
	if(fres)
	{
		return 0;
	}

	temp = ftell(fp);
	fseek(fp,0,SEEK_END);	
	len = ftell(fp);
	fseek(fp,temp,0);	

	fclose(fp);
	
	return len;
}



LRESULT CALLBACK ToneWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) 
{ 
	PAINTSTRUCT ps; 
	HDC hdc; 
	RECT rect; 
	static hs_tone *pThis;
	short ctrl_id;
	int index;
	LPCREATESTRUCT pPara = (LPCREATESTRUCT)lParam;

	switch (message) 
	{ 
		case WM_PAINT: //重绘消息 
			hdc = BeginPaint(hWnd, &ps); 
			GetClientRect(hWnd, &rect); 
			MoveToEx(hdc,10,10,0); 
			SelectObject( hdc,::CreatePen(PS_SOLID,1,RGB(0xFF,0,0))); 
			EndPaint(hWnd, &ps); 			
			break; 
		case WM_CREATE: 
			pPara = (LPCREATESTRUCT)lParam;
			pThis = (hs_tone *)pPara->lpCreateParams;
			break; 
		case WM_HOTKEY: 
			MessageBox(0,0,0,0); 
			break; 
		case WM_TIMER: 
			break; 
		case WM_DESTROY: //窗口销毁消息 
			index = getWinIndex(hWnd, pThis);
			if(index < 0)
			{
				PostQuitMessage(0);
				pThis->SetFocus();
				break;
			}

			OnSaveToneInfo(index, pThis);
			PostQuitMessage(0); 
			pThis->SetFocus();
			break; 

		case WM_COMMAND:
			index = getWinIndex(hWnd, pThis);
			if(index < 0)
				break;

			ctrl_id = wParam & 0xffff;
			if(ctrl_id == (IDC_TONE_BUTTON_FILE + index))
			{
				OnGetToneFile(index, pThis);
			}
			else if(ctrl_id == (IDC_TONE_BUTTON_OK + index))
			{
				pThis->pCWnd[index]->DestroyWindow();
			}

			break;

		default: 
			return DefWindowProc(hWnd, message, wParam, lParam); 
	} 
	return 0; 
}

void hs_tone::createChildWin(int index)
{
	CDialog *pParentDlg;
	MSG msg; 	
	CRect rect;
	int p_left = 20, p_top = 30, p_right = 80, p_button = 50;
	int i = index, tmp;
	char str[256], *hex;

	pParentDlg = (CDialog *)GetParent()->GetParent(); 
	pParentDlg->GetWindowRect(&rect);

	char szTitle[]="Tone Information"; // The title bar text 
	char szWindowClass[]="tone"; // The title bar text 
	WNDCLASSEX wcex={0}; 
	wcex.cbSize = sizeof(WNDCLASSEX); //WNDCLASSEX结构体大小 
	wcex.style = CS_HREDRAW | CS_VREDRAW; //位置改变时重绘 
	wcex.lpfnWndProc = (WNDPROC)ToneWndProc; //消息处理函数 
	wcex.hInstance = 0; //当前实例句柄 
	wcex.hbrBackground = (HBRUSH)COLOR_BTNSHADOW;//COLOR_WINDOWFRAME; //背景色 
	wcex.lpszClassName = szWindowClass; //参窗口类名 
	wcex.hIcon =0; //图标 
	wcex.hCursor =LoadCursor(NULL, IDC_ARROW); //光标 
	wcex.lpszMenuName =0; //菜单名称 
	wcex.hIconSm =0; //最小化图标 
	RegisterClassEx(&wcex); //注册窗口类 

	hWnd[i] = CreateWindow(szWindowClass, szTitle, WS_SYSMENU|WS_VISIBLE, rect.left, rect.top-5, 615, 450, this->m_hWnd, NULL, 0, this); 
	if (!hWnd[i])
	{ 
		return ; 
	} 

	pCWnd[i] = FromHandle(hWnd[i]);
	pCWnd[i]->ShowWindow(true); 
	pCWnd[i]->UpdateWindow();

	sTxtVolume[i].Create( "Volume", WS_VISIBLE, CRect(p_left, p_top, p_right, p_button), pCWnd[i], IDC_TONE_TEXT_VOLUME + i );
	eEditVolume[i].Create( WS_CHILD | WS_VISIBLE | WS_BORDER, CRect(p_left + 65, p_top - 5, p_right + 115, p_button), pCWnd[i], IDC_TONE_EDIT_VOLUME + i  ); 
	eEditVolume[i].SetWindowTextA("60");

	sTxtBitrate[i].Create( "BitRate", WS_VISIBLE , CRect(p_left + 190, p_top, p_right + 190, p_button), pCWnd[i], IDC_TONE_TEXT_BITRATE + i  );
	cListBitrate[i].Create( WS_VISIBLE | CBS_DROPDOWNLIST | CBS_SORT | WS_VSCROLL | WS_TABSTOP, CRect(p_left + 255, p_top - 5, p_right + 305, p_button), pCWnd[i], IDC_TONE_LIST_BITRATE + i);
	cListBitrate[i].InsertString(0, _T("16000"));
	cListBitrate[i].InsertString(1, _T("32000"));
	cListBitrate[i].InsertString(2, _T("44100"));
	cListBitrate[i].InsertString(3, _T("48000"));
	cListBitrate[i].SetCurSel(0);

	sTxtBlocks[i].Create( "Blocks", WS_VISIBLE , CRect(p_left + 380, p_top, p_right + 380, p_button), pCWnd[i], IDC_TONE_TEXT_BITRATE + i  );
	cListBlocks[i].Create( WS_VISIBLE | CBS_DROPDOWNLIST | CBS_SORT | WS_VSCROLL | WS_TABSTOP, CRect(p_left + 445, p_top - 5, p_right + 505, p_button), pCWnd[i], IDC_TONE_LIST_BITRATE + i);
	cListBlocks[i].InsertString(0, _T("4"));
	cListBlocks[i].InsertString(1, _T("8"));
	cListBlocks[i].InsertString(2, _T("12"));
	cListBlocks[i].InsertString(3, _T("16"));
	cListBlocks[i].SetCurSel(3);

	p_top += 30;
	p_button += 30;

	sTxtBitpool[i].Create( "Bitpool", WS_VISIBLE, CRect(p_left, p_top, p_right, p_button), pCWnd[i], IDC_TONE_TEXT_BITPOOL + i );
	eEditBitpool[i].Create( WS_CHILD | WS_VISIBLE | WS_BORDER, CRect(p_left + 65, p_top - 5, p_right + 115, p_button), pCWnd[i], IDC_TONE_EDIT_BITPOOL + i  ); 
	eEditBitpool[i].SetWindowTextA("16");

	sTxtSubband[i].Create( "Subband", WS_VISIBLE, CRect(p_left + 190, p_top, p_right + 190, p_button), pCWnd[i], IDC_TONE_TEXT_SUBBANDS + i );
	cListSubband[i].Create( WS_VISIBLE | CBS_DROPDOWNLIST | CBS_SORT | WS_VSCROLL | WS_TABSTOP, CRect(p_left + 255, p_top - 5, p_right + 305, p_button), pCWnd[i], IDC_TONE_LIST_SUBBANDS + i  ); 
	cListSubband[i].InsertString(0, _T("4"));
	cListSubband[i].InsertString(1, _T("8"));
	cListSubband[i].SetCurSel(1);

	sTxtChnmode[i].Create( "ChnMode", WS_VISIBLE , CRect(p_left + 380, p_top, p_right + 380, p_button), pCWnd[i], IDC_TONE_TEXT_BITRATE + i  );
	cListChnmode[i].Create( WS_VISIBLE | CBS_DROPDOWNLIST | CBS_SORT | WS_VSCROLL | WS_TABSTOP, CRect(p_left + 445, p_top - 5, p_right + 505, p_button), pCWnd[i], IDC_TONE_LIST_BITRATE + i);
	cListChnmode[i].InsertString(0, _T("mono"));
	cListChnmode[i].InsertString(1, _T("dual_channel"));
	cListChnmode[i].InsertString(2, _T("stereo"));
	cListChnmode[i].InsertString(3, _T("joint_stereo"));
	cListChnmode[i].SetCurSel(0);
	
	p_top += 30;
	p_button += 30;

	sTxtLoopNum[i].Create( "Loop", WS_VISIBLE, CRect(p_left, p_top, p_right, p_button), pCWnd[i], IDC_TONE_TEXT_LOOPNUM + i );
	eEditLoopNum[i].Create( WS_CHILD | WS_VISIBLE | WS_BORDER, CRect(p_left + 65, p_top - 5, p_right + 115, p_button), pCWnd[i], IDC_TONE_EDIT_LOOPNUM + i  ); 
	eEditLoopNum[i].SetWindowTextA("1");

	sTxtDelay[i].Create( "Delay(s)", WS_VISIBLE , CRect(p_left + 190, p_top, p_right + 190, p_button), pCWnd[i], IDC_TONE_TEXT_DELAY + i  );
	eEditDelay[i].Create( WS_CHILD | WS_VISIBLE | WS_BORDER, CRect(p_left + 255, p_top - 5, p_right + 305, p_button), pCWnd[i], IDC_TONE_EDIT_DELAY + i);
	eEditDelay[i].SetWindowTextA("1");

#if 0
	if(i<CFG_TONE_4K_MAXNUM)
		sprintf_s(str,"Notice: smaller than 4KB!");
	if((i>=CFG_TONE_4K_MAXNUM)&&(i<(CFG_TONE_4K_MAXNUM+CFG_TONE_8K_MAXNUM)))
		sprintf_s(str,"Notice: smaller than 8KB!");
	if(i>=(CFG_TONE_4K_MAXNUM+CFG_TONE_8K_MAXNUM))
		sprintf_s(str,"Notice: smaller than 16KB!");

	sTxtNotice[i].Create( str, WS_VISIBLE , CRect(p_left + 380, p_top, p_right + 495, p_button), pCWnd[i], IDC_TONE_TEXT_NOTICE + i  );
#endif

	sprintf_s(str,"Open File of Tone %d", i);
	bToneFile[i].Create( str, WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, CRect(p_left + 380, p_top-6, p_right + 505, p_button), pCWnd[i], IDC_TONE_BUTTON_FILE + i  ); 

	p_top += 30;
	p_button += 30;

	sTxtFile[i].Create( "File", WS_VISIBLE, CRect(p_left, p_top, p_right, p_button), pCWnd[i], IDC_TONE_TEXT_FILE + i );
	eEditFile[i].Create( WS_CHILD | WS_VISIBLE | ES_MULTILINE | ES_AUTOVSCROLL | WS_BORDER | ES_WANTRETURN | WS_VSCROLL | ES_READONLY, CRect(p_left + 65, p_top - 5, p_right + 505, p_button+240), pCWnd[i], IDC_TONE_EDIT_FILE + i); 

	CFont newFont;
	newFont.CreateFontA(0, 0, 0, 0, FW_NORMAL, FALSE, FALSE, 0,ANSI_CHARSET, 0, 0,0,0, _T("仿宋") );
	eEditFile[i].SetFont(&newFont);

	//sprintf_s(str,"Tone %d", i);
	//bToneFile[i].Create( str, WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, CRect(p_right + 430, p_top - 6 + 320, p_right + 495, p_button+320), pCWnd[i], IDC_TONE_BUTTON_FILE + i  ); 

	p_top += 270;
	p_button += 270;

	bToneOk[i].Create( "OK", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, CRect(p_left + 250, p_top - 6, p_left + 360, p_button + 1), pCWnd[i], IDC_TONE_BUTTON_OK + i  ); 

	if(p_option.createFlg[i] == 1)
	{
		sprintf_s(str,"%d", p_option.w_toneInfo[i].u16ToneVolume);
		eEditVolume[i].SetWindowTextA(str);

		sprintf_s(str,"%d", p_option.w_toneInfo[i].u8BitPool);
		eEditBitpool[i].SetWindowTextA(str);

		sprintf_s(str,"%d", p_option.w_toneInfo[i].u8LoopNum);
		eEditLoopNum[i].SetWindowTextA(str);

		sprintf_s(str,"%d", p_option.w_toneInfo[i].u8Delay);
		eEditDelay[i].SetWindowTextA(str);
		
		hex = (char *)malloc(CFG_TONE_MAX_SIZE * 8);
		hex2ascii(p_option.w_toneData[index], p_option.w_toneInfo[index].u16DataLen, hex);
		eEditFile[index].SetWindowText(hex);
		free(hex);

		cListChnmode[i].SetCurSel(p_option.w_toneInfo[i].u8ChnMode);

		tmp = 0x7f;
		if(p_option.w_toneInfo[i].u32BitRate == 16000)
			tmp = 0;
		else if(p_option.w_toneInfo[i].u32BitRate == 32000)
			tmp = 1;
		else if(p_option.w_toneInfo[i].u32BitRate == 44100)
			tmp = 2;
		else if(p_option.w_toneInfo[i].u32BitRate == 48000)
			tmp = 3;

		if(tmp <= 3)
			cListBitrate[i].SetCurSel(tmp);

		tmp = 0x7f;
		if(p_option.w_toneInfo[i].u8Blocks == 4)
			tmp = 0;
		else if(p_option.w_toneInfo[i].u8Blocks == 8)
			tmp = 1;
		else if(p_option.w_toneInfo[i].u8Blocks == 12)
			tmp = 2;
		else if(p_option.w_toneInfo[i].u8Blocks == 16)
			tmp = 3;

		if(tmp <= 3)
			cListBlocks[i].SetCurSel(tmp);

		tmp = 0x7f;
		if(p_option.w_toneInfo[i].u8SubBands == 4)
			tmp = 0;
		else if(p_option.w_toneInfo[i].u8SubBands == 8)
			tmp = 1;

		if(tmp <= 1)
			cListSubband[i].SetCurSel(tmp);
	}	

	while (GetMessage(&msg, NULL, 0, 0)) // 消息循环: 
	{ 
		TranslateMessage(&msg); //转化虚拟按键到字符消息 
		DispatchMessage(&msg); //分派消息调用回调函数 
	} 
	return ; 
} 

void hs_tone::doToneData(void)
{
	int i, temp;

	p_option.data_ptr = 0;
	memset(p_option.ptr, 0, CFG_TONE_MAX_SIZE * CFG_TONE_MAXNUM);

	for(i=0; i<CFG_TONE_MAXNUM; i++)
	{
		if(i < CFG_TONE_4K_MAXNUM)
			temp = 4*1024;
		else if(i < (CFG_TONE_8K_MAXNUM + CFG_TONE_4K_MAXNUM))
			temp = 8*1024;
		else
			temp = 16*1024;

		memcpy(&p_option.ptr[p_option.data_ptr], &p_option.w_toneInfo[i], sizeof(hs_sound_info_t));
		p_option.data_ptr += sizeof(hs_sound_info_t);

		if(p_option.w_toneInfo[i].u16DataLen == 0)
			memset(p_option.w_toneData[i], 0, temp);

		memcpy(&p_option.ptr[p_option.data_ptr], p_option.w_toneData[i], temp);
		p_option.data_ptr += temp;
	}
}


void hs_tone::OnBnClickedTone0()
{
	int index = 0;

	createChildWin(index);	
	p_option.createFlg[index] = 1;
}

void hs_tone::OnBnClickedTone1()
{
	int index = 1;

	createChildWin(index);	
	p_option.createFlg[index] = 1;
}

void hs_tone::OnBnClickedTone2()
{
	int index = 2;

	createChildWin(index);	
	p_option.createFlg[index] = 1;
}

void hs_tone::OnBnClickedTone3()
{
	int index = 3;

	createChildWin(index);	
	p_option.createFlg[index] = 1;
}

void hs_tone::OnBnClickedTone4()
{
	int index = 4;

	createChildWin(index);	
	p_option.createFlg[index] = 1;
}

void hs_tone::OnBnClickedTone5()
{
	int index = 5;

	createChildWin(index);	
	p_option.createFlg[index] = 1;
}

void hs_tone::OnBnClickedTone6()
{
	int index = 6;

	createChildWin(index);	
	p_option.createFlg[index] = 1;
}

void hs_tone::OnBnClickedTone7()
{
	int index = 7;

	createChildWin(index);	
	p_option.createFlg[index] = 1;
}

void hs_tone::OnBnClickedTone8()
{
	int index = 8;

	createChildWin(index);	
	p_option.createFlg[index] = 1;
}

void hs_tone::OnBnClickedTone9()
{
	int index = 9;

	createChildWin(index);	
	p_option.createFlg[index] = 1;
}



void hs_tone::OnBnClickedTone10()
{
	int index = 10;

	createChildWin(index);	
	p_option.createFlg[index] = 1;
}

void hs_tone::OnBnClickedTone11()
{
	int index = 11;

	createChildWin(index);	
	p_option.createFlg[index] = 1;
}

void hs_tone::OnBnClickedTone12()
{
	int index = 12;

	createChildWin(index);	
	p_option.createFlg[index] = 1;
}

void hs_tone::OnBnClickedTone13()
{
	int index = 13;

	createChildWin(index);	
	p_option.createFlg[index] = 1;
}

void hs_tone::OnBnClickedTone14()
{
	int index = 14;

	createChildWin(index);	
	p_option.createFlg[index] = 1;
}

void hs_tone::OnBnClickedTone15()
{
	int index = 15;

	createChildWin(index);	
	p_option.createFlg[index] = 1;
}

void hs_tone::OnBnClickedTone16()
{
	int index = 16;

	createChildWin(index);	
	p_option.createFlg[index] = 1;
}

void hs_tone::OnBnClickedTone17()
{
	int index = 17;

	createChildWin(index);	
	p_option.createFlg[index] = 1;
}

void hs_tone::OnBnClickedTone18()
{
	int index = 18;

	createChildWin(index);	
	p_option.createFlg[index] = 1;
}

void hs_tone::OnBnClickedTone19()
{
	int index = 19;

	createChildWin(index);	
	p_option.createFlg[index] = 1;
}



void hs_tone::OnBnClickedTone20()
{
	int index = 20;

	createChildWin(index);	
	p_option.createFlg[index] = 1;
}

void hs_tone::OnBnClickedTone21()
{
	int index = 21;

	createChildWin(index);	
	p_option.createFlg[index] = 1;
}

void hs_tone::OnBnClickedTone22()
{
	int index = 22;

	createChildWin(index);	
	p_option.createFlg[index] = 1;
}

void hs_tone::OnBnClickedTone23()
{
	int index = 23;

	createChildWin(index);	
	p_option.createFlg[index] = 1;
}

void hs_tone::OnBnClickedTone24()
{
	int index = 24;

	createChildWin(index);	
	p_option.createFlg[index] = 1;
}

void hs_tone::OnBnClickedTone25()
{
	int index = 25;

	createChildWin(index);	
	p_option.createFlg[index] = 1;
}

void hs_tone::OnBnClickedTone26()
{
	int index = 26;

	createChildWin(index);	
	p_option.createFlg[index] = 1;
}

void hs_tone::OnBnClickedTone27()
{
	int index = 27;

	createChildWin(index);	
	p_option.createFlg[index] = 1;
}

void hs_tone::OnBnClickedTone28()
{
	int index = 28;

	createChildWin(index);	
	p_option.createFlg[index] = 1;
}

void hs_tone::OnBnClickedTone29()
{
	int index = 29;

	createChildWin(index);	
	p_option.createFlg[index] = 1;
}






void hs_tone::OnBnClickedTone30()
{
	int index = 30;

	createChildWin(index);	
	p_option.createFlg[index] = 1;
}

void hs_tone::OnBnClickedTone31()
{
	int index = 31;

	createChildWin(index);	
	p_option.createFlg[index] = 1;
}

void hs_tone::OnBnClickedTone32()
{
	int index = 32;

	createChildWin(index);	
	p_option.createFlg[index] = 1;
}

void hs_tone::OnBnClickedTone33()
{
	int index = 33;

	createChildWin(index);	
	p_option.createFlg[index] = 1;
}

void hs_tone::OnBnClickedTone34()
{
	int index = 34;

	createChildWin(index);	
	p_option.createFlg[index] = 1;
}

void hs_tone::OnBnClickedTone35()
{
	int index = 35;

	createChildWin(index);	
	p_option.createFlg[index] = 1;
}

void hs_tone::OnBnClickedTone36()
{
	int index = 36;

	createChildWin(index);	
	p_option.createFlg[index] = 1;
}

void hs_tone::OnBnClickedTone37()
{
	int index = 37;

	createChildWin(index);	
	p_option.createFlg[index] = 1;
}

void hs_tone::OnBnClickedTone38()
{
	int index = 38;

	createChildWin(index);	
	p_option.createFlg[index] = 1;
}

void hs_tone::OnBnClickedTone39()
{
	int index = 39;

	createChildWin(index);	
	p_option.createFlg[index] = 1;
}



void hs_tone::OnBnClickedTone40()
{
	int index = 40;

	createChildWin(index);	
	p_option.createFlg[index] = 1;
}

void hs_tone::OnBnClickedTone41()
{
	int index = 41;

	createChildWin(index);	
	p_option.createFlg[index] = 1;
}

void hs_tone::OnBnClickedTone42()
{
	int index = 42;

	createChildWin(index);	
	p_option.createFlg[index] = 1;
}

void hs_tone::OnBnClickedTone43()
{
	int index = 43;

	createChildWin(index);	
	p_option.createFlg[index] = 1;
}

void hs_tone::OnBnClickedTone44()
{
	int index = 44;

	createChildWin(index);	
	p_option.createFlg[index] = 1;
}

void hs_tone::OnBnClickedTone45()
{
	int index = 45;

	createChildWin(index);	
	p_option.createFlg[index] = 1;
}

void hs_tone::OnBnClickedTone46()
{
	int index = 46;

	createChildWin(index);	
	p_option.createFlg[index] = 1;
}

void hs_tone::OnBnClickedTone47()
{
	int index = 47;

	createChildWin(index);	
	p_option.createFlg[index] = 1;
}

void hs_tone::OnBnClickedTone48()
{
	int index = 48;

	createChildWin(index);	
	p_option.createFlg[index] = 1;
}

void hs_tone::restoreDlg()
{
	hs_chkToneEn.SetCheck(p_option.tone_enbale);

	UpdateData(false);
}


void hs_tone::OnBnClickedChkToneEn()
{
	if(hs_chkToneEn.GetCheck())
	{
		p_option.tone_enbale = 1;
	}
	else
	{
		p_option.tone_enbale = 0;
	}
}
