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
			static void Init(void);
			static void UnInit(void);
		public:
			typedef enum {
				modeVert, //!< Vertical mode
				modeHori, //!< Horizontal mode
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
			bvec2 m_lockExpendContamination; //!< If some sub-widget request the expend==> this permit to unpropagate the problem
		public:
			/**
			 * @brief Change state of the expend contatmination (if some sub-widget request the expent this permit to not propagate if at this widget)
			 * @param[in] lockExpend New expend state in vertical and horisantal
			 */
			void LockExpendContamination(const bvec2& lockExpend);
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
			virtual void CalculateSize(const vec2& availlable);
			virtual void CalculateMinSize(void);
			virtual void SetMinSize(const vec2& size);
			virtual void SetExpand(const bvec2& newExpend);
			virtual bvec2 CanExpent(void);;
	};
	
};

#endif
