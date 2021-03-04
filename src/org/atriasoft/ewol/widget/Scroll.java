/** @file
 * @author Edouard DUPIN
 * @copyright 2011, Edouard DUPIN, all right reserved
 * @license MPL v2.0 (see license file)
 */
#pragma once

#include <etk/types.hpp>
#include <ewol/debug.hpp>
#include <ewol/widget/Container.hpp>
#include <ewol/compositing/Compositing.hpp>
#include <ewol/compositing/Shaper.hpp>
#include <ewol/widget/Manager.hpp>

namespace ewol {
	namespace widget {
		class Scroll;
		using Scroll = ememory::Ptr<ewol::widget::Scroll>;
		using ScrollWeak = ememory::WeakPtr<ewol::widget::Scroll>;
		class Scroll : public ewol::widget::Container {
			public: // properties
				eproperty::Range<Vector2f> propertyLimit; //!< Set the limitation of the ratio in the sreen
				eproperty::Value<etk::Uri> propertyShapeVert; //!< Vertical shaper name
				eproperty::Value<etk::Uri> propertyShapeHori; //!< Horizontal shaper name
				eproperty::Value<bool> propertyHover; //!< Horizontal shaper name
			public:
				enum highSpeedMode {
					speedModeDisable,
					speedModeInit,
					speedModeEnableFinger, // Specific for touchpad
					speedModeEnableHorizontal, // Specific for mouse
					speedModeEnableVertical, // Specific for mouse
					speedModeGrepEndEvent
				};
			private:
				ewol::compositing::Shaper this.shaperH; //!< Compositing theme Horizontal.
				ewol::compositing::Shaper this.shaperV; //!< Compositing theme Vertical.
			private:
				float this.pixelScrolling;
				Vector2f this.highSpeedStartPos;
				enum highSpeedMode this.highSpeedMode;
				int this.highSpeedButton;
				KeyType this.highSpeedType;
			protected:
				Scroll();
				void init() ;
			public:
				DECLARE_WIDGET_FACTORY(Scroll, "Scroll");
				 ~Scroll();
			public:
				void onChangeSize() ;
				void calculateMinMaxSize() ;
				void onRegenerateDisplay() ;
				boolean onEventInput( ewol::event::Input _event) ;
				void systemDraw( ewol::DrawProperty _displayProp) ;
				Widget getWidgetAtPos( Vector2f _pos) ;
			protected:
				void onDraw() ;
			protected:
				 void onChangePropertyLimit();
				 void onChangePropertyShapeVert();
				 void onChangePropertyShapeHori();
		};
	}
}

