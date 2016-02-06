/**
 * @author Edouard DUPIN
 * 
 * @copyright 2011, Edouard DUPIN, all right reserved
 * 
 * @license APACHE v2.0 (see license file)
 */
#pragma once

#include <etk/types.h>
#include <ewol/widget/Sizer.h>
#include <etk/Color.h>
#include <ewol/compositing/Drawing.h>

namespace appl {
	namespace widget {
		class SizerColor : public ewol::widget::Sizer {
			public:
				/**
				 * @brief Main call of recording the widget on the List of "widget named creator"
				 */
				static void init(ewol::widget::Manager& _widgetManager);
			protected:
				/**
				 * @brief Constructor
				 * @param[in] _mode The mode to display the elements
				 */
				SizerColor();
				void init(enum displayMode _mode=ewol::widget::Sizer::modeHori);
			public:
				DECLARE_WIDGET_FACTORY(SizerColor, "SizerColor");
				/**
				 * @brief Desstructor
				 */
				virtual ~SizerColor();
			private:
				ewol::parameter::Value<etk::Color<>> m_borderColor; //!< Border color.
				ewol::parameter::Value<etk::Color<>> m_leftColor; //!< Left color.
				ewol::parameter::Value<etk::Color<>> m_rightColor; //!< Right color.
				ewol::parameter::Value<etk::Color<>> m_topColor; //!< Top color.
				ewol::parameter::Value<etk::Color<>> m_buttomColor; //!< Buttom color.
			private:
				ewol::compositing::Drawing m_draw; //!< Compositing drawing element for display the border.
			public:
				virtual void onRegenerateDisplay();
				virtual void onDraw();
		};
	}
}

