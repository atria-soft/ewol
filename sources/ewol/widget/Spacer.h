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
#include <ewol/widget/Widget.h>
#include <ewol/compositing/Drawing.h>

namespace widget {
	class Spacer :public ewol::Widget
	{
		private:
			ewol::Drawing  m_draw;      //!< Compositing drawing element
			float          m_localSize; //!< Local request size of the display
			draw::Color    m_color;     //!< Background color
		public:
			/**
			 * @brief Main constructer
			 */
			Spacer(void);
			/**
			 * @brief Main destructer
			 */
			virtual ~Spacer(void);
			/**
			 * @brief Set the minimum size requested
			 * @param[in] size Requested size of the minimum display (in X and Y)
			 */
			void SetSize(float size);
			/**
			 * @brief Spziby the background color (basicly transparent)
			 * @param[in] newColor the display background color
			 */
			void SetColor(draw::Color newColor) { m_color = newColor; MarkToRedraw(); };
		public:
			// Derived function
			virtual const char * const GetObjectType(void) { return "EwolSpacer"; };
			// Derived function
			virtual bool CalculateMinSize(void);
			// Derived function
			virtual ewol::Widget * GetWidgetAtPos(vec2  pos) { return NULL; };
			// Derived function
			virtual void OnRegenerateDisplay(void);
			// Derived function
			virtual void OnDraw(ewol::DrawProperty& displayProp);
	};
	
};

#endif
