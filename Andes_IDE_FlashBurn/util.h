/*******************************************************************************
 * File name: util.h
 * Description: Flash Burner
 * Author: Shiva
 * Revision history:
 * 2011/05/23	Create this file.
 * ****************************************************************************/
#ifndef __UTIL__
#define __UTIL__

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <signal.h>
#include <sys/types.h>
#include <getopt.h>

#ifdef __MINGW32__
#include <winsock2.h>
#undef socklen_t
#define socklen_t int
#define sleep(t) _sleep(t * 1000)
#else
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <netdb.h>
#include <arpa/inet.h>
#endif

#ifdef __MINGW32__
#define    SEND(SOCKET, DATA, SIZE) send(SOCKET, DATA, SIZE, 0);
#define    RECV(SOCKET, DATA, SIZE) recv(SOCKET, DATA, SIZE, 0);
#else
#define    SEND(SOCKET, DATA, SIZE) write(SOCKET, DATA, SIZE);
#define    RECV(SOCKET, DATA, SIZE) read(SOCKET, DATA, SIZE);
#endif

#define MAX_MULTI_WRITE_PAIR  256

// Command code
#define WRITE_WORD   (0x1A)
#define READ_WORD    (0x1B)
#define WRITE_BYTE   (0x2A)
#define READ_BYTE    (0x2B)
#define WRITE_HALF   (0x4A)
#define READ_HALF    (0x4B)
#define FAST_READ    (0x1C)
#define WRITE_IO     (0x1F)
#define FAST_WRITE   (0x20)
#define BURNER_QUIT  (0x04)
#define MULTIPLE_WRITE_WORD (0x5A)
#define MULTIPLE_WRITE_HALF (0x5B)
#define MULTIPLE_WRITE_BYTE (0x5C)
#define MULTIPLE_READ_WORD  (0x5D)
#define MULTIPLE_READ_HALF  (0x5E)
#define MULTIPLE_READ_BYTE  (0x5F)
#define RESET_TARGET (0x3A)
#define RESET_HOLD   (0x3B)
#define RESET_AICE   (0x3C)
#define HOLD_CORE    (0x1D)

/* type define */
typedef unsigned long long      UINT64;
typedef long long               INT64;
typedef unsigned int            UINT32;
typedef int                     INT32;
typedef unsigned short          UINT16;
typedef short                   INT16;
typedef unsigned char           UINT8;
//typedef char                    INT8;


/* read/write API */
int outw(unsigned int address, unsigned int data);
int outh(unsigned int address, unsigned short data);
int outb(unsigned int address, unsigned char data);
int out_io(unsigned int address, unsigned int size, char * buffer);
char inb(unsigned int address);
short inh(unsigned int address);
int inw(unsigned int address);
int fastin(unsigned int address, unsigned int size, char * buffer);
int fastout(unsigned int address, unsigned int size, char * buffer);
int multiout_w(unsigned int *address, unsigned int *data, const unsigned int number_of_pairs);
int multiout_h(unsigned int *address, unsigned short *data, const unsigned int number_of_pairs);
int multiout_b(unsigned int *address, unsigned char *data, const unsigned int number_of_pairs);
int multiin_w(unsigned int *address, unsigned int *data, const unsigned int number_of_pairs);
int multiin_h(unsigned int *address, unsigned short *data, const unsigned int number_of_pairs);
int multiin_b(unsigned int *address, unsigned char *data, const unsigned int number_of_pairs);
void close_connection (void);
void terminate(void);
void handle_int(int signo);
int send_cmd(char cmd);

void initial_socket(const char *host, unsigned int port);
/* get image from file */
unsigned char* get_image(FILE* image, unsigned int* size);
void timeval_diff (struct timeval *a_result, struct timeval *a_timeval_begin, struct timeval *a_timeval_end);

#endif

