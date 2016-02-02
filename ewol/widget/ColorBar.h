/**
 * @author Edouard DUPIN
 * 
 * @copyright 2011, Edouard DUPIN, all right reserved
 * 
 * @license APACHE v2.0 (see license file)
 */
#pragma once

#include <etk/types.h>
#include <ewol/debug.h>
#include <etk/Color.h>
#include <ewol/widget/Widget.h>
#include <ewol/compositing/Drawing.h>
#include <ewol/signal/Signal.h>


namespace ewol {
	namespace widget {
		class ColorBar : public ewol::Widget {
			public:
				// Event list of properties
				ewol::Signal<etk::Color<>> signalChange;
			protected:
				ColorBar();
				void init();
			public:
				DECLARE_WIDGET_FACTORY(ColorBar, "ColorBar");
				virtual ~ColorBar();
				etk::Color<> getCurrentColor();
				void setCurrentColor(etk::Color<> _newOne);
			private:
				ewol::compositing::Drawing m_draw; //!< Compositing drawing element
				etk::Color<> m_currentColor;
				vec2 m_currentUserPos;
			protected: // Derived function
				virtual void onDraw();
			public: // Derived function
				virtual void calculateMinMaxSize();
				virtual void onRegenerateDisplay();
				virtual bool onEventInput(const ewol::event::Input& _event);
		};
	};
};
