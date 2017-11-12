#ifndef __PLAT_HS6601__
#define __PLAT_HS6601__

#include <stdint.h>

#define SF_CMD_WREN          0x06000000
#define SF_CMD_RDSTAS        0x05000000
#define SF_CMD_WRSTAS        0x01000000
#define SF_CMD_FTRD          0x0b000000
#define SF_CMD_RD            0x03000000
#define SF_CMD_PP            0x02000000
#define SF_CMD_SE            0x20000000
#define SF_CMD_BE            0xd8000000
#define SF_CMD_CE            0xc7000000
#define SF_CMD_RDID          0x9f000000
#define SF_CMD_DEEPPD        0xB9000000
#define SF_CMD_RELEASEPD     0xAB000000

#define SF_DATA_CNT(len)     ((len) << 16)
#define SF_CMD_WIDTH(bits)   ((bits) << 8)
#define SF_CMD_KEEPCS        (0u  << 6)
#define SF_CMD_READ          1
#define SF_CMD_WRITE         2

#define FLASH_PPPAGE_SIZE    256

#define XCLK_CLOCK_16M       16000000
#define PLL_CLOCK_192M       192000000

#define RAM_BASE_ADDR        0x10000000
#define FLASHROM_BASE_ADDR   0x80000000

#define  SF_DEFAULT_CLOCK    8000000


#define     __I                       /*!< defines 'read only' permissions      */
#define     __O                       /*!< defines 'write only' permissions     */
#define     __IO                      /*!< defines 'read / write' permissions   */


typedef struct
{
  __I  uint32_t INTR_STATUS;
  __IO uint32_t RAW_INTR_STATUS;
  __IO uint32_t INTR_MASK;
  __IO uint32_t COMMAND;
  __IO uint32_t COMMAND_DATA0_REG;
  __IO uint32_t COMMAND_DATA1_REG;
  __IO uint32_t READ0_REG;
  __IO uint32_t READ1_REG;
  __IO uint32_t ADDRESS_REG;
  __IO uint32_t READ_OPCODE_REG;
  __IO uint32_t CONFIGURATION_0;
  __IO uint32_t CS_CONFIGURATION_0;
  __IO uint32_t CONFIGURATION_1;
  __IO uint32_t CS_CONFIGURATION_1;
  __IO uint32_t CONFIGURATION_2;
  __IO uint32_t CS_CONFIGURATION_2;
  __IO uint32_t CONFIGURATION_3;
  __IO uint32_t CS_CONFIGURATION_3;
} HS_SF_Type;

typedef struct
{
  __IO uint32_t BASIC;
  __IO uint32_t PSO_PM_CON;
  __IO uint32_t RAM_PM_CON[10];
  __IO uint32_t POWER_PIN; //0x30
  __IO uint32_t x34;
  __IO uint32_t SCRATCH; //PM_CON0
  __IO uint32_t REMAP;
  __IO uint32_t PADC_CON[32]; //0x40 0xbc
  __IO uint32_t GPIO_STATUS[2]; //0xc0 0xc4
  __IO uint32_t GPIO_PL_UP_30K; //pull up 30ohm for gpio[23:16], i.e. SD
  uint32_t xcc;
  __IO uint32_t OSC_SEL; //0-16MHz 1-32MHz
  uint8_t xd4_xf7[0xf8-0xd4];
  __IO uint32_t SCRATCH1;//PM_CON1
  __IO uint32_t ANA_CON; //system LDO & PLL
} HS_PMU_Type;

typedef struct
{
  __I  uint32_t REV;
  __IO uint32_t CPU_CFG;
  __IO uint32_t REG_UPD;
  __IO uint32_t APB_CFG;
  __IO uint32_t SFLASH_CFG;
  __IO uint32_t USB_CFG;
  __IO uint32_t SD_CFG;
  __IO uint32_t SD_DRV_CFG;
  __IO uint32_t TIM0_CFG;
  __IO uint32_t TIM1_CFG;
  __IO uint32_t TIM2_CFG;
  __IO uint32_t x2c;
  __IO uint32_t UART0_CFG;
  __IO uint32_t UART1_CFG;
  __IO uint32_t I2C0_CFG;
  __IO uint32_t I2S_CFG;
  __IO uint32_t x40;
  __IO uint32_t CODEC_CFG;
  __IO uint32_t x48;
  __IO uint32_t BTPHY_CFG;
  __IO uint32_t SPI0_CFG;
  __IO uint32_t SPI1_CFG;
  __IO uint32_t WDT_CFG;
  __IO uint32_t CPU_TCLK_CFG;
  __IO uint32_t x60;
  __IO uint32_t x64;
  __IO uint32_t x68;
  __IO uint32_t x6c;
  __IO uint32_t AHB_GATE_CFG;
  __IO uint32_t CPM_GATE_CFG;
 } HS_PSO_CPM_Type;

