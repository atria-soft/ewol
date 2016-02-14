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
#include <ewol/widget/Widget.h>
#include <ewol/compositing/Drawing.h>
#include <ewol/widget/Manager.h>

namespace ewol {
	namespace widget {
		/**
		 * @ingroup ewolWidgetGroup
		 */
		class Spacer : public ewol::Widget {
			public: // properties:
				eproperty::Value<etk::Color<>> propertyColor; //!< Background color
			protected:
				/**
				 * @brief Main constructer
				 */
				Spacer();
				void init();
			public:
				DECLARE_WIDGET_FACTORY(Spacer, "Spacer");
				/**
				 * @brief Main destructer
				 */
				virtual ~Spacer();
			private:
				ewol::compositing::Drawing m_draw; //!< Compositing drawing element
			public: // Derived function
				virtual std::shared_ptr<ewol::Widget> getWidgetAtPos(const vec2& _pos) { return nullptr; };
				virtual void onRegenerateDisplay();
				virtual void onDraw();
				virtual void onPropertyChangeValue(const eproperty::Ref& _paramPointer);
		};
	}
}

