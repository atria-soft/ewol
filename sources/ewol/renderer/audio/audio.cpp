/**
 * @author Edouard DUPIN
 * 
 * @copyright 2011, Edouard DUPIN, all right reserved
 * 
 * @license BSD v3 (see license file)
 */


#include <etk/types.h>
#include <ewol/renderer/audio/audio.h>
#include <ewol/renderer/audio/decWav.h>
#include <ewol/debug.h>
#include <math.h>

static int64_t currentTimePlaying = 0;

static bool    musicMute = false;
static float   musicVolume = 0;
static int32_t musicVolumeApply = 1<<16;
static int32_t musicFadingTime = 0;

static bool    effectsMute = false;
static float   effectsVolume = 0;
static int32_t effectsVolumeApply = 1<<16;

static bool isInit = false;

#ifdef BUILD_PORTAUDIO
#	include <ewol/renderer/audio/interfacePortAudio.h>
#endif

void ewol::audio::init(void) {
	if (isInit == true) {
		EWOL_ERROR("multiple init requested ... at the audio system ...");
	}
	ewol::audio::effects::volumeSet(0);
	ewol::audio::effects::muteSet(false);
	ewol::audio::music::volumeSet(0);
	ewol::audio::music::muteSet(false);
	musicFadingTime = 100;
	isInit = true;
	#ifdef BUILD_PORTAUDIO
		ewol::portAudio::init();
	#endif
}


void ewol::audio::unInit(void) {
	if (false == isInit) {
		EWOL_ERROR("multiple un-init requested ... at the audio system ...");
		return;
	}
	#ifdef BUILD_PORTAUDIO
		ewol::portAudio::unInit();
	#endif
	musicMute = true;
	musicVolume = -5000;
	effectsMute = true;
	effectsVolume = -5000;
	musicFadingTime = 0;
}

static ewol::audio::AudioCallback userLocalCallback = NULL;

void ewol::audio::getData(int16_t * _bufferInterlace, int32_t _nbSample, int32_t _nbChannels) {
	// TODO : set the real playing time ...
	currentTimePlaying += 10;
	if (_nbChannels != 2) {
		EWOL_ERROR("TODO : Support the signal mono or more tha stereo ...");
		return;
	}
	// reset the current buffer
	memset(_bufferInterlace, 0, _nbSample*sizeof(int16_t)*_nbChannels);
	// get user data ...
	if (NULL != userLocalCallback) {
		(*userLocalCallback)(_bufferInterlace, _nbSample, _nbChannels);
	}
	// get background music :
	ewol::audio::music::getData(_bufferInterlace, _nbSample, _nbChannels);
	// add effects :
	ewol::audio::effects::getData(_bufferInterlace, _nbSample, _nbChannels);
	static FILE * plop = fopen("/home/edupin/testFile48khzstereo.raw", "w");
	if (plop!=NULL) {
		fwrite(_bufferInterlace, sizeof(int16_t), _nbSample*_nbChannels, plop);
	}
}


void ewol::audio::addCallbackOutput(ewol::audio::AudioCallback _userCallback) {
	userLocalCallback = _userCallback;
}

void ewol::audio::music::fading(int32_t _timeMs) {
	musicFadingTime = _timeMs;
	musicFadingTime = etk_avg(-100, musicFadingTime, 20);
	EWOL_INFO("Set music fading time at " << _timeMs << "ms  == > " << musicFadingTime << "ms");
}


bool ewol::audio::music::listAdd(etk::UString _file) {
	return false;
}


bool ewol::audio::music::listRm(etk::UString _file) {
	return false;
}


bool ewol::audio::music::listClean(void) {
	return false;
}


bool ewol::audio::music::listPrevious(void) {
	return false;
}


bool ewol::audio::music::listNext(void) {
	return false;
}


bool ewol::audio::music::listFirst(void) {
	return false;
}


bool ewol::audio::music::listLast(void) {
	return false;
}



bool ewol::audio::music::listPlay(void) {
	return false;
}


bool ewol::audio::music::listStop(void) {
	return false;
}




bool ewol::audio::music::play(etk::UString _file) {
	return false;
}


bool ewol::audio::music::stop(void) {
	return false;
}



