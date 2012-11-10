/**
 * @author Edouard DUPIN
 * 
 * @copyright 2011, Edouard DUPIN, all right reserved
 * 
 * @license BSD v3 (see license file)
 */

#ifndef __EWOL_SHORT_CUT_MANAGER_H__
#define __EWOL_SHORT_CUT_MANAGER_H__

#include <etk/Types.h>
#include <etk/UString.h>
#include <ewol/widget/Widget.h>

namespace ewol {
	namespace shortCut {
		void Init(void);
		void UnInit(void);
		bool Process(ewol::specialKey_ts& special, uniChar_t unicodeValue, ewol::eventKbMoveType_te kbMove, bool isDown);
	};
};



#endif

