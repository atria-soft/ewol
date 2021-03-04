/** @file
 * @author Edouard DUPIN
 * @copyright 2011, Edouard DUPIN, all right reserved
 * @license MPL v2.0 (see license file)
 */
#pragma once

#include <etk/types.hpp>
#include <ewol/debug.hpp>
#include <ewol/widget/Widget.hpp>

namespace ewol {
	namespace widget {
		class ContainerN;
		using ContainerN = ememory::Ptr<ewol::widget::ContainerN>;
		using ContainerNWeak = ememory::WeakPtr<ewol::widget::ContainerN>;
		/**
		 * @ingroup ewolWidgetGroup
		 * @brief the Cotainer widget is a widget that have an only one subWidget
		 */
		class ContainerN : public Widget {
			public: // properties:
				eproperty::Value<Vector2b> propertyLockExpand; //!< Lock the expend of the sub widget to this one  == > this permit to limit bigger subWidget
			protected:
				List<Widget> this.subWidget;
			protected:
				/**
				 * @brief Constructor
				 */
				ContainerN();
			public:
				/**
				 * @brief Destructor
				 */
				 ~ContainerN();
			protected:
				Vector2b this.subExpend; //!< reference of the sub element expention requested.
				// herited function
				 Vector2b canExpand() ;
			public:
				/**
				 * @brief remove all sub element from the widget.
				 */
				 void subWidgetRemoveAll();
				/**
				 * @brief remove all sub element from the widget (delayed to prevent remove in the callbback).
				 */
				 void subWidgetRemoveAllDelayed();
				/**
				 * @brief Replace a old subwidget with a new one.
				 * @param[in] _oldWidget The widget to replace.
				 * @param[in] _newWidget The widget to set.
				 */
				 void subWidgetReplace(Widget _oldWidget,
				                              Widget _newWidget);
				/**
				 * @brief add at end position a Widget (note : This system use an inverted phylisophie (button to top, and left to right)
				 * @param[in] _newWidget the element pointer
				 * @return the ID of the set element
				 */
				 int subWidgetAdd(Widget _newWidget);
				//! @previous
				  int subWidgetAddBack(Widget _newWidget) {
					return subWidgetAdd(_newWidget);
				};
				//! @previous
				  int subWidgetAddEnd(Widget _newWidget) {
					return subWidgetAdd(_newWidget);
				};
				/**
				 * @brief add at start position a Widget (note : This system use an inverted phylisophie (button to top, and left to right)
				 * @param[in] _newWidget the element pointer
				 * @return the ID of the set element
				 */
				 int subWidgetAddStart(Widget _newWidget);
				//! @previous
				  int subWidgetAddFront(Widget _newWidget) {
					return subWidgetAddStart(_newWidget);
				};
				/**
				 * @brief remove definitly a widget from the system and this layer.
				 * @param[in] _newWidget the element pointer.
				 */
				 void subWidgetRemove(Widget _newWidget);
				/**
				 * @brief Just unlick the specify widget, this function does not remove it from the system (if you can, do nt use it ...)
				 * @param[in] _newWidget the element pointer.
				 */
				 void subWidgetUnLink(Widget _newWidget);
			public:
				void systemDraw( ewol::DrawProperty _displayProp) ;
				void onRegenerateDisplay() ;
				void onChangeSize() ;
				void calculateMinMaxSize() ;
				Widget getWidgetAtPos( Vector2f _pos) ;
				EwolObject getSubObjectNamed( String _objectName) ;
				boolean loadXML( exml::Element _node) ;
				void setOffset( Vector2f _newVal) ;
				void requestDestroyFromChild( EwolObject _child) ;
				void drawWidgetTree(int _level=0) ;
			protected:
				 void onChangePropertyLockExpand();
		};
	};
};
