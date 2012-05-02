/**
 *******************************************************************************
 * @file ewol/Audio/audio.h
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

namespace ewol {
	namespace audio {
		void Init(void);
		void UnInit(void);
		
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
			
		};
		namespace effects {
			int32_t Add(etk::UString file);
			void    Rm(int32_t effectId);
			void    Play(int32_t effectId, etkFloat_t xxx, etkFloat_t yyy);
			
			// in db
			float VolumeGet(void);
			void  VolumeSet(float newVolume);
			bool  MuteGet(void);
			void  MuteSet(bool newMute);
			
		};
	};
};


#endif
