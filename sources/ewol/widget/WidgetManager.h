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

namespace ewol
{
	class WidgetManager
	{
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
			void Rm(ewol::Widget* _newWidget);
			
			void FocusKeep(ewol::Widget* _newWidget); // set the focus at the specific widget
			void FocusSetDefault(ewol::Widget* _newWidget); // select the default focus getter
			void FocusRelease(void); // Release focus from the current widget to the default
			ewol::Widget* FocusGet(void);
			void FocusRemoveIfRemove(ewol::Widget* _newWidget);
			
			void PeriodicCallAdd(ewol::Widget* _pWidget);
			void PeriodicCallRm(ewol::Widget* _pWidget);
			void PeriodicCall(int64_t _localTime);
			void PeriodicCallResume(int64_t _localTime);
			bool PeriodicCallHave(void);
			
			void MarkDrawingIsNeeded(void);
			bool IsDrawingNeeded(void);
			
			// element that generate the list of elements
			void AddWidgetCreator(const etk::UString& _name, creator_tf _pointer);
			ewol::Widget* Create(const etk::UString& _name);
			bool Exist(const etk::UString& _name);
			etk::UString List(void);
	};
};

#endif

