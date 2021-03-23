/** @file
 * @author Edouard DUPIN
 * @copyright 2011, Edouard DUPIN, all right reserved
 * @license MPL v2.0 (see license file)
 */
#pragma once

#include <etk/types.hpp>
#include <ewol/debug.hpp>
#include <ewol/widget/Widget.hpp>
#include <ewol/widget/Container.hpp>
#include <ewol/compositing/Drawing.hpp>
#include <ewol/compositing/Shaper.hpp>
#include <ewol/widget/Manager.hpp>

namespace ewol {
	namespace widget {
		class PopUp;
		using PopUp = ememory::Ptr<ewol::widget::PopUp>;
		using PopUpWeak = ememory::WeakPtr<ewol::widget::PopUp>;
		/**
		 * @ingroup ewolWidgetGroup
		 */
		class PopUp : public ewol::widget::Container {
			public: // properties
				eproperty::Value<etk::Uri> propertyShape; //!< Compositing theme.
				eproperty::Value<Vector2b> propertyLockExpand; //!< Lock the expend of the sub widget to this one  == > this permit to limit bigger subWidget
				eproperty::Value<bool> propertyCloseOutEvent; //!< ratio progression of a sliding
			protected:
				/**
				 * Constructor
				 * @param _shaperName Shaper file properties
				 */
				PopUp();
				void init() ;
			public:
				DECLARE_WIDGET_FACTORY(PopUp, "PopUp");
				/**
				 * Destructor
				 */
				 ~PopUp();
			protected:
				ewol::compositing::Shaper this.shaper; //!< Compositing theme.
			protected:
				void onDraw() ;
			public:
				void systemDraw( ewol::DrawProperty _displayProp) ;
				void onRegenerateDisplay() ;
				void onChangeSize() ;
				boolean onEventInput( ewol::event::Input _event) ;
				Widget getWidgetAtPos( Vector2f _pos) ;
			protected:
				 void onChangePropertyShape();
				 void onChangePropertyLockExpand();
		};
	};
};

