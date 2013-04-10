/**
 * @author Edouard DUPIN
 * 
 * @copyright 2011, Edouard DUPIN, all right reserved
 * 
 * @license BSD v3 (see license file)
 */

#ifndef __EWOL_SLIDER_H__
#define __EWOL_SLIDER_H__

#include <etk/types.h>
#include <draw/Color.h>
#include <ewol/debug.h>
#include <ewol/widget/Drawable.h>

extern const char * const ewolEventSliderChange;

namespace widget {
	class Slider :public widget::Drawable
	{
		public:
			static void Init(void);
			static void UnInit(void);
		public:
			Slider(void);
			virtual ~Slider(void);
			void SetValue(int32_t val);
			int32_t GetValue(void);
			void SetMin(int32_t val);
			void SetMax(int32_t val);
			void SetColor(draw::Color newColor) { m_textColorFg = newColor; };
		private:
			int32_t            m_value;
			int32_t            m_min;
			int32_t            m_max;
			draw::Color        m_textColorFg;  //!< Text color
			draw::Color        m_textColorBg;  //!< Background color
		public:
			// Derived function
			virtual const char * const GetObjectType(void) { return "Ewol::Slider"; } ;
			virtual void CalculateMinMaxSize(void);
			virtual void OnRegenerateDisplay(void);
			virtual bool OnEventInput(ewol::keyEvent::type_te type, int32_t IdInput, ewol::keyEvent::status_te typeEvent, const vec2& pos);
	};
	
};

#endif
