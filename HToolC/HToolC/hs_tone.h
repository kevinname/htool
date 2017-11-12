#pragma once
#include "afxwin.h"
#include "sbc.h"

#define CFG_TONE_4K_MAXNUM        4
#define CFG_TONE_8K_MAXNUM        20
#define CFG_TONE_16K_MAXNUM       4

#define CFG_TONE_MAXNUM			  (CFG_TONE_4K_MAXNUM + CFG_TONE_8K_MAXNUM + CFG_TONE_16K_MAXNUM)

#define CFG_TONE_INDEX			  0x0b

#define CFG_TONE_MAX_SIZE         (16 * 1024)
#define TONE_FILE_LENGTH			0x100

enum
{
  CFG_SBC_CHNMODE_MONO        = 0,
  CFG_SBC_CHNMODE_DUALCHN     ,
  CFG_SBC_CHNMODE_STEREO      ,
  CFG_SBC_CHNMODE_JOINTSTEREO ,
};

typedef struct
{
  uint32_t   u32BitRate;        /*!< sample rate    */
  uint16_t   u16ToneVolume;     /*!< tone volume    */  
  uint16_t   u16DataLen;
  uint16_t   u8LoopNum;         /*!< loop number    */
  uint16_t   u8Delay;           /*!< unit: s    */ 
  
  uint8_t    u8Blocks;          /*!<     */
  uint8_t    u8SubBands;        /*!<     */
  uint8_t    u8BitPool;         /*!<     */
  uint8_t    u8ChnMode;         /*!<     */
}hs_sound_info_t;

typedef struct
{
	//char	w_tonefile[CFG_TONE_MAXNUM][TONE_FILE_LENGTH];
	char w_curdir[TONE_FILE_LENGTH];
	char w_toneData[CFG_TONE_MAXNUM][CFG_TONE_MAX_SIZE];

	char *ptr;
	unsigned int data_ptr;
	
	hs_sound_info_t w_toneInfo[CFG_TONE_MAXNUM];
	int  createFlg[CFG_TONE_MAXNUM]; 

	int tone_enbale;

	unsigned int chksum;
}hs_tone_option_t;

// hs_tone dialog

class hs_tone : public CDialogEx
{
	DECLARE_DYNAMIC(hs_tone)

public:
	hs_tone(CWnd* pParent = NULL);   // standard constructor
	virtual ~hs_tone();

// Dialog Data
	enum { IDD = IDD_TONE_CONFIG };
	virtual BOOL OnInitDialog(void);

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	hs_tone_option_t p_option;
	//CButton CChkToneCfgEnable;
	CButton hs_chkToneEn;

	CButton bToneOk[CFG_TONE_MAXNUM], bToneOpen[CFG_TONE_MAXNUM], bToneFile[CFG_TONE_MAXNUM];
	CStatic sTxtVolume[CFG_TONE_MAXNUM], sTxtBitrate[CFG_TONE_MAXNUM], sTxtBlocks[CFG_TONE_MAXNUM], sTxtSubband[CFG_TONE_MAXNUM], sTxtBitpool[CFG_TONE_MAXNUM];
	CStatic sTxtChnmode[CFG_TONE_MAXNUM], sTxtLoopNum[CFG_TONE_MAXNUM], sTxtDelay[CFG_TONE_MAXNUM], sTxtNotice[CFG_TONE_MAXNUM], sTxtFile[CFG_TONE_MAXNUM];
	CComboBox cListBitrate[CFG_TONE_MAXNUM], cListBlocks[CFG_TONE_MAXNUM], cListSubband[CFG_TONE_MAXNUM], cListChnmode[CFG_TONE_MAXNUM];
	CEdit  eEditVolume[CFG_TONE_MAXNUM], eEditBitpool[CFG_TONE_MAXNUM],eEditLoopNum[CFG_TONE_MAXNUM], eEditDelay[CFG_TONE_MAXNUM], eEditFile[CFG_TONE_MAXNUM];

	CWnd * pCWnd[CFG_TONE_MAXNUM];
	HWND hWnd[CFG_TONE_MAXNUM]; 

	void createChildWin(int index);

	afx_msg void OnBnClickedTone0();
	afx_msg void OnBnClickedTone1();
	afx_msg void OnBnClickedTone2();
	afx_msg void OnBnClickedTone3();
	afx_msg void OnBnClickedTone4();
	afx_msg void OnBnClickedTone5();
	afx_msg void OnBnClickedTone6();
	afx_msg void OnBnClickedTone7();
	afx_msg void OnBnClickedTone8();
	afx_msg void OnBnClickedTone9();

	afx_msg void OnBnClickedTone10();
	afx_msg void OnBnClickedTone11();
	afx_msg void OnBnClickedTone12();
	afx_msg void OnBnClickedTone13();
	afx_msg void OnBnClickedTone14();
	afx_msg void OnBnClickedTone15();
	afx_msg void OnBnClickedTone16();
	afx_msg void OnBnClickedTone17();
	afx_msg void OnBnClickedTone18();
	afx_msg void OnBnClickedTone19();

	afx_msg void OnBnClickedTone20();
	afx_msg void OnBnClickedTone21();
	afx_msg void OnBnClickedTone22();
	afx_msg void OnBnClickedTone23();
	afx_msg void OnBnClickedTone24();
	afx_msg void OnBnClickedTone25();
	afx_msg void OnBnClickedTone26();
	afx_msg void OnBnClickedTone27();
	afx_msg void OnBnClickedTone28();
	afx_msg void OnBnClickedTone29();

	afx_msg void OnBnClickedTone30();
	afx_msg void OnBnClickedTone31();
	afx_msg void OnBnClickedTone32();
	afx_msg void OnBnClickedTone33();
	afx_msg void OnBnClickedTone34();
	afx_msg void OnBnClickedTone35();
	afx_msg void OnBnClickedTone36();
	afx_msg void OnBnClickedTone37();
	afx_msg void OnBnClickedTone38();
	afx_msg void OnBnClickedTone39();

	afx_msg void OnBnClickedTone40();
	afx_msg void OnBnClickedTone41();
	afx_msg void OnBnClickedTone42();
	afx_msg void OnBnClickedTone43();
	afx_msg void OnBnClickedTone44();
	afx_msg void OnBnClickedTone45();
	afx_msg void OnBnClickedTone46();
	afx_msg void OnBnClickedTone47();
	afx_msg void OnBnClickedTone48();

	void restoreDlg();
	void doToneData(void);
	
	afx_msg void OnBnClickedChkToneEn();	
};
