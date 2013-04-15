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
		public:
			static void Init(void);
			static void UnInit(void);
		private:
			ewol::Drawing m_draw; //!< Compositing drawing element
		public:
			/**
			 * @brief Main constructer
			 */
			Spacer(void);
			/**
			 * @brief Main destructer
			 */
			virtual ~Spacer(void);
		protected:
			draw::Color m_color; //!< Background color
		public:
			/**
			 * @brief Spziby the background color (basicly transparent)
			 * @param[in] newColor the display background color
			 */
			void SetColor(draw::Color newColor) { m_color = newColor; MarkToRedraw(); };
		public:
			// Derived function
			virtual const char * const GetObjectType(void) { return "ewol::spacer"; };
			virtual ewol::Widget * GetWidgetAtPos(const vec2& pos) { return NULL; };
			virtual void OnRegenerateDisplay(void);
			virtual void OnDraw(ewol::DrawProperty& displayProp);
	};
	
};

#endif
