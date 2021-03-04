/** @file
 * @author Edouard DUPIN
 * @copyright 2011, Edouard DUPIN, all right reserved
 * @license MPL v2.0 (see license file)
 */
#pragma once

#include <etk/types.hpp>
#include <etk/Color.hpp>
#include <ewol/debug.hpp>
#include <ewol/widget/Widget.hpp>
#include <ewol/compositing/Drawing.hpp>
#include <ewol/widget/Manager.hpp>

namespace ewol {
	namespace widget {
		class Spacer;
		using Spacer = ememory::Ptr<ewol::widget::Spacer>;
		using SpacerWeak = ememory::WeakPtr<ewol::widget::Spacer>;
		/**
		 * @ingroup ewolWidgetGroup
		 */
		class Spacer : public Widget {
			public: // properties:
				eproperty::Value<etk::Color<>> propertyColor; //!< Background color
			protected:
				/**
				 * @brief Main ructer
				 */
				Spacer();
			public:
				DECLARE_WIDGET_FACTORY(Spacer, "Spacer");
				/**
				 * @brief Main destructer
				 */
				 ~Spacer();
			private:
				ewol::compositing::Drawing this.draw; //!< Compositing drawing element
			public:
				Widget getWidgetAtPos( Vector2f _pos)  {
					return null;
				};
				void onRegenerateDisplay() ;
				void onDraw() ;
			protected:
				 void onChangePropertyColor();
		};
	}
}

