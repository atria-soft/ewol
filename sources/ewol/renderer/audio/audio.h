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
		
		typedef void (*AudioCallback)(int16_t * _bufferInterlace, int32_t _nbSample, int32_t _nbChannels);
		void addCallbackOutput(AudioCallback _userCallback);
		
		void getData(int16_t * _bufferInterlace, int32_t _nbSample, int32_t _nbChannels);
		
		namespace music {
			void fading(int32_t _timeMs);
			// list playing system : is cyclic ...
			bool  listAdd(std::string _file);
			bool  listRm(std::string _file);
			bool  listClean(void);
			bool  listPrevious(void);
			bool  listNext(void);
			bool  listFirst(void);
			bool  listLast(void);
			bool  listPlay(void); // List playing
			bool  listStop(void); // List stopping
			
			bool  play(std::string _file); // play specific file ... pause the list element;
			bool  stop(void);
			
			// in db
			float volumeGet(void);
			void  volumeSet(float _newVolume);
			bool  muteGet(void);
			void  muteSet(bool _newMute);
			void  getData(int16_t * _bufferInterlace, int32_t _nbSample, int32_t _nbChannels);
			
		};
		// note effect is loaded in memory (then don't create long effect) and unload only when requested
		namespace effects {
			// note : support file (Mono, 16bit, 48kHz) : .raw or .wav (no encodage) or .ogg (decoded with tremor lib)
			int32_t add(std::string _file);
			void    rm(int32_t _effectId);
			void    play(int32_t _effectId, float _xxx, float _yyy);
			
			// in db
			float volumeGet(void);
			void  volumeSet(float _newVolume);
			bool  muteGet(void);
			void  muteSet(bool _newMute);
			void  getData(int16_t * _bufferInterlace, int32_t _nbSample, int32_t _nbChannels);
			
		};
	};
};


#endif
