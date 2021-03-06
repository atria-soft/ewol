/** @file
 * @author Edouard DUPIN
 * @copyright 2011, Edouard DUPIN, all right reserved
 * @license MPL v2.0 (see license file)
 */
#pragma once

#include <etk/types.hpp>
#include <ewol/widget/Sizer.hpp>
#include <etk/Color.hpp>
#include <ewol/compositing/Drawing.hpp>

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
				eproperty::Value<etk::Color<>> propertyBorderColor; //!< Border color.
				eproperty::Value<etk::Color<>> propertyLeftColor; //!< Left color.
				eproperty::Value<etk::Color<>> propertyRightColor; //!< Right color.
				eproperty::Value<etk::Color<>> propertyTopColor; //!< Top color.
				eproperty::Value<etk::Color<>> propertyButtomColor; //!< Buttom color.
			private:
				ewol::compositing::Drawing m_draw; //!< Compositing drawing element for display the border.
			public:
				virtual void onRegenerateDisplay();
				virtual void onDraw();
		};
	}
}

