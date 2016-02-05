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
			public:
				/**
				 * @brief Set the current border color:
				 * @param[in] _value The border color to set @ref etk::color::none : no color
				 */
				void setBorderColor(const etk::Color<>& _value) {
					m_borderColor.set(_value);
				}
				/**
				 * @brief get the current border size of the current element:
				 * @return the border size (0 if not used)
				 */
				const etk::Color<>& getBorderColor() const {
					return m_borderColor;
				};
			private:
				ewol::compositing::Drawing m_draw; //!< Compositing drawing element for display the border.
			public:
				virtual void onRegenerateDisplay();
				virtual void onDraw();
		};
	}
}

