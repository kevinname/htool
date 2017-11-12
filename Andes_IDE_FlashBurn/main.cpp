#include <sys/time.h>
#ifdef __MINGW32__
#include <winsock2.h>  // for Sleep()
#endif
#include "util.h"
#include "flash-rom.h"

#define DEF_CONNECT_HOST      "127.0.0.1"
#define DEF_CONNECT_PORT      2354
// TODO: select target-specific flash driver
#if HSSF_BURN
extern flash_dev flash_MD;
#else
extern flash_dev flash_Micron;
extern flash_dev flash_IntelJ3;
#endif
flash_dev *gpFlash;

int nds32_preserve(unsigned int *pFlashAddr, unsigned char **pDataStart, unsigned int *pDataSize);
int nds32_erase(unsigned int FlashAddr, unsigned int DataSize);
int nds32_burn(unsigned int FlashAddr, unsigned char *pDataStart, unsigned int DataSize);
int nds32_verify(unsigned int FlashAddr, unsigned char *pDataStart, unsigned int DataSize);
int nds32_usage(void);
int Record_content(unsigned int address, unsigned char* buffer, unsigned int size);
int Verification(unsigned int address, unsigned char* start, unsigned int size);
extern unsigned int platform_init(void);

unsigned int guiCtrlBase = 0;
unsigned int guiMode16MB = 0;
/* In fast mode, it will skip some checks during burning */
unsigned int guiFastMode = 0;
/* Test only */
unsigned int guiWithMultiout = 0;
/* Measure performance - burning time */
unsigned int guiMeasureTime = 0;
/* Log file descriptor */
FILE *log = stdout;

/* option structure */
static struct option long_option[] =
{
  {"help", no_argument, 0, 'h'},
  {"preserve", no_argument, 0, 'e'},
  {"reset-target", no_argument, 0, 't'},
  {"reset-and-run", no_argument, 0, 't'},
  {"reset-hold", no_argument, 0, 'o'},
  {"reset-and-hold", no_argument, 0, 'o'},
  {"target", required_argument, 0, 'x'},
  {"lock", no_argument, 0, 'z'},
  {"unlock", no_argument, 0, 'y'},
  {"verify", no_argument, 0, 'v'},
  {"verify-only", no_argument, 0, 'c'},
  {"source", no_argument, 0, 's'},
  {"version", no_argument, 0, 'V'},
  {"fast", no_argument, 0, 'g'},
  {"erase-all", no_argument, 0, 'u'},
  {"force", no_argument, 0, 'F'},
  {"log", required_argument, 0, 'l'},
  {"host", required_argument, 0, 'H'},
  {"port", required_argument, 0, 'p'},
  {"addr", required_argument, 0, 'a'},
  /*{"flash", required_argument, 0, 'f'},*/
  {"base", required_argument, 0, 'b'},
  {"image", required_argument, 0, 'i'},
  {"measure-time", no_argument, 0, 'm'},
  {"test-multiout", no_argument, 0, 'T'},
  {0, 0, 0, 0}
};

/* main function 
 *  . parse parameters
 *  . initial socket
 *  . reset AICE 
 *  . issue RESET_HOLD to let target board restore to reset status 
 *    avoid boot code to damage flash registers' settings 
 *  . call target-dependent initial function and calculate flash base address (Device Address) 
 *     xc5()/p24()/ag101()/ag301p() 
 *  . read image binary data from --image specified file 
 *  . if user specifies --verify-only, verify the target with the image data and exit 
 *  . preserve first/last blocks if needed (As start/end addresses are not block-aligned)
 *  . erase flash
 *  . burn image - call Flash_BurnImage()
 *  . verify    */
