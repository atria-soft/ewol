/** @file
 * @author Edouard DUPIN
 * @copyright 2011, Edouard DUPIN, all right reserved
 * @license MPL v2.0 (see license file)
 */
#pragma once

#include <etk/types.hpp>
#include <etk/Vector.hpp>
#include <ewol/debug.hpp>
#include <ewol/widget/Widget.hpp>
#include <ewol/widget/Manager.hpp>

namespace ewol {
	namespace widget {
		class Gird;
		using Gird = ememory::Ptr<ewol::widget::Gird>;
		using GirdWeak = ememory::WeakPtr<ewol::widget::Gird>;
		/**
		 * @ingroup ewolWidgetGroup
		 */
		class Gird :public Widget {
			private:
				class GirdProperties {
					public:
						Widget widget;
						int row;
						int col;
				};
				int this.sizeRow; //!< size of all lines (row) (if set (otherwise 0))  == > we have a only one size ==> multiple size will have no use ...
				int this.uniformSizeRow;
				List<int> this.sizeCol; //!< size of all colomn (if set (otherwise 0))
				List<GirdProperties> this.subWidget; //!< all sub widget are contained in this element
				Widget this.tmpWidget; //!< use when replace a widget ...
				boolean this.gavityButtom;
			protected:
				/**
				 * Constructor
				 */
				Gird();
			public:
				DECLARE_WIDGET_FACTORY(Gird, "Gird");
				/**
				 * Desstructor
				 */
				 ~Gird();
				/**
				 * set the number of colomn
				 * @param colNumber Nuber of colomn
				 */
				void setColNumber(int _colNumber);
				/**
				 * change a size view of a colomn.
				 * @param colId Id of the colomn [0..x].
				 * @param size size of the colomn.
				 */
				void setColSize(int _colId, int _size);
				/**
				 * change a size view of a line.
				 * @param size size of the line.
				 */
				void setRowSize(int _size);
				/**
				 * get the size view of a colomn.
				 * @param colId Id of the colomn [0..x].
				 * @return The size of the colomn.
				 */
				int getColSize(int _colId);
				/**
				 * get the size view of the lines.
				 * @return The size of the lines.
				 */
				int getRowSize();
				/**
				 * set the gravity of the widget on the Button (index 0 is on buttom)
				 */
				void setGravityButtom() {
					this.gavityButtom = true;
					markToRedraw();
				}
				/**
				 * set the gravity of the widget on the Top (index 0 is on top)
				 */
				void setGravityTop() {
					this.gavityButtom = false;
					markToRedraw();
				}
			public:
				/**
				 * remove all sub element from the widget.
				 */
				 void subWidgetRemoveAll();
				/**
				 * add at end position a Widget (note : This system use an inverted phylisophie (button to top, and left to right)
				 * @param _colId Id of the colomn [0..x].
				 * @param _rowId Id of the row [0..y].
				 * @param _newWidget the element pointer
				 */
				 void subWidgetAdd(int _colId, int _rowId, Widget _newWidget);
				/**
				 * remove definitly a widget from the system and this Gird.
				 * @param _newWidget the element pointer.
				 */
				 void subWidgetRemove(Widget _newWidget);
				/**
				 * remove definitly a widget from the system and this Gird.
				 * @param _colId Id of the colomn [0..x].
				 * @param _rowId Id of the row [0..y].
				 */
				 void subWidgetRemove(int _colId, int _rowId);
				/**
				 * Just unlick the specify widget, this function does not remove it from the system (if you can, do nt use it ...).
				 * @param _newWidget the element pointer.
				 */
				 void subWidgetUnLink(Widget _newWidget);
				/**
				 * Just unlick the specify widget, this function does not remove it from the system (if you can, do nt use it ...).
				 * @param _colId Id of the colomn [0..x].
				 * @param _rowId Id of the row [0..y].
				 */
				 void subWidgetUnLink(int _colId, int _rowId);
			private:
				// TODO : property
				Vector2i this.borderSize; //!< Border size needed for all the display
			public:
				/**
				 * set the current border size of the current element:
				 * @param _newBorderSize The border size to set (0 if not used)
				 */
				void setBorderSize( Vector2i _newBorderSize);
				/**
				 * get the current border size of the current element:
				 * @return the border size (0 if not used)
				 */
				 Vector2i getBorderSize() {
					return this.borderSize;
				};
			public:
				 void systemDraw( ewol::DrawProperty _displayProp) ;
				 void onRegenerateDisplay() ;
				 Widget getWidgetAtPos( Vector2f pos) ;
				 void onChangeSize() ;
				 void calculateMinMaxSize() ;
		};
	};
};
