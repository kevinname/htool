#include "stdafx.h"
#include "HToolC.h"
#include "comm_base.h"


// comm_base

IMPLEMENT_DYNAMIC(comm_base, CDialogEx)

comm_base::comm_base()
{

}

comm_base::comm_base(UINT nIDTemplate, CWnd *pParent)
{
	CDialogEx(nIDTemplate, pParent);
}

comm_base::~comm_base()
{
}


BEGIN_MESSAGE_MAP(comm_base, CDialogEx)
END_MESSAGE_MAP()



// comm_base message handlers




comm_cmd * comm_base::getCmdHandler(void)
{
	CDialog *pParentDlg;
	CHToolCDlg *pMainDlg;
	comm_config *pCommHandler;
	std::list<CChildDlgInfo>::iterator child;
	bool findDlg = false;	

	//pParentDlg = (CDialog *)this->GetParent()->GetParent();
	pParentDlg =  (CDialog *)AfxGetApp()->GetMainWnd();
	pMainDlg = (CHToolCDlg *)pParentDlg;
	for(child = pMainDlg->m_lstChildDlg.begin(); child != pMainDlg->m_lstChildDlg.end(); child++)
	{
		if(strcmp(child->strName, "Transmit") == 0)
		{
			findDlg = true;
			break;
		}
	}

	if(findDlg == false)
	{
		MessageBox("Can't find the Transmit Tab", "Error", MB_ICONERROR|MB_OK);
		return NULL;
	}

	pCommHandler = (comm_config *)child->pDlg;

	return &pCommHandler->hs_comm;
}
