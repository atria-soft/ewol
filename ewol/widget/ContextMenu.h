/**
 * @author Edouard DUPIN
 * 
 * @copyright 2011, Edouard DUPIN, all right reserved
 * 
 * @license APACHE v2.0 (see license file)
 */
#pragma once

#include <etk/types.h>
#include <etk/Color.h>
#include <ewol/debug.h>
#include <ewol/widget/Container.h>
#include <ewol/compositing/Drawing.h>
#include <ewol/compositing/Shaper.h>
#include <ewol/widget/Manager.h>

namespace ewol {
	namespace widget {
		/**
		 * @ingroup ewolWidgetGroup
		 */
		class ContextMenu : public ewol::widget::Container {
			public:
				enum markPosition {
					markTop,
					markRight,
					markButtom,
					markLeft,
					markNone
				};
			protected:
				ContextMenu();
				void init(const std::string& _shaperName="{ewol}THEME:GUI:ContextMenu.json");
			public:
				DECLARE_WIDGET_FACTORY(ContextMenu, "ContextMenu");
				virtual ~ContextMenu();
			private:
				ewol::parameter::Value<ewol::compositing::Shaper> m_shaper; //!< Compositing theme.
			public:
				/**
				 * @brief set the shaper name (use the contructer one this permit to not noad unused shaper)
				 * @param[in] _shaperName The new shaper filename
				 */
				void setShaperName(const std::string& _shaperName) {
					m_shaper.set(_shaperName);
				}
			private:
				// TODO : Rework the displayer ....
				ewol::compositing::Drawing m_compositing;
				etk::Color<> m_colorBackGroung;
				etk::Color<> m_colorBorder;
				float m_offset;
			private:
				ewol::parameter::Value<vec2> m_arrowPos;
				ewol::parameter::List<enum markPosition> m_arrawBorder;
			public:
				void setPositionMark(enum markPosition _position, const vec2& _arrowPos) {
					m_arrawBorder.set(_position);
					m_arrowPos.set(_arrowPos);
				}
			protected: // Derived function
				virtual void onDraw();
				virtual void onParameterChangeValue(const ewol::parameter::Ref& _paramPointer);
			public: // Derived function
				virtual void onRegenerateDisplay();
				virtual bool onEventInput(const ewol::event::Input& _event);
				virtual void onChangeSize();
				virtual void calculateMinMaxSize();
				virtual std::shared_ptr<ewol::Widget> getWidgetAtPos(const vec2& _pos);
		};
	};
};

