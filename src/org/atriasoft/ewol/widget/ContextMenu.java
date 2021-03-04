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
		using ContextMenu = ememory::Ptr<ewol::widget::ContextMenu>;
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
				eproperty::Value<etk::Uri> propertyShape; //!< shape of the widget.
				eproperty::Value<Vector2f> propertyArrowPos;
				eproperty::List<enum markPosition> propertyArrawBorder;
			protected:
				ContextMenu();
				void init() ;
			public:
				DECLARE_WIDGET_FACTORY(ContextMenu, "ContextMenu");
				 ~ContextMenu();
			private:
				ewol::compositing::Shaper this.shaper; //!< Compositing theme.
				
				// TODO : Use shaper for the arraw ...
				ewol::compositing::Drawing this.compositing;
				etk::Color<> this.colorBorder; // use shaper ID
				
				
				float this.offset;
			public:
				void setPositionMarkAuto( Vector2f _origin,  Vector2f _size);
				void setPositionMark(enum markPosition _position,  Vector2f _arrowPos);
			protected:
				void onDraw() ;
			public:
				void onRegenerateDisplay() ;
				boolean onEventInput( ewol::event::Input _event) ;
				void onChangeSize() ;
				void calculateMinMaxSize() ;
				Widget getWidgetAtPos( Vector2f _pos) ;
			protected:
				 void onChangePropertyArrowPos();
				 void onChangePropertyArrawBorder();
				 void onChangePropertyShape();
		};
	};
};

