/**
 * @author Edouard DUPIN
 * 
 * @copyright 2011, Edouard DUPIN, all right reserved
 * 
 * @license BSD v3 (see license file)
 */

#ifndef __EWOL_SLIDER_H__
#define __EWOL_SLIDER_H__

#include <etk/Types.h>
#include <ewol/Debug.h>
#include <ewol/widget/Drawable.h>

extern const char * const ewolEventSliderChange;

namespace ewol {
	class Slider :public ewol::Drawable
	{
		public:
			Slider(void);
			virtual ~Slider(void);
			/**
			 * @brief Get the current Object type of the EObject
			 * @note In Embended platforme, it is many time no -rtti flag, then it is not possible to use dynamic cast ==> this will replace it
			 * @param[in] objectType type description
			 * @return true if the object is compatible, otherwise false
			 */
			virtual const char * const GetObjectType(void) { return "EwolSlider"; } ;
			virtual bool   CalculateMinSize(void);
			void           SetValue(int32_t val);
			int32_t        GetValue(void);
			void           SetMin(int32_t val);
			void           SetMax(int32_t val);
			void           SetColor(draw::Color newColor) { m_textColorFg = newColor; };
		private:
			int32_t            m_value;
			int32_t            m_min;
			int32_t            m_max;
			draw::Color           m_textColorFg;  //!< Text color
			draw::Color           m_textColorBg;  //!< Background color
		public:
			virtual void   OnRegenerateDisplay(void);
		public:
			/**
			 * @brief Event on an input of this Widget
			 * @param[in] type Type of the input (ewol::INPUT_TYPE_MOUSE/ewol::INPUT_TYPE_FINGER ...)
			 * @param[in] IdInput Id of the current Input (PC : left=1, right=2, middle=3, none=0 / Tactil : first finger=1 , second=2 (only on this widget, no knowledge at ouside finger))
			 * @param[in] typeEvent ewol type of event like EVENT_INPUT_TYPE_DOWN/EVENT_INPUT_TYPE_MOVE/EVENT_INPUT_TYPE_UP/EVENT_INPUT_TYPE_SINGLE/EVENT_INPUT_TYPE_DOUBLE/...
			 * @param[in] pos Absolute position of the event
			 * @return true the event is used
			 * @return false the event is not used
			 */
			virtual bool OnEventInput(ewol::inputType_te type, int32_t IdInput, eventInputType_te typeEvent, etk::Vector2D<float>  pos);
	};
	
};

#endif
