/** @file
 * @author Edouard DUPIN
 * @copyright 2011, Edouard DUPIN, all right reserved
 * @license MPL v2.0 (see license file)
 */
#pragma once

#include <etk/types.hpp>
#include <ewol/debug.hpp>
#include <ewol/widget/Widget.hpp>
#include <etk/Color.hpp>
#include <ewol/resource/ColorFile.hpp>

namespace ewol {
	namespace widget {
		class Windows;
		using WindowsShared = ememory::SharedPtr<ewol::widget::Windows>;
		using WindowsWeak = ememory::WeakPtr<ewol::widget::Windows>;
		/**
		 * @brief Windows basic interface
		 */
		class Windows : public ewol::Widget {
			public:
				eproperty::Value<etk::Uri> propertyColorConfiguration; //!< Configuration file of the windows theme
				eproperty::Value<etk::String> propertyTitle; //!< Current title of the windows
			protected:
				ememory::SharedPtr<ewol::resource::ColorFile> m_resourceColor; //!< theme color property (name of file in @ref propertyColorConfiguration)
				int32_t m_colorBg; //!< Default background color of the windows
			protected:
				Windows();
				void init() override;
			public:
				virtual ~Windows();
			// internal event at ewol system:
			public:
				void sysDraw();
			protected:
				ewol::WidgetShared m_subWidget; //!< main sub-widget of the Windows.
			public:
				/**
				 * @brief Set the main widget of the application.
				 * @param[in] _widget Widget to set in the windows.
				 */
				void setSubWidget(ewol::WidgetShared _widget);
			protected:
				etk::Vector<ewol::WidgetShared> m_popUpWidgetList; //!< List of pop-up displayed
			public:
				/**
				 * @brief Add a pop-up on the Windows.
				 * @param[in] _widget Widget to set on top of the pop-up.
				 */
				void popUpWidgetPush(ewol::WidgetShared _widget);
				/**
				 * @brief Remove the pop-up on top.
				 */
				void popUpWidgetPop();
				/**
				 * @brief Get the number of pop-up
				 * @return Count of pop-up
				 */
				size_t popUpCount() {
					return m_popUpWidgetList.size();
				}
			protected:
				void systemDraw(const ewol::DrawProperty& _displayProp) override;
			public:
				void onRegenerateDisplay() override;
				void onChangeSize() override;
				ewol::WidgetShared getWidgetAtPos(const vec2& _pos) override;
				void requestDestroyFromChild(const ewol::ObjectShared& _child) override;
				ewol::ObjectShared getSubObjectNamed(const etk::String& _objectName) override;
				void drawWidgetTree(int32_t _level=0) override;
			protected:
				/**
				 * @brief Called when property change: Title
				 */
				virtual void onChangePropertyTitle();
				/**
				 * @brief Called when property change: Color configuration file
				 */
				virtual void onChangePropertyColor();
		};
	}
}

