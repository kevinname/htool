#pragma once
#include "comm_config.h"

#pragma once
#include "comm_config.h"
#include "HToolCDlg.h"

// comm_base

class comm_base : public CDialogEx
{
	DECLARE_DYNAMIC(comm_base)

public:
	comm_base();
	comm_base(UINT nIDTemplate, CWnd *pParent = NULL);
	virtual ~comm_base();

protected:
	DECLARE_MESSAGE_MAP()
public:
	comm_cmd * getCmdHandler(void);
};