typedef struct
{
  __I  uint32_t REV;
  __IO uint32_t UPD;
  __IO uint32_t APB_CFG;
  __IO uint32_t RTC_CFG;
  __IO uint32_t CRY_CFG;
  __IO uint32_t PLL_SRC_CFG;
} HS_PMU_CPM_Type;

typedef struct
{
  __IO uint32_t CR;
  __IO uint32_t TORR;
  __I  uint32_t CCVR;
  __O  uint32_t CRR;
  __I  uint32_t STAT;
  __I  uint32_t EOI;
       uint32_t   Reserved0;
  __IO uint32_t CLOCK_EN;	
} HS_WDT_Type;

typedef struct
{
  __IO uint32_t RX_STR_EN;                     //offset address 0x0000
  __IO uint32_t XCORR_MOD_SEL;
  __IO uint32_t BLE_XCORR_TH;
  __IO uint32_t FSYNC_DET_SEL_IDX;
  __IO uint32_t CFO_FB_CNT_TH_SEL;             //offset address 0x0010    
  __IO uint32_t CFO_CAP_TH;
  __IO uint32_t EN_CFO_EST;
  __IO uint32_t CFO_FOR_DPSK_SEL;
  __IO uint32_t CFO_CAP_FSYNC;                 //offset address 0x0020
  __IO uint32_t RX_TOP_CFO_MODE;                
  __IO uint32_t H_IDX;
  __IO uint32_t TX_FILT_GAIN_CMP;
  __IO uint32_t EN_GFSK_24X_SAT;               //offset address 0x0030        
  __IO uint32_t TX_EDR2_DLY_SEL;
  __IO uint32_t EN_DC_REMOVAL;
  __IO uint32_t DCNOTCH_K_SEL;                 
  __IO uint32_t RES_32X_EN;                    //offset address 0x0040
  __IO uint32_t ERR_SCALE_SEL;
  __IO uint32_t K1_GFSK_SEL;
  __IO uint32_t K2_GFSK_SEL;
  __IO uint32_t K1_DPSK_SEL;                   //offset address 0x0050
  __IO uint32_t K2_DPSK_SEL;
  __IO uint32_t K2_BYPASS;
  __IO uint32_t SW_VLD_DLY_SEL;
  __IO uint32_t SW_WIN_LTH_OFFSET;             //offset address 0x0060
  __IO uint32_t SW_WIN_HTH_OFFSET;
  __IO uint32_t DEM_DLY_BKOFF_GFSK;
  __IO uint32_t DEM_DLY_BKOFF_DPSK;
  __IO uint32_t N_EXTRA_OFFSET_SEL;            //offset address 0x0070 
  __IO uint32_t N_FTUNE_GFSK_SEL;
  __IO uint32_t N_FTUNE_DPSK_SEL;
  __IO uint32_t XCORR_TH_GFSK_SEL;
  __IO uint32_t XCORR_TH_DPSK_SEL;             //offset address 0x0080   
  __IO uint32_t XCORR_TH_BLE_SEL;
  __IO uint32_t CNT2TH;
  __IO uint32_t K_FIX0_2_TIMEOUT_SL;
  __IO uint32_t x90;                           //offset address 0x0090
  __IO uint32_t x94;
  __IO uint32_t FBFB_ACC_FIX_FSYNC;
  __IO uint32_t RX_FE_CFO_EST_K;
  __IO uint32_t CHF_INI_SEL;                   //offset address 0x00A0
  __IO uint32_t EN_CFO_CMP;
  __IO uint32_t CFO_CMP_DIS_LTH;
  __IO uint32_t CFO_CMP_DIS_HTH;
  __IO uint32_t EN_FAGC;                       //offset address 0x00B0
  __IO uint32_t LWIN;
  __IO uint32_t RX_FAGC_REF;
  __IO uint32_t GFAGC_REG;
  __IO uint32_t GFAGC_WEN;                     //offset address 0x00C0
  __IO uint32_t SNR_EST;
  __IO uint32_t RSSI_TIMEOUT_CNST;
  __IO uint32_t RSSI_SAVE_MODE;                 
  __IO uint32_t RSSI_K_SEL;                    //offset address 0x00D0
  __IO uint32_t RSSI_EST_SEL;
  __IO uint32_t RSSI_EST_REAL_TIME;
  __IO uint32_t SIG_DBM_EST;
  __IO uint32_t K_CHG_INI0;                    //offset address 0x00E0  
  __IO uint32_t K_CHG_INI1;
  __IO uint32_t K_DCY_INI0;
  __IO uint32_t K_DCY_INI1;
  __IO uint32_t MAXMIN0_LIM;                   //offset address 0x00F0
  __IO uint32_t MAXMIN1_LIM;
  __IO uint32_t DC_LIM;
  __IO uint32_t DCY_COND_TH_SEL;
  __IO uint32_t VAR_DCY_FSYNC_EN;              //offset address 0x0100
  __IO uint32_t K_CHG_LIM;
  __IO uint32_t K_DCY_LIM;
  __IO uint32_t CNT_CHG_TH_SEL;
  __IO uint32_t CNT_DCY_TH_SEL;                //offset address 0x0110      
  __IO uint32_t FINE_DCY_SEL;
  __IO uint32_t DC_LPF_SEL;
  __IO uint32_t REF_TH_SEL;
  __IO uint32_t K_DCY_CNT_EN;                  //offset address 0x0120
  __IO uint32_t K_DCY_REF_EN;
  __IO uint32_t K_DCY_COND_EN;
  __IO uint32_t K_CHG_CNT_EN;
  __IO uint32_t K_CHG_REF_EN;                  //offset address 0x0130
  __IO uint32_t K_CHG_COND_EN;
  __IO uint32_t K_SLOPE_SEL;
  __IO uint32_t K_DC_LPF;
  __IO uint32_t VAR_DCY_SLOPE0;                //offset address 0x0140
  __IO uint32_t VAR_DCY_SLOPE1;
  __IO uint32_t VAR_DX_EN_RANGE0;
  __IO uint32_t VAR_DX_EN_RANGE1;
  __IO uint32_t VAR_DCY0_EN;                   //offset address 0x0150
  __IO uint32_t VAR_DCY1_EN;
  __IO uint32_t NOISE_DET_EN;
  __IO uint32_t MAXMIN_LIM_SEL;
  __IO uint32_t RESERVED0;                     //offset address 0x0160
  __IO uint32_t TX_EDR3_DLY_SEL;
  __IO uint32_t GAU_DLY_CNT;
  __IO uint32_t GUARD_DLY_CNT;
  __IO uint32_t TX_EXT_SEL;                    //offset address 0x0170
  __IO uint32_t TX_RX_EN;
  __IO uint32_t RESERVED1;
  __IO uint32_t BT_PHY_ID;
  __IO uint32_t RESERVED2[3];                  //offset address 0x0180
  __IO uint32_t RX_GAINC1; 
  __IO uint32_t RX_GAINC2;                     //offset address 0x0190
  __IO uint32_t RX_GAINC3;
  __IO uint32_t RX_RFAGC1;
  __IO uint32_t RX_RFAGC2;
  __IO uint32_t RX_RFAGC3;                     //offset address 0x1A0
  __IO uint32_t RX_RFAGC4;
  __IO uint32_t RX_IFAGC1;
  __IO uint32_t RX_IFAGC2;
  __IO uint32_t RX_IFAGC3;                     //offset address 0x01B0  
  __IO uint32_t RX_IFAGC4;
  __IO uint32_t RX_IFAGC5;
  __IO uint32_t RX_IFAGC6;
  __IO uint32_t RX_IFAGC7;                     //offset address 0x01C0
  __IO uint32_t RX_IFAGC8;
  __IO uint32_t DIS_PLD_CFO_CMP;
  __IO uint32_t EN_ANTI_INTF;
  __IO uint32_t MAXMIN0_LIM2;                  //offset address 0x01D0
  __IO uint32_t MAXMIN1_LIM2;
  __IO uint32_t CLR_TH;
  __IO uint32_t EN_CLR;
  __IO uint32_t DEM_DLY_BKOFF_GFSK2;           //offset address 0x01E0
  __IO uint32_t RX_EN_STR_ST1_WRT;
  __IO uint32_t EN_CFO_ACC_CLR;
  __IO uint32_t EN_CFO_CLR_AFTER_SYNC;
  __IO uint32_t CFO_ACC_SAT_LIM;               //offset address 0x01F0
  __IO uint32_t DCNOTCH_K2;
  __IO uint32_t DC_MODE;
  __IO uint32_t STR_XCORR_CUT;
  __IO uint32_t TH_SEL;                        //0x200
  __IO uint32_t ADC_CLR_DUR_SEL;
  __IO uint32_t SAT_CNT_TH_SEL;
  __IO uint32_t CNT_MODE;
  __IO uint32_t A_REG;                         //0x210
  __IO uint32_t C_REG;
  __IO uint32_t PD_CLR_DUR_SEL;
  __IO uint32_t DIN_SEL;
  __IO uint32_t PD_DET_MODE;                   //0x220
  __IO uint32_t BUF_L_SEL;
  __IO uint32_t TH_ANALOG;
  __IO uint32_t BLE_XCORR_WIN_TH;
  __IO uint32_t GFSK_XCORR_WIN_TH;             //0x230
  __IO uint32_t DPSK_XCORR_WIN_TH;
  __IO uint32_t IF_REG;
  __IO uint32_t IQ_IN_SWAP;
  __IO uint32_t FM_MODE;                       //0x240
  __IO uint32_t FM_RSSI;
  __IO uint32_t RMV2_DEEM;
  __IO uint32_t FM_FAGC;
  __IO uint32_t FM_CHHC_FILT;                  //0x250
  __IO uint32_t FM_LRHC_FILT;
  __IO uint32_t SPI_APB_SWITCH;
  __IO uint32_t CFO_EST_ACC_TRUNC;
  __IO uint32_t FSYNC_EARLY;                   //0x260
  __IO uint32_t CFO_CAP_FOR_DPSK;
  __IO uint32_t TX_IQ_MUX;
  __IO uint32_t FM_CORDIC_BP;
  __IO uint32_t TX_IF_REG;                     //0x270
  __IO uint32_t FM_DECIMATION;
  __IO uint32_t x278;
  __IO uint32_t IQ_COM_SINGLE;
  __IO uint32_t RXI_OFFSET1;                   //0x280
  __IO uint32_t RXI_OFFSET2;
  __IO uint32_t RXI_OFFSET3;
  __IO uint32_t RXQ_OFFSET1;
  __IO uint32_t RXQ_OFFSET2;                   //0x290
  __IO uint32_t RXQ_OFFSET3;
  uint8_t x298_x3ff[0x400-0x298];

  __IO uint32_t ANALOGUE[0x100];               //offset address: 0x400-0x7FF
  
  __IO uint32_t ADC_INTR;                      //offset address: 0x800, adc: 0x200
  __IO uint32_t ADC_INTR_MASK;
  __IO uint32_t ADC_CR;
  __IO uint32_t ADC_CFG;
  __IO uint32_t ADC_CHSELR;                    //offset address: 0x810, adc: 0x204 
  __IO uint32_t TESTCTRL0;
  __IO uint32_t TESTCTRL1;
  __IO uint32_t TESTCTRL2;
  __IO uint32_t TESTCTRL3;                     //offset address: 0x820, adc: 0x208 
  __IO uint32_t TESTCTRL4;
  __IO uint32_t TESTCTRL5;
  __IO uint32_t TESTCTRL6;
  __IO uint32_t TESTCTRL7;                     //offset address: 0x830, adc: 0x20C
  __IO uint32_t CHK0_DATA;
  __IO uint32_t CHK1_DATA;
  __IO uint32_t CHK2_DATA;
  __IO uint32_t CHK3_DATA;                     //offset address: 0x840, adc: 0x210 
  __IO uint32_t CHK4_DATA;
  __IO uint32_t CHK5_DATA;
  __IO uint32_t CHK6_DATA;
  __IO uint32_t TESTCFG;                       //offset address: 0x850, adc: 0x214
  __IO uint32_t CHK0_TIME;
  __IO uint32_t CHK1_TIME;
  __IO uint32_t CHK2_TIME;
  __IO uint32_t CHK3_TIME;                     //offset address: 0x860, adc: 0x218
  __IO uint32_t CHK4_TIME;
  __IO uint32_t CHK5_TIME;
  __IO uint32_t CHK6_TIME;
  __IO uint32_t TIMER_SEL;                     //offset address: 0x870, adc: 0x21C
  __IO uint32_t TESTDATA_REG;                  //offset address: 0x874, adc: 0x21D
} HS_BTPHY_Type;



