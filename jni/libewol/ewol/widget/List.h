/**
 *******************************************************************************
 * @file ewol/widget/List.h
 * @brief ewol list widget system (header)
 * @author Edouard DUPIN
 * @date 07/11/2011
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

#ifndef __EWOL_LIST_H__
#define __EWOL_LIST_H__

#include <etk/Types.h>
#include <ewol/Debug.h>
#include <ewol/Widget.h>

namespace ewol {
	class List :public ewol::Widget
	{
		public:
			List(void) { };
			virtual ~List(void) { };
	};
};

#endif
