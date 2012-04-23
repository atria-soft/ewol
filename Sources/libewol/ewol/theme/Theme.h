/**
 *******************************************************************************
 * @file ewol/theme/Theme.h
 * @brief basic ewol Theme basic class (Header)
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



#ifndef __EWOL_EOL_THEME_H__
#define __EWOL_EOL_THEME_H__

#include <etk/Types.h>
#include <etk/UString.h>
#include <etk/File.h>

namespace ewol {
	namespace theme {
		void Load(etk::File & localFile);
	};
};

#endif

