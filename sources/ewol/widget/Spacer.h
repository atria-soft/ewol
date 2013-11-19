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
#include <etk/Color.h>
#include <ewol/debug.h>
#include <ewol/widget/Widget.h>
#include <ewol/compositing/Drawing.h>
#include <ewol/widget/WidgetManager.h>

namespace widget {
	/**
	 * @ingroup ewolWidgetGroup
	 */
	class Spacer :public ewol::Widget {
		public:
			static void init(ewol::WidgetManager& _widgetManager);
			// Config list of properties
			static const char* const configColor;
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
			etk::Color<> m_color; //!< Background color
		public:
			/**
			 * @brief Spziby the background color (basicly transparent)
			 * @param[in] newColor the display background color
			 */
			void setColor(etk::Color<> _newColor) { m_color = _newColor; markToRedraw(); };
		public: // Derived function
			virtual ewol::Widget * getWidgetAtPos(const vec2& _pos) { return NULL; };
			virtual void onRegenerateDisplay(void);
			virtual void onDraw(void);
			virtual bool onSetConfig(const ewol::EConfig& _conf);
			virtual bool onGetConfig(const char* _config, std::string& _result) const;
	};
	
};

#endif
