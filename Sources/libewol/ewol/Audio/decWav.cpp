/**
 *******************************************************************************
 * @file ewol/Audio/decWav.cpp
 * @brief basic ewol Wav decoder (sources)
 * @author Edouard DUPIN
 * @date 28/06/2012
 * @par Project
 * ewol
 *
 * @par Copyright
 * Copyright 2011 Edouard DUPIN, all right reserved
 *
 * This software is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY.
 *
 * Licence summary : 
 *    You can modify and redistribute the sources code and binaries.
 *    You can send me the bug-fix
 *
 * Term of the licence in in the file licence.txt.
 *
 *******************************************************************************
 */

#include <etk/Types.h>
#include <etk/UString.h>
#include <etk/File.h>
#include <ewol/Audio/decWav.h>
#include <ewol/Debug.h>


typedef struct {
	char       riffTag[4];     //!< 00-03
	uint32_t   size;           //!< 04-07
	char       waveTag[4];     //!< 08-0b
	char       fmtTag[4];      //!< 0c-0f
	uint32_t   waveFormatSize; //!< 10-13
	struct {
		uint16_t type;           //!< 00-01
		uint16_t channelCount;   //!< 02-03
		uint32_t samplesPerSec;  //!< 04-07
		uint32_t bytesPerSec;    //!< 08-0b
		uint16_t bytesPerFrame;  //!< 0c-0d
		uint16_t bitsPerSample;  //!< 0e-0f
	}waveFormat;               //!< 14-23
	char       dataTag[4];     //!< 24-27
	uint32_t   dataSize;       //!< 28-2b
}waveHeader;


#define CONVERT_UINT32(littleEndien,data) (littleEndien)?(((uint32_t)((uint8_t*)data)[0] | (uint32_t)((uint8_t*)data)[1] << 8 | (uint32_t)((uint8_t*)data)[2] << 16 | (uint32_t)((uint8_t*)data)[3] << 24)): \
                                                         (((uint32_t)((uint8_t*)data)[3] | (uint32_t)((uint8_t*)data)[2] << 8 | (uint32_t)((uint8_t*)data)[1] << 16 | (uint32_t)((uint8_t*)data)[0] << 24))

#define CONVERT_INT32(littleEndien,data)  (littleEndien)?(((int32_t)((uint8_t*)data)[0] | (int32_t)((uint8_t*)data)[1] << 8 | (int32_t)((uint8_t*)data)[2] << 16 | (int32_t)((int8_t*)data)[3] << 24)): \
                                                         (((int32_t)((uint8_t*)data)[3] | (int32_t)((uint8_t*)data)[2] << 8 | (int32_t)((uint8_t*)data)[1] << 16 | (int32_t)((int8_t*)data)[0] << 24))

#define CONVERT_UINT24(littleEndien,data) (littleEndien)?(((uint32_t)((uint8_t*)data)[0]<<8 | (uint32_t)((uint8_t*)data)[1] << 16 | (uint32_t)((uint8_t*)data)[2] << 24)): \
                                                         (((uint32_t)((uint8_t*)data)[2]<<8 | (uint32_t)((uint8_t*)data)[1] << 16 | (uint32_t)((uint8_t*)data)[0] << 24))

#define CONVERT_INT24(littleEndien,data)  (littleEndien)?(((int32_t)((uint8_t*)data)[0]<<8 | (int32_t)((uint8_t*)data)[1] << 16 | (int32_t)((int8_t*)data)[2] << 24)): \
                                                         (((int32_t)((uint8_t*)data)[2]<<8 | (int32_t)((uint8_t*)data)[1] << 16 | (int32_t)((int8_t*)data)[0] << 24))

#define CONVERT_UINT16(littleEndien,data) (littleEndien)?(((uint16_t)((uint8_t*)data)[0] | (uint16_t)((uint8_t*)data)[1] << 8)): \
                                                         (((uint16_t)((uint8_t*)data)[1] | (uint16_t)((uint8_t*)data)[0] << 8))

