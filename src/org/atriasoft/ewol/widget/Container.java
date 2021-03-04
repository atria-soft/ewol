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
		class Container;
		using Container = ememory::Ptr<ewol::widget::Container>;
		using ContainerWeak = ememory::WeakPtr<ewol::widget::Container>;
		/**
		 * @ingroup ewolWidgetGroup
		 * @brief the Cotainer widget is a widget that have an only one subWidget
		 */
		class Container : public Widget {
			protected:
				Widget this.subWidget;
			protected:
				/**
				 * @brief Constructor
				 */
				Container();
			public:
				/**
				 * @brief Destructor
				 */
				 ~Container();
			public:
				/**
				 * @brief get the main node widget
				 * @return the requested pointer on the node
				 */
				Widget getSubWidget();
				/**
				 * @brief set the subWidget node widget.
				 * @param[in] _newWidget The widget to add.
				 */
				void setSubWidget(Widget _newWidget);
				/**
				 * @brief Replace a old subwidget with a new one.
				 * @param[in] _oldWidget The widget to replace.
				 * @param[in] _newWidget The widget to set.
				 */
				 void subWidgetReplace( Widget _oldWidget,
				                               Widget _newWidget);
				/**
				 * @brief remove the subWidget node (async).
				 */
				void subWidgetRemove();
				/**
				 * @brief Unlink the subwidget Node.
				 */
				void subWidgetUnLink();
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
		};
	};
};
