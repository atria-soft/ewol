/** @file
 * @author Edouard DUPIN
 * @copyright 2011, Edouard DUPIN, all right reserved
 * @license MPL v2.0 (see license file)
 */
#pragma once

#include <etk/types.hpp>
#include <ewol/debug.hpp>
#include <etk/Vector.hpp>
#include <ewol/widget/Widget.hpp>

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
				using widgetCreatorFunction = etk::Function<ewol::WidgetShared()>; //!< funtion factory basic definition
				using widgetCreatorFunctionXml = etk::Function<ewol::WidgetShared(const exml::Element& _node)>; //!< funtion factory basic definition
			private:
				etk::Map<etk::String, widgetCreatorFunction> m_creatorList; //!< List of factory of a widget
				etk::Map<etk::String, widgetCreatorFunctionXml> m_creatorListXml; //!< List of factory of a widget
			public:
				/**
				 * @brief add a factory of a specific widget.
				 * @param[in] _name Name of the widget that is associated of the factory.
				 * @param[in] _factory Function pointer to create the widget
				 * @param[in] _factoryXml Function pointer to create the widget with XML node for parsing of XML
				 */
				void addWidgetCreator(const etk::String& _name, widgetCreatorFunction _factory, widgetCreatorFunctionXml _factoryXml);
				/**
				 * @brief Create a widget with his name.
				 * @param[in] _name Name of the widget to create.
				 * @return The widget created (null if it does not exist).
				 */
				ewol::WidgetShared create(const etk::String& _name);
				/**
				 * @brief Create a widget with his name.
				 * @param[in] _name Name of the widget to create.
				 * @param[in] _node Reference on the XML node.
				 * @return The widget created (null if it does not exist).
				 */
				ewol::WidgetShared create(const etk::String& _name, const exml::Element& _node);
				/**
				 * @brief Check if an Widget exist
				 * @param[in] _name Name of the widget to check.
				 * @return true The Widget exist.
				 * @return false The Widget Does NOT exist.
				 */
				bool exist(const etk::String& _name);
				/**
				 * @brief Get the list of all Widget that can be created.
				 * @return Separate with ',' string list.
				 */
				etk::String list();
			// ---------------------------------------------
			// --  Something change area (TODO: maybe set it in the windows)
			// ---------------------------------------------
			private:
				bool m_haveRedraw; //!< something request a redraw
			private:
				etk::Function<void()> m_funcRedrawNeeded;
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
				void setCallbackonRedrawNeeded(const etk::Function<void()>& _func);
			
		};
	};
};

