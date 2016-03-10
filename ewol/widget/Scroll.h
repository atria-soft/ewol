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
#include <ewol/widget/Container.h>
#include <ewol/compositing/Compositing.h>
#include <ewol/compositing/Shaper.h>
#include <ewol/widget/Manager.h>

namespace ewol {
	namespace widget {
		class Scroll;
		using ScrollShared = ememory::SharedPtr<ewol::widget::Scroll>;
		using ScrollWeak = ememory::WeakPtr<ewol::widget::Scroll>;
		class Scroll : public ewol::widget::Container {
			public: // properties
				eproperty::Range<vec2> propertyLimit; //!< Set the limitation of the ratio in the sreen
				eproperty::Value<std::string> propertyShapeVert; //!< Vertical shaper name
				eproperty::Value<std::string> propertyShapeHori; //!< Horizontal shaper name
			public:
				enum highSpeedMode {
					speedModeDisable,
					speedModeInit,
					speedModeEnableFinger,     // Specific for touchpad
					speedModeEnableHorizontal, // Specific for mouse
					speedModeEnableVertical,   // Specific for mouse
					speedModeGrepEndEvent
				};
			private:
				ewol::compositing::Shaper m_shaperH; //!< Compositing theme Horizontal.
				ewol::compositing::Shaper m_shaperV; //!< Compositing theme Vertical.
			private:
				float m_pixelScrolling;
				vec2 m_highSpeedStartPos;
				enum highSpeedMode m_highSpeedMode;
				int32_t m_highSpeedButton;
				enum gale::key::type m_highSpeedType;
			protected:
				Scroll();
				void init() override;
			public:
				DECLARE_WIDGET_FACTORY(Scroll, "Scroll");
				virtual ~Scroll();
			public:
				void calculateMinMaxSize() override;
				void onRegenerateDisplay() override;
				bool onEventInput(const ewol::event::Input& _event) override;
				void systemDraw(const ewol::DrawProperty& _displayProp) override;
				ewol::WidgetShared getWidgetAtPos(const vec2& _pos) override;
			protected:
				void onDraw() override;
			protected:
				virtual void onChangePropertyLimit();
				virtual void onChangePropertyShapeVert();
				virtual void onChangePropertyShapeHori();
		};
	}
}

