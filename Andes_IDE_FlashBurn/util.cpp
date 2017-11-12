/*******************************************************************************
 * File name: util.cpp
 * Description: Intel Flash burner
 * Author: Shiva	
 * Revision history:
 * 2011/05/23	Create this file.
 * 2012/05/24   Add comments - Kai
 * ****************************************************************************/

#include "util.h"

#ifdef __MINGW32__
SOCKET sock;
#else
int sock;
#endif

extern unsigned int guiWithMultiout;
extern FILE *log;                  /* log file */

#define TARGET_ERROR "Burning failed due to target errors."

/* send command to iceman
 * command code refer to util.h */
int send_cmd(char cmd)
{
  char send_data[2],recv_data[2];
  send_data[0] = cmd;

  SEND(sock, send_data, 1);
  RECV(sock, recv_data, 2);

  if (recv_data[0] == cmd && recv_data[1] == 0) 
  {
    return int(recv_data[1]);
  }
  else 
  {
    fprintf(log, "\n!! send_cmd: %s !!\n", TARGET_ERROR);
    fflush(log);

    close_connection ();
    exit (0);
  }
}

/* write word */
/* could use outw write word to target memory */
int outw(unsigned int address, unsigned int data)
{
  char send_data[10],recv_data[2];
  send_data[0] = WRITE_WORD;
  send_data[2] = (char)((address & 0x000000FF) >> 0);
  send_data[3] = (char)((address & 0x0000FF00) >> 8);
  send_data[4] = (char)((address & 0x00FF0000) >> 16);
  send_data[5] = (char)((address & 0xFF000000) >> 24);
  send_data[6] = (char)((data & 0x000000FF) >> 0);
  send_data[7] = (char)((data & 0x0000FF00) >> 8);
  send_data[8] = (char)((data & 0x00FF0000) >> 16);
  send_data[9] = (char)((data & 0xFF000000) >> 24);

  SEND(sock, send_data, 10);
  RECV(sock, recv_data, 2);

  if (recv_data[0] == WRITE_WORD)
    return int(recv_data[1]);
  else 
  {
    fprintf(log, "\n!! outw: %s !!\n", TARGET_ERROR);
    fflush(log);

    close_connection ();
    exit (0);
  }
}

/* write byte */
/* could use outb write byte to target memory */
int outb(unsigned int address, unsigned char data)
{
  char send_data[7],recv_data[2];
  send_data[0] = WRITE_BYTE;
  send_data[2] = (char)((address & 0x000000FF) >> 0);
  send_data[3] = (char)((address & 0x0000FF00) >> 8);
  send_data[4] = (char)((address & 0x00FF0000) >> 16);
  send_data[5] = (char)((address & 0xFF000000) >> 24);
  send_data[6] = (char)data;

  SEND(sock, send_data, 7);
  RECV(sock, recv_data, 2);

  if (recv_data[0] == WRITE_BYTE)
    return int(recv_data[1]);
  else 
  {
    fprintf(log, "\n!! outb: %s !!\n", TARGET_ERROR);
    fflush(log);

    close_connection ();
    exit (0);
  }
}

/* write half */
/* could use outh write half to target memory */
int outh(unsigned int address, unsigned short data)
{
  char send_data[8],recv_data[2];
  send_data[0] = WRITE_HALF;
  send_data[2] = (char)((address & 0x000000FF) >> 0);
  send_data[3] = (char)((address & 0x0000FF00) >> 8);
  send_data[4] = (char)((address & 0x00FF0000) >> 16);
  send_data[5] = (char)((address & 0xFF000000) >> 24);
  send_data[6] = (char)(data & 0x00FF);
  send_data[7] = (char)((data & 0xFF00) >> 8);

  SEND(sock, send_data, 8);
  RECV(sock, recv_data, 2);

  if (recv_data[0] == WRITE_HALF)
    return int(recv_data[1]);
  else 
  {
    fprintf(log, "\n!! outh: %s !!\n", TARGET_ERROR);
    fflush(log);

    close_connection ();
    exit (0);
  }
}

