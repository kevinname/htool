#include <sys/time.h>
#ifdef __MINGW32__
#include <winsock2.h>  // for Sleep()
#endif
#include "platform-hs6601.h"
#include "util.h"
#include "flash-rom.h"



#define FLASH_BLKSIZE             (4*1024)
#define FLASH_CHIPSIZE            (FLASH_BLKSIZE * 256)
#define FLASH_RETRY_TIMES         100

#define SF_BLOCK_SIZE             0x10000
#define SF_SECTOR_SIZE            0x1000 
#define SF_PAGE_SIZE              0x100  
#define SF_PAGEPROG_ADDR_MASK     0xFFFFFF00


int md_check(void);
int md_program(unsigned int FlashAddr, unsigned char *start, unsigned int DataSize);
int md_erase(unsigned int FlashAddr, unsigned int DataSize);
int md_read(unsigned int FlashAddr, unsigned char *start, unsigned int DataSize);
int md_lock(unsigned int FlashAddr, unsigned int DataSize);
int md_unlock(unsigned int FlashAddr, unsigned int DataSize);

flash_dev flash_MD = {
    md_check,
    md_erase,
    md_program,
    md_read,
    md_lock,
    md_unlock,
    FLASH_BLKSIZE,
    FLASH_CHIPSIZE
};

extern FILE *log;
//extern unsigned int guiFastMode;

int md_check(void)
{
    unsigned int RetData;

    fprintf(log, "md_check() run!\n");
	fflush(log);

    RetData = sf_read_mid();
    if ((RetData == 0) || (RetData == 0xffffff))
    {
        fprintf(log, "ERROR: read spi flash id fail:0x%08x \n", RetData);
        return 1;
    }
    
    fprintf(log, "MD: Flash ID = 0x%08x \n", RetData);
	fflush(log);
    return 0;
}

// TODO: modify this function to target-specific function - erase flash
int md_erase(unsigned int FlashAddr, unsigned int DataSize)
{
    unsigned int EraseAddrStart, EraseSize, EraseSectorCnt, EraseSectorIndex, i;

    fprintf(log, "md_erase() run!\n");
    fflush(log);

#if 1
    EraseAddrStart = (FlashAddr / SF_BLOCK_SIZE) * SF_BLOCK_SIZE;
    EraseSize = (FlashAddr - EraseAddrStart) + DataSize;
    EraseSectorCnt = (EraseSize + (SF_BLOCK_SIZE - 1)) / SF_BLOCK_SIZE;
    EraseSectorIndex = (EraseAddrStart / SF_BLOCK_SIZE);

    for (i = 0; i < EraseSectorCnt; i++)
    {
        
        fprintf(log, "erasing block %03d (0x%06x ~ 0x%06x)\n", EraseSectorIndex, EraseAddrStart, EraseAddrStart + SF_BLOCK_SIZE);
        fflush(log);

        /*-- erase --*/
        sf_block_erase(EraseAddrStart);
        
        EraseAddrStart += SF_BLOCK_SIZE;
        EraseSectorIndex++;
    }
#else
    EraseAddrStart = (FlashAddr / SF_SECTOR_SIZE) * SF_SECTOR_SIZE;
    EraseSize = (FlashAddr - EraseAddrStart) + DataSize;
    EraseSectorCnt = (EraseSize + (SF_SECTOR_SIZE - 1)) / SF_SECTOR_SIZE;
    EraseSectorIndex = (EraseAddrStart / SF_SECTOR_SIZE);

    for (i = 0; i < EraseSectorCnt; i++)
    {
        
        fprintf(log, "erasing block %03d (0x%06x ~ 0x%06x)\n", EraseSectorIndex, EraseAddrStart, EraseAddrStart + SF_SECTOR_SIZE);
        fflush(log);

        /*-- erase --*/
        sf_sector_erase(EraseAddrStart);
        
        EraseAddrStart += SF_SECTOR_SIZE;
        EraseSectorIndex++;
    }
#endif
    return 0;
}


// TODO: modify this function to target-specific function - program flash
int md_program(unsigned int FlashAddr, unsigned char *start, unsigned int DataSize)
{
    unsigned int PageCnt, addr, i, lines = 0;

    fprintf(log, "md_program() run!\n");
	fflush(log);

    if(FlashAddr & (~SF_PAGEPROG_ADDR_MASK))
    {
        fprintf(log, "md_program: addr is NOT page-align!!");
        fflush(log);
        return 1;
    }
    PageCnt = (DataSize + (SF_PAGE_SIZE - 1)) / SF_PAGE_SIZE;
    
    /*---------------------------*/
    /*-- PAGE PROGRAM procedure  */
    /*---------------------------*/
    for (i = 0; i < PageCnt; i++)
    {
        addr = FlashAddr + (i * SF_PAGE_SIZE); 
        if (i < (PageCnt - 1))
          sf_page_write(addr, start, SF_PAGE_SIZE);
        else
          /* avoid SEGV when write the last page */
          sf_page_write(addr, start, DataSize - SF_PAGE_SIZE * (PageCnt - 1));

        if((i%(4*32)) == 0)
          fprintf(log, " %dKB\n", 32*lines++); //32KB each line
        if((i%4) == 0)
        fprintf(log, ".");
        fflush(log);
        start += SF_PAGE_SIZE;
    }  
    
    return 0;
}

// TODO: modify this function to target-specific function - read flash
int md_read(unsigned int FlashAddr, unsigned char *start, unsigned int DataSize)
{
    unsigned int *dptr = (unsigned int *)start;
  
    FlashAddr += FLASHROM_BASE_ADDR;
    DataSize = (DataSize + 3) / 4;
    while(DataSize--)
    {
      *dptr = inw(FlashAddr);
      dptr ++;
      FlashAddr += 4;
    }

    dptr = (unsigned int *)start;
    fprintf(log, "md_read: 0x%08X\n", dptr[0]);
    fflush(log);
    
    return 0;
}

// TODO: modify this function to target-specific function - lock flash
int md_lock(unsigned int FlashAddr, unsigned int DataSize)
{
    return 0;
}
// TODO: modify this function to target-specific function - unlock flash
int md_unlock(unsigned int FlashAddr, unsigned int DataSize)
{
    return 0;
}

