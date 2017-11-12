// hs_debug.cpp : implementation file
//

#include "stdafx.h"
#include "HToolC.h"
#include "hs_debug.h"
#include "afxdialogex.h"


// hs_debug dialog

IMPLEMENT_DYNAMIC(hs_debug, CDialogEx)

hs_debug::hs_debug(CWnd* pParent /*=NULL*/)
	: comm_base(hs_debug::IDD, pParent)
{

}

hs_debug::~hs_debug()
{
}

void hs_debug::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_MEM, m_MemList);
	DDX_Control(pDX, IDC_EDIT_DEBUG_ADDR, m_dbgEditAddr);
	DDX_Control(pDX, IDC_EDIT_DEBUG_LENGTH, m_dbgEditLength);
	DDX_Control(pDX, IDC_COMBO_DEBUG_ALIGN, m_dbgCombAlign);
	DDX_Control(pDX, IDC_BUTTON_DEBUG_REFREASH, m_bnRefresh);
	DDX_Control(pDX, IDC_BUTTON_DEBUG_ERASE_DATA, hs_bnEraseMem);
}


BEGIN_MESSAGE_MAP(hs_debug, CDialogEx)
	ON_EN_KILLFOCUS(IDC_EDIT_DEBUG_LENGTH, &hs_debug::OnEnKillfocusEditDebugLength)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST_MEM, &hs_debug::OnNMDblclkListMem)	
	ON_NOTIFY(NM_CLICK, IDC_LIST_MEM, &hs_debug::OnNMClickListMem)
	ON_BN_CLICKED(IDC_BUTTON_DEBUG_REFREASH, &hs_debug::OnBnClickedButtonDebugRefreash)

	ON_EN_KILLFOCUS(IDC_EDIT_MEM_LIST, &hs_debug::OnEnKillfocusEditMem)	
	ON_BN_CLICKED(IDC_BUTTON_DEBUG_ERASE_DATA, &hs_debug::OnBnClickedButtonDebugEraseData)
	ON_EN_CHANGE(IDC_EDIT_DEBUG_ADDR, &hs_debug::OnEnChangeEditDebugAddr)
END_MESSAGE_MAP()


// hs_debug message handlers
BOOL hs_debug::OnInitDialog(void)
{
	CDialogEx::OnInitDialog();
	
	m_dbgEditAddr.SetWindowTextA(_T("10000000"));
	m_dbgEditLength.SetWindowTextA(_T("100"));

	m_dbgCombAlign.InsertString(0, _T("1"));
	m_dbgCombAlign.InsertString(1, _T("2"));
	m_dbgCombAlign.InsertString(2, _T("4"));
	m_dbgCombAlign.SetCurSel(0);

	hs_bnEraseMem.EnableWindow(0);

	init_memlist();
	return true;
}

static void string_s2b(char *buffer)
{
	while(buffer[0] != '\0')
	{
		if((buffer[0] >= 'a') && (buffer[0] <= 'f'))
			buffer[0] -= 0x20;

		if((buffer[0] > 'F') && (buffer[0] <= 'Z'))
			buffer[0] = 0x30;

		if((buffer[0] > 'f') && (buffer[0] <= 'z'))
			buffer[0] = 0x30;

		buffer++;
	};
}

void hs_debug::init_memlist(void)
{
	int i, j, width, line = 16;
	char col_name[32];	
	int nRow;
	LONG lStyle; 

	lStyle = GetWindowLong(m_MemList.m_hWnd, GWL_STYLE);// 获取当前窗口style 
	lStyle |= LVS_REPORT; // 设置style 
	SetWindowLong(m_MemList.m_hWnd, GWL_STYLE, lStyle);	// 设置style 

	DWORD dwStyle = m_MemList.GetExtendedStyle(); 	
	dwStyle |= LVS_EX_GRIDLINES;						// 网格线（只适用与report 风格的listctrl ） 
	m_MemList.SetExtendedStyle(dwStyle);				// 设置扩展风格	

	m_MemList.InsertColumn(0, "Address", LVCFMT_CENTER, 60 );
	for(i=0; i<16; i++)
	{
		sprintf_s(col_name, "%02x", i);
		string_s2b(col_name);
		m_MemList.InsertColumn( i+1, col_name, LVCFMT_CENTER, 26 );
	}

	m_MemList.InsertColumn( i+1, "ASCII", LVCFMT_CENTER, 50 );

	for(i=0; i<line; i++)
	{
		sprintf_s(col_name, "%08x", (i * 0x10));
		string_s2b(col_name);
		nRow = m_MemList.InsertItem(i, col_name);

		for(j=1; j<=16; j++)
		{
			m_MemList.SetItemText(nRow, j, "00");
		}		
	}

}

