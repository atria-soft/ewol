/** @file
 * @author Edouard DUPIN
 * @copyright 2011, Edouard DUPIN, all right reserved
 * @license MPL v2.0 (see license file)
 */
#pragma once

#include <etk/types.hpp>
#include <ewol/debug.hpp>
#include <etk/Color.hpp>
#include <ewol/widget/Widget.hpp>
#include <ewol/compositing/Drawing.hpp>
#include <esignal/Signal.hpp>


namespace ewol {
	namespace widget {
		class ColorBar;
		using ColorBar = ememory::Ptr<ewol::widget::ColorBar>;
		using ColorBarWeak = ememory::WeakPtr<ewol::widget::ColorBar>;
		class ColorBar : public Widget {
			public: // signals
				esignal::Signal<etk::Color<>> signalChange;
			public:
				eproperty::Value<etk::Color<>> propertyValue;
			protected:
				ColorBar();
			public:
				DECLARE_WIDGET_FACTORY(ColorBar, "ColorBar");
				 ~ColorBar();
			private:
				ewol::compositing::Drawing this.draw; //!< Compositing drawing element
				Vector2f this.currentUserPos;
			protected:
				void onDraw() ;
			public:
				void calculateMinMaxSize() ;
				void onRegenerateDisplay() ;
				boolean onEventInput( ewol::event::Input _event) ;
			protected:
				 void onChangePropertyValue();
		};
	};
};
