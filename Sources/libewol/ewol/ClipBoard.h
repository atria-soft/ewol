/**
 *******************************************************************************
 * @file ClipBoard.h
 * @brief ewol : copy / past main system (header)
 * @author Edouard DUPIN
 * @date 04/04/2012
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

#ifndef __EWOL_CLIPBOARD_H__
#define __EWOL_CLIPBOARD_H__

#include <ewol/Debug.h>
#include <etk/UString.h>


namespace ewol {
	namespace clipBoard
	{
		enum {
			CLIPBOARD_0,
			CLIPBOARD_1,
			CLIPBOARD_2,
			CLIPBOARD_3,
			CLIPBOARD_4,
			CLIPBOARD_5,
			CLIPBOARD_6,
			CLIPBOARD_7,
			CLIPBOARD_8,
			CLIPBOARD_9,
			CLIPBOARD_STD,
			CLIPBOARD_SELECTION,
			TOTAL_OF_CLICKBOARD,
		};
		void	Init(void);
		void	UnInit(void);
		void	Set(uint8_t clipboardID, etk::UString &data);
		void	Get(uint8_t clipboardID, etk::UString &data);
	};
};

#endif


