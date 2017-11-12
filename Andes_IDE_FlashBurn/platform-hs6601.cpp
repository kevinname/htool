#include "platform-hs6601.h"
#include "util.h"


extern FILE *log;

#define SF_CS_SELECT			0

#define DIV_ROUND(n,d)        (((n) + ((d) / 2)) / (d))
#define sf_inw(reg)           inw((uint32_t)(&(reg)))
#define sf_outw(reg, data)    outw((uint32_t)(&(reg)), (data))



void delay_nops(uint16_t cnt)
{
  while (cnt-- > 0) ;
}


/*===========================================*/
/*   SPI driver                              */
/*===========================================*/
HS_SF_Type *g_pSFD;

void sf_wait(void)
{
  uint32_t t=0x10000;
  
  while(((sf_inw(g_pSFD->RAW_INTR_STATUS) & 1) == 0) && (t--)) ;

  sf_outw(g_pSFD->RAW_INTR_STATUS, sf_inw(g_pSFD->RAW_INTR_STATUS));
}

uint32_t sf_read_status(void) 
{
  uint32_t status, cmd_r = 0;

  cmd_r |= SF_CMD_WIDTH(16);
  cmd_r |= (SF_CS_SELECT & 3) << 4;
  cmd_r |= SF_CMD_READ;
  cmd_r |= SF_CMD_KEEPCS;

  sf_outw(g_pSFD->COMMAND_DATA0_REG, SF_CMD_RDSTAS);
  sf_outw(g_pSFD->COMMAND, cmd_r);

  sf_wait();

  status = sf_inw(g_pSFD->READ0_REG) & 0xff;
  return status;   
}

void sf_write_status(uint32_t status) 
{
  uint32_t cmd_r = 0, cmd_data = 0;

  cmd_r |= SF_CMD_WIDTH(16);
  cmd_r |= (SF_CS_SELECT & 3) << 4;
  cmd_r |= SF_CMD_WRITE;
  cmd_r |= SF_CMD_KEEPCS;

  cmd_data = SF_CMD_WRSTAS;
  cmd_data |= (status & 0xff) << 16;

  sf_outw(g_pSFD->COMMAND_DATA0_REG, cmd_data);
  sf_outw(g_pSFD->COMMAND, cmd_r);  

  sf_wait();
}

void sf_write_enable(void) 
{
  uint32_t cmd_r = 0;

  cmd_r |= SF_CMD_WIDTH(8);
  cmd_r |= (SF_CS_SELECT & 3) << 4;
  cmd_r |= SF_CMD_WRITE;
  cmd_r |= SF_CMD_KEEPCS;

  sf_outw(g_pSFD->COMMAND_DATA0_REG, SF_CMD_WREN);
  sf_outw(g_pSFD->COMMAND, cmd_r);

  sf_wait();
}


void sf_setflash_wren(void) 
{
  uint32_t status;

  status = sf_read_status();
  sf_write_status((status&0xc3)|2);
 
  while(1) 
  {
    sf_write_enable();
    
    status = sf_read_status();
    if((status & 0x1f) == 2) 
    {
      break;
    }
    
    if((status & 0x1c) != 0) 
	  {
      sf_write_status(0x83);
    }
  }
}

uint32_t sf_read_mid(void) 
{
  uint32_t mid, cmd_r = 0; 

  cmd_r |= SF_CMD_WIDTH(32);
  cmd_r |= (SF_CS_SELECT & 3) << 4;
  cmd_r |= SF_CMD_READ;
  cmd_r |= SF_CMD_KEEPCS;

  sf_outw(g_pSFD->COMMAND_DATA0_REG, SF_CMD_RDID);
  sf_outw(g_pSFD->COMMAND, cmd_r);

  sf_wait();

  mid = sf_inw(g_pSFD->READ0_REG) & 0xffffff;

  return mid;   
}

void sf_chip_erase(void) 
{
  uint32_t status, cmd_r = 0;
  
  sf_setflash_wren();
  
  cmd_r |= SF_CMD_WIDTH(8);
  cmd_r |= (SF_CS_SELECT & 3) << 4;
  cmd_r |= SF_CMD_WRITE;
  cmd_r |= SF_CMD_KEEPCS;

  sf_outw(g_pSFD->COMMAND_DATA0_REG, SF_CMD_CE);
  sf_outw(g_pSFD->COMMAND, cmd_r);

  sf_wait();

  while(1) {
    status = sf_read_status();
    if((status & 1) == 0) {
      break;
    }
  }     
}

/* 64KB Block Erase */
void sf_block_erase(uint32_t offset) 
{
  uint32_t status, cmd_r = 0, cmd_d = 0;
  
  sf_setflash_wren();

  cmd_r |= SF_CMD_WIDTH(32);
  cmd_r |= (SF_CS_SELECT & 3) << 4;
  cmd_r |= SF_CMD_WRITE;
  cmd_r |= SF_CMD_KEEPCS;

  cmd_d |= SF_CMD_BE;
  cmd_d |= offset & 0xffffff;

  sf_outw(g_pSFD->COMMAND_DATA0_REG, cmd_d);
  sf_outw(g_pSFD->COMMAND, cmd_r);  
  
  sf_wait();

  while(1) {
    status = sf_read_status();
    
    if((status & 1) == 0) {
      break;
    }
  }     
}

