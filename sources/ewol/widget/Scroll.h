/**
 * @author Edouard DUPIN
 * 
 * @copyright 2011, Edouard DUPIN, all right reserved
 * 
 * @license APACHE v2.0 (see license file)
 */

#ifndef __EWOL_WIDGET_SCROLL_H__
#define __EWOL_WIDGET_SCROLL_H__

#include <etk/types.h>
#include <ewol/debug.h>
#include <ewol/widget/Container.h>
#include <ewol/compositing/Compositing.h>
#include <ewol/compositing/Shaper.h>
#include <ewol/widget/Manager.h>

namespace ewol {
	namespace widget {
		class Scroll : public ewol::widget::Container {
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
			protected:
				ewol::parameter::Range<vec2> m_limit;
			private:
				float m_pixelScrolling;
				vec2 m_highSpeedStartPos;
				enum highSpeedMode m_highSpeedMode;
				int32_t m_highSpeedButton;
				enum ewol::key::type m_highSpeedType;
			protected:
				Scroll();
				void init(const std::string& _shaperName="THEME:GUI:WidgetScrolled.json");
			public:
				DECLARE_WIDGET_FACTORY(Scroll, "Scroll");
				virtual ~Scroll();
				/**
				 * @brief set the limit of scrolling
				 * @note This permit to scoll element upper the end of the display
				 * @param[in] _limit scrolling limit [0..1] (represent a pourcent)
				 */
				void setLimit(const vec2& _limit);
				/**
				 * @brief get the limit of scrolling
				 * @return scrolling limit
				 */
				const vec2& getLimit() const {
					return m_limit;
				};
				
			public: // Derived function
				void calculateMinMaxSize();
				virtual void onRegenerateDisplay();
				virtual bool onEventInput(const ewol::event::Input& _event);
				virtual void systemDraw(const ewol::DrawProperty& _displayProp);
				virtual std::shared_ptr<ewol::Widget> getWidgetAtPos(const vec2& _pos);
			protected: // Derived function
				virtual void onDraw();
				virtual void onParameterChangeValue(const ewol::parameter::Ref& _paramPointer);
		};
	};
};

#endif
