/**
 *******************************************************************************
 * @file ewol/ShortCutManager.h
 * @brief ewol shortCut manager (Header)
 * @author Edouard DUPIN
 * @date 22/02/2012
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

