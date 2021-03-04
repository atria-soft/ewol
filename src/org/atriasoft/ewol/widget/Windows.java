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
		using Windows = ememory::Ptr<ewol::widget::Windows>;
		using WindowsWeak = ememory::WeakPtr<ewol::widget::Windows>;
		/**
		 * @brief Windows basic interface
		 */
		class Windows : public Widget {
			public:
				eproperty::Value<etk::Uri> propertyColorConfiguration; //!< Configuration file of the windows theme
				eproperty::Value<String> propertyTitle; //!< Current title of the windows
			protected:
				ememory::Ptr<ewol::resource::ColorFile> this.resourceColor; //!< theme color property (name of file in @ref propertyColorConfiguration)
				int this.colorBg; //!< Default background color of the windows
			protected:
				Windows();
				void init() ;
			public:
				 ~Windows();
			// internal event at ewol system:
			public:
				void sysDraw();
			protected:
				Widget this.subWidget; //!< main sub-widget of the Windows.
			public:
				/**
				 * @brief Set the main widget of the application.
				 * @param[in] _widget Widget to set in the windows.
				 */
				void setSubWidget(Widget _widget);
			protected:
				List<Widget> this.popUpWidgetList; //!< List of pop-up displayed
			public:
				/**
				 * @brief Add a pop-up on the Windows.
				 * @param[in] _widget Widget to set on top of the pop-up.
				 */
				void popUpWidgetPush(Widget _widget);
				/**
				 * @brief Remove the pop-up on top.
				 */
				void popUpWidgetPop();
				/**
				 * @brief Get the number of pop-up
				 * @return Count of pop-up
				 */
				int popUpCount() {
					return this.popUpWidgetList.size();
				}
			protected:
				void systemDraw( ewol::DrawProperty _displayProp) ;
			public:
				void onRegenerateDisplay() ;
				void onChangeSize() ;
				Widget getWidgetAtPos( Vector2f _pos) ;
				void requestDestroyFromChild( EwolObject _child) ;
				EwolObject getSubObjectNamed( String _objectName) ;
				void drawWidgetTree(int _level=0) ;
			protected:
				/**
				 * @brief Called when property change: Title
				 */
				 void onChangePropertyTitle();
				/**
				 * @brief Called when property change: Color configuration file
				 */
				 void onChangePropertyColor();
		};
	}
}