#define CPM_PSO_BASE          ((uint32_t)0x40001000)
#define CPM_PMU_BASE          ((uint32_t)0x40002000)
#define SF_BASE               ((uint32_t)0x42800000)
#define WDT_BASE              ((uint32_t)0x400D0000)
#define PMU_BASE              ((uint32_t)0x400E0000)
#define BTPHY_BASE            ((uint32_t)0x40020000)


#define HS_PSO                ((HS_PSO_CPM_Type *) CPM_PSO_BASE)
#define HS_PMU_CPM            ((HS_PMU_CPM_Type *) CPM_PMU_BASE)
#define HS_WDT                ((HS_WDT_Type *) WDT_BASE)
#define HS_PMU                ((HS_PMU_Type *) PMU_BASE)
#define HS_BTPHY              ((HS_BTPHY_Type *) BTPHY_BASE)













unsigned int platform_init(void);
uint32_t sf_read_status(void);
void sf_write_status(uint32_t status);
void sf_write_enable(void);
void sf_setflash_wren(void);
uint32_t sf_read_mid(void);
void sf_chip_erase(void);
void sf_block_erase(uint32_t offset);
void sf_sector_erase(uint32_t offset);
void sf_page_write(uint32_t offset, unsigned char *wrbuf, int len);


#endif

