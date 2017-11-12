#pragma once
#include "afxwin.h"
#include "comm_cmd.h"
#include "afxcmn.h"

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

#define HS_CFG_BT_DEV			0x10      //  the name of device
#define HS_CFG_BT_HC			0x20      //  the address of device
#define BT_NAME_SIZE			0x10

#define KEY_ADDR                  (0x80000000 + (1024*1024) - CFG_BLOCK_SIZE)


//  发.送状态
typedef struct
{
	unsigned char *pBufPtr;
	unsigned int fileLen;
}hs_trans_para_s;

// 配.置文件信息
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

// 操.作界面的信息收集
typedef struct
{
	CString w_imagefile;
	CString w_configfile;
	CString w_outputfile;
	CString w_packfile;
	CString w_addrfile;

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

//  pack打.包文件头信息结构
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

// bin.文件信息结构
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


// pack下.载信息记录
typedef struct
{
	uint32_t  pack_version;
	
	char      pack_bt_address_org[256];
	uint32_t  pack_addr_change_mod;
	uint32_t  pack_addr_loop_num;
	uint32_t  pack_addr_loop_position;
	char      w_packAddrFile[DIR_PATCH_LENGTH];
	uint32_t  addr_offset;

	char      pack_bt_name_org[256];
	uint32_t  pack_name_change_mod;
	uint32_t  pack_name_loop_num;
	uint32_t  pack_name_loop_position;
	char      w_packNameFile[DIR_PATCH_LENGTH];
	uint32_t  name_offset;
}pack_load_record_t;


// 对.话框信息
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
	char w_imageFile[DIR_PATCH_LENGTH];
	char w_configCurDir[DIR_PATCH_LENGTH];
	char w_configFile[DIR_PATCH_LENGTH];
	char w_packCurDir[DIR_PATCH_LENGTH];
	char w_outpackFile[DIR_PATCH_LENGTH];
	char w_addrCurDir[DIR_PATCH_LENGTH];
	char w_addrFile[DIR_PATCH_LENGTH];

	//////////////////////////////////////////////////////////////////////////
	uint32_t serialport;
	char     baudrate[10];
	char     w_packFileDir[DIR_PATCH_LENGTH];
	char     w_packFile[DIR_PATCH_LENGTH];
	uint32_t packChkEn;

	uint32_t bt_address_change_mode;
	char     bt_address[256];
	char     bt_name[256];
	char     bt_loop_change_num[256];

	bool     have_config_file;

	// for the config of pack_download
	pack_load_record_t pack_load_record;
	//////////////////////////////////////////////////////////////////////////

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
	//////////////////////////////////////////////////////////////////////////
	bool only_addr_mode_change;
	bool new_config_file;
	comm_cmd hs_comm;
	char uartDevList[10][32];
	bool bConnectFlg;
	bool bTransFlg;
	//CString hs_file;

	DWORD dwThreadId[2];
	HANDLE hThread[2];

	hs_backcall_para hs_status;
	int enTransRes;
	hs_trans_para_s hs_stTransPara;
	CComboBox hs_combUartDevice;
	CEdit hs_editBaudrate;

	CEdit hs_editFileName;
	CButton hs_bnSelectFile;

	CButton hs_bnDownload;
	CButton hs_CheckPackNeed;
	CProgressCtrl hs_s_statusProcess;
	CEdit hs_s_statusFileSize;
	CEdit hs_s_statusTotalCnt;
	CEdit hs_s_statusSuccessCnt;
	CEdit hs_s_statusTotalErrCnt;
	CEdit hs_s_statusLastErrCnt;
	CEdit hs_s_statusLastErrIdx;

	//////////////////////////////////////////////////////////////////////////
	CArray <CString, CString&> m_BatchData;
	HSOption hs_option;	
	dlg_option p_option ;
	hs_packinfo_t hs_packinfo;
	hs_imginfo_t  *hs_pImgInfo;
	pack_load_record_t hs_pack_load_info_init;
	
	void ParseStr(char * str, int *argc, char *argv[]);
	void getPara(char ** str);
	void PromptError(char *ptr);
	CString GetValidString(LPCSTR chInput);
	char CompileSentence(CString strLine);
	void compileConfig(void);
	void configDataUpdate(uint32_t cfg_addr);
	int getBtConfigFlashAddr(void);

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
	void UartConnect(bool connect_or_not);
	void Download_Start();
	void Download_Stop();
	void saveDlg(void);
	int restoreDlg(void);

	void compileConfigPos(uint32_t cfg_addr);
	void readConfig(void);
	void DoTone(void);
	
	afx_msg void OnBnClickedButtonGetPackFile();
	afx_msg void OnBnClickedCheckSPack();
	CButton hs_getImagefile;
	CButton hs_outPackfile;


	CEdit hs_editGetAddrFile;
	CButton hs_bnGetAddrFile;
	afx_msg void OnBnClickedButtonGetAddrFile();
	CEdit hs_editShowAddress;
	CEdit hs_editShowAddrMode;
	CEdit hs_editShowName;
	CEdit hs_editShowAddrOrg;
	CEdit hs_editShowPackVersion;
	afx_msg void OnBnClickedButton2();
	CEdit hs_editCfgFlashAddr;
};
