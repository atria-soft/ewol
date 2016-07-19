/** @file
 * @author Edouard DUPIN
 * @copyright 2011, Edouard DUPIN, all right reserved
 * @license APACHE v2.0 (see license file)
 */
#pragma once

#include <etk/types.h>
#include <ewol/debug.h>
#include <vector>
#include <unordered_map>
#include <ewol/widget/Widget.h>

namespace ewol {
	namespace widget {
		class Manager {
			public:
				Manager();
				virtual ~Manager();
			// ---------------------------------------------
			// --  Focus area
			// ---------------------------------------------
			private:
				ewol::WidgetWeak m_focusWidgetDefault; //!< default focus when no current focus is set
				ewol::WidgetWeak m_focusWidgetCurrent; //!< Currect focus selected
			public:
				/**
				 * @brief Request a focus on a specify widget.
				 * @param[in] _newWidget Widget that might get the focus.
				 */
				void focusKeep(ewol::WidgetShared _newWidget);
				/**
				 * @brief Set the default focus when none selected.
				 * @param[in] _newWidget Widget that might get the focus (when nothing else).
				 */
				void focusSetDefault(ewol::WidgetShared _newWidget);
				/**
				 * @brief Release the current focus (back on default if possible).
				 */
				void focusRelease();
				/**
				 * @brief Get the current Focused widget.
				 * @return The pointer on the current focused element.
				 */
				ewol::WidgetShared focusGet();
			// ---------------------------------------------
			// --  Factory area
			// ---------------------------------------------
			public:
				using widgetCreatorFunction = std::function<ewol::WidgetShared()>; //!< funtion factory basic definition
			private:
				std::unordered_map<std::string, widgetCreatorFunction> m_creatorList; //!< List of factory of a widget
			public:
				/**
				 * @brief add a factory of a specific widget.
				 * @param[in] _name Name of the widget that is associated of the factory.
				 * @param[in] _factory Function pointer to create the widget
				 */
				void addWidgetCreator(const std::string& _name, widgetCreatorFunction _factory);
				/**
				 * @brief Create a widget with his name.
				 * @param[in] _name Name of the widget to create.
				 * @return The widget created (nullptr if it does not exist).
				 */
				ewol::WidgetShared create(const std::string& _name);
				/**
				 * @brief Check if an Widget exist
				 * @param[in] _name Name of the widget to check.
				 * @return true The Widget exist.
				 * @return false The Widget Does NOT exist.
				 */
				bool exist(const std::string& _name);
				/**
				 * @brief Get the list of all Widget that can be created.
				 * @return Separate with ',' string list.
				 */
				std::string list();
			// ---------------------------------------------
			// --  Something change area (TODO: maybe set it in the windows)
			// ---------------------------------------------
			private:
				bool m_haveRedraw; //!< something request a redraw
			private:
				std::function<void()> m_funcRedrawNeeded;
			public:
				/**
				 * @brief Mark the display to redraw
				 */
				void markDrawingIsNeeded();
				/**
				 * @brief Check if a redraw has been requested (set the local value back at false)
				 * @return true if something to be redraw
				 */
				bool isDrawingNeeded();
			private:
				
				/**
				 * @brief Set a callback when we need redraw the display (need by MacOs)
				 * @param[in] _func function to call
				 */
				void setCallbackonRedrawNeeded(const std::function<void()>& _func);
			
		};
	};
};