/* could use out_io write mult-word to same address(io port) */
int out_io(unsigned int address, unsigned int size, char * buffer)
{
  char send_data[2048],recv_data[2048];
  unsigned int i, cmd_size = 2048;
  send_data[0] = 0x1f;
  send_data[2] = (char)((address & 0x000000FF) >> 0);
  send_data[3] = (char)((address & 0x0000FF00) >> 8);
  send_data[4] = (char)((address & 0x00FF0000) >> 16);
  send_data[5] = (char)((address & 0xFF000000) >> 24);
  send_data[6] = (char)((size & 0x000000FF) >> 0);
  send_data[7] = (char)((size & 0x0000FF00) >> 8);
  send_data[8] = (char)((size & 0x00FF0000) >> 16);
  send_data[9] = (char)((size & 0xFF000000) >> 24);

  for (i = 0; i < size; i++) send_data[i+12] = buffer[i];

  SEND(sock, send_data, cmd_size);
  RECV(sock, recv_data, size + 2);

  if (recv_data[0] == 0x1f) return 0;
  else 
  {
    return 0xffffffff;
  }
}

/* fast read */
/* could use fastin read mult-word from target memory */
int fastin(unsigned int address, unsigned int size, char * buffer)
{
  char send_data[10],recv_data[2048];
  unsigned int i;
  send_data[0] = FAST_READ;
  send_data[2] = (char)((address & 0x000000FF) >> 0);
  send_data[3] = (char)((address & 0x0000FF00) >> 8);
  send_data[4] = (char)((address & 0x00FF0000) >> 16);
  send_data[5] = (char)((address & 0xFF000000) >> 24);
  send_data[6] = (char)((size & 0x000000FF) >> 0);
  send_data[7] = (char)((size & 0x0000FF00) >> 8);
  send_data[8] = (char)((size & 0x00FF0000) >> 16);
  send_data[9] = (char)((size & 0xFF000000) >> 24);

  SEND(sock, send_data, 10);
  RECV(sock, recv_data, size + 2);

  for (i = 0; i < size; i++) buffer[i] = recv_data[2+i];

  if (recv_data[0] == FAST_READ) 
    return 0;
  else 
  {
    fprintf(log, "\n!! fastin: %s !!\n", TARGET_ERROR);
    fflush(log);

    close_connection ();
    exit (0);
  }
}
/* fast write */
/* could use fastout write mult-word from target memory */
int fastout(unsigned int address, unsigned int size, char * buffer)
{
/*
  if (guiWithMultiout)
  {
    // pack address/data
    unsigned int num_of_pairs = size / 4;
    unsigned int *addr_entry = (unsigned int *) malloc (size);
    unsigned int *data_entry = (unsigned int *) malloc (size);
    int result;

    for (unsigned int i = 0 ; i < num_of_pairs ; i++)
    {
      addr_entry[i] = address + i * 4;
      data_entry[i] = ((buffer[0 + i * 4] << 0) & 0x000000FF) |
                      ((buffer[1 + i * 4] << 8) & 0x0000FF00) |
                      ((buffer[2 + i * 4] << 16) & 0x00FF0000) |
                      ((buffer[3 + i * 4] << 24) & 0xFF000000);
    }

    result = multiout_w (addr_entry, data_entry, num_of_pairs);

    free (addr_entry);
    free (data_entry);

    return result;
  }
  else*/
  {
    char send_data[2048],recv_data[2];
    send_data[0] = FAST_WRITE;
    *(int *)(send_data + 2) = address;
    *(int *)(send_data + 6) = size;

    memcpy(send_data + 12, buffer, size);

    SEND(sock, send_data, size+12);
    RECV(sock, recv_data, 2);

    if (recv_data[0] == FAST_WRITE)
      return 0;
    else 
    {
      fprintf(log, "\n!! fastout: %s !!\n", TARGET_ERROR);
      fflush(log);

      close_connection ();
      exit (0);
    }
  }
}

