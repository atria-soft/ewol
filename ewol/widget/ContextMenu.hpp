/** @file
 * @author Edouard DUPIN
 * @copyright 2011, Edouard DUPIN, all right reserved
 * @license MPL v2.0 (see license file)
 */
#pragma once

#include <etk/types.hpp>
#include <etk/Color.hpp>
#include <ewol/debug.hpp>
#include <ewol/widget/Container.hpp>
#include <ewol/compositing/Drawing.hpp>
#include <ewol/compositing/Shaper.hpp>
#include <ewol/widget/Manager.hpp>

namespace ewol {
	namespace widget {
		class ContextMenu;
		using ContextMenuShared = ememory::SharedPtr<ewol::widget::ContextMenu>;
		using ContextMenuWeak = ememory::WeakPtr<ewol::widget::ContextMenu>;
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
			public: // properties
				eproperty::Value<std::string> propertyShape; //!< shape of the widget.
				eproperty::Value<vec2> propertyArrowPos;
				eproperty::List<enum markPosition> propertyArrawBorder;
			protected:
				ContextMenu();
				void init() override;
			public:
				DECLARE_WIDGET_FACTORY(ContextMenu, "ContextMenu");
				virtual ~ContextMenu();
			private:
				ewol::compositing::Shaper m_shaper; //!< Compositing theme.
				
				// TODO : Use shaper for the arraw ...
				ewol::compositing::Drawing m_compositing;
				etk::Color<> m_colorBorder; // use shaper ID
				
				
				float m_offset;
			public:
				void setPositionMarkAuto(const vec2& _origin, const vec2& _size);
				void setPositionMark(enum markPosition _position, const vec2& _arrowPos);
			protected:
				void onDraw() override;
			public:
				void onRegenerateDisplay() override;
				bool onEventInput(const ewol::event::Input& _event) override;
				void onChangeSize() override;
				void calculateMinMaxSize() override;
				ewol::WidgetShared getWidgetAtPos(const vec2& _pos) override;
			protected:
				virtual void onChangePropertyArrowPos();
				virtual void onChangePropertyArrawBorder();
				virtual void onChangePropertyShape();
		};
	};
};

