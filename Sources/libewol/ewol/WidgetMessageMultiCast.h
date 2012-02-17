/**
 *******************************************************************************
 * @file ewol/WidgetMessageMultiCast.h
 * @brief basic ewol Widget Message Multi-cast (Header)
 * @author Edouard DUPIN
 * @date 31/01/2012
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

#ifndef __EWOL_WIDGET_MESSAGE_MULTIICAST_H__
#define __EWOL_WIDGET_MESSAGE_MULTIICAST_H__

#include <etk/Types.h>
#include <ewol/Debug.h>
#include <ewol/OObject.h>
#include <etk/VectorType.h>
#include <ewol/Widget.h>
#include <etk/UString.h>

namespace ewol {
	namespace widgetMessageMultiCast {
			void Init(  void);
			void UnInit(void);
			void Add( int32_t widgetId, const char* const message);
			void Rm(  int32_t widgetId);
			void Send(int32_t widgetId, const char* const message, int32_t data);
			// TODO : Mus be deprecated ....
			void Send(int32_t widgetId, const char* const message, const char * data = NULL);
			void Send(int32_t widgetId, const char* const message, etk::UString& data);
	};
};

#endif