int main(int argc, char **argv)
{
  int c = 0;
  char host[64] = DEF_CONNECT_HOST;     /* default connecting host */
  unsigned int port = DEF_CONNECT_PORT; /* default connecting port */
  //unsigned int base = 0;                /* flash base address */
  unsigned int address = 0;             /* default write to flash base */
  unsigned int size;                    /* image size */
  unsigned int erase_size;              /* erase size */
  unsigned char *start = NULL;          /* image pointer */
  FILE *image = NULL;                   /* image to burn */
  char cmd = 0;
  char *name = new char[1024];
  int specify_base = 0;
  //int specify_address = 0;
  int preserve = 0, verify = 0;
  int version_only = 0, verify_only = 0, erase_all = 0;
  int lock = 0, unlock = 0;
  int Ret;

  /* parameter handler */
  while(1)
  {
    int option_index;
    c = getopt_long(argc, argv, "ugvcVtoehzyTm:l:x:k:H:p:a:b:f:i", long_option, &option_index);
    if (c == EOF)
      break;

    switch (c)
    {
      case 'u':
        erase_all = 1;
        break;
      case 'z':
        lock = 1;
      break;
      case 'g':
        guiFastMode = 1;
      break;
      case 'y':
        unlock = 1;
      break;
      case 'v':
        verify = 1;
        break;
      case 'c':
        verify_only = 1;
        break;
      case 'l':
        if ((log = fopen(optarg, "w"))== NULL) {
          fprintf (log, "Error: can't open file %s\n", optarg);
          fflush(log);
          exit(1);
        }
        break;
      case 's':
        //fprintf (log, "Branch%s\n", BRANCH_NAME);
        //fprintf (log, "%s\n", COMMIT_ID);
        fflush(log);
        exit(0);
      case 'V':
        version_only = 1;
        break;
      case 'x':
        if (strncmp(optarg, "ag101p_16mb", 6) == 0)
            guiMode16MB = 1;
        break;
      case 't':
        cmd = RESET_TARGET;
        break;
      case 'o':
        cmd = RESET_HOLD;
        break;
      case 'e':
        preserve = 1;
        break;
      case 'H':
        strcpy(host, optarg);
        break;
      case 'p':
        port = strtol(optarg, NULL, 0);
        break;
      case 'a':
        sscanf(optarg,"0x%x", &address);
        //specify_address = 1;
        break;
      case 'b':
        sscanf(optarg,"0x%x", &specify_base);
        break;
      case 'i':
        if (strncmp(optarg, "/cygdrive/", 10) == 0)
        {
          name[0] = optarg[10];
          name[1] = ':';
          name[2] = '\0';
          strcat(name, optarg + 11);
        }
        else
          name = optarg;
        if ((image = fopen(name, "rb"))== NULL)
        {
          fprintf (log, "Error: can't open file %s\n", optarg);
          fflush(log);
          exit(1);
        }
        break;
      case 'm':
        guiMeasureTime = 1;
        break;
      case 'T':
        guiWithMultiout = 1;
        break;
      case 'h':
      case '?':
      default:
        nds32_usage();
        exit(0);
        break;
    }  /* switch (c)  */
  }  /* while(1)  */

  fprintf (log, "NDS32 Burner BUILD_ID: %d\n", BUILD_ID);
  fflush(log);

  if(version_only)
  {
    exit(0);
  }

  initial_socket(host, port);

#ifdef __MINGW32__
  Sleep (200);
#else
  usleep (200000);
#endif

  // init AICE
  if (send_cmd(RESET_AICE) < 0)
  {
    close_connection ();
    exit(0);
  }

  if (cmd != 0)
  {
    send_cmd (cmd);
    close_connection ();
    exit(0);
  }

  // reset-and-hold to init target
  send_cmd (RESET_HOLD);
  platform_init();

  /* error check */
  /*
  if (specify_address == 0)
  {
    fprintf(log, "ERROR!! please add parameter --addr \n");
    fflush(log);
    terminate ();
    return 0;
  }*/
  if (flash_MD.flash_chk() != 0)
  {
    fprintf(log, "ERROR!! flash type error \n");
    fflush(log);
    terminate ();
    return 0;
  }
  gpFlash = (flash_dev *)&flash_MD;

  /* get burn image content */
  start = get_image (image, &size);
  fclose (image);

  /* handle signal */
  signal(SIGTERM, handle_int);
  signal(SIGINT, handle_int);
#ifndef __MINGW32__
  signal(SIGKILL, handle_int);
#endif

  if(size > gpFlash->flash_chipsize)
  {
    printf("ERROR!! image size(%d) > ROM size(%d)!!\n", size, gpFlash->flash_chipsize);
    return 0;
  }
  else
  {
    fprintf (log, "image size: %d bytes\n", size);
    fflush(log);
  }

  /* verify only */
  if (verify_only)
  {
    nds32_verify(address, start, size);
    terminate ();
    return 0;
  }

  /* unlock flash */
  if (unlock)
  {
    fprintf(log, "unlocking");
    fflush(log);
    gpFlash->flash_unlock(address, size);
    fprintf(log, "\n");
    fflush(log);
  }
  /* record the content in first and last erase block */
  if (preserve)
    nds32_preserve(&address, &start, &size);
  /*
  fprintf(log, "address=%x, start=%x, size=%x \n", address, (unsigned int)start, size);
  fflush(log);
  */

  /* block erase */
  if (erase_all)
    erase_size = gpFlash->flash_chipsize;
  else
    erase_size = size;
  Ret = nds32_erase(address, erase_size);
  if (Ret != 0)
  {
    terminate();
    return 0;
  }

  /* burn image */
  nds32_burn(address, start, size);

  /* verify */
  verify=0;
  if (verify)
  {
    Ret = nds32_verify(address, start, size);
    if (Ret == 0)
      fprintf(log, "Flash burning done.\n");
    else
      fprintf(log, "Please reburn again.\n");
  }
  else
  {
    fprintf(log, "Flash burning done.\n");
  }
  fflush(log);

  /* lock flash */
  if (lock)
  {
    fprintf(log, "locking");
    fflush(log);
    gpFlash->flash_lock(address, size);
    fprintf(log, "\n");
    fflush(log);
  }
  /* send terminate signal */
  terminate();
  return 0;
}