float ewol::audio::music::volumeGet(void) {
	return musicVolume;
}


static void uptateMusicVolume(void) {
	if (musicMute == true) {
		musicVolumeApply = 0;
	} else {
		// convert in an fixpoint value
		// V2 = V1*10^(db/20)
		double coef = pow(10, (musicVolume/20) );
		musicVolumeApply = (int32_t)(coef * (double)(1<<16));
	}
}

void ewol::audio::music::volumeSet(float _newVolume) {
	musicVolume = _newVolume;
	musicVolume = etk_avg(-1000, musicVolume, 40);
	EWOL_INFO("Set music Volume at " << _newVolume << "dB  == > " << musicVolume << "dB");
	uptateMusicVolume();
}


bool ewol::audio::music::muteGet(void) {
	return musicMute;
}


void ewol::audio::music::muteSet(bool _newMute) {
	musicMute = _newMute;
	EWOL_INFO("Set music Mute at " << _newMute);
	uptateMusicVolume();
}


void ewol::audio::music::getData(int16_t * _bufferInterlace, int32_t _nbSample, int32_t _nbChannels) {
	/*static int32_t maxValue = 0;
	static float angle = 0;
	maxValue +=10;
	if (maxValue > 16000) {
		maxValue = 0;
	}
	for (int iii = 0; iii<nbSample ; iii++) {
		bufferInterlace[iii*2] = (float)maxValue * sin(angle/180.0 * M_PI);
		bufferInterlace[iii*2+1] = bufferInterlace[iii*2];
		angle+=0.9;
		if (angle >= 360) {
			angle -= 360.0;
		}
	}*/
}



//----------------------------------------------------------------------------------------------------------
//                               Effects ...
//----------------------------------------------------------------------------------------------------------
//liste d'effet
class EffectsLoaded {
	public :
		EffectsLoaded(etk::UString _file)
		{
			m_file = _file;
			m_requestedTime = 1;
			m_data = ewol::audio::wav::loadData(_file, 1, 48000, m_nbSamples);
			if (m_data == NULL) {
				// write an error ...
			}
		}
		etk::UString m_file;
		int32_t      m_nbSamples;
		int32_t      m_requestedTime;
		int16_t*     m_data;
};

class RequestPlay {
	private:
		bool m_freeSlot;
		EffectsLoaded* m_effect; // reference to the effects
		int32_t m_playTime; // position in sample playing in the audio effects
	public :
		RequestPlay(EffectsLoaded * _effect) : m_freeSlot(false), m_effect(_effect), m_playTime(0) { };
		void reset(EffectsLoaded * _effect) { m_effect=_effect; m_playTime=0; m_freeSlot=false; };
		bool isFree(void) { return m_freeSlot; };
		void play(int16_t * _bufferInterlace, int32_t _nbSample, int32_t _nbChannels)
		{
			if (true == m_freeSlot) {
				return;
			}
			if (m_effect->m_data == NULL) {
				m_freeSlot = true;
				return;
			}
			int32_t processTimeMax = etk_min(_nbSample, m_effect->m_nbSamples - m_playTime);
			processTimeMax = etk_max(0, processTimeMax);
			int16_t * pointer = _bufferInterlace;
			int16_t * newData = &m_effect->m_data[m_playTime];
			//EWOL_DEBUG("AUDIO : Play slot... nb sample : " << processTimeMax << " playTime=" <<m_playTime << " nbCannels=" << nbChannels);
			for (int32_t iii=0; iii<processTimeMax; iii++) {
				// TODO : set volume and spacialisation ...
				for (int32_t jjj=0; jjj<_nbChannels; jjj++) {
					int32_t tmppp = *pointer + ((((int32_t)*newData)*effectsVolumeApply)>>16);
					*pointer = etk_avg(-32767, tmppp, 32766);
					//EWOL_DEBUG("AUDIO : element : " << *pointer);
					pointer++;
				}
				newData++;
			}
			m_playTime += processTimeMax;
			// check end of playing ...
			if (m_effect->m_nbSamples <= m_playTime) {
				m_freeSlot=true;
			}
		}
};