/* multiple write */
/* could use multiout write multi-addr/word pairs from target memory */
unsigned int multi_out(unsigned int multiCmmd, unsigned int *p_address, unsigned char *p_data, const unsigned int number_of_pairs)
{
  char send_data[2048], recv_data[2];
  unsigned int i, WriteAddr=0, WriteData=0;
  unsigned int *p_int_data = (unsigned int *)p_data;
  unsigned short *p_short_data = (unsigned short *)p_data;
  unsigned char *p_char_data = (unsigned char *)p_data;

	if (number_of_pairs > MAX_MULTI_WRITE_PAIR)
		fprintf(log, "\n!! multi_out: num of pairs is too large %s !!\n", TARGET_ERROR);

  send_data[0] = multiCmmd;
  send_data[1] = number_of_pairs & 0xFF;

  for (i = 0 ; i < number_of_pairs ; i++)
  {
  	if (multiCmmd == MULTIPLE_WRITE_WORD)
  		WriteData = *p_int_data++;
  	else if (multiCmmd == MULTIPLE_WRITE_HALF)
  		WriteData = *p_short_data++;
  	else if (multiCmmd == MULTIPLE_WRITE_BYTE)
  		WriteData = *p_char_data++;

		WriteAddr = *p_address++;
    send_data[i*8 + 2] = (char)((WriteAddr & 0x000000FF) >> 0);
    send_data[i*8 + 3] = (char)((WriteAddr & 0x0000FF00) >> 8);
    send_data[i*8 + 4] = (char)((WriteAddr & 0x00FF0000) >> 16);
    send_data[i*8 + 5] = (char)((WriteAddr & 0xFF000000) >> 24);

    send_data[i*8 + 6] = (char)((WriteData & 0x000000FF) >> 0);
    send_data[i*8 + 7] = (char)((WriteData & 0x0000FF00) >> 8);
    send_data[i*8 + 8] = (char)((WriteData & 0x00FF0000) >> 16);
    send_data[i*8 + 9] = (char)((WriteData & 0xFF000000) >> 24);
  }

  SEND(sock, send_data, 2 + number_of_pairs * 8);
  RECV(sock, recv_data, 2);
	if (recv_data[0] != (char)multiCmmd) {
		fprintf(log, "\n!! multi_out: %s !!\n", TARGET_ERROR);
    fflush(log);
    close_connection ();
    exit (0);
  }
  return 0;
}

int multiout_w(unsigned int *p_address, unsigned int *p_data, const unsigned int number_of_pairs)
{
	unsigned int RetValue;

	RetValue = multi_out(MULTIPLE_WRITE_WORD, p_address, (unsigned char *)p_data, number_of_pairs);
	return (int)RetValue;
}

int multiout_h(unsigned int *p_address, unsigned short *p_data, const unsigned int number_of_pairs)
{
	unsigned int RetValue;

	RetValue = multi_out(MULTIPLE_WRITE_HALF, p_address, (unsigned char *)p_data, number_of_pairs);
  return (int)RetValue;
}

int multiout_b(unsigned int *p_address, unsigned char *p_data, const unsigned int number_of_pairs)
{
	unsigned int RetValue;

	RetValue = multi_out(MULTIPLE_WRITE_BYTE, p_address, (unsigned char *)p_data, number_of_pairs);
  return (int)RetValue;
}

/* multiple read */
unsigned int multi_in(unsigned int multiCmmd, unsigned int *p_address, unsigned char *p_data, const unsigned int number_of_pairs)
{
  char send_data[2048], recv_data[2048];
  unsigned int i, ReadAddr=0, ReadData=0;
  unsigned int *p_int_data = (unsigned int *)p_data;
  unsigned short *p_short_data = (unsigned short *)p_data;
  unsigned char *p_char_data = (unsigned char *)p_data;

	if (number_of_pairs > MAX_MULTI_WRITE_PAIR)
		fprintf(log, "\n!! multi_in: num of pairs is too large %s !!\n", TARGET_ERROR);

  send_data[0] = multiCmmd;
  send_data[1] = number_of_pairs & 0xFF;

  for (i = 0 ; i < number_of_pairs ; i++)
  {
		ReadAddr = *p_address++;
    send_data[i*4 + 2] = (char)((ReadAddr & 0x000000FF) >> 0);
    send_data[i*4 + 3] = (char)((ReadAddr & 0x0000FF00) >> 8);
    send_data[i*4 + 4] = (char)((ReadAddr & 0x00FF0000) >> 16);
    send_data[i*4 + 5] = (char)((ReadAddr & 0xFF000000) >> 24);
  }

  SEND(sock, send_data, 2 + number_of_pairs * 4);
  RECV(sock, recv_data, 2 + number_of_pairs * 4);
  
	if (recv_data[0] != (char)multiCmmd) {
		fprintf(log, "\n!! multi_in: %s !!\n", TARGET_ERROR);
  	fflush(log);
  	close_connection ();
  	exit (0);
  }

  if (multiCmmd == MULTIPLE_READ_WORD) {
  	for (i = 0 ; i < number_of_pairs ; i++) {
  		ReadData = (unsigned char)recv_data[i*4 + 2];
  		ReadData |= ((unsigned char)recv_data[i*4 + 3] << 8);
  		ReadData |= ((unsigned char)recv_data[i*4 + 4] << 16);
  		ReadData |= ((unsigned char)recv_data[i*4 + 5] << 24);
  		*p_int_data++ = ReadData;
  		//printf("MULTIPLE_READ: %x %x %x %x\n", recv_data[i*4 + 2], recv_data[i*4 + 3], recv_data[i*4 + 4], ReadData);
  	}
  }
  else if (multiCmmd == MULTIPLE_READ_HALF) {
  	for (i = 0 ; i < number_of_pairs ; i++) {
  		ReadData = (unsigned char)recv_data[i*2 + 2];
  		ReadData |= ((unsigned char)recv_data[i*2 + 3] << 8);
  		*p_short_data++ = ReadData;
  	}
  }
  else if (multiCmmd == MULTIPLE_READ_BYTE) {
  	for (i = 0 ; i < number_of_pairs ; i++) {
  		ReadData = (unsigned char)recv_data[i + 2];
  		*p_char_data++ = ReadData;
  	}
  }
  return 0;
}

