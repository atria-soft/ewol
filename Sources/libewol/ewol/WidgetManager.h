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
		// need to call when remove a widget to clear all dependency of the focus system
		void           Rm(    ewol::Widget * newWidget);
		
		void           FocusKeep(      ewol::Widget * newWidget); // set the focus at the specific widget
		void           FocusSetDefault(ewol::Widget * newWidget); // select the default focus getter
		void           FocusRelease(   void); // Release focus from the current widget to the default
		ewol::Widget * FocusGet(       void);
		void           FocusRemoveIfRemove(ewol::Widget * newWidget);
		
		void           PeriodicCallAdd(ewol::Widget * pWidget);
		void           PeriodicCallRm( ewol::Widget * pWidget);
		void           PeriodicCall(int64_t localTime);
		bool           PeriodicCallHave(void);
		
		void           MarkDrawingIsNeeded(void);
		bool           IsDrawingNeeded(void);
	};
};

#endif

