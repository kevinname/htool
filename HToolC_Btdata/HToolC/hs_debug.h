#pragma once

#include "comm_base.h"
#include "afxcmn.h"
#include "afxwin.h"

#define SHOW_LINE_NUMBER			16
#define FLASH_MEM_BASE				0x80000000

// hs_debug dialog

class hs_debug : public comm_base
{
	DECLARE_DYNAMIC(hs_debug)

public:
	hs_debug(CWnd* pParent = NULL);   // standard constructor
	virtual ~hs_debug();

// Dialog Data
	enum { IDD = IDD_DIALOG_DEBUG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog(void);

	DECLARE_MESSAGE_MAP()
public:
	
	CListCtrl m_MemList;
	CEdit  editItem;
	CEdit m_dbgEditAddr;
	CEdit m_dbgEditLength;
	CComboBox m_dbgCombAlign;
	CButton m_bnRefresh;

	int hitRow, hitCol;	

	void init_memlist(void);
	afx_msg void OnNMDblclkListMem(NMHDR *pNMHDR, LRESULT *pResult);	
	afx_msg void OnEnKillfocusEditMem();
	afx_msg void OnNMClickListMem(NMHDR *pNMHDR, LRESULT *pResult);
	void editMem(NMHDR * pNMHDR);
	
	void refreshMemoryData(void);
	afx_msg void OnBnClickedButtonDebugRefreash();

	afx_msg void OnEnKillfocusEditDebugLength();
	void GetMemData(unsigned char *buf, unsigned int address, int length);	
	afx_msg void OnBnClickedButtonDebugEraseData();
	afx_msg void OnEnChangeEditDebugAddr();
	CButton hs_bnEraseMem;
};
