/**
 *******************************************************************************
 * @file ewolWidgetManager.h
 * @brief basic ewol Widget Manager (Header)
 * @author Edouard DUPIN
 * @date 13/11/2011
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

#ifndef __EWOL_WIDGET_MANAGER_H__
#define __EWOL_WIDGET_MANAGER_H__

#include <etkTypes.h>
#include <ewolDebug.h>
#include <ewolOObject.h>
#include <etkVectorType.h>
#include <ewolWidget.h>

namespace ewol {
	namespace widgetManager {
			void           Init(  void);
			void           UnInit(void);
			void           Add(   ewol::Widget * newWidget);
			void           Rm(    ewol::Widget * newWidget);
			void           Rm(    int32_t widgetId);
			int32_t        GetId( ewol::Widget * newWidget);
			ewol::Widget * Get(   int32_t widgetId);
	};
};

#endif

