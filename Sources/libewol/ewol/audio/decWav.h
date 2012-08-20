/**
 *******************************************************************************
 * @file ewol/audio/decWav.h
 * @brief basic ewol Wav decoder (header)
 * @author Edouard DUPIN
 * @date 28/06/2012
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

#ifndef __EWOL_AUDIO_DEC_WAV_H__
#define __EWOL_AUDIO_DEC_WAV_H__

#include <etk/Types.h>
#include <etk/UString.h>

namespace ewol {
	namespace audio {
		namespace wav {
			int16_t * LoadData(etk::UString filename, int8_t nbChan, int32_t frequency, int32_t & nbSampleOut);
		};
	};
};

#endif

