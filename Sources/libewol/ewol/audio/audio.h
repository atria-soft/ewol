/**
 *******************************************************************************
 * @file ewol/audio/audio.h
 * @brief basic ewol Audio interface (header)
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

#ifndef __EWOL_AUDIO_H__
#define __EWOL_AUDIO_H__

#include <etk/Types.h>
#include <etk/UString.h>

namespace ewol {
	namespace audio {
		void Init(void);
		void UnInit(void);
		
		typedef void (*AudioCallback)(int16_t * bufferInterlace, int32_t nbSample, int32_t nbChannels);
		void AddCallbackOutput(AudioCallback userCallback);
		
		void GetData(int16_t * bufferInterlace, int32_t nbSample, int32_t nbChannels);
		
		namespace music {
			void           Fading(int32_t timeMs);
			// list playing system : is cyclic ...
			erreurCode_te  ListAdd(etk::UString file);
			erreurCode_te  ListRm(etk::UString file);
			erreurCode_te  ListClean(void);
			erreurCode_te  ListPrevious(void);
			erreurCode_te  ListNext(void);
			erreurCode_te  ListFirst(void);
			erreurCode_te  ListLast(void);
			erreurCode_te  ListPlay(void); // List playing
			erreurCode_te  ListStop(void); // List stopping
			
			erreurCode_te  Play(etk::UString file); // play specific file ... pause the list element;
			erreurCode_te  Stop(void);
			
			// in db
			float VolumeGet(void);
			void  VolumeSet(float newVolume);
			bool  MuteGet(void);
			void  MuteSet(bool newMute);
			void  GetData(int16_t * bufferInterlace, int32_t nbSample, int32_t nbChannels);
			
		};
		// note effect is loaded in memory (then don't create long effect) and unload only when requested
		namespace effects {
			// note : support file (Mono, 16bit, 48kHz) : .raw or .wav (no encodage) or .ogg (decoded with tremor lib)
			int32_t Add(etk::UString file);
			void    Rm(int32_t effectId);
			void    Play(int32_t effectId, float xxx, float yyy);
			
			// in db
			float VolumeGet(void);
			void  VolumeSet(float newVolume);
			bool  MuteGet(void);
			void  MuteSet(bool newMute);
			void  GetData(int16_t * bufferInterlace, int32_t nbSample, int32_t nbChannels);
			
		};
	};
};


#endif
