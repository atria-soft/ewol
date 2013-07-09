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
#include <etk/Color.h>
#include <ewol/widget/Widget.h>
#include <ewol/compositing/Drawing.h>

extern const char * const ewolEventColorBarChange;

namespace widget {
	class ColorBar :public ewol::Widget
	{
		public:
			ColorBar(void);
			virtual ~ColorBar(void);
			etk::Color<> GetCurrentColor(void);
			void SetCurrentColor(etk::Color<> _newOne);
		private:
			ewol::Drawing m_draw; //!< Compositing drawing element
			etk::Color<> m_currentColor;
			vec2 m_currentUserPos;
		protected: // Derived function
			virtual void OnDraw(void);
		public: // Derived function
			virtual const char * const GetObjectType(void) { return "widget::ColorBar"; };
			virtual void CalculateMinMaxSize(void);
			virtual void OnRegenerateDisplay(void);
			virtual bool OnEventInput(const ewol::EventInput& _event);
	};
	
};

#endif
