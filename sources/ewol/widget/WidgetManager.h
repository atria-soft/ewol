/**
 * @author Edouard DUPIN
 * 
 * @copyright 2011, Edouard DUPIN, all right reserved
 * 
 * @license BSD v3 (see license file)
 */

#ifndef __EWOL_WIDGET_MANAGER_H__
#define __EWOL_WIDGET_MANAGER_H__

#include <etk/types.h>
#include <ewol/debug.h>
#include <etk/Vector.h>
#include <etk/Hach.h>
#include <ewol/widget/Widget.h>

namespace ewol {
	class WidgetManager {
		public:
			typedef ewol::Widget* (*creator_tf)(void);
		private:
			// For the focus Management
			ewol::Widget* m_focusWidgetDefault;
			ewol::Widget* m_focusWidgetCurrent;
			etk::Vector<ewol::Widget*> m_listOfPeriodicWidget;
			bool m_havePeriodic;
			bool m_haveRedraw;
			etk::Hash<creator_tf> m_creatorList;
			int64_t m_applWakeUpTime; //!< Time of the application initialize
			int64_t m_lastPeriodicCallTime; //!< last call time ...
		public:
			WidgetManager(void);
			~WidgetManager(void);
			// need to call when remove a widget to clear all dependency of the focus system
			void rm(ewol::Widget* _newWidget);
			
			void focusKeep(ewol::Widget* _newWidget); // set the focus at the specific widget
			void focusSetDefault(ewol::Widget* _newWidget); // select the default focus getter
			void focusRelease(void); // release focus from the current widget to the default
			ewol::Widget* focusGet(void);
			void focusRemoveIfRemove(ewol::Widget* _newWidget);
			
			void periodicCallAdd(ewol::Widget* _pWidget);
			void periodicCallRm(ewol::Widget* _pWidget);
			void periodicCall(int64_t _localTime);
			void periodicCallResume(int64_t _localTime);
			bool periodicCallHave(void);
			
			void markDrawingIsNeeded(void);
			bool isDrawingNeeded(void);
			
			// element that generate the list of elements
			void addWidgetCreator(const etk::UString& _name, creator_tf _pointer);
			ewol::Widget* create(const etk::UString& _name);
			bool exist(const etk::UString& _name);
			etk::UString list(void);
	};
};

#endif

