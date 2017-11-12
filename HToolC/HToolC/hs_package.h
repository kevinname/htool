#pragma once
#include "afxwin.h"

#define DIR_PATCH_LENGTH		0x100

#define CFG_DATA_FLAG_1         0x3CC3A55A
#define CFG_DATA_FLAG_2         0x48533636

#define CONFIG_LEN				4*1024*1024
#define MAX_PARA_COUNT			0x200

#define CFG_INDEX_MAX			250
#define CFG_INDEX_LEN			3

#define CFG_BLOCK_SIZE			(4*1024)		
#define CFG_BLOCK_ALIGN_MASK	(~(CFG_BLOCK_SIZE - 1))

#define INFO_WR_FLAG            0x31305257
#define INFO_LD_FLAG            0x3130444c
#define INFO_SIZE               sizeof(hs_packinfo_t)
#define IMGINFO_SIZE            sizeof(hs_imginfo_t)
#define IMG_PLAIN               0
#define IMG_INFO_ADDR           0x10
#define IMG_INFO_FLAG           0x01664B50

#define CFG_DEFINE_NUM			500
#define CFG_DEFINE_LEN			256

#define CFG_GPIO_NUM			26

#define ALGIN_SIZE				PROTOCOL_PACKDATA_LEN

#define HS_CFG_BT_DEV			0x10
#define HS_CFG_BT_HC			0x20
#define BT_NAME_SIZE			0x10

#define KEY_ADDR                  (0x80000000 + (1024*1024) - CFG_BLOCK_SIZE)

typedef struct 
{
  unsigned int      u32InfoFlg1;          /*!< config data valid flag             */
  unsigned int      u32InfoFlg2;          /*!< config data valid flag             */
  
  unsigned int      u32Version;           /*!< config data version                */
  unsigned int      u32Date;              /*!< the date of config data generated  */

  unsigned int      u32Local;             /*!< the local of config data           */
  unsigned int      u32IndexCnt;          /*!< config data count                  */
  unsigned int      u32BodyAddr;          /*!< the address of config data body    */

  unsigned int      u32BackupLen;         /*!< for backup write                   */

  unsigned int      u32Chksum;  
} hs_cfg_info_t;

typedef struct
{
	CString w_imagefile;
	CString w_configfile;
	CString w_outputfile;

	char *pInfoDataBuf;
	int  infoDataAreaPtr;
	int  infoDataBackPtr;

	char indexTable[CFG_BLOCK_SIZE];
	int  infoDataIndex;
	int  infoIndexData;
	int  infoMaxIndex;

	int  cfgBinLen;

	char infoManageFlg;
	hs_cfg_info_t infoManageData;
	int  manageDataPtr;

	char define[CFG_DEFINE_NUM][CFG_DEFINE_LEN];
	char defineVal[CFG_DEFINE_NUM][CFG_DEFINE_LEN];
	int  defineIndex;

	//char w_curdir[DIR_PATCH_LENGTH];
}HSOption;

enum
{
  BTADDR_CHGMODE_FIX        = 0,
  BTADDR_CHGMODE_INC        ,
  BTADDR_CHGMODE_LOOP       ,
  BTADDR_CHGMODE_FILE       ,
};

typedef enum
{
  FLASHMEM_MODE_SINGLE    = 0x03,
  FLASHMEM_MODE_DUAL      = 0xBB,
  FLASHMEM_MODE_QUAD      = 0xEB,
}flashmem_mode_t;

typedef struct 
{
  uint32_t      u32InfoFlg;          /*!< Info Flag                          */
  uint32_t      u32InfoChkSum;       /*!< Info checksum                      */
  uint32_t      u32InfoDate;         /*!< Info generate date                 */
  
  uint32_t      u32ImageLen;         /*!< Image length                       */
  uint32_t      u32CipherTxtFlg;
  
  uint32_t      u32CfgDataEn;
  uint32_t      u32BTAddrOffset;
  uint32_t      u32BTAddrChgMode;
  uint32_t      u32ChgModeArgc0;
  uint32_t      u32ChgModeArgc1;
  
  uint32_t      u32StatusIO;
  uint32_t      u32StatusIOLvl;

  uint32_t      u32ResultIO;
  uint32_t      u32ResultIOLvl;

  uint32_t      u32KeyAddr;

  uint32_t      u32Reserve[17];
} hs_packinfo_t;

