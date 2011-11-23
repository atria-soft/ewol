/**
 *******************************************************************************
 * @file ewol/theme/EolBase.h
 * @brief basic ewol theme eol file basic element virtual Classes (Header)
 * @author Edouard DUPIN
 * @date 23/11/2011
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



#ifndef __EWOL_THEME_EOL_BASE__H__
#define __EWOL_THEME_EOL_BASE__H__

#include <etk/Types.h>
#include <etk/String.h>
#include <ewol/OObject.h>


namespace ewol {
	namespace theme {
		class EolBase {
			public:
				EolBase(void) { };
				virtual ~EolBase(void) { };
		};
	};
};


#endif

