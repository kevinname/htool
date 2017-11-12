
// HToolCDlg.cpp : implementation file
//

#include "stdafx.h"
#include "HToolC.h"
#include "HToolCDlg.h"
#include "afxdialogex.h"
#include "hs_debug.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CHToolCDlg dialog
CHToolCDlg::CHToolCDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CHToolCDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

    // add new tag in here	
    m_lstChildDlg.push_back(CChildDlgInfo(IDD_COMM_CONFIG,				_T("Transmit"),		    new comm_config()));
	m_lstChildDlg.push_back(CChildDlgInfo(IDD_DIALOG_DEBUG,				_T("Memory"),			new hs_debug()));  
	m_lstChildDlg.push_back(CChildDlgInfo(IDD_HS_PACKAGE,				_T("Package"),			new hs_package()));
	m_lstChildDlg.push_back(CChildDlgInfo(IDD_TONE_CONFIG,				_T("Tone"),				new hs_tone()));
}

CHToolCDlg::~CHToolCDlg()
{
	FILE *dataFile;
	errno_t fres;

	SetFileAttributes(CFG_DATA_DIR, FILE_ATTRIBUTE_NORMAL);
	SetFileAttributes(CFG_DATA_FILE, FILE_ATTRIBUTE_NORMAL);

	fres = fopen_s(&dataFile, CFG_DATA_FILE, "wb+" );
	if(fres)
	{		
		dataFile = NULL;
	}

	FOR_EACH_DLG(ior)
    {
        ASSERT(ior->pDlg != NULL);
		SaveDlgData(dataFile, ior->pDlg, ior->strName);
        delete(ior->pDlg);
    }

	SetFileAttributes(CFG_DATA_DIR, FILE_ATTRIBUTE_HIDDEN);
	SetFileAttributes(CFG_DATA_FILE, FILE_ATTRIBUTE_HIDDEN);

	if(dataFile)
		fclose(dataFile);
}

void CHToolCDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);	
	DDX_Control(pDX, IDC_TAB_MAIN, hs_mainTab);
}

BEGIN_MESSAGE_MAP(CHToolCDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB_MAIN, &CHToolCDlg::OnTcnSelchangeTabMain)
END_MESSAGE_MAP()


BOOL FolderExist(CString strPath)
{
    WIN32_FIND_DATA wfd;
    BOOL rValue = FALSE;
    HANDLE hFind = FindFirstFile(strPath, &wfd);
    if ((hFind!=INVALID_HANDLE_VALUE) &&
         (wfd.dwFileAttributes&FILE_ATTRIBUTE_DIRECTORY))
    {
        rValue = TRUE;
    }
    FindClose(hFind);
    return rValue;
}

BOOL CreateFolder(CString strPath)
{
    SECURITY_ATTRIBUTES attrib;
    attrib.bInheritHandle = FALSE;
    attrib.lpSecurityDescriptor = NULL;
    attrib.nLength = sizeof(SECURITY_ATTRIBUTES);
    return ::CreateDirectory(strPath, &attrib);
}

BOOL CHToolCDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	SetIcon(m_hIcon, TRUE);
	SetIcon(m_hIcon, FALSE);
	CRect rect, olRect, newRect; 
	hs_mainTab.GetClientRect(&rect); 
	rect.top    +=20;
	rect.bottom -=4;
	rect.left   +=1;
	rect.right  -=4;
	m_page = 1;

    int i = 0;
    FOR_EACH_DLG(ior)
    {
        ASSERT(ior->pDlg != NULL);
        
        hs_mainTab.InsertItem(i++, ior->strName); 
        ior->pDlg->Create(ior->dwID,  GetDlgItem(IDC_TAB_MAIN));

		ior->pDlg->GetClientRect(&olRect); 

		newRect.top = rect.top;
		newRect.left = rect.left;
		newRect.bottom = newRect.top + (olRect.bottom - olRect.top);
		newRect.right = rect.left + (olRect.right - olRect.left);
        ior->pDlg->MoveWindow(&newRect);
    }

	m_lstChildDlg.begin()->pDlg->ShowWindow(TRUE);

	hs_mainTab.SetCurSel(0);

	if(FolderExist(CFG_DATA_DIR) == FALSE)
		CreateFolder(CFG_DATA_DIR);
	RestoreDlgData();

	return TRUE; 
}

void CHToolCDlg::OnTcnSelchangeTabMain(NMHDR *pNMHDR, LRESULT *pResult)
{
	int iCurSel = hs_mainTab.GetCurSel();
    int i = 0;
	CRect mrect, rect;

    FOR_EACH_DLG(ior)
    { 
		if(i++ == iCurSel)
		{
			ior->pDlg->ShowWindow(TRUE); 
			ior->pDlg->GetClientRect(&rect); 
		}
		else
		{
			ior->pDlg->ShowWindow(FALSE); 
		}
    }

	GetWindowRect(&mrect);	

	rect.bottom +=20;
	rect.right  -=1;

	mrect.bottom = mrect.top + rect.bottom + 42;
	mrect.right = mrect.left + rect.right + 15;

	hs_mainTab.MoveWindow(&rect);
	MoveWindow(&mrect);

    *pResult = 0;
}