int multiin_w(unsigned int *p_address, unsigned int *p_data, const unsigned int number_of_pairs)
{
	unsigned int RetValue;

	RetValue = multi_in(MULTIPLE_READ_WORD, p_address, (unsigned char *)p_data, number_of_pairs);
	return (int)RetValue;
}

int multiin_h(unsigned int *p_address, unsigned short *p_data, const unsigned int number_of_pairs)
{
	unsigned int RetValue;

	RetValue = multi_in(MULTIPLE_READ_HALF, p_address, (unsigned char *)p_data, number_of_pairs);
  return (int)RetValue;
}

int multiin_b(unsigned int *p_address, unsigned char *p_data, const unsigned int number_of_pairs)
{
	unsigned int RetValue;

	RetValue = multi_in(MULTIPLE_READ_BYTE, p_address, (unsigned char *)p_data, number_of_pairs);
  return (int)RetValue;
}

/* read byte */
/* could use inb read byte from target memory */
char inb(unsigned int address)
{
  char data;
  char send_data[6],recv_data[3];
  send_data[0] = READ_BYTE;
  send_data[2] = (char)((address & 0x000000FF) >> 0);
  send_data[3] = (char)((address & 0x0000FF00) >> 8);
  send_data[4] = (char)((address & 0x00FF0000) >> 16);
  send_data[5] = (char)((address & 0xFF000000) >> 24);

  SEND(sock, send_data, 6);
  RECV(sock, recv_data, 3);

  data = recv_data[2] & 0x000000FF;

  if (recv_data[0] == READ_BYTE)
    return data;
  else 
  {
    fprintf(log, "\n!! inb: %s !!\n", TARGET_ERROR);
    fflush(log);

    close_connection ();
    exit (0);
  }
}

/* read half */
/* could use inh read half from target memory */
short inh(unsigned int address)
{
  short data;
  char send_data[6],recv_data[4];
  send_data[0] = READ_HALF;
  send_data[2] = (char)((address & 0x000000FF) >> 0);
  send_data[3] = (char)((address & 0x0000FF00) >> 8);
  send_data[4] = (char)((address & 0x00FF0000) >> 16);
  send_data[5] = (char)((address & 0xFF000000) >> 24);

  SEND(sock, send_data, 6);
  RECV(sock, recv_data, 4);

  data = (((recv_data[2]<< 8) & 0x0000FF00) |
          ((recv_data[3]<< 0) & 0x000000FF));

  if (recv_data[0] == READ_HALF)
    return data;
  else 
  {
    fprintf(log, "\n!! inh: %s !!\n", TARGET_ERROR);
    fflush(log);

    close_connection ();
    exit (0);
  }
}

/* read word */
/* could use inw read word from target memory */
int inw(unsigned int address)
{
  int data;
  char send_data[6],recv_data[6];
  send_data[0] = READ_WORD;
  send_data[2] = (char)((address & 0x000000FF) >> 0);
  send_data[3] = (char)((address & 0x0000FF00) >> 8);
  send_data[4] = (char)((address & 0x00FF0000) >> 16);
  send_data[5] = (char)((address & 0xFF000000) >> 24);

  SEND(sock, send_data, 6);
  RECV(sock, recv_data, 6);

  data = (((recv_data[2]<< 24) & 0xFF000000) |
          ((recv_data[3]<< 16) & 0x00FF0000) |
          ((recv_data[4]<< 8) & 0x0000FF00) |
          ((recv_data[5]<< 0) & 0x000000FF));

  if (recv_data[0] == READ_WORD) 
    return data;
  else 
  {
    fprintf(log, "\n!! inw: %s !!\n", TARGET_ERROR);
    fflush(log);

    close_connection ();
    exit (0);
  }
}

