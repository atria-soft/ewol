/**
 *******************************************************************************
 * @file ewol/Audio/intefacePortAudio.cpp
 * @brief poratudio ewol Audio interface (Sources)
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
#include <ewol/Debug.h>
#include <ewol/Audio/interfacePortAudio.h>
#include <ewol/Audio/audio.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <poll.h>
#include <math.h>
extern "C" {
	#include <portaudio/portaudio.h>
}

typedef struct
{
    int32_t sampleRate;
    int32_t nbChanelsInput;
    int32_t nbChanelsOutput;
    int32_t frameSize;
} userOutputData;

userOutputData data;
PaStream *stream;

/* This routine will be called by the PortAudio engine when audio is needed.
 It may called at interrupt level on some machines so don't do anything
 that could mess up the system like calling malloc() or free().
*/ 
static int patestCallback( const void *inputBuffer, void *outputBuffer,
                           unsigned long framesPerBuffer,
                           const PaStreamCallbackTimeInfo* timeInfo,
                           PaStreamCallbackFlags statusFlags,
                           void *userData )
{
    /* Cast data passed through stream to our structure. */
    userOutputData *data = (userOutputData*)userData; 
    // no use of the input buffer ... (mightt be NULL)
    (void) inputBuffer;
    ewol::audio::GetData((int16_t*)outputBuffer, framesPerBuffer, data->nbChanelsOutput);
    return 0;
}

#define SAMPLE_RATE (44100)

void ewol::portAudio::Init(void)
{
	PaError err;
	EWOL_DEBUG("Create Audio Thread...");
	
	err = Pa_Initialize();
	if( err != paNoError ) {
		EWOL_ERROR("PortAudio error: " << (char*)Pa_GetErrorText(err) );
		return;
	}
	data.sampleRate = 48000;
	data.nbChanelsInput = 0;
	data.nbChanelsOutput = 2;
	data.frameSize = 256;
	/* Open an audio I/O stream. */
	err = Pa_OpenDefaultStream( &stream, data.nbChanelsInput, data.nbChanelsOutput,
	                            paInt16, data.sampleRate, data.frameSize,
	                            patestCallback, &data );
	if( err != paNoError ) {
		EWOL_ERROR("PortAudio error: " << (char*)Pa_GetErrorText(err) );
		return;
	}
	err = Pa_StartStream( stream );
	if( err != paNoError ) {
		EWOL_ERROR("PortAudio error: " << (char*)Pa_GetErrorText(err) );
		return;
	}
	EWOL_DEBUG("Create Audio Thread ... might have start");
}

void ewol::portAudio::UnInit(void)
{
	PaError err;
	// destroy the thread ...
	EWOL_DEBUG("Wait end of the thread ...");
	err = Pa_StopStream( stream );
	if( err != paNoError ) {
		EWOL_ERROR("PortAudio error: " << (char*)Pa_GetErrorText(err) );
		return;
	}
	err = Pa_CloseStream( stream );
	if( err != paNoError ) {
		EWOL_ERROR("PortAudio error: " << (char*)Pa_GetErrorText(err) );
		return;
	}
	err = Pa_Terminate();
	if( err != paNoError ) {
		EWOL_ERROR("PortAudio error: " << (char*)Pa_GetErrorText(err) );
		return;
	}
	EWOL_DEBUG("Create Audio Thread ... might have stop");
	
}

