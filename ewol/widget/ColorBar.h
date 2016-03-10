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
#include <esignal/ISignal.h>


namespace ewol {
	namespace widget {
		class ColorBar;
		using ColorBarShared = ememory::SharedPtr<ewol::widget::ColorBar>;
		using ColorBarWeak = ememory::WeakPtr<ewol::widget::ColorBar>;
		class ColorBar : public ewol::Widget {
			public: // signals
				esignal::ISignal<etk::Color<>> signalChange;
			public:
				eproperty::Value<etk::Color<>> propertyValue;
			protected:
				ColorBar();
			public:
				DECLARE_WIDGET_FACTORY(ColorBar, "ColorBar");
				virtual ~ColorBar();
			private:
				ewol::compositing::Drawing m_draw; //!< Compositing drawing element
				vec2 m_currentUserPos;
			protected:
				void onDraw() override;
			public:
				void calculateMinMaxSize() override;
				void onRegenerateDisplay() override;
				bool onEventInput(const ewol::event::Input& _event) override;
			protected:
				virtual void onChangePropertyValue();
		};
	};
};