#include <etk/Vector.h>
etk::Vector<EffectsLoaded*> ListEffects;
etk::Vector<RequestPlay*>   ListEffectsPlaying;


int32_t ewol::audio::effects::add(etk::UString _file) {
	for (int32_t iii=0; iii<ListEffects.size(); iii++) {
		if (NULL != ListEffects[iii]) {
			if (ListEffects[iii]->m_file == _file) {
				ListEffects[iii]->m_requestedTime++;
				return iii;
			}
		}
	}
	// effect does not exist ... create a new one ...
	EffectsLoaded * tmpEffect = new EffectsLoaded(_file);
	if (NULL == tmpEffect) {
		EWOL_ERROR("Error to load the effects : \"" << _file << "\"");
		return -1;
	}
	ListEffects.pushBack(tmpEffect);
	return ListEffects.size()-1;
}


void ewol::audio::effects::rm(int32_t _effectId) {
	// find element ...
	if (_effectId <0 || _effectId >= ListEffects.size()) {
		EWOL_ERROR("Wrong effect ID : " << _effectId << " != [0.." << ListEffects.size()-1 << "]  == > can not remove it ...");
		return;
	}
	if (ListEffects[_effectId] == NULL) {
		EWOL_ERROR("effect ID : " << _effectId << "  == > has already been removed");
		return;
	}
	// check number of requested
	if (ListEffects[_effectId]->m_requestedTime  <= 0) {
		EWOL_ERROR("effect ID : " << _effectId << "  == > request more than predicted a removed of an effects");
		return;
	}
	ListEffects[_effectId]->m_requestedTime--;
	// mark to be removed ... TODO : Really removed it when no other element readed it ...
	// TODO : ...
}


void ewol::audio::effects::play(int32_t _effectId, float _xxx, float _yyy) {
	if (_effectId <0 || _effectId >= ListEffects.size()) {
		EWOL_ERROR("Wrong effect ID : " << _effectId << " != [0.." << ListEffects.size()-1 << "]  == > can not play it ...");
		return;
	}
	if (ListEffects[_effectId] == NULL) {
		EWOL_ERROR("effect ID : " << _effectId << "  == > has been removed");
		return;
	}
	EWOL_VERBOSE("effect play : " << _effectId );
	// try to find an empty slot :
	for (int32_t iii=0; iii<ListEffectsPlaying.size(); iii++) {
		if (ListEffectsPlaying[iii]->isFree()) {
			ListEffectsPlaying[iii]->reset(ListEffects[_effectId]);
			return;
		}
	}
	RequestPlay* newPlay = new RequestPlay(ListEffects[_effectId]);
	if (NULL == newPlay) {
		EWOL_CRITICAL("Allocation error of a playing element : " << _effectId);
		return;
	}
	ListEffectsPlaying.pushBack(newPlay);
}


float ewol::audio::effects::volumeGet(void) {
	return effectsVolume;
}


static void uptateEffectVolume(void) {
	if (effectsMute == true) {
		effectsVolumeApply = 0;
	} else {
		// convert in an fixpoint value
		// V2 = V1*10^(db/20)
		double coef = pow(10, (effectsVolume/20) );
		effectsVolumeApply = (int32_t)(coef * (double)(1<<16));
	}
}

void ewol::audio::effects::volumeSet(float _newVolume) {
	effectsVolume = _newVolume;
	effectsVolume = etk_avg(-100, effectsVolume, 20);
	EWOL_INFO("Set music Volume at " << _newVolume << "dB  == > " << effectsVolume << "dB");
	uptateEffectVolume();
}


bool ewol::audio::effects::muteGet(void) {
	return effectsMute;
}


void ewol::audio::effects::muteSet(bool _newMute) {
	effectsMute = _newMute;
	EWOL_INFO("Set effects Mute at " << _newMute);
}



void ewol::audio::effects::getData(int16_t * _bufferInterlace, int32_t _nbSample, int32_t _nbChannels) {
	for (int32_t iii=0; iii<ListEffectsPlaying.size(); iii++) {
		if (ListEffectsPlaying[iii]!= NULL) {
			ListEffectsPlaying[iii]->play(_bufferInterlace, _nbSample, _nbChannels);
		}
	}
}

