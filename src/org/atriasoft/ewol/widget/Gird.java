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
		using GirdShared = ememory::SharedPtr<ewol::widget::Gird>;
		using GirdWeak = ememory::WeakPtr<ewol::widget::Gird>;
		/**
		 * @ingroup ewolWidgetGroup
		 */
		class Gird :public ewol::Widget {
			private:
				class GirdProperties {
					public:
						ewol::WidgetShared widget;
						int32_t row;
						int32_t col;
				};
				int32_t m_sizeRow; //!< size of all lines (row) (if set (otherwise 0))  == > we have a only one size ==> multiple size will have no use ...
				int32_t m_uniformSizeRow;
				etk::Vector<int32_t> m_sizeCol; //!< size of all colomn (if set (otherwise 0))
				etk::Vector<GirdProperties> m_subWidget; //!< all sub widget are contained in this element
				ewol::WidgetShared m_tmpWidget; //!< use when replace a widget ...
				bool m_gavityButtom;
			protected:
				/**
				 * @brief Constructor
				 */
				Gird();
			public:
				DECLARE_WIDGET_FACTORY(Gird, "Gird");
				/**
				 * @brief Desstructor
				 */
				virtual ~Gird();
				/**
				 * @brief set the number of colomn
				 * @param[in] colNumber Nuber of colomn
				 */
				void setColNumber(int32_t _colNumber);
				/**
				 * @brief change a size view of a colomn.
				 * @param[in] colId Id of the colomn [0..x].
				 * @param[in] size size of the colomn.
				 */
				void setColSize(int32_t _colId, int32_t _size);
				/**
				 * @brief change a size view of a line.
				 * @param[in] size size of the line.
				 */
				void setRowSize(int32_t _size);
				/**
				 * @brief get the size view of a colomn.
				 * @param[in] colId Id of the colomn [0..x].
				 * @return The size of the colomn.
				 */
				int32_t getColSize(int32_t _colId);
				/**
				 * @brief get the size view of the lines.
				 * @return The size of the lines.
				 */
				int32_t getRowSize();
				/**
				 * @brief set the gravity of the widget on the Button (index 0 is on buttom)
				 */
				void setGravityButtom() {
					m_gavityButtom = true;
					markToRedraw();
				}
				/**
				 * @brief set the gravity of the widget on the Top (index 0 is on top)
				 */
				void setGravityTop() {
					m_gavityButtom = false;
					markToRedraw();
				}
			public:
				/**
				 * @brief remove all sub element from the widget.
				 */
				virtual void subWidgetRemoveAll();
				/**
				 * @brief add at end position a Widget (note : This system use an inverted phylisophie (button to top, and left to right)
				 * @param[in] _colId Id of the colomn [0..x].
				 * @param[in] _rowId Id of the row [0..y].
				 * @param[in] _newWidget the element pointer
				 */
				virtual void subWidgetAdd(int32_t _colId, int32_t _rowId, ewol::WidgetShared _newWidget);
				/**
				 * @brief remove definitly a widget from the system and this Gird.
				 * @param[in] _newWidget the element pointer.
				 */
				virtual void subWidgetRemove(ewol::WidgetShared _newWidget);
				/**
				 * @brief remove definitly a widget from the system and this Gird.
				 * @param[in] _colId Id of the colomn [0..x].
				 * @param[in] _rowId Id of the row [0..y].
				 */
				virtual void subWidgetRemove(int32_t _colId, int32_t _rowId);
				/**
				 * @brief Just unlick the specify widget, this function does not remove it from the system (if you can, do nt use it ...).
				 * @param[in] _newWidget the element pointer.
				 */
				virtual void subWidgetUnLink(ewol::WidgetShared _newWidget);
				/**
				 * @brief Just unlick the specify widget, this function does not remove it from the system (if you can, do nt use it ...).
				 * @param[in] _colId Id of the colomn [0..x].
				 * @param[in] _rowId Id of the row [0..y].
				 */
				virtual void subWidgetUnLink(int32_t _colId, int32_t _rowId);
			private:
				// TODO : property
				ivec2 m_borderSize; //!< Border size needed for all the display
			public:
				/**
				 * @brief set the current border size of the current element:
				 * @param[in] _newBorderSize The border size to set (0 if not used)
				 */
				void setBorderSize(const ivec2& _newBorderSize);
				/**
				 * @brief get the current border size of the current element:
				 * @return the border size (0 if not used)
				 */
				const ivec2& getBorderSize() {
					return m_borderSize;
				};
			public:
				virtual void systemDraw(const ewol::DrawProperty& _displayProp) override;
				virtual void onRegenerateDisplay() override;
				virtual ewol::WidgetShared getWidgetAtPos(const vec2& pos) override;
				virtual void onChangeSize() override;
				virtual void calculateMinMaxSize() override;
		};
	};
};
