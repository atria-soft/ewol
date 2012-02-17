/**
 *******************************************************************************
 * @file ewol/WidgetManager.h
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

#include <etk/Types.h>
#include <ewol/Debug.h>
#include <ewol/OObject.h>
#include <etk/VectorType.h>
#include <ewol/Widget.h>

namespace ewol {
	namespace widgetManager {
		void           Init(  void);
		void           UnInit(void);
		void           Add(   ewol::Widget * newWidget);
		void           Rm(    ewol::Widget * newWidget);
		int32_t        Get(   ewol::Widget * newWidget);
		ewol::Widget * Get(   int32_t widgetId);
		
		void           FocusKeep(      ewol::Widget * newWidget); // set the focus at the specific widget
		void           FocusSetDefault(ewol::Widget * newWidget); // select the default focus getter
		void           FocusRelease(   void); // Release focus from the current widget to the default
		ewol::Widget * FocusGet(       void);
		void           FocusRemoveIfRemove(ewol::Widget * newWidget);
		
		
		int32_t GetDoubleBufferCreate(void);
		int32_t GetDoubleBufferDraw(void);
		void    GetDoubleBufferFlipFlop(void);
		bool    GetDoubleBufferNeedDraw(void);
		void    GetDoubleBufferStartDraw(void);
		void    GetDoubleBufferStopDraw(void);
		
		// For the multithreaded context the widget mus not de removed by the user ==> he might mark if to be remove later with a mutex protection...
		void    MarkWidgetToBeRemoved(int32_t widgetId);
		void    MarkWidgetToBeRemoved(ewol::Widget * expectedWidget);
		void    RemoveAllMarkWidget(void);
	};
};

#endif

