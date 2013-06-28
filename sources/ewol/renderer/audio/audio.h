/**
 * @author Edouard DUPIN
 * 
 * @copyright 2011, Edouard DUPIN, all right reserved
 * 
 * @license BSD v3 (see license file)
 */


#ifndef __EWOL_AUDIO_H__
#define __EWOL_AUDIO_H__

#include <etk/types.h>
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
			bool  ListAdd(etk::UString file);
			bool  ListRm(etk::UString file);
			bool  ListClean(void);
			bool  ListPrevious(void);
			bool  ListNext(void);
			bool  ListFirst(void);
			bool  ListLast(void);
			bool  ListPlay(void); // List playing
			bool  ListStop(void); // List stopping
			
			bool  Play(etk::UString file); // play specific file ... pause the list element;
			bool  Stop(void);
			
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
