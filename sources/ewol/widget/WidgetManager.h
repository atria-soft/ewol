/**
 * @author Edouard DUPIN
 * 
 * @copyright 2011, Edouard DUPIN, all right reserved
 * 
 * @license BSD v3 (see license file)
 */

#ifndef __EWOL_WIDGET_MANAGER_H__
#define __EWOL_WIDGET_MANAGER_H__

#include <etk/Types.h>
#include <ewol/Debug.h>
#include <ewol/oObject/OObject.h>
#include <etk/Vector.h>
#include <ewol/widget/Widget.h>

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

