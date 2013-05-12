/**
 * @author Edouard DUPIN
 * 
 * @copyright 2011, Edouard DUPIN, all right reserved
 * 
 * @license BSD v3 (see license file)
 */

#ifndef __EWOL_WIDGET_SCROLL_H__
#define __EWOL_WIDGET_SCROLL_H__

#include <etk/types.h>
#include <ewol/debug.h>
#include <ewol/widget/Container.h>
#include <ewol/compositing/Compositing.h>
#include <ewol/compositing/Drawing.h>

namespace widget {
	typedef enum {
		SCROLL_DISABLE,
		SCROLL_INIT,
		SCROLL_ENABLE_FINGER,     // Specific for touchpad
		SCROLL_ENABLE_HORIZONTAL, // Specific for mouse
		SCROLL_ENABLE_VERTICAL,   // Specific for mouse
		SCROLL_GREP_END_EVENT,
	}highSpeedMode_te;
	
	class Scroll : public widget::Container
	{
		public:
			static void Init(void);
			static void UnInit(void);
		private:
			ewol::Drawing m_draw; // TODO : Change in shaper ... ==> better for annimation and dynamic display ...
		protected:
			vec2 m_limit;
		private:
			float m_pixelScrolling;
			vec2 m_highSpeedStartPos;
			highSpeedMode_te m_highSpeedMode;
			int32_t m_highSpeedButton;
			ewol::keyEvent::type_te m_highSpeedType;
		public:
			Scroll(void);
			virtual ~Scroll(void);
			/**
			 * @brief Set the limit of scrolling
			 * @note This permit to scoll element upper the end of the display
			 * @param[in] _limit scrolling limit [0..1] (represent a pourcent)
			 */
			void SetLimit(const vec2& _limit);
			/**
			 * @brief Get the limit of scrolling
			 * @return scrolling limit
			 */
			const vec2& GetLimit(void) { return m_limit; };
			
		public: // Derived function
			virtual const char * const GetObjectType(void) { return "ewol::widget::Scroll"; };
			void CalculateMinMaxSize(void);
			virtual void OnRegenerateDisplay(void);
			virtual bool OnEventInput(const ewol::EventInput& _event);
			virtual void SystemDraw(const ewol::DrawProperty& _displayProp);
			virtual ewol::Widget* GetWidgetAtPos(const vec2& _pos);
		protected: // Derived function
			virtual void OnDraw(void);
	};
	
};

#endif
