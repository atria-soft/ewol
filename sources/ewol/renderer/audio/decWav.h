/**
 * @author Edouard DUPIN
 * 
 * @copyright 2011, Edouard DUPIN, all right reserved
 * 
 * @license BSD v3 (see license file)
 */

#ifndef __EWOL_AUDIO_DEC_WAV_H__
#define __EWOL_AUDIO_DEC_WAV_H__

#include <etk/types.h>
#include <etk/UString.h>

namespace ewol {
	namespace audio {
		namespace wav {
			int16_t * loadData(std::string filename, int8_t nbChan, int32_t frequency, int32_t & nbSampleOut);
		};
	};
};

#endif

