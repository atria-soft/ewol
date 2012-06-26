/**
 *******************************************************************************
 * @file etk/tool.h
 * @brief Ewol Tool Kit : generique tools (header)
 * @author Edouard DUPIN
 * @date 26/06/2012
 * @par Project
 * Ewol TK
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

#ifndef __ETK_TOOL_H__
#define __ETK_TOOL_H__

#include <etk/Types.h>

namespace etk {
	namespace tool {
		float   frand(float a, float b);
		int32_t irand(int32_t a, int32_t b);
	};
};

#endif
