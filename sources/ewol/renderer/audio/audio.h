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
		void init(void);
		void unInit(void);
		
		typedef void (*AudioCallback)(int16_t * bufferInterlace, int32_t nbSample, int32_t nbChannels);
		void addCallbackOutput(AudioCallback userCallback);
		
		void getData(int16_t * bufferInterlace, int32_t nbSample, int32_t nbChannels);
		
		namespace music {
			void fading(int32_t timeMs);
			// list playing system : is cyclic ...
			bool  listAdd(etk::UString file);
			bool  listRm(etk::UString file);
			bool  listClean(void);
			bool  listPrevious(void);
			bool  listNext(void);
			bool  listFirst(void);
			bool  listLast(void);
			bool  listPlay(void); // List playing
			bool  listStop(void); // List stopping
			
			bool  play(etk::UString file); // play specific file ... pause the list element;
			bool  stop(void);
			
			// in db
			float volumeGet(void);
			void  volumeSet(float newVolume);
			bool  muteGet(void);
			void  muteSet(bool newMute);
			void  getData(int16_t * bufferInterlace, int32_t nbSample, int32_t nbChannels);
			
		};
		// note effect is loaded in memory (then don't create long effect) and unload only when requested
		namespace effects {
			// note : support file (Mono, 16bit, 48kHz) : .raw or .wav (no encodage) or .ogg (decoded with tremor lib)
			int32_t add(etk::UString file);
			void    rm(int32_t effectId);
			void    play(int32_t effectId, float xxx, float yyy);
			
			// in db
			float volumeGet(void);
			void  volumeSet(float newVolume);
			bool  muteGet(void);
			void  muteSet(bool newMute);
			void  getData(int16_t * bufferInterlace, int32_t nbSample, int32_t nbChannels);
			
		};
	};
};


#endif