static unsigned int cal_checksum(unsigned char *ptr,unsigned int len)
{
	unsigned int i,u32ChkSum = 0;

	for(i=0; i<len; i++)
		u32ChkSum += ptr[i];

	return u32ChkSum;
}

void CHToolCDlg::SaveDlgData(FILE *dataFile, CDialogEx *pcDlg, CString strName)
{	 
	unsigned int f = CFG_DATA_FLAG;

	if(dataFile == NULL)
		return ;

	fwrite((void *)&f, 1, 4, dataFile);

	if(strcmp(strName, "Transmit") == 0)
	{
		comm_config *pDlg = (comm_config *)pcDlg;
		fwrite((void *)pDlg->w_curdir, 1, 256, dataFile);
	}

	if(strcmp(strName, "Package") == 0)
	{
		hs_package *pDlg = (hs_package *)pcDlg;

		pDlg->p_option.chksum = 0;
		pDlg->p_option.chksum = cal_checksum((unsigned char *)&pDlg->p_option, sizeof(pDlg->p_option));

		fwrite((void *)&pDlg->p_option, 1, sizeof(pDlg->p_option), dataFile);
	}

	if(strcmp(strName, "Tone") == 0)
	{
		hs_tone *pTone = (hs_tone *)pcDlg;

		pTone->p_option.chksum = 0;
		pTone->p_option.chksum = cal_checksum((unsigned char *)&pTone->p_option, sizeof(pTone->p_option));

		fwrite((void *)&pTone->p_option, 1, sizeof(pTone->p_option), dataFile);
	}
}


void CHToolCDlg::GetDlgData(FILE *dataFile, CDialogEx *pcDlg, CString strName)
{
	int i;
	size_t size;
	unsigned int f, isValid;
	unsigned int chksum;

	if(!dataFile)
		isValid = 0;
	else
	{
		size = fread((void *)&f, 1, 4, dataFile);
		if((size == 0) || (f != CFG_DATA_FLAG))
			isValid = 0;
		else
			isValid = 1;
	}

	if(strcmp(strName, "Transmit") == 0)
	{
		comm_config *pDlg = (comm_config *)pcDlg;

		if(isValid)
			fread((void *)pDlg->w_curdir, 1, 256, dataFile);
		else
			memset((void *)pDlg->w_curdir, 0, 256);
	}

	if(strcmp(strName, "Package") == 0)
	{
		hs_package *pDlg = (hs_package *)pcDlg;

		memset((void *)&pDlg->p_option, 0, sizeof(pDlg->p_option));
		if(dataFile)
		{			
			size = fread((void *)&pDlg->p_option, 1, sizeof(pDlg->p_option), dataFile);
			if(size != sizeof(pDlg->p_option))
				isValid = 0;

			if(isValid)
			{
				chksum = pDlg->p_option.chksum;
				pDlg->p_option.chksum = 0;
				if(chksum != cal_checksum((unsigned char *)&pDlg->p_option, sizeof(pDlg->p_option)))
					isValid = 0;
			}
		}

		if(isValid)
			pDlg->restoreDlg();
	}

	if(strcmp(strName, "Tone") == 0)
	{
		hs_tone *pTone = (hs_tone *)pcDlg;

		if(dataFile)
		{	
			size = fread((void *)&pTone->p_option, 1, sizeof(pTone->p_option), dataFile);
			if(size != sizeof(pTone->p_option))
				isValid = 0;

			if(isValid)
			{
				chksum = pTone->p_option.chksum;
				pTone->p_option.chksum = 0;
				if(chksum != cal_checksum((unsigned char *)&pTone->p_option, sizeof(pTone->p_option)))
					isValid = 0;
			}
		}

		if(!isValid)
		{
			memset((void *)&pTone->p_option.w_toneInfo, 0, sizeof(hs_sound_info_t) * CFG_TONE_MAXNUM);
			memset((void *)&pTone->p_option.w_curdir, 0, TONE_FILE_LENGTH);
			for(i=0; i<CFG_TONE_MAXNUM; i++)
			{
				memset(pTone->p_option.w_toneData[i], 0, TONE_FILE_LENGTH);
				pTone->p_option.createFlg[i] = 0;
			}

			pTone->p_option.tone_enbale = 0;
		}

		pTone->p_option.ptr = (char *)malloc(CFG_TONE_MAX_SIZE * CFG_TONE_MAXNUM);
		pTone->restoreDlg();
	}
}

void CHToolCDlg::RestoreDlgData()
{	 
	FILE *dataFile;
	errno_t fres;

	fres = fopen_s(&dataFile, CFG_DATA_FILE, "rb+" );
	if(fres)
	{		
		dataFile = NULL;
	}

	FOR_EACH_DLG(ior)
	{
		GetDlgData(dataFile, ior->pDlg, ior->strName);
	}

	if(dataFile)
	{
		fclose(dataFile);
	}
}