/* Close connection with ICEman */
void close_connection (void)
{
  char send_data[2];

  send_data[0] = 4; // Termination command code
  SEND(sock, send_data, 1);

  // free socket resource
#ifdef __MINGW32__
  closesocket (sock);
  do
  {
    WSACleanup ();
  } while (WSAGetLastError () != WSANOTINITIALISED);
#else
  close (sock);
#endif
}

/* Need send terminate to ICEman before quit Burner */
void terminate(void)
{
#ifndef HSSF_BURN
  // reset-and-run
  send_cmd (RESET_TARGET); // If you want target to free run after burning done.
  // send_cmd (RESET_HOLD); // If you want target to hold after burning done.
#endif

  close_connection ();
}

/* SIGINT handling function */
void handle_int(int signo)
{
  terminate();
  fprintf(log, "\n!! burn not done yet !!\n");
  fflush(log);
  exit(0);
}

/* Prepare socket communication */
void initial_socket(const char *host, unsigned int port)
{
  struct sockaddr_in server_addr;  

#ifdef __MINGW32__
  WSADATA wsadata;
  if (WSAStartup(MAKEWORD(1,1), &wsadata) == SOCKET_ERROR) {
      fprintf(log, "Error creating socket.");
      fflush(log);
      fflush(stdout);
  }
#endif

#ifdef __MINGW32__
  if ((sock = socket(PF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET)
#else
  if ((sock = socket(PF_INET, SOCK_STREAM, 0)) == -1)
#endif
  {
     fprintf(log, "can't create socket\n");
     fflush(log);
     exit(1);
  }
  server_addr.sin_family = PF_INET;     
  server_addr.sin_port = htons(port);   
#ifdef __MINGW32__
  server_addr.sin_addr.s_addr = inet_addr(host);
#else
  inet_aton(host, &server_addr.sin_addr);
#endif 
  memset(&(server_addr.sin_zero),0,8); 
  if (connect(sock, (struct sockaddr *)&server_addr,
                    sizeof(struct sockaddr)) == -1)
  {
    fprintf(log, "connecting fail !!\n");
    fflush(log);
    exit(1);
  }
}

/* Read image binary data */
unsigned char* get_image(FILE* image, unsigned int* size)
{
  unsigned char* flash;
  unsigned int len;
  
  if (image == NULL)
  {
    fprintf (log, "Error: Burn image have to specify.\n");
    fflush(log);
    exit(1);
  }

  fseek(image, 0L, SEEK_END);
  (*size) = (ftell(image));
  fseek(image, 0L, SEEK_SET);

  len = ((*size) + 3) & 0xfffffffc;;

  if ((flash = (UINT8*)malloc(len * (sizeof(UINT8)))) == NULL) {
     fprintf(log, "Error: can't allocate memory (%d bytes) for flash file\n", len);
     fflush(log);
     return 0;
  }
  fread(flash, (*size), 1, image);
  
  (*size) = len; 
  
  return flash;
}

/* Performance utility functions - to calculate the difference of a_timeval_begin and a_timeval_end */
void timeval_diff (struct timeval *a_result, struct timeval *a_timeval_begin, struct timeval *a_timeval_end)
{
  if (a_timeval_end->tv_usec >= a_timeval_begin->tv_usec)
  {
    a_result->tv_usec = (a_timeval_end->tv_usec - a_timeval_begin->tv_usec);
    a_result->tv_sec = (a_timeval_end->tv_sec - a_timeval_begin->tv_sec);
    if (a_result->tv_usec >= 1000000)
    {
      a_result->tv_usec -= 1000000;
      a_result->tv_sec += 1;
    }
  }
  else
  {
    a_result->tv_usec = (a_timeval_end->tv_usec + (1000000 - a_timeval_begin->tv_usec));
    a_result->tv_sec = (a_timeval_end->tv_sec - a_timeval_begin->tv_sec - 1);
  }
}
