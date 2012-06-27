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
#include <ewol/Audio/audio.h>
#include <ewol/Debug.h>
#include <math.h>

static int64_t currentTimePlaying = 0;

static bool    musicMute = true;
static float   musicVolume = -5000;
static int32_t musicFadingTime = 0;

static bool    effectsMute = true;
static float   effectsVolume = -5000;

static bool isInit = false;

#ifdef __PLATFORM__Linux
#	include <ewol/Audio/audioAlsa.h>
#endif

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
	#ifdef __PLATFORM__Linux
		ewol::audioAlsa::Init();
	#endif
}


void ewol::audio::UnInit(void)
{
	if (false == isInit) {
		EWOL_ERROR("multiple un-init requested ... at the audio system ...");
		return;
	}
	#ifdef __PLATFORM__Linux
		ewol::audioAlsa::UnInit();
	#endif
	musicMute = true;
	musicVolume = -5000;
	effectsMute = true;
	effectsVolume = -5000;
	musicFadingTime = 0;
}

static float angle = 0;

void ewol::audio::GetData(int16_t * bufferInterlace, int32_t nbSample, int32_t nbChannels)
{
	if (nbChannels != 2) {
		EWOL_ERROR("TODO : Support the signal mono or more tha stereo ...");
		return;
	}
	memset(bufferInterlace, 0, nbSample*sizeof(int16_t)*nbChannels);
	static int32_t maxValue = 0;
	maxValue +=10;
	if (maxValue > 16000) {
		maxValue = 0;
	}
	for (int iii = 0; iii<nbSample ; iii++) {
		bufferInterlace[iii*2] = (float)maxValue * sin(angle/180.0 * M_PI);
		bufferInterlace[iii*2+1] = bufferInterlace[iii*2];
		angle+=0.9;
		if (angle>=360) {
			angle -= 360.0;
		}
	}
	// add effects :
	ewol::audio::effects::GetData(bufferInterlace, nbSample, nbChannels);
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


void ewol::audio::music::VolumeSet(float newVolume)
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





//----------------------------------------------------------------------------------------------------------
//                               Effects ...
//----------------------------------------------------------------------------------------------------------
//liste d'effet
class EffectsLoaded {
	public :
		EffectsLoaded(etk::UString file)
		{
			m_file = file;
			m_requestedTime = 1;
			m_nbSamples = 12000; // 0.25s
			m_data = (int16_t*)malloc(sizeof(int16_t)*m_nbSamples);
			if (NULL == m_data) {
				EWOL_CRITICAL("MEM allocation error ...");
				return;
			}
			float tmp_angle = 0.0;
			//int32_t test1 = M_PI / (50.0*m_file.Size());
			for (int iii = 0; iii<m_nbSamples ; iii++) {
				m_data[iii] = 32000.0 * sin(tmp_angle/180.0 * M_PI);
				tmp_angle+=0.8;
				if (tmp_angle>=360) {
					tmp_angle -= 360.0;
				}
			}
		}
		etk::UString m_file;
		int32_t      m_nbSamples;
		int32_t      m_requestedTime;
		int16_t*     m_data;
};

class RequestPlay {
	private:
		bool            m_freeSlot;
		EffectsLoaded * m_effect; // reference to the effects
		int32_t         m_playTime; // position in sample playing in the audio effects
	public :
		RequestPlay(EffectsLoaded * effect) : m_freeSlot(false), m_effect(effect), m_playTime(0) { };
		void Reset(EffectsLoaded * effect) { m_effect=effect; m_playTime=0; m_freeSlot=false; };
		bool IsFree(void) { return m_freeSlot; };
		void Play(int16_t * bufferInterlace, int32_t nbSample, int32_t nbChannels)
		{
			if (true==m_freeSlot) {
				return;
			}
			int32_t processTimeMax = etk_min(nbSample, m_effect->m_nbSamples - m_playTime);
			processTimeMax = etk_max(0, processTimeMax);
			int16_t * pointer = bufferInterlace;
			int16_t * newData = &m_effect->m_data[m_playTime];
			//EWOL_DEBUG("AUDIO : Play slot... nb sample : " << processTimeMax << " playTime=" <<m_playTime << " nbCannels=" << nbChannels);
			for (int32_t iii=0; iii<processTimeMax; iii++) {
				// TODO : Set volume and spacialisation ...
				for (int32_t jjj=0; jjj<nbChannels; jjj++) {
					int32_t tmppp = *pointer + *newData;
					*pointer = etk_avg(-32768, tmppp, 32767);
					//EWOL_DEBUG("AUDIO : element : " << *pointer);
					pointer++;
				}
				newData++;
			}
			m_playTime += processTimeMax;
			// check end of playing ...
			if (m_effect->m_nbSamples<=m_playTime) {
				m_freeSlot=true;
			}
		}
};

#include <etk/VectorType.h>
etk::VectorType<EffectsLoaded*> ListEffects;
etk::VectorType<RequestPlay*>   ListEffectsPlaying;


int32_t ewol::audio::effects::Add(etk::UString file)
{
	// TODO : search the previous loaded element ...
	EffectsLoaded * tmpEffect = new EffectsLoaded(file);
	if (NULL == tmpEffect) {
		EWOL_ERROR("Error to load the effects : \"" << file << "\"");
		return -1;
	}
	ListEffects.PushBack(tmpEffect);
	return ListEffects.Size()-1;
}


void ewol::audio::effects::Rm(int32_t effectId)
{
	// find element ...
	
	// chenck number of requested
	
	// mark to be removed ... TODO : Really removed it when no other element readed it ...
}


void ewol::audio::effects::Play(int32_t effectId, float xxx, float yyy)
{
	if (effectId <0 || effectId >= ListEffects.Size()) {
		EWOL_ERROR("Wrong effect ID : " << effectId << " != [0.." << ListEffects.Size()-1 << "] ==> can not play it ...");
		return;
	}
	if (ListEffects[effectId] == NULL) {
		EWOL_ERROR("effect ID : " << effectId << " ==> has been removed");
		return;
	}
	// try to find an empty slot :
	for (int32_t iii=0; iii<ListEffectsPlaying.Size(); iii++) {
		if (ListEffectsPlaying[iii]->IsFree()) {
			ListEffectsPlaying[iii]->Reset(ListEffects[effectId]);
			return;
		}
	}
	RequestPlay* newPlay = new RequestPlay(ListEffects[effectId]);
	if (NULL == newPlay) {
		EWOL_CRITICAL("Allocation error of a playing element : " << effectId);
		return;
	}
	ListEffectsPlaying.PushBack(newPlay);
}


float ewol::audio::effects::VolumeGet(void)
{
	return effectsVolume;
}


void ewol::audio::effects::VolumeSet(float newVolume)
{
	effectsVolume = newVolume;
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



void ewol::audio::effects::GetData(int16_t * bufferInterlace, int32_t nbSample, int32_t nbChannels)
{
	for (int32_t iii=0; iii<ListEffectsPlaying.Size(); iii++) {
		if (ListEffectsPlaying[iii]!= NULL) {
			ListEffectsPlaying[iii]->Play(bufferInterlace, nbSample, nbChannels);
		}
	}
}

