/**
 * @author Edouard DUPIN
 * 
 * @copyright 2011, Edouard DUPIN, all right reserved
 * 
 * @license BSD v3 (see license file)
 */

#ifndef __EWOL_SPACER_H__
#define __EWOL_SPACER_H__

#include <etk/types.h>
#include <draw/Color.h>
#include <ewol/debug.h>
#include <ewol/widget/Drawable.h>

namespace widget {
	class Spacer :public widget::Drawable
	{
		public:
			Spacer(void);
			virtual ~Spacer(void);
			// Derived function
			virtual const char * const GetObjectType(void) { return "EwolSpacer"; };
			// Derived function
			virtual bool   CalculateMinSize(void);
			void SetSize(float size);
			// Derived function
			virtual ewol::Widget * GetWidgetAtPos(etk::Vector2D<float>  pos) { return NULL; };
			// Derived function
			virtual void   OnRegenerateDisplay(void);
			void SetColor(draw::Color newColor) { m_color = newColor; MarkToRedraw(); };
		private:
			float        m_localSize;
			draw::Color  m_color;
	};
	
};

#endif