int nds32_preserve(unsigned int *pFlashAddr, unsigned char **pDataStart, unsigned int *pDataSize)
{
  unsigned int OriFlashAddr, OriDataSize;
  unsigned int NewFlashAddr, NewDataSize, UnalignBytes;
  unsigned int FirstBlkSize, LastBlkSize;
  unsigned char *pNewDataStart, *pOriDataStart, *pCurrData;

  OriFlashAddr = *pFlashAddr;
  OriDataSize = *pDataSize;
  pOriDataStart = *pDataStart;

  NewFlashAddr = (OriFlashAddr / gpFlash->flash_blksize) * gpFlash->flash_blksize;
  FirstBlkSize = (OriFlashAddr - NewFlashAddr);
  UnalignBytes = (OriDataSize % 4);
  if(UnalignBytes)
    LastBlkSize = (4 - UnalignBytes);
  else
    LastBlkSize = 0;
  NewDataSize = OriDataSize + FirstBlkSize + LastBlkSize;

  pNewDataStart = (unsigned char *) malloc (NewDataSize);
  pCurrData = pNewDataStart;
  /* copy the 1st blk from flash */
  if(FirstBlkSize)
    Record_content(NewFlashAddr, pCurrData, FirstBlkSize);
  pCurrData += FirstBlkSize;
  
  /* copy ori-image file */
  memcpy(pCurrData, pOriDataStart, OriDataSize);
  pCurrData += OriDataSize;

  /* copy last unalign bytes from flash */
  if(LastBlkSize)
    Record_content(OriFlashAddr + OriDataSize, pCurrData, LastBlkSize);
  
  *pFlashAddr = NewFlashAddr;
  *pDataSize = NewDataSize;
  *pDataStart = pNewDataStart;
  free (pOriDataStart);
  return 0;
}

int nds32_erase(unsigned int FlashAddr, unsigned int DataSize)
{
  struct timeval erase_begin_time;
  struct timeval erase_end_time;
  struct timeval erase_total_time;
  double erase_time_sec;
  int Ret;
    
  fprintf(log, "erase from address = 0x%x\n", FlashAddr);
  fflush(log);

  if (guiMeasureTime)
    gettimeofday (&erase_begin_time, NULL);

  Ret = gpFlash->flash_erase(FlashAddr, DataSize);

  if (guiMeasureTime)
  {
    gettimeofday (&erase_end_time, NULL);

    timeval_diff (&erase_total_time, &erase_begin_time, &erase_end_time);
    erase_time_sec = erase_total_time.tv_sec + (double)erase_total_time.tv_usec / 1000000;

    fprintf(log, "\nTotal erase time: %lu sec, %lu usec\n", 
        erase_total_time.tv_sec, erase_total_time.tv_usec);
    fprintf(log, "Average erase rate: %.2lf KBytes/s\n", (double)(DataSize >> 10) / erase_time_sec);
    fflush(log);
  }
  return Ret;    
}

int nds32_burn(unsigned int FlashAddr, unsigned char *pDataStart, unsigned int DataSize)
{
  struct timeval burn_begin_time;
  struct timeval burn_end_time;
  struct timeval burn_total_time;
  double burn_time_sec;
  int Ret;
  
  fprintf(log, "burn flash from 0x%x to 0x%x\n", FlashAddr ,FlashAddr+DataSize);
  fflush(log);
  fprintf(log, "burning\n");
  fflush(log);
  
  if (guiMeasureTime)
    gettimeofday (&burn_begin_time, NULL);

  Ret = gpFlash->flash_program(FlashAddr, pDataStart, DataSize);

  if (guiMeasureTime)
  {
    gettimeofday (&burn_end_time, NULL);

    timeval_diff (&burn_total_time, &burn_begin_time, &burn_end_time);
    burn_time_sec = burn_total_time.tv_sec + (double)burn_total_time.tv_usec / 1000000;

    fprintf(log, "\n\nTotal burn time: %lu sec, %lu usec\n", 
        burn_total_time.tv_sec, burn_total_time.tv_usec);
    fprintf(log, "Average burn rate: %.2lf KBytes/s\n", (double)(DataSize >> 10) / burn_time_sec);
    fflush(log);
  }
  if (Ret == 0)
  {
    fprintf(log, "\nburn success.\n");
    fflush(log);
  }
  else
  {
    fprintf(log, "\nburn fail !!!\n");
    fflush(log);
    close_connection ();
    exit (0);
  }
  return Ret;
}