void hs_debug::OnNMDblclkListMem(NMHDR *pNMHDR, LRESULT *pResult)
{
	editMem(pNMHDR);
	*pResult = 0;
}


void hs_debug::OnEnKillfocusEditMem()
{
	char str[256];
	CString addr;
	unsigned int val, align, address;
	int res, type;
	unsigned char input[20], data[20];
	unsigned short r_cmd, w_cmd;

	r_cmd = COMM_CMD_MEMORY_READ;
	w_cmd = COMM_CMD_MEMORY_WRITE;

	m_dbgCombAlign.GetWindowTextA(str, 9);
	string_s2b(str);
	sscanf_s(str,"%x",&align);

	editItem.GetWindowTextA(str, 9);
	string_s2b(str);
	sscanf_s(str,"%x",&val);	

	addr = m_MemList.GetItemText(hitRow, 0);
	sscanf_s(addr,"%x",&address);

	address += (hitCol - 1) * align;

	input[0] = address;
	input[1] = address>>8;
	input[2] = address>>16;
	input[3] = address>>24;

	input[4] = val;
	input[5] = val>>8;
	input[6] = val>>16;
	input[7] = val>>24;

	res = getCmdHandler()->SendCmd(w_cmd, input, 4+align, NULL, 0);
	if(res != 0)
	{
		sprintf_s(str, "Write memory(address:%08x, val:%x) error.\n Error Code: %d", address, val, res);
		MessageBox(_T(str), _T("Read memory error"), MB_ICONEXCLAMATION|MB_OK);			
	}

	res = getCmdHandler()->SendCmd(r_cmd, input, 4, data, align);
	if(res != 0)
	{
		sprintf_s(str, "Read memory(address:%08x) error.\n Error Code: %d", address, res);
		MessageBox(_T(str), _T("Read memory error"), MB_ICONEXCLAMATION|MB_OK);			
	}

	if(align == 1)
	{
		val = data[0];

		sprintf_s(str, "%02x", val);
		str[2] = 0;
	}
	else if(align == 2)
	{
		val = data[1];
		val = (val << 8) + data[0];

		sprintf_s(str, "%04x", val);
		str[4] = 0;
	}
	else if(align == 4)
	{
		val = data[3];
		val = (val << 8) + data[2];
		val = (val << 8) + data[1];
		val = (val << 8) + data[0];

		sprintf_s(str, "%08x", val);
		str[8] = 0;
	}

	string_s2b(str);
	m_MemList.SetItemText(hitRow, hitCol, str);

	if (editItem.m_hWnd  !=  NULL)
		editItem.DestroyWindow();
}


void hs_debug::OnNMClickListMem(NMHDR *pNMHDR, LRESULT *pResult)
{	
	editMem(pNMHDR);
	*pResult = 0;
}


