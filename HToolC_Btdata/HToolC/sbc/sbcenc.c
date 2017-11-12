/*
 *
 *  Bluetooth low-complexity, subband codec (SBC) encoder
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
#include "StdAfx.h"
#include <stdio.h>
#include <errno.h>
#include <fcntl.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <sys/stat.h>

#include "sbc.h"
#include "formats.h"

#include <io.h>
typedef long ssize_t;

static int verbose = 0;

static ssize_t __read(int fd, void *buf, size_t count)
{
	ssize_t len, pos = 0;

	while (count > 0) {
		len = read(fd, (char *)buf + pos, count);
		if (len <= 0)
			return pos > len ? pos : len;

		count -= len;
		pos   += len;
	}

	return pos;
}

static ssize_t __write(int fd, const void *buf, size_t count)
{
	ssize_t len, pos = 0;

	while (count > 0) {
		len = write(fd, (char *)buf + pos, count);
		if (len <= 0)
			return len;

		count -= len;
		pos   += len;
	}

	return pos;
}

int encode(char *outputBuf, const char *inputfile, int subbands, int bitpool, int chnMode, int blocks, int bitRate)
{	
	unsigned char input[2048], output[2048];
	sbc_t sbc;	
	FILE *file = 0;
	int len, size, count, encoded;
	int outLength = -1;

	if ((file= fopen(inputfile, "rb+")) == 0) {
		fprintf(stderr, "Can't open file %s: %s\n",
					inputfile, strerror(errno));

		return 0;
	}

	len = fread(input,1, sizeof(input), file);	

	if(strstr(inputfile, ".wav") != NULL){
		if (len < sizeof(wave_header)) {		
			fprintf(stderr, "Can't read header from file %s: %s\n",
						inputfile, strerror(errno));
			goto done;
		}

		wave_header *wave_hdr;
		wave_hdr = (wave_header *) input;

		if (memcmp(wave_hdr->riff_sig, "RIFF", strlen( "RIFF")) ||
				memcmp(wave_hdr->wave_sig, "WAVE", strlen( "WAVE"))) {
			fprintf(stderr, "Not in wave format\n");
			goto done;
		}
		
		if(wave_hdr->sample_rate != bitRate){
			goto done;
		}

		if(wave_hdr->channels != 1 && chnMode == SBC_MODE_MONO){
			goto done;
		}

		if(wave_hdr->channels == 1 && chnMode != SBC_MODE_MONO){
			goto done;
		}

		size = len - sizeof(wave_header);
		memmove(input, input + sizeof(wave_header), size);
	}
	else if(strstr(inputfile, ".pcm") != NULL){		
		size = len;
	}
	else
	{
		fprintf(stderr, "Unknown from file %s: %s\n",
					inputfile, strerror(errno));
		goto done;
	}

	sbc_init(&sbc, 0L);

	switch (bitRate) {
		case 16000:
			sbc.frequency = SBC_FREQ_16000;
			break;
		case 32000:
			sbc.frequency = SBC_FREQ_32000;
			break;
		case 44100:
			sbc.frequency = SBC_FREQ_44100;
			break;
		case 48000:
			sbc.frequency = SBC_FREQ_48000;
			break;
	}

	sbc.subbands = subbands == 4 ? SBC_SB_4 : SBC_SB_8;

	/*
	if (wave_hdr->channels == 1) {
		sbc.mode = SBC_MODE_MONO;
		if (joint || dualchannel) {
			fprintf(stderr, "Audio is mono but joint or "
				"dualchannel mode has been specified\n");
			goto done;
		}
	} else if (joint && !dualchannel)
		sbc.mode = SBC_MODE_JOINT_STEREO;
	else if (!joint && dualchannel)
		sbc.mode = SBC_MODE_DUAL_CHANNEL;
	else if (!joint && !dualchannel)
		sbc.mode = SBC_MODE_STEREO;
	else {
		fprintf(stderr, "Both joint and dualchannel mode have been "
								"specified\n");
		goto done;
	}
	*/

	sbc.mode = chnMode;
	sbc.endian = SBC_LE;
	sbc.bitpool = bitpool;
	//sbc.allocation = snr ? SBC_AM_SNR : SBC_AM_LOUDNESS;
	sbc.allocation = SBC_AM_LOUDNESS;
	sbc.blocks = blocks == 4 ? SBC_BLK_4 :
			blocks == 8 ? SBC_BLK_8 :
				blocks == 12 ? SBC_BLK_12 : SBC_BLK_16;

	if (verbose) {
		fprintf(stderr, "encoding %s with rate %d, %d blocks, "
			"%d subbands, %d bits, allocation method %s, "
							"and mode %s\n",
			inputfile, bitRate, blocks, subbands, bitpool,
			sbc.allocation == SBC_AM_SNR ? "SNR" : "LOUDNESS",
			sbc.mode == SBC_MODE_MONO ? "MONO" :
					sbc.mode == SBC_MODE_STEREO ?
						"STEREO" : "JOINTSTEREO");
	}

	outLength = 0;
	while (1) {
		if (size < sizeof(input)) {
			len = fread(input + size, 1, sizeof(input) - size, file);
			if (len == 0 && size == 0)
				break;

			if (len < 0) {
				perror("Can't read audio data");
				break;
			}

			size += len;
		}

		len = sbc_encode(&sbc, input, size,
					output, sizeof(output), &encoded);
		if (len <= 0)
			break;
		if (len < size)
			memmove(input, input + len, size - len);

		size -= len;

		//len = fwrite(output, 1, encoded, outfile);
		memcpy(outputBuf + outLength, output, encoded);
		outLength += encoded;
	}

	sbc_finish(&sbc);

done:
	if (file != 0)
		fclose(file);

	return outLength;
}