int nds32_verify(unsigned int FlashAddr, unsigned char *pDataStart, unsigned int DataSize)
{
  struct timeval verify_begin_time;
  struct timeval verify_end_time;
  struct timeval verify_total_time;
  double verify_time_sec;
  int Ret;

  fprintf(log, "verifying\n");
  fflush(log);

  if (guiMeasureTime)
    gettimeofday (&verify_begin_time, NULL);

  Ret = Verification(FlashAddr, pDataStart, DataSize);

  if (guiMeasureTime)
  {
    gettimeofday (&verify_end_time, NULL);

    timeval_diff (&verify_total_time, &verify_begin_time, &verify_end_time);
    verify_time_sec = verify_total_time.tv_sec + (double)verify_total_time.tv_usec / 1000000;

    fprintf(log, "\n\nTotal verify time: %lu sec, %lu usec\n",
        verify_total_time.tv_sec, verify_total_time.tv_usec);
    fprintf(log, "Average verify rate: %.2lf KBytes/s\n", (double)(DataSize >> 10) / verify_time_sec);
    fflush(log);
  }

  if (Ret == 0)
    fprintf(log, "\nVerify success.\n");
  else
    fprintf(log, "\nVerify fail !!!\n");

  fflush(log);
  return Ret;
}

/* Help menu */
int nds32_usage(void)
{
  printf( "--host:\t\t\tHost name/ip to connect with ICEman\n");
  printf( "--port:\t\t\tPort number to connect with ICEman (default to 2354)\n");
  printf( "--base:\t\t\tFlash base address (default to 0x0)\n");
  printf( "--addr:\t\t\tFlash target address to write (default to 0x0)\n");
  printf( "--image:\t\tImage name to burn\n");
  printf( "--preserve:\t\tPreserve the content in first and last erase blocks\n");
  printf( "--log:\t\t\tThe log file to store output message (default to stdout)\n");
  printf( "--reset-and-run:\tReset target\n");
  printf( "--reset-and-hold:\tReset target and stop at $IVB\n");
#if 0
  printf( "--target:\t\tBoard type to find ROM start address (xc5/ag101p_16mb)\n");
#endif  
  printf( "--verify:\t\tVerify after flash burning\n");
  printf( "--verify-only:\t\tUse image file to verify content of ROM, no burn\n");
  printf( "--version:\t\tShow flash burning version\n");
  printf( "--unlock:\t\tUnlock flash before burning\n");
  printf( "--lock:\t\t\tLock flash after burning\n");
#if HSSF_BURN
#else
  printf( "--fast:\t\t\tBurning flash in fast mode\n");
#endif  
  printf( "--erase-all:\t\tErase entire flash before burning\n");
  printf( "--help:\t\t\tThe usage for NDS32 Burner\n");
  printf( "--measure-time:\t\tEstimate total time to burn image to ROM\n");
  return 0;
}

/* record the flash memory content to buffer */
int Record_content(unsigned int address, unsigned char* buffer, unsigned int size)
{
  unsigned char content[2048];
  unsigned int buffer_size, i, j = 0;

  while (size > 0)
  {
    if (size > 1024)
      buffer_size = 1024;
    else
      buffer_size = size;

    /* fastin(address, buffer_size, content); */
    gpFlash->flash_read(address, &content[0], buffer_size);
    for (i = 0; i < buffer_size; i++)
    {
      buffer[j++] = (unsigned char)content[i];
    }
    size = size - buffer_size;
    address = address + buffer_size;
  }
  return 0;
}

/* verify after burn */
/* return 1 if varify fail */
static unsigned char content[2048];
int Verification(unsigned int address, unsigned char* start, unsigned int size)
{
  unsigned int buffer_size, i;

  while (size > 0)
  {
    if (size > 1024)
      buffer_size = 1024;
    else
      buffer_size = size;
    /* fastin(address, buffer_size, content); */
    gpFlash->flash_read(address, &content[0], buffer_size);
    for (i = 0; i < buffer_size; i++)
    {
        if ((unsigned char)content[i] != *(start++))
        {
            start--;
            printf("Verification: ERROR i=%x, address=%x, start=%x content=%x \n", i, address, *start, content[i]);
            return 1;
        }
    }
    size = size - buffer_size;
    address = address + buffer_size;

    if (address % 4096 == 0)
    {
      fprintf(log, ".");
      fflush(log);
    }
  }
  return 0;
}
