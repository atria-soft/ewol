/**
 *******************************************************************************
 * @file ewol/Audio/audioAlsa.h
 * @brief Alsa ewol Audio interface (header)
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

#ifndef __EWOL_ALSA_AUDIO_H__
#define __EWOL_ALSA_AUDIO_H__

#include <etk/Types.h>

namespace ewol {
	namespace audioAlsa {
		void Init(void);
		void UnInit(void);
	};
};

#endif