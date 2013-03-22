/**
 * @author Edouard DUPIN
 * 
 * @copyright 2011, Edouard DUPIN, all right reserved
 * 
 * @license BSD v3 (see license file)
 */

#ifndef __EWOL_SIZER_H__
#define __EWOL_SIZER_H__

#include <etk/types.h>
#include <ewol/debug.h>
#include <ewol/widget/Widget.h>

namespace widget {
	class Sizer :public ewol::Widget
	{
		public:
			typedef enum {
				modeVert,
				modeHori,
			} displayMode_te;
		private:
			etk::Vector<ewol::Widget*> m_subWidget; //!< all sub widget are contained in this element
			displayMode_te m_mode; //!< Methode to display the widget list (vert/hory ...)
		public:
			/**
			 * @brief Constructor
			 * @param[in] mode The mode to display the elements
			 */
			Sizer(displayMode_te mode=widget::Sizer::modeHori);
			/**
			 * @brief Desstructor
			 */
			virtual ~Sizer(void);
			/**
			 * @brief Set the mode to display elements.
			 * @param[in] mode The mode to display the elements.
			 */
			void SetMode(displayMode_te mode);
			/**
			 * @brief Get the mode to display elements.
			 * @return The current mode to display the elements.
			 */
			displayMode_te GetMode(void);
		private:
			etk::Vector2D<bool> m_lockExpendContamination; //!< If some sub-widget request the expend==> this permit to unpropagate the problem
		public:
			/**
			 * @brief Change state of the expend contatmination (if some sub-widget request the expent this permit to not propagate if at this widget)
			 * @param[in] lockExpend New expend state in vertical and horisantal
			 */
			void LockExpendContamination(bool lockExpend);
			/**
			 * @brief Change state of the expend contatmination (if some sub-widget request the expent this permit to not propagate if at this widget)
			 * @param[in] lockExpend New expend state in vertical
			 */
			void LockExpendContaminationVert(bool lockExpend);
			/**
			 * @brief Change state of the expend contatmination (if some sub-widget request the expent this permit to not propagate if at this widget)
			 * @param[in] lockExpend New expend state in horisantal
			 */
			void LockExpendContaminationHori(bool lockExpend);
		public:
			/**
			 * @brief Remove all sub element from the widget.
			 */
			virtual void SubWidgetRemoveAll(void);
			/**
			 * @brief Add at end position a Widget (note : This system use an inverted phylisophie (button to top, and left to right)
			 * @param[in] newWidget the element pointer
			 */
			virtual void SubWidgetAdd(ewol::Widget* newWidget);
			/**
			 * @brief Add at start position a Widget (note : This system use an inverted phylisophie (button to top, and left to right)
			 * @param[in] newWidget the element pointer
			 */
			virtual void SubWidgetAddStart(ewol::Widget* newWidget);
			/**
			 * @brief Remove definitly a widget from the system and this layer.
			 * @param[in] newWidget the element pointer.
			 */
			virtual void SubWidgetRemove(ewol::Widget* newWidget);
			/**
			 * @brief Just unlick the specify widget, this function does not remove it from the system (if you can, do nt use it ...)
			 * @param[in] newWidget the element pointer.
			 */
			virtual void SubWidgetUnLink(ewol::Widget* newWidget);
		protected:
			// Derived function
			virtual void OnDraw(ewol::DrawProperty& displayProp);
		public:
			// Derived function
			virtual void OnRegenerateDisplay(void);
			// Derived function
			virtual ewol::Widget* GetWidgetAtPos(vec2 pos);
			// Derived function
			virtual void OnObjectRemove(ewol::EObject* removeObject);
			// Derived function
			virtual const char * const GetObjectType(void) { return "EwolSizerVert"; };
			// Derived function
			virtual bool CalculateSize(float availlableX, float availlableY);
			// Derived function
			virtual bool CalculateMinSize(void);
			// Derived function
			virtual void SetMinSize(float x=-1, float y=-1);
			// Derived function
			virtual void SetExpendX(bool newExpend=false);
			// Derived function
			virtual bool CanExpentX(void);
			// Derived function
			virtual void SetExpendY(bool newExpend=false);
			// Derived function
			virtual bool CanExpentY(void);
	};
	
};

#endif
