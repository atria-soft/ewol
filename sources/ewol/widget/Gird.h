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
#include <etk/Vector.h>
#include <ewol/debug.h>
#include <ewol/widget/Widget.h>

namespace widget {
	class Gird :public ewol::Widget
	{
		private:
			class GirdProperties {
				public:
					ewol::Widget* widget;
					int32_t row;
					int32_t col;
			};
			int32_t m_sizeRow; //!< size of all Lines (row) (if set (otherwise 0)) ==> we have a only one size ==> multiple size will have no use ...
			int32_t m_uniformSizeRow;
			etk::Vector<int32_t> m_sizeCol; //!< size of all colomn (if set (otherwise 0))
			etk::Vector<GirdProperties> m_subWidget; //!< all sub widget are contained in this element
			ewol::Widget* m_tmpWidget; //!< use when replace a widget ...
			bool m_gavityButtom;
		public:
			/**
			 * @brief Constructor
			 */
			Gird(int32_t colNumber=1);
			/**
			 * @brief Desstructor
			 */
			virtual ~Gird(void);
			/**
			 * @brief Set the number of colomn
			 * @param[in] colNumber Nuber of colomn
			 */
			void SetColNumber(int32_t colNumber);
			/**
			 * @brief Change a size view of a colomn.
			 * @param[in] colId Id of the colomn [0..x].
			 * @param[in] size Size of the colomn.
			 */
			void SetColSize(int32_t colId, int32_t size);
			/**
			 * @brief Change a size view of a Line.
			 * @param[in] size Size of the line.
			 */
			void SetRowSize(int32_t size);
			/**
			 * @brief Get the size view of a colomn.
			 * @param[in] colId Id of the colomn [0..x].
			 * @return The size of the colomn.
			 */
			int32_t GetColSize(int32_t colId);
			/**
			 * @brief Get the size view of the Lines.
			 * @return The size of the lines.
			 */
			int32_t GetRowSize(void);
			/**
			 * @brief Set the gravity of the widget on the Button (index 0 is on buttom)
			 */
			void SetGravityButtom(void)
			{
				m_gavityButtom = true;
				MarkToRedraw();
			}
			/**
			 * @brief Set the gravity of the widget on the Top (index 0 is on top)
			 */
			void SetGravityTop(void)
			{
				m_gavityButtom = false;
				MarkToRedraw();
			}
		public:
			/**
			 * @brief Remove all sub element from the widget.
			 */
			virtual void SubWidgetRemoveAll(void);
			/**
			 * @brief Add at end position a Widget (note : This system use an inverted phylisophie (button to top, and left to right)
			 * @param[in] colId Id of the colomn [0..x].
			 * @param[in] rowId Id of the row [0..y].
			 * @param[in] newWidget the element pointer
			 */
			virtual void SubWidgetAdd(int32_t colId, int32_t rowId, ewol::Widget* newWidget);
			/**
			 * @brief Remove definitly a widget from the system and this Gird.
			 * @param[in] newWidget the element pointer.
			 */
			virtual void SubWidgetRemove(ewol::Widget* newWidget);
			/**
			 * @brief Remove definitly a widget from the system and this Gird.
			 * @param[in] colId Id of the colomn [0..x].
			 * @param[in] rowId Id of the row [0..y].
			 */
			virtual void SubWidgetRemove(int32_t colId, int32_t rowId);
			/**
			 * @brief Just unlick the specify widget, this function does not remove it from the system (if you can, do nt use it ...).
			 * @param[in] newWidget the element pointer.
			 */
			virtual void SubWidgetUnLink(ewol::Widget* newWidget);
			/**
			 * @brief Just unlick the specify widget, this function does not remove it from the system (if you can, do nt use it ...).
			 * @param[in] colId Id of the colomn [0..x].
			 * @param[in] rowId Id of the row [0..y].
			 */
			virtual void SubWidgetUnLink(int32_t colId, int32_t rowId);
		private:
			ivec2 m_borderSize; //!< Border size needed for all the display
		public:
			/**
			 * @brief Set the current border size of the current element:
			 * @param[in] newBorderSize The border size to set (0 if not used)
			 */
			void SetBorderSize(const ivec2& newBorderSize);
			/**
			 * @brief Get the current border size of the current element:
			 * @return the border size (0 if not used)
			 */
			const ivec2& GetBorderSize(void) { return m_borderSize; };
		protected: // Derived function
			virtual void OnDraw(ewol::DrawProperty& displayProp);
		public: // Derived function
			virtual void OnRegenerateDisplay(void);
			virtual ewol::Widget* GetWidgetAtPos(vec2 pos);
			virtual void OnObjectRemove(ewol::EObject* removeObject);
			virtual const char * const GetObjectType(void) { return "Ewol::Sizer"; };
			virtual bool CalculateSize(float availlableX, float availlableY);
			virtual bool CalculateMinSize(void);
	};
	
};

#endif
