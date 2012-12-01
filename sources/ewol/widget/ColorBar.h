/**
 * @author Edouard DUPIN
 * 
 * @copyright 2011, Edouard DUPIN, all right reserved
 * 
 * @license BSD v3 (see license file)
 */

#ifndef __EWOL_COLORBAR_H__
#define __EWOL_COLORBAR_H__

#include <etk/types.h>
#include <ewol/debug.h>
#include <draw/Color.h>
#include <ewol/compositing/Drawing.h>
#include <ewol/widget/Drawable.h>

extern const char * const ewolEventColorBarChange;

namespace widget {
	class ColorBar :public ewol::Widget
	{
		public:
			ColorBar(void);
			virtual ~ColorBar(void);
			draw::Color GetCurrentColor(void);
			void SetCurrentColor(draw::Color newOne);
		private:
			ewol::Drawing         m_draw;              //!< Compositing drawing element
			draw::Color           m_currentColor;
			etk::Vector2D<float>  m_currentUserPos;
		public:
			// Derived function
			virtual const char * const GetObjectType(void) { return "EwolColorBar"; };
			// Derived function
			virtual bool CalculateMinSize(void);
			// Derived function
			virtual void OnRegenerateDisplay(void);
			// Derived function
			virtual void OnDraw(ewol::DrawProperty& displayProp);
			// Derived function
			virtual bool OnEventInput(ewol::keyEvent::type_te type, int32_t IdInput, ewol::keyEvent::status_te typeEvent, etk::Vector2D<float> pos);
	};
	
};

#endif
