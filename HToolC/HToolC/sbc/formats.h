/*
 *
 *  Bluetooth low-complexity, subband codec (SBC) library
 *
 *  Copyright (C) 2004-2009  Marcel Holtmann <marcel@holtmann.org>
 *
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 *
 */


#pragma once

#ifndef __FORMATS_H
#define __FORMATS_H

#ifdef __cplusplus
extern "C" {
#endif

#include "stdint.h"

uint16_t bswap_16(uint16_t x)  
{  
    return (((uint16_t)(x) & 0x00ff) << 8) | \
		(((uint16_t)(x) & 0xff00) >> 8) ;  
}  
uint32_t bswap_32(uint32_t x)  
{  
    return (((uint32_t)(x) & 0xff000000) >> 24) | \
		(((uint32_t)(x) & 0x00ff0000) >> 8) | \
		(((uint32_t)(x) & 0x0000ff00) << 8) | \
		(((uint32_t)(x) & 0x000000ff) << 24) ;  
}  

#define __BYTE_ORDER __LITTLE_ENDIAN
#if __BYTE_ORDER == __LITTLE_ENDIAN
#define COMPOSE_ID(a,b,c,d)	((a) | ((b)<<8) | ((c)<<16) | ((d)<<24))
#define LE_SHORT(v)		(v)
#define LE_INT(v)		(v)
#define BE_SHORT(v)		bswap_16(v)
#define BE_INT(v)		bswap_32(v)
#elif __BYTE_ORDER == __BIG_ENDIAN
#define COMPOSE_ID(a,b,c,d)	((d) | ((c)<<8) | ((b)<<16) | ((a)<<24))
#define LE_SHORT(v)		bswap_16(v)
#define LE_INT(v)		bswap_32(v)
#define BE_SHORT(v)		(v)
#define BE_INT(v)		(v)
#else
#error "Wrong endian"
#endif

#define AU_MAGIC		COMPOSE_ID('.','s','n','d')

#define AU_FMT_ULAW		1
#define AU_FMT_LIN8		2
#define AU_FMT_LIN16		3

typedef struct{
	char riff_sig[4];            	// 'RIFF'
	long waveform_chunk_size;    	// 8
	char wave_sig[4];            	// 'WAVE'
	char format_sig[4];          	// 'fmt ' (notice space after)
	long format_chunk_size;      	// 16;
	short format_tag;             // WAVE_FORMAT_PCM
	short channels;               // # of channels
	long sample_rate;            	// sampling rate
	long bytes_per_sec;          	// bytes per second
	short block_align;            // sample block alignment
	short bits_per_sample;        // bits per second
	char data_sig[4];            	// 'data'
	long data_size;              	// size of waveform data
}wave_header;

struct au_header {
	uint32_t magic;		/* '.snd' */
	uint32_t hdr_size;	/* size of header (min 24) */
	uint32_t data_size;	/* size of data */
	uint32_t encoding;	/* see to AU_FMT_XXXX */
	uint32_t sample_rate;	/* sample rate */
	uint32_t channels;	/* number of channels (voices) */
};

#ifdef __cplusplus
}
#endif

#endif