void hs_debug::editMem(NMHDR * pNMHDR)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);	
	CRect item_rect, rect;
	LVHITTESTINFO info;
	info.pt  =  pNMItemActivate -> ptAction;
	char str[25];
	int align;

	m_dbgCombAlign.GetWindowTextA(str, 9);
	sscanf_s(str,"%x",&align);
  
	if (m_MemList.SubItemHitTest( & info)  !=   - 1  )
	{
		hitRow  =  info.iItem;
		hitCol  =  info.iSubItem;
		if((hitCol == 0) || (hitCol > (SHOW_LINE_NUMBER/align)))
			return ;
		
		m_MemList.GetWindowRect(&rect);
		m_MemList.GetParent()->GetWindowRect(&item_rect);
		item_rect.left = rect.left - item_rect.left;
		item_rect.top = rect.top - item_rect.top;

        m_MemList.GetSubItemRect(hitRow, hitCol, LVIR_BOUNDS, rect);
 
		if (editItem.m_hWnd  ==  NULL) // editItem为一输入框控件， 
		{
			rect.left  +=   item_rect.left ;
			rect.top  +=   item_rect.top - 1; 
			rect.right +=   item_rect.left ;
			rect.bottom +=   item_rect.top ; 
            editItem.Create(WS_CHILD | WS_VISIBLE | WS_BORDER | ES_WANTRETURN | ES_MULTILINE, rect,  this,  IDC_EDIT_MEM_LIST);
			editItem.SetFont( this -> GetFont(), FALSE);
        }        

        editItem.SetWindowText(m_MemList.GetItemText(hitRow, hitCol));
        editItem.ShowWindow( 1 );
        editItem.SetFocus();
		editItem.SetSel(0, 8);
    }
}


static void fetchMemData(char *str, unsigned char *dataPtr, int i, int j, unsigned int align)
{
	unsigned int offset;
	unsigned int val;
	
	offset = i * SHOW_LINE_NUMBER + j * align;

	if(align == 4)
	{
		val = dataPtr[offset + 3];
		val = (val << 8) + dataPtr[offset + 2];
		val = (val << 8) + dataPtr[offset + 1];
		val = (val << 8) + dataPtr[offset + 0];

		sprintf(str, "%08x", val);
	}
	else if(align == 2)
	{
		val = dataPtr[offset + 1];
		val = (val << 8) + dataPtr[offset + 0];

		sprintf(str, "%04x", val);
	}
	else
	{
		val = dataPtr[offset];
		sprintf(str, "%02x", val);
	}
}

static void mem2Ascii(char *str, unsigned char *dataPtr, int i)
{
	unsigned int offset;
	unsigned int val, m;
	unsigned char tmp[18];
	
	offset = i * SHOW_LINE_NUMBER;

	memset(tmp, 0, SHOW_LINE_NUMBER);
	memcpy(tmp, &dataPtr[offset], SHOW_LINE_NUMBER);

	for(m=0; m<SHOW_LINE_NUMBER; m++)
	{
		if(tmp[m] == 0)
			tmp[m] = ' ';
	}
	tmp[SHOW_LINE_NUMBER] = 0;
	sprintf(str, "%s", tmp);
}

void hs_debug::refreshMemoryData(void)
{
	unsigned int address, length, align;
	int i, j, line;
	char str[32];
	unsigned char *dataPtr;

	m_dbgEditAddr.GetWindowTextA(str, 9);
	string_s2b(str);
	sscanf_s(str,"%x",&address);

	m_dbgEditLength.GetWindowTextA(str, 9);
	string_s2b(str);
	sscanf_s(str,"%x",&length);

	m_dbgCombAlign.GetWindowTextA(str, 9);
	string_s2b(str);
	sscanf_s(str,"%x",&align);

	dataPtr = (unsigned char *)malloc(length + SHOW_LINE_NUMBER);
	memset(dataPtr, 0, length + SHOW_LINE_NUMBER);

	/* get data */
	GetMemData(dataPtr, address, length);

	line = (length + SHOW_LINE_NUMBER - 1) / SHOW_LINE_NUMBER;
	for(i=0; i<(SHOW_LINE_NUMBER/align); i++)
	{
		sprintf_s(str, "%02x", i*align);
		string_s2b(str);
		m_MemList.InsertColumn( i+1, str, LVCFMT_CENTER, (27-align) * align );
	}

	m_MemList.InsertColumn( i+1, "ASCII", LVCFMT_LEFT, 150);

	for(i=0; i<line; i++)
	{
		sprintf_s(str, "%08x", (address + (i * 0x10)));
		string_s2b(str);
		m_MemList.InsertItem(i, str);

		for(j=1; j<=(SHOW_LINE_NUMBER/align); j++)
		{
			fetchMemData(str, dataPtr, i, j-1, align);
			string_s2b(str);
			m_MemList.SetItemText(i, j, str);
		}	

		mem2Ascii(str, dataPtr, i);
		m_MemList.SetItemText(i, j, str);
	}

	free(dataPtr);

}


