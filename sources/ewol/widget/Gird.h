/**
 * @author Edouard DUPIN
 * 
 * @copyright 2011, Edouard DUPIN, all right reserved
 * 
 * @license BSD v3 (see license file)
 */

#ifndef __EWOL_WIDGET_GIRD_H__
#define __EWOL_WIDGET_GIRD_H__

#include <etk/types.h>
#include <vector>
#include <ewol/debug.h>
#include <ewol/widget/Widget.h>
#include <ewol/widget/WidgetManager.h>

namespace widget {
	/**
	 * @ingroup ewolWidgetGroup
	 */
	class Gird :public ewol::Widget {
		public:
			static void init(ewol::WidgetManager& _widgetManager);
		private:
			class GirdProperties {
				public:
					ewol::Widget* widget;
					int32_t row;
					int32_t col;
			};
			int32_t m_sizeRow; //!< size of all lines (row) (if set (otherwise 0))  == > we have a only one size ==> multiple size will have no use ...
			int32_t m_uniformSizeRow;
			std::vector<int32_t> m_sizeCol; //!< size of all colomn (if set (otherwise 0))
			std::vector<GirdProperties> m_subWidget; //!< all sub widget are contained in this element
			ewol::Widget* m_tmpWidget; //!< use when replace a widget ...
			bool m_gavityButtom;
		public:
			/**
			 * @brief Constructor
			 */
			Gird(int32_t _colNumber=1);
			/**
			 * @brief Desstructor
			 */
			virtual ~Gird(void);
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
			int32_t getRowSize(void);
			/**
			 * @brief set the gravity of the widget on the Button (index 0 is on buttom)
			 */
			void setGravityButtom(void) {
				m_gavityButtom = true;
				markToRedraw();
			}
			/**
			 * @brief set the gravity of the widget on the Top (index 0 is on top)
			 */
			void setGravityTop(void) {
				m_gavityButtom = false;
				markToRedraw();
			}
		public:
			/**
			 * @brief remove all sub element from the widget.
			 */
			virtual void subWidgetRemoveAll(void);
			/**
			 * @brief add at end position a Widget (note : This system use an inverted phylisophie (button to top, and left to right)
			 * @param[in] _colId Id of the colomn [0..x].
			 * @param[in] _rowId Id of the row [0..y].
			 * @param[in] _newWidget the element pointer
			 */
			virtual void subWidgetAdd(int32_t _colId, int32_t _rowId, ewol::Widget* _newWidget);
			/**
			 * @brief remove definitly a widget from the system and this Gird.
			 * @param[in] _newWidget the element pointer.
			 */
			virtual void subWidgetRemove(ewol::Widget* _newWidget);
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
			virtual void subWidgetUnLink(ewol::Widget* _newWidget);
			/**
			 * @brief Just unlick the specify widget, this function does not remove it from the system (if you can, do nt use it ...).
			 * @param[in] _colId Id of the colomn [0..x].
			 * @param[in] _rowId Id of the row [0..y].
			 */
			virtual void subWidgetUnLink(int32_t _colId, int32_t _rowId);
		private:
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
			const ivec2& getBorderSize(void) { return m_borderSize; };
		public: // Derived function
			virtual void systemDraw(const ewol::DrawProperty& _displayProp);
			virtual void onRegenerateDisplay(void);
			virtual ewol::Widget* getWidgetAtPos(const vec2& pos);
			virtual void onObjectRemove(ewol::EObject* _removeObject);
			virtual void calculateSize(const vec2& _availlable);
			virtual void calculateMinMaxSize(void);
	};
	
};

#endif
