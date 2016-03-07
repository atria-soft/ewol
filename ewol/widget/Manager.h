/**
 * @author Edouard DUPIN
 * 
 * @copyright 2011, Edouard DUPIN, all right reserved
 * 
 * @license APACHE v2.0 (see license file)
 */
#pragma once

#include <etk/types.h>
#include <ewol/debug.h>
#include <vector>
#include <etk/Hash.h>
#include <ewol/widget/Widget.h>

namespace ewol {
	namespace widget {
		class Manager {
			public:
				using creator_tf = std::function<ewol::WidgetShared()>;
			private:
				// For the focus Management
				ewol::WidgetWeak m_focusWidgetDefault;
				ewol::WidgetWeak m_focusWidgetCurrent;
				bool m_havePeriodic;
				bool m_haveRedraw;
				etk::Hash<creator_tf> m_creatorList;
			public:
				Manager();
				virtual ~Manager();
				
				void focusKeep(const ewol::WidgetShared& _newWidget); // set the focus at the specific widget
				void focusSetDefault(const ewol::WidgetShared& _newWidget); // select the default focus getter
				void focusRelease(); // release focus from the current widget to the default
				ewol::WidgetShared focusGet();
				void focusRemoveIfRemove(const ewol::WidgetShared& _newWidget);
			private:
				std::function<void()> m_funcRedrawNeeded;
			public:
				void markDrawingIsNeeded();
				bool isDrawingNeeded();
				void setCallbackonRedrawNeeded(const std::function<void()>& _func);
				
				// element that generate the list of elements
				void addWidgetCreator(const std::string& _name, creator_tf _pointer);
				ewol::WidgetShared create(const std::string& _name);
				bool exist(const std::string& _name);
				std::string list();
			private:
				void periodicCallUpdateCount();
		};
	};
};