void hs_debug::OnBnClickedButtonDebugRefreash()
{
	m_MemList.DeleteAllItems();	

	for(int i=1; i<=SHOW_LINE_NUMBER+1; i++)
		m_MemList.DeleteColumn(1);

	refreshMemoryData();
}



void hs_debug::OnEnKillfocusEditDebugLength()
{
	// TODO: Add your control notification handler code here
}

#define GET_MEM_NOCE_LEN		0x40
void hs_debug::GetMemData(unsigned char *buf, unsigned int address, int length)
{
	unsigned char once_len;
	unsigned char input[10];
	unsigned short cmd;
	char str[256];
	int res, type;

	cmd = COMM_CMD_MEMORY_READ;
	while(1)
	{
		if(length >= GET_MEM_NOCE_LEN)
			once_len = GET_MEM_NOCE_LEN;
		else
			once_len = length;

		input[0] = address & 0xff;
		input[1] = (address >> 8) & 0xff;
		input[2] = (address >> 16) & 0xff;
		input[3] = (address >> 24) & 0xff;

		res = getCmdHandler()->SendCmd(cmd, input, 4, buf, once_len);
		if(res != 0)
		{
			sprintf_s(str, "Read memory(address:%08x, length:%x) error.\n Error Code: %d", address, once_len, res);
			MessageBox(_T(str), _T("Read memory error"), MB_ICONEXCLAMATION|MB_OK);	
			return ;
		}

		buf += once_len;
		address += once_len;
		length -= once_len;

		if(length <= 0)
			break;
	};
}


void hs_debug::OnBnClickedButtonDebugEraseData()
{
	unsigned int address, length;
	char str[256];
	unsigned char input[20], buf[20];
	int res;

	m_dbgEditAddr.GetWindowTextA(str, 9);
	string_s2b(str);
	sscanf_s(str,"%x",&address);

	m_dbgEditLength.GetWindowTextA(str, 9);
	string_s2b(str);
	sscanf_s(str,"%x",&length);

	input[0] = address & 0xff;
	input[1] = (address >> 8) & 0xff;
	input[2] = (address >> 16) & 0xff;
	input[3] = (address >> 24) & 0xff;

	input[4] = length & 0xff;
	input[5] = (length >> 8) & 0xff;
	input[6] = (length >> 16) & 0xff;
	input[7] = (length >> 24) & 0xff;

	if((address & 0xf0000000) != FLASH_MEM_BASE)
	{
		sprintf_s(str, "Erase memory error.\n Address:%08x is not flash memory area!", address);
		MessageBox(_T(str), _T("Erase memory error"), MB_ICONEXCLAMATION|MB_OK);	
		return ;
	}

	res = getCmdHandler()->SendCmd(COMM_CMD_MEMORY_ERASE, input, 8, buf, 0);
	if(res != 0)
	{
		sprintf_s(str, "Erase memory(address:%08x, length:%x) error.\n Error Code: %d", address, length, res);
		MessageBox(_T(str), _T("Erase memory error"), MB_ICONEXCLAMATION|MB_OK);	
		return ;
	}

	OnBnClickedButtonDebugRefreash();
}


void hs_debug::OnEnChangeEditDebugAddr()
{
	unsigned int address;
	char str[32];

	m_dbgEditAddr.GetWindowTextA(str, 9);
	string_s2b(str);
	sscanf_s(str,"%x",&address);

	if((address & 0xf0000000) == FLASH_MEM_BASE)
	{
		hs_bnEraseMem.EnableWindow(1);
	}
	else
	{
		hs_bnEraseMem.EnableWindow(0);
	}
}
