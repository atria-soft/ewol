/** @file
 * @author Edouard DUPIN
 * @copyright 2011, Edouard DUPIN, all right reserved
 * @license MPL v2.0 (see license file)
 */
#pragma once

#include <etk/types.hpp>
#include <ewol/debug.hpp>
#include <ewol/widget/Widget.hpp>
#include <ewol/Padding.hpp>

namespace ewol {
	namespace widget {
		class Container2;
		using Container2 = ememory::Ptr<ewol::widget::Container2>;
		using Container2Weak = ememory::WeakPtr<ewol::widget::Container2>;
		/**
		 * @ingroup ewolWidgetGroup
		 * the Cotainer widget is a widget that have an only one subWidget
		 */
		class Container2 : public Widget {
			protected:
				Widget this.subWidget[2]; //!< 2 subwidget possible
				int this.idWidgetDisplayed; //!< current widget displayed
			protected:
				/**
				 * Constructor
				 * @param _subElement Widget to set on the normal position
				 * @param _subElementToggle Widget to set on the toggle position
				 */
				Container2();
			public:
				/**
				 * Destructor
				 */
				 ~Container2();
			private:
				/**
				 * Specify the current widget
				 * @param _subWidget Widget to add normal
				 * @param _idWidget Id of the widget to set
				 */
				void setSubWidget(Widget _subWidget, int _idWidget);
			public:
				/**
				 * Specify the current widget
				 * @param _subWidget Widget to add normal
				 */
				void setSubWidget(Widget _subWidget) {
					setSubWidget(_subWidget, 0);
				}
				/**
				 * Specify the current toggle widget
				 * @param _subWidget Widget to add Toggle
				 */
				void setSubWidgetToggle(Widget _subWidget) {
					setSubWidget(_subWidget, 1);
				}
			private:
				/**
				 * get the current displayed composition
				 * @param _idWidget Id of the widget to set
				 * @return The base widget
				 */
				Widget getSubWidget(int _idWidget)  {
					return this.subWidget[_idWidget];
				};
			public:
				/**
				 * get the current displayed composition
				 * @return The base widget
				 */
				Widget getSubWidget()  {
					return getSubWidget(0);
				};
				/**
				 * get the current displayed composition
				 * @return The toggle widget
				 */
				Widget getSubWidgetToggle()  {
					return getSubWidget(1);
				};
			private:
				/**
				 * remove the subWidget node (async).
				 * @param _idWidget Id of the widget to set
				 */
				void subWidgetRemove(int _idWidget);
			public:
				/**
				 * remove the subWidget node (async).
				 */
				void subWidgetRemove() {
					subWidgetRemove(0);
				}
				/**
				 * remove the subWidget Toggle node (async).
				 */
				void subWidgetRemoveToggle() {
					subWidgetRemove(1);
				}
			private:
				/**
				 * Unlink the subwidget Node.
				 * @param _idWidget Id of the widget to set
				 */
				void subWidgetUnLink(int _idWidget);
			public:
				/**
				 * Unlink the subwidget Node.
				 */
				void subWidgetUnLink() {
					subWidgetUnLink(0);
				}
				/**
				 * Unlink the subwidget Toggle Node.
				 */
				void subWidgetUnLinkToggle() {
					subWidgetUnLink(1);
				}
			protected:
				/**
				 * Parent set the possible diplay size of the current widget whith his own possibilities
				 *        By default this save the widget available size in the widget size
				 * @param _padding Padding of the widget.
				 * @note : INTERNAL EWOL SYSTEM
				 */
				 ewol::Padding onChangeSizePadded( ewol::Padding _padding = ewol::Padding(0,0,0,0));
				/**
				 * calculate the minimum and maximum size (need to estimate expend properties of the widget)
				 * @param _padding Padding of the widget.
				 * @note : INTERNAL EWOL SYSTEM
				 */
				 void calculateMinMaxSizePadded( ewol::Padding _padding = ewol::Padding(0,0,0,0));
				/**
				 * Called when parsing a XML and detect the presence of a second Widget
				 */
				 void onDetectPresenceToggleWidget() {}
				/**
				 * convert ID of the widget if not existed
				 * @param _id Id of the widget to display.
				 * @return the id of the widget displayable
				 */
				int convertId(int _id) {
					if (this.subWidget[_id] == null) {
						return (_id+1)%2;
					}
					return _id;
				}
				/**
				 * Replace a old subwidget with a new one.
				 * @param _oldWidget The widget to replace.
				 * @param _newWidget The widget to set.
				 */
				 void subWidgetReplace( Widget _oldWidget,
				                               Widget _newWidget);
			public:
				void systemDraw( ewol::DrawProperty _displayProp) ;
				void onRegenerateDisplay() ;
				void onChangeSize()  {
					onChangeSizePadded();
				}
				void calculateMinMaxSize()  {
					calculateMinMaxSizePadded();
				}
				EwolObject getSubObjectNamed( String _objectName) ;
				boolean loadXML( exml::Element _node) ;
				void setOffset( Vector2f _newVal) ;
				void requestDestroyFromChild( EwolObject _child) ;
				void drawWidgetTree(int _level=0) ;
		};
	};
};
