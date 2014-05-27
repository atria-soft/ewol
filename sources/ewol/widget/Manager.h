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
#include <vector>
#include <etk/Hash.h>
#include <ewol/widget/Widget.h>
#include <ewol/object/RemoveEvent.h>

namespace ewol {
	namespace widget {
		class Manager {
			public:
				typedef ewol::Widget* (*creator_tf)();
			private:
				// For the focus Management
				ewol::object::Shared<ewol::Widget> m_focusWidgetDefault;
				ewol::object::Shared<ewol::Widget> m_focusWidgetCurrent;
				std::vector<ewol::object::Shared<ewol::Widget>> m_listOfPeriodicWidget;
				bool m_havePeriodic;
				bool m_haveRedraw;
				etk::Hash<creator_tf> m_creatorList;
				int64_t m_applWakeUpTime; //!< Time of the application initialize
				int64_t m_lastPeriodicCallTime; //!< last call time ...
			public:
				Manager();
				virtual ~Manager();
				
				void focusKeep(const ewol::object::Shared<ewol::Widget>& _newWidget); // set the focus at the specific widget
				void focusSetDefault(const ewol::object::Shared<ewol::Widget>& _newWidget); // select the default focus getter
				void focusRelease(); // release focus from the current widget to the default
				const ewol::object::Shared<ewol::Widget>& focusGet();
				void focusRemoveIfRemove(const ewol::object::Shared<ewol::Widget>& _newWidget);
				
				void periodicCallAdd(const ewol::object::Shared<ewol::Widget>& _pWidget);
				void periodicCallRm(const ewol::object::Shared<ewol::Widget>& _pWidget);
				void periodicCall(int64_t _localTime);
				void periodicCallResume(int64_t _localTime);
				bool periodicCallHave();
				
				void markDrawingIsNeeded();
				bool isDrawingNeeded();
				
				// element that generate the list of elements
				void addWidgetCreator(const std::string& _name, creator_tf _pointer);
				ewol::object::Shared<ewol::Widget> create(const std::string& _name);
				bool exist(const std::string& _name);
				std::string list();
				virtual void onObjectRemove(const ewol::object::Shared<ewol::Object>& _object);
			private:
				void periodicCallUpdateCount();
		};
	};
};

#endif

