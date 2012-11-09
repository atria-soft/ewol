/**
 * @author Edouard DUPIN
 * 
 * @copyright 2011, Edouard DUPIN, all right reserved
 * 
 * @license BSD v3 (see license file)
 */


#include <etk/Types.h>
#include <ewol/audio/audio.h>
#include <ewol/audio/decWav.h>
#include <ewol/Debug.h>
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

#ifdef __TARGET_OS__Linux
#	include <ewol/audio/interfacePortAudio.h>
#endif

void ewol::audio::Init(void)
{
	if (isInit == true) {
		EWOL_ERROR("multiple init requested ... at the audio system ...");
	}
	ewol::audio::effects::VolumeSet(0);
	ewol::audio::effects::MuteSet(false);
	ewol::audio::music::VolumeSet(0);
	ewol::audio::music::MuteSet(false);
	musicFadingTime = 100;
	isInit = true;
	#ifdef __TARGET_OS__Linux
		ewol::portAudio::Init();
	#endif
}


void ewol::audio::UnInit(void)
{
	if (false == isInit) {
		EWOL_ERROR("multiple un-init requested ... at the audio system ...");
		return;
	}
	#ifdef __TARGET_OS__Linux
		ewol::portAudio::UnInit();
	#endif
	musicMute = true;
	musicVolume = -5000;
	effectsMute = true;
	effectsVolume = -5000;
	musicFadingTime = 0;
}

static ewol::audio::AudioCallback userLocalCallback = NULL;

void ewol::audio::GetData(int16_t * bufferInterlace, int32_t nbSample, int32_t nbChannels)
{
	// TODO : set the real playing time ...
	currentTimePlaying += 10;
	if (nbChannels != 2) {
		EWOL_ERROR("TODO : Support the signal mono or more tha stereo ...");
		return;
	}
	// reset the current buffer
	memset(bufferInterlace, 0, nbSample*sizeof(int16_t)*nbChannels);
	// get user data ...
	if (NULL != userLocalCallback) {
		(*userLocalCallback)(bufferInterlace, nbSample, nbChannels);
	}
	// get background music :
	ewol::audio::music::GetData(bufferInterlace, nbSample, nbChannels);
	// add effects :
	ewol::audio::effects::GetData(bufferInterlace, nbSample, nbChannels);
	static FILE * plop = fopen("/home/edupin/testFile48khzstereo.raw", "w");
	if (plop!=NULL) {
		fwrite(bufferInterlace, sizeof(int16_t), nbSample*nbChannels, plop);
	}
}


void ewol::audio::AddCallbackOutput(ewol::audio::AudioCallback userCallback)
{
	userLocalCallback = userCallback;
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


static void uptateMusicVolume(void)
{
	if (musicMute==true) {
		musicVolumeApply = 0;
	} else {
		// Convert in an fixpoint value
		// V2 = V1*10^(db/20)
		double coef = pow(10, (musicVolume/20) );
		musicVolumeApply = (int32_t)(coef * (double)(1<<16));
	}
}

void ewol::audio::music::VolumeSet(float newVolume)
{
	musicVolume = newVolume;
	musicVolume = etk_avg(-1000, musicVolume, 40);
	EWOL_INFO("Set music Volume at " << newVolume << "dB ==> " << musicVolume << "dB");
	uptateMusicVolume();
}


bool ewol::audio::music::MuteGet(void)
{
	return musicMute;
}


void ewol::audio::music::MuteSet(bool newMute)
{
	musicMute = newMute;
	EWOL_INFO("Set music Mute at " << newMute);
	uptateMusicVolume();
}


void ewol::audio::music::GetData(int16_t * bufferInterlace, int32_t nbSample, int32_t nbChannels)
{
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
		if (angle>=360) {
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
		EffectsLoaded(etk::UString file)
		{
			m_file = file;
			m_requestedTime = 1;
			m_data = ewol::audio::wav::LoadData(file, 1, 48000, m_nbSamples);
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
			if (m_effect->m_data == NULL) {
				m_freeSlot = true;
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
					int32_t tmppp = *pointer + ((((int32_t)*newData)*effectsVolumeApply)>>16);
					*pointer = etk_avg(-32767, tmppp, 32766);
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

#include <etk/Vector.h>
etk::Vector<EffectsLoaded*> ListEffects;
etk::Vector<RequestPlay*>   ListEffectsPlaying;


int32_t ewol::audio::effects::Add(etk::UString file)
{
	for (int32_t iii=0; iii<ListEffects.Size(); iii++) {
		if (NULL != ListEffects[iii]) {
			if (ListEffects[iii]->m_file == file) {
				ListEffects[iii]->m_requestedTime++;
				return iii;
			}
		}
	}
	// effect does not exist ... create a new one ...
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
	if (effectId <0 || effectId >= ListEffects.Size()) {
		EWOL_ERROR("Wrong effect ID : " << effectId << " != [0.." << ListEffects.Size()-1 << "] ==> can not remove it ...");
		return;
	}
	if (ListEffects[effectId] == NULL) {
		EWOL_ERROR("effect ID : " << effectId << " ==> has already been removed");
		return;
	}
	// check number of requested
	if (ListEffects[effectId]->m_requestedTime <=0) {
		EWOL_ERROR("effect ID : " << effectId << " ==> request more than predicted a removed of an effects");
		return;
	}
	ListEffects[effectId]->m_requestedTime--;
	// mark to be removed ... TODO : Really removed it when no other element readed it ...
	// TODO : ...
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
	EWOL_VERBOSE("effect play : " << effectId );
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


static void uptateEffectVolume(void)
{
	if (effectsMute==true) {
		effectsVolumeApply = 0;
	} else {
		// Convert in an fixpoint value
		// V2 = V1*10^(db/20)
		double coef = pow(10, (effectsVolume/20) );
		effectsVolumeApply = (int32_t)(coef * (double)(1<<16));
	}
}

void ewol::audio::effects::VolumeSet(float newVolume)
{
	effectsVolume = newVolume;
	effectsVolume = etk_avg(-100, effectsVolume, 20);
	EWOL_INFO("Set music Volume at " << newVolume << "dB ==> " << effectsVolume << "dB");
	uptateEffectVolume();
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