typedef struct 
{
  uint32_t      u32ImgFlg;          
  uint32_t      u32Version;  
  uint32_t      u32ImageLen; 

  uint32_t      u32FmemMode;

  uint32_t      u32ClkUsed;
  uint32_t      u32SFRegCfg;
  uint32_t      u32CpmRegSf;  
  
  uint32_t      u32InfoChkSum;    
} hs_imginfo_t;

typedef struct 
{
	uint32_t imageTypeSel;
	char exeAddr[12];

	uint32_t chipStatusSel;
	uint32_t chipStatusLvlSel;
	uint32_t chipResSel;
	uint32_t chipResLvlSel;

	uint32_t flashModeSel;
	char cpmVal[12];
	char sfVal[12];

	uint32_t clkCfgChkEn;
	uint32_t ideBurnerChkEn;

	char w_imageCurDir[DIR_PATCH_LENGTH];
	char w_configCurDir[DIR_PATCH_LENGTH];
	char w_packCurDir[DIR_PATCH_LENGTH];

	unsigned int chksum;
}dlg_option;

#define IsHex(c)			( ( (c>='0')&&(c<='9') ) || ( (c>='A')&&(c<='F') ) || ( (c>='a')&&(c<='f') ) )
#define checkLabel(cmd)		( strcmp(cmd,"$index$")==0 )

class hs_package : public CDialogEx
{
	DECLARE_DYNAMIC(hs_package)

public:
	hs_package(CWnd* pParent = NULL);   // standard constructor
	virtual ~hs_package();

// Dialog Data
	enum { IDD = IDD_HS_PACKAGE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
	virtual BOOL OnInitDialog(void);

public:
	CArray <CString, CString&> m_BatchData;
	HSOption hs_option;	
	dlg_option p_option ;
	hs_packinfo_t hs_packinfo;
	hs_imginfo_t  *hs_pImgInfo;

	void ParseStr(char * str, int *argc, char *argv[]);
	void getPara(char ** str);
	void PromptError(char *ptr);
	CString GetValidString(LPCSTR chInput);
	char CompileSentence(CString strLine);
	void compileConfig(void);
	void configDataUpdate(void);

	CButton hs_chkAndesBurner;

	CComboBox hs_combImgType;
	CEdit hs_editExeAddr;

	CComboBox hs_combChipStatusGpio;
	CComboBox hs_combUpgradeResGpio;
	CComboBox hs_combChipBusyLvl;
	CComboBox hs_combUpgradeOkLvl;

	CEdit hs_editImgFile;
	CEdit hs_editCfgFile;
	CEdit hs_editOutFile;

	CComboBox hs_combBTAddrMode;
	CComboBox hs_combFmemMode;
	CEdit hs_editBTName;
	CEdit hs_editBTAddrStart;
	CEdit hs_editBTAddrEnd;
	CEdit hs_editSFClkCfgCPM;
	CEdit hs_editSFClkDiv;
	CButton hs_chkClkCfgEn;
	CButton hs_bnSelCfg;

	afx_msg void OnBnClickedButtonPackage();	
	afx_msg void OnBnClickedButtonInputImage();
	afx_msg void OnBnClickedButtonInputConfig();
	afx_msg void OnBnClickedButtonPackedBin();
	afx_msg void OnCbnSelchangeComboBtaddrChangeMode();		
	afx_msg void OnBnClickedCheckSfClockCfg();			
	afx_msg void OnCbnSelchangeComboImageType();	
	void saveDlg(void);
	int restoreDlg(void);

	void DoTone(void);
};