void sf_sector_erase(uint32_t offset) 
{
  uint32_t status, cmd_r = 0, cmd_d = 0;
  
  sf_setflash_wren();

  cmd_r |= SF_CMD_WIDTH(32);
  cmd_r |= (SF_CS_SELECT & 3) << 4;
  cmd_r |= SF_CMD_WRITE;
  cmd_r |= SF_CMD_KEEPCS;

  cmd_d |= SF_CMD_SE;
  cmd_d |= offset & 0xffffff;

  sf_outw(g_pSFD->COMMAND_DATA0_REG, cmd_d);
  sf_outw(g_pSFD->COMMAND, cmd_r);  
  
  sf_wait();

  while(1) {
    status = sf_read_status();
    
    if((status & 1) == 0) {
      break;
    }
  }     
}

void sf_page_write(uint32_t offset, unsigned char *wrbuf, int len) 
{
  uint32_t ram, status, cmd_r = 0, cmd_d = 0;
  int words, i, page_size = FLASH_PPPAGE_SIZE;
  unsigned int *dptr = (unsigned int *)wrbuf;

  if(len > page_size) 
  {
    return ;
  }

#if 1
  fastout(RAM_BASE_ADDR, len, (char *)wrbuf);
#else
  //multiout_b(pRam, wrbuf, len);
  ram = RAM_BASE_ADDR;
  words = len / 4; 
  for(i=0; i<words; i++)
  {
    outw(ram, dptr[i]);
    ram += 4;
  }
#endif
  
  sf_setflash_wren();
  
  cmd_r |= SF_DATA_CNT(len);
  cmd_r |= SF_CMD_WIDTH(32);
  cmd_r |= (SF_CS_SELECT & 3) << 4;
  cmd_r |= SF_CMD_WRITE;
  cmd_r |= SF_CMD_KEEPCS;

  cmd_d |= SF_CMD_PP;
  cmd_d |= offset & 0xffffff;

  sf_outw(g_pSFD->COMMAND_DATA0_REG, cmd_d);
  sf_outw(g_pSFD->ADDRESS_REG, RAM_BASE_ADDR);  
  sf_outw(g_pSFD->COMMAND, cmd_r);

  sf_wait();

  while(1) 
  {
    status = sf_read_status();
    if((status & 1) == 0) 
    {
      break;
    }
  }   
}


/*===========================================*/
/*   SPI driver                              */
/*===========================================*/

void cpm_reset_pll(void) 
{
  uint32_t temp;
  
  /* 1. reset pll */
  temp = sf_inw(HS_PMU->BASIC);
  temp &= ~(1u<<1);
  temp |= (1u<<30);
  sf_outw(HS_PMU->BASIC,  temp);  
  delay_nops(32);
	
  /* 2. power on pll: pd_sys_pll = 0 */
  temp = sf_inw(HS_PMU->BASIC);
  temp &= ~(1u<<0);
  temp |= (1u<<30);
  sf_outw(HS_PMU->BASIC,  temp);  

  /* 3. not reset, MUST BE delay >5us */
  delay_nops(32*6);
  temp = sf_inw(HS_PMU->BASIC);
  temp |= (1u<<1);
  temp |= (1u<<30);
  sf_outw(HS_PMU->BASIC,  temp);  
}

void cpm_start_pll(void) 
{
  uint32_t temp, i=20000;

  temp = sf_inw(HS_PMU->ANA_CON);
  temp &= ~(1<<25);
  sf_outw(HS_PMU->ANA_CON,  temp);  

  delay_nops(1*32);

  temp = inw(0x4000F010);

  temp |= (1u <<9);
  outw(0x4000F010,  temp);

  delay_nops(32*200);
  
  temp &= ~(1<<9);
  outw(0x4000F010,  temp); 
  
  delay_nops(32*2*200);
  
  while(i--) {
    if ((sf_inw(HS_PMU->BASIC) & 0x80000000) == 0x80000000)
       break;
  }
}


unsigned int platform_init(void)
{
	uint32_t temp;

  fprintf(log, "platform_init start\n");
  fflush(log);
	
  g_pSFD = (HS_SF_Type *)SF_BASE;
  
  temp = sf_inw(HS_PMU->BASIC);

  temp |= (1u<<3);
  temp |= (1u<<30);
  sf_outw(HS_PMU->BASIC,  temp);  

  temp = (DIV_ROUND(XCLK_CLOCK_16M, SF_DEFAULT_CLOCK) << 8) | 0x1d;  
  sf_outw(HS_PSO->SFLASH_CFG, temp);
  sf_outw(HS_PSO->REG_UPD, 0x01);

  sf_outw(g_pSFD->INTR_MASK, 0x00000000);
  sf_outw(g_pSFD->CONFIGURATION_0, 0x2);

  return 0;
}