#define CONVERT_INT16(littleEndien,data)  (littleEndien)?(((int16_t)((uint8_t*)data)[0] | (int16_t)((int8_t*)data)[1] << 8)): \
                                                         (((int16_t)((uint8_t*)data)[1] | (int16_t)((int8_t*)data)[0] << 8))

#define COMPR_PCM   (1)
#define COMPR_MADPCM (2)
#define COMPR_ALAW   (6)
#define COMPR_MULAW  (7)
#define COMPR_ADPCM  (17)
#define COMPR_YADPCM (20)
#define COMPR_GSM    (49)
#define COMPR_G721   (64)
#define COMPR_MPEG   (80)

int16_t * ewol::audio::wav::LoadData(etk::UString filename, int8_t nbChan, int32_t frequency, int32_t & nbSampleOut)
{
	nbSampleOut = 0;
	waveHeader myHeader;
	memset(&myHeader, 0, sizeof(waveHeader));
	etk::File fileAccess(filename, etk::FILE_TYPE_DATA);
	// Start loading the XML : 
	EWOL_DEBUG("open file (WAV) \"" << fileAccess << "\"");

	if (false == fileAccess.Exist()) {
		EWOL_ERROR("File Does not exist : \"" << fileAccess << "\"");
		return NULL;
	}
	int32_t fileSize = fileAccess.Size();
	if (0==fileSize) {
		EWOL_ERROR("This file is empty : \"" << fileAccess << "\"");
		return NULL;
	}
	if (false == fileAccess.fOpenRead()) {
		EWOL_ERROR("Can not open the file : \"" << fileAccess << "\"");
		return NULL;
	}
	// try to find endienness :
	if (fileSize < sizeof(waveHeader)) {
		EWOL_ERROR("File : \"" << fileAccess << "\" ==> has not enouth data inside might be minumum of " << sizeof(waveHeader));
		return NULL;
	}
	// ----------------------------------------------
	// read the header :
	// ----------------------------------------------
	if (fileAccess.fRead(&myHeader.riffTag, 1, 4)!=4) {
		EWOL_ERROR("Can not 4 element in the file : \"" << fileAccess << "\"");
		return NULL;
	}
	bool littleEndien = false;
	if(    myHeader.riffTag[0] == 'R'
	    && myHeader.riffTag[1] == 'I'
	    && myHeader.riffTag[2] == 'F'
	    && (myHeader.riffTag[3] == 'F' || myHeader.riffTag[3] == 'X') ) {
		if (myHeader.riffTag[3] == 'F' ) {
			littleEndien = true;
		}
	} else {
		EWOL_ERROR("file: \"" << fileAccess << "\" Does not start with \"RIF\" " );
		return NULL;
	}
	// get the data size :
	unsigned char tmpData[32];
	if (fileAccess.fRead(tmpData, 1, 4)!=4) {
		EWOL_ERROR("Can not 4 element in the file : \"" << fileAccess << "\"");
		return NULL;
	}
	myHeader.size = CONVERT_UINT32(littleEndien, tmpData);
	
	// get the data size :
	if (fileAccess.fRead(&myHeader.waveTag, 1, 4)!=4) {
		EWOL_ERROR("Can not 4 element in the file : \"" << fileAccess << "\"");
		return NULL;
	}
	if(    myHeader.waveTag[0] != 'W'
	    || myHeader.waveTag[1] != 'A'
	    || myHeader.waveTag[2] != 'V'
	    || myHeader.waveTag[3] != 'E' ) {
		EWOL_ERROR("file: \"" << fileAccess << "\" This is not a wave file " << myHeader.waveTag[0] << myHeader.waveTag[1] << myHeader.waveTag[2] << myHeader.waveTag[3] );
		return NULL;
	}
	
	// get the data size :
	if (fileAccess.fRead(&myHeader.fmtTag, 1, 4)!=4) {
		EWOL_ERROR("Can not 4 element in the file : \"" << fileAccess << "\"");
		return NULL;
	}
	if(    myHeader.fmtTag[0] != 'f'
	    || myHeader.fmtTag[1] != 'm'
	    || myHeader.fmtTag[2] != 't'
	    || myHeader.fmtTag[3] != ' ' ) {
		EWOL_ERROR("file: \"" << fileAccess << "\" header error ..."  << myHeader.fmtTag[0] << myHeader.fmtTag[1] << myHeader.fmtTag[2] << myHeader.fmtTag[3]);
		return NULL;
	}
	// get the data size :
	if (fileAccess.fRead(tmpData, 1, 4)!=4) {
		EWOL_ERROR("Can not 4 element in the file : \"" << fileAccess << "\"");
		return NULL;
	}
	myHeader.waveFormatSize = CONVERT_UINT32(littleEndien, tmpData);
	
	if (myHeader.waveFormatSize != 16) {
		EWOL_ERROR("file : \"" << fileAccess << "\"  ==> header error ...");
		return NULL;
	}
	if (fileAccess.fRead(tmpData, 1, 16)!=16) {
		EWOL_ERROR("Can not 16 element in the file : \"" << fileAccess << "\"");
		return NULL;
	}
	unsigned char * tmppp = tmpData;
	myHeader.waveFormat.type = CONVERT_UINT16(littleEndien, tmppp);
	tmppp += 2;
	myHeader.waveFormat.channelCount = CONVERT_UINT16(littleEndien, tmppp);
	tmppp += 2;
	myHeader.waveFormat.samplesPerSec = CONVERT_UINT32(littleEndien, tmppp);
	tmppp += 4;
	myHeader.waveFormat.bytesPerSec = CONVERT_UINT32(littleEndien, tmppp);
	tmppp += 4;
	myHeader.waveFormat.bytesPerFrame = CONVERT_UINT16(littleEndien, tmppp);
	tmppp += 2;
	myHeader.waveFormat.bitsPerSample = CONVERT_UINT16(littleEndien, tmppp);
	EWOL_DEBUG("audio properties : ");
	EWOL_DEBUG("    type : "          << myHeader.waveFormat.type);
	EWOL_DEBUG("    channelCount : "  << myHeader.waveFormat.channelCount);
	EWOL_DEBUG("    samplesPerSec : " << myHeader.waveFormat.samplesPerSec);
	EWOL_DEBUG("    bytesPerSec : "   << myHeader.waveFormat.bytesPerSec);
	EWOL_DEBUG("    bytesPerFrame : " << myHeader.waveFormat.bytesPerFrame);
	EWOL_DEBUG("    bitsPerSample : " << myHeader.waveFormat.bitsPerSample);
	// get the data size :
	if (fileAccess.fRead(&myHeader.dataTag, 1, 4)!=4) {
		EWOL_ERROR("Can not 4 element in the file : \"" << fileAccess << "\"");
		return NULL;
	}
	if(    myHeader.dataTag[0] != 'd'
	    || myHeader.dataTag[1] != 'a'
	    || myHeader.dataTag[2] != 't'
	    || myHeader.dataTag[3] != 'a' ) {
		EWOL_ERROR("file: \"" << fileAccess << "\" header error ..."  << myHeader.dataTag[0] << myHeader.dataTag[1] << myHeader.dataTag[2] << myHeader.dataTag[3]);
		return NULL;
	}
	// get the data size :
	if (fileAccess.fRead(tmpData, 1, 4)!=4) {
		EWOL_ERROR("Can not 4 element in the file : \"" << fileAccess << "\"");
		return NULL;
	}
	myHeader.dataSize = CONVERT_UINT32(littleEndien, tmpData);
	
	// ----------------------------------------------
	// end of the header reading done ...
	// ----------------------------------------------
	
	//Parse the data and transform it if needed ...
	if (COMPR_PCM != myHeader.waveFormat.type) {
		EWOL_ERROR("File : \"" << fileAccess << "\" ==> support only PCM compression ...");
		return NULL;
	}
	if (myHeader.waveFormat.channelCount==0 || myHeader.waveFormat.channelCount>2) {
		EWOL_ERROR("File : \"" << fileAccess << "\" ==> support only mono or stereo ..." << myHeader.waveFormat.channelCount);
		return NULL;
	}
	if ( ! (    myHeader.waveFormat.bitsPerSample==16
	         || myHeader.waveFormat.bitsPerSample==24
	         || myHeader.waveFormat.bitsPerSample==32 ) ) {
		EWOL_ERROR("File : \"" << fileAccess << "\" ==> not supported bit/sample ..." << myHeader.waveFormat.bitsPerSample);
		return NULL;
	}
	if( ! (   44100 == myHeader.waveFormat.samplesPerSec
	       || 48000 == myHeader.waveFormat.samplesPerSec) ) {
		EWOL_ERROR("File : \"" << fileAccess << "\" ==> not supported frequency " << myHeader.waveFormat.samplesPerSec << " != 48000");
		return NULL;
	}
	EWOL_DEBUG("    dataSize : " << myHeader.dataSize);
	int32_t globalDataSize = myHeader.dataSize;
	int32_t nbSample = (myHeader.dataSize/((myHeader.waveFormat.bitsPerSample/8)*myHeader.waveFormat.channelCount));
	int32_t outputSize = nbChan*nbSample;
	int16_t * outputData = (int16_t*)malloc(outputSize*sizeof(int16_t));
	if (NULL == outputData) {
		EWOL_ERROR("Allocation ERROR try to allocate " << (outputSize*sizeof(int16_t) ) << "bytes");
		return NULL;
	}
	int16_t * tmpOut = outputData;
	for( int32_t iii=0; iii<nbSample; iii++) {
		int32_t left;
		int32_t right;
		char audioSample[8];
		if (myHeader.waveFormat.bitsPerSample == 16) {
			if (myHeader.waveFormat.channelCount == 1) {
				if (fileAccess.fRead(audioSample, 1, 2)!=2) {
					EWOL_ERROR("Read Error at position : " << iii);
					return NULL;
				}
				left = ((int32_t)((int16_t)CONVERT_INT16(littleEndien, audioSample))) << 16;
				right = left;
			} else {
				if (fileAccess.fRead(audioSample, 1, 4)!=4) {
					EWOL_ERROR("Read Error at position : " << iii);
					return NULL;
				}
				left = (int32_t)((int16_t)CONVERT_INT16(littleEndien, audioSample)) << 16;
				right = (int32_t)((int16_t)CONVERT_INT16(littleEndien, audioSample+2)) << 16;
			}
		} else if (myHeader.waveFormat.bitsPerSample == 24) {
			if (myHeader.waveFormat.channelCount == 1) {
				if (fileAccess.fRead(audioSample, 1, 3)!=3) {
					EWOL_ERROR("Read Error at position : " << iii);
					return NULL;
				}
				left = CONVERT_INT24(littleEndien, audioSample);
				right = left;
			} else {
				if (fileAccess.fRead(audioSample, 1, 6)!=6) {
					EWOL_ERROR("Read Error at position : " << iii);
					return NULL;
				}
				left = CONVERT_INT24(littleEndien, audioSample);
				right = CONVERT_INT24(littleEndien, audioSample+3);
			}
		} else if (myHeader.waveFormat.bitsPerSample == 32) {
			if (myHeader.waveFormat.channelCount == 1) {
				if (fileAccess.fRead(audioSample, 1, 4)!=4) {
					EWOL_ERROR("Read Error at position : " << iii);
					return NULL;
				}
				left = CONVERT_INT32(littleEndien, audioSample);
				right = left;
			} else {
				if (fileAccess.fRead(audioSample, 1, 8)!=8) {
					EWOL_ERROR("Read Error at position : " << iii);
					return NULL;
				}
				left = CONVERT_INT32(littleEndien, audioSample);
				right = CONVERT_INT32(littleEndien, audioSample+4);
			}
		}
		if (nbChan == 1) {
			*tmpOut++ = (int16_t)((left>>1 + right>>1)>>16);
		} else {
			*tmpOut++ = (int16_t)(left>>16);
			*tmpOut++ = (int16_t)(left>>16);
		}
	}
	// close the file:
	fileAccess.fClose();
	nbSampleOut = nbSample;
	return outputData;
}


