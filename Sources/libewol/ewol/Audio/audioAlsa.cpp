/**
 *******************************************************************************
 * @file ewol/Audio/audioAlsa.cpp
 * @brief Alsa ewol Audio interface (Sources)
 * @author Edouard DUPIN
 * @date 26/06/2012
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
#include <ewol/Audio/audioAlsa.h>
#include <ewol/Audio/audio.h>
#include <pthread.h>
#include <sched.h>

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <poll.h>
#include <math.h>
/* Use the newer ALSA API */
#define ALSA_PCM_NEW_HW_PARAMS_API
#include <alsa/asoundlib.h>


static pthread_t                        audioHandleThread;
static pthread_attr_t                   audioHandleThreadAttr;
static bool                             g_stopRequested=false;


static void* audioThread(void* param)
{
	EWOL_DEBUG("==> Init audioAlsa Thread (START)");
	int rc;
	int size;
	snd_pcm_t *handle;
	snd_pcm_hw_params_t *params;
	unsigned int val;
	int dir;
	snd_pcm_uframes_t frames;
	char *buffer;
	
	int32_t frequency = 48000;
	int32_t nbChan = 2;
	int32_t frameSize = 32;
	
	// Open PCM device name "default" to play audio on it ...
	rc = snd_pcm_open(&handle, "default", SND_PCM_STREAM_PLAYBACK, 0);
	if (rc < 0) {
		EWOL_ERROR("AUDIO : unable to open pcm device: " << (char*)snd_strerror(rc));
		// stop the thread ...
		pthread_exit(NULL);
		return NULL;
	}
	// Allocate a hardware parameters object.
	snd_pcm_hw_params_alloca(&params);
	// Fill it in with default values.
	snd_pcm_hw_params_any(handle, params);
	
	// Set the desired hardware parameters.
	// Interleaved mode
	snd_pcm_hw_params_set_access(handle, params, SND_PCM_ACCESS_RW_INTERLEAVED);
	// Signed 16-bit little-endian format
	snd_pcm_hw_params_set_format(handle, params, SND_PCM_FORMAT_S16_LE);
	// number of channel : 2 (stereo) ==> might never need more for simple game
	snd_pcm_hw_params_set_channels(handle, params, nbChan);
	// Frequency : 48000 Hz
	val = frequency;
	snd_pcm_hw_params_set_rate_near(handle, params, &val, &dir);
	// Set pushing data frameRate : 32 frames. ==> might be a little small ???
	frames = frameSize;
	snd_pcm_hw_params_set_period_size_near(handle, params, &frames, &dir);
	// Write the parameters to the driver ==> and hope it work corectly ...
	rc = snd_pcm_hw_params(handle, params);
	if (rc < 0) {
		EWOL_ERROR("AUDIO : unable to set hw parameters: " << (char*)snd_strerror(rc));
		// stop the thread ...
		pthread_exit(NULL);
		return NULL;
	}
	
	EWOL_DEBUG("AUDIO : Audio Properties : nbChan=" << nbChan << ", freg=" << frequency << "Hz, frameRate=" << frameSize);
	// Use a buffer large enough to hold one period
	snd_pcm_hw_params_get_period_size(params, &frames, &dir);
	size = frames * sizeof(int16_t) * nbChan;
	buffer = (char *) malloc(size);
	EWOL_DEBUG("AUDIO : Buffer size= " << size);
	
	// request the period time to display it ...
	snd_pcm_hw_params_get_period_time(params, &val, &dir);
	EWOL_DEBUG("AUDIO : periode time = " << (float)((float)val/1000.0) << "ms" );
	
	FILE * fileSYS = fopen("/home/edupin/export.raw", "w");
	EWOL_DEBUG("==> Init audioAlsa Thread (END)");
	while (g_stopRequested==false) {
		//request data from the standard system generation ...
		ewol::audio::GetData((int16_t*)buffer, frames, nbChan);
		fwrite(buffer,2,frames*nbChan, fileSYS);
		// write it to ALSA system
		rc = snd_pcm_writei(handle, buffer, frames);
		if (rc == -EPIPE) {
			EWOL_ERROR("underrun occurred");
			// restart the Audio system
			snd_pcm_prepare(handle);
		} else if (rc < 0) {
			EWOL_ERROR("error from writei: " << (char*)snd_strerror(rc));
		} else if (rc != (int)frames) {
			EWOL_ERROR("short write, write " << rc << " frames");
		}
	}
	EWOL_DEBUG("==> Un-Init audioAlsa Thread (START)");
	snd_pcm_drain(handle);
	snd_pcm_close(handle);
	free(buffer);
	EWOL_DEBUG("==> Un-Init audioAlsa Thread (END)");
	pthread_exit(NULL);
	return NULL;
}

void ewol::audioAlsa::Init(void)
{
	EWOL_DEBUG("Create Audio Thread...");
	g_stopRequested=false;
	// create th thread ...
	pthread_setname_np(audioHandleThread, "ewol_audio_thread");
	pthread_create(&audioHandleThread, &audioHandleThreadAttr, audioThread, NULL);
	pthread_attr_init(&audioHandleThreadAttr);
	pthread_attr_setdetachstate(&audioHandleThreadAttr, PTHREAD_CREATE_JOINABLE);
	EWOL_DEBUG("Create Audio Thread ... might have start");
}

void ewol::audioAlsa::UnInit(void)
{
	g_stopRequested=true;
	// destroy the thread ...
	EWOL_DEBUG("Wait end of the thread ...");
	// Wait end of the thread
	pthread_join(audioHandleThread, NULL);
	
	EWOL_DEBUG("Create Audio Thread ... might have stop");
	
}

