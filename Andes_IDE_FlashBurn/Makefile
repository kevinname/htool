#
# Makefile for Burner
#
BUILD_ID = "`date +%Y%m%d%H`"

C++ = g++
CC = gcc
RM = rm -f
AR = ar

CFLAGS += -m32 -g3 -O3 -Wall -DHSSF_BURN -DBUILD_ID=$(BUILD_ID) $(LIBUSB)
LIBS =


CPPFLAGS = $(CFLAGS) 

srcdir = $(shell pwd)

OBJS =	$(srcdir)/util.o \
	$(srcdir)/main.o \
	$(srcdir)/platform-hs6601.o \
	$(srcdir)/spiflash-MD.o

all: HSSF_burn.elf

HSSF_burn.elf: $(OBJS)
	$(C++) -static -o $@ $(CPPFLAGS) $(OBJS) $(LIBS)

clean:
	rm -f HSSF_burn.exe *.o

%.o: %.cpp
	$(C++) $(CPPFLAGS) -c -o $@ $<

