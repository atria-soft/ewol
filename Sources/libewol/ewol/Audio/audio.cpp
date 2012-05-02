/**
 *******************************************************************************
 * @file ewol/Audio/audio.c
 * @brief basic ewol Audio interface (Sources)
 * @author Edouard DUPIN
 * @date 02/05/2012
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
#include <ewol/Audo/audio.h>

static bool    musicMute = true;
static float   musicVolume = -5000;
static int32_t musicFadingTime = 0;

static bool    effectsMute = true;
static float   effectsVolume = -5000;

static bool isInit = false;


void ewol::audio::Init(void)
{
	if (isInit == true) {
		EWOL_ERROR("multiple init requested ... at the audio system ...");
	}
	musicMute = false;
	musicVolume = 0;
	effectsMute = false;
	effectsVolume = 0;
	musicFadingTime = 100;
	isInit = true;
}


void ewol::audio::UnInit(void)
{
	if (false == isInit) {
		EWOL_ERROR("multiple un-init requested ... at the audio system ...");
	}
	musicMute = true;
	musicVolume = -5000;
	effectsMute = true;
	effectsVolume = -5000;
	musicFadingTime = 0;
}


void ewol::audio::music::Fading(int32_t timeMs)
{
	musicFadingTime = timeMs;
	musicFadingTime = etk_avg(-100, musicFadingTime, 20);
	EWOL_INFO("Set music fading time at " << timeMs << "ms ==> " << musicFadingTime << "ms");
}


erreurCode_te ewol::audio::music::ListAdd(etk::UString file)
{
	return ERR_FAIL;
}


erreurCode_te ewol::audio::music::ListRm(etk::UString file)
{
	return ERR_FAIL;
}


erreurCode_te ewol::audio::music::ListClean(void)
{
	return ERR_FAIL;
}


erreurCode_te ewol::audio::music::ListPrevious(void)
{
	return ERR_FAIL;
}


erreurCode_te ewol::audio::music::ListNext(void)
{
	return ERR_FAIL;
}


erreurCode_te ewol::audio::music::ListFirst(void)
{
	return ERR_FAIL;
}


erreurCode_te ewol::audio::music::ListLast(void)
{
	return ERR_FAIL;
}


erreurCode_te ewol::audio::music::ListLast(void)
{
	return ERR_FAIL;
}


erreurCode_te ewol::audio::music::ListPlay(void)
{
	return ERR_FAIL;
}


erreurCode_te ewol::audio::music::ListStop(void)
{
	return ERR_FAIL;
}




erreurCode_te ewol::audio::music::Play(etk::UString file)
{
	return ERR_FAIL;
}


erreurCode_te ewol::audio::music::Stop(void)
{
	return ERR_FAIL;
}



float ewol::audio::music::VolumeGet(void)
{
	return musicVolume;
}


void ewol::audio::music::(float newVolume)
{
	musicVolume = newVolume;
	musicVolume = etk_avg(-100, musicVolume, 20);
	EWOL_INFO("Set music Volume at " << newVolume << "dB ==> " << musicVolume << "dB");
}


bool ewol::audio::music::MuteGet(void)
{
	return musicMute;
}


void ewol::audio::music::MuteSet(bool newMute)
{
	musicMute = newMute;
	EWOL_INFO("Set music Mute at " << newMute);
}





int32_t ewol::audio::effects::Add(etk::UString file)
{
	return -1;
}


void ewol::audio::effects::Rm(int32_t effectId)
{
	
}


void ewol::audio::effects::Play(int32_t effectId, etkFloat_t xxx, etkFloat_t yyy)
{
	
}


float ewol::audio::effects::VolumeGet(void)
{
	return effectsVolume;
}


void ewol::audio::effects::VolumeSet(float newVolume)
{
	effectsVolume = effectsVolume;
	effectsVolume = etk_avg(-100, effectsVolume, 20);
	EWOL_INFO("Set music Volume at " << newVolume << "dB ==> " << effectsVolume << "dB");
}


bool ewol::audio::effects::MuteGet(void)
{
	return effectsMute;
}


void ewol::audio::effects::MuteSet(bool newMute)
{
	effectsMute = newMute;
	EWOL_INFO("Set effects Mute at " << newMute);
}




