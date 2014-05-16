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
#include <ewol/widget/Manager.h>

namespace ewol {
	namespace widget {
		/**
		 * @ingroup ewolWidgetGroup
		 */
		class Spacer : public ewol::Widget {
			public:
				static void init(ewol::object::Shared<ewol::widget::Manager> _widgetManager);
				// Config list of properties
				static const char* const configColor;
			private:
				ewol::compositing::Drawing m_draw; //!< Compositing drawing element
			public:
				/**
				 * @brief Main constructer
				 */
				Spacer();
				/**
				 * @brief Main destructer
				 */
				virtual ~Spacer();
			protected:
				etk::Color<> m_color; //!< Background color
			public:
				/**
				 * @brief Spziby the background color (basicly transparent)
				 * @param[in] newColor the display background color
				 */
				void setColor(etk::Color<> _newColor) { m_color = _newColor; markToRedraw(); };
			public: // Derived function
				virtual ewol::object::Shared<ewol::Widget> getWidgetAtPos(const vec2& _pos) { return NULL; };
				virtual void onRegenerateDisplay();
				virtual void onDraw();
				virtual bool onSetConfig(const ewol::object::Config& _conf);
				virtual bool onGetConfig(const char* _config, std::string& _result) const;
		};
	};
};

#endif
