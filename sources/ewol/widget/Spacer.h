/**
 * @author Edouard DUPIN
 * 
 * @copyright 2011, Edouard DUPIN, all right reserved
 * 
 * @license APACHE v2.0 (see license file)
 */

#ifndef __EWOL_SPACER_H__
#define __EWOL_SPACER_H__

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
			private:
				ewol::compositing::Drawing m_draw; //!< Compositing drawing element
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
			protected:
				ewol::object::Param<etk::Color<>> m_color; //!< Background color
			public:
				/**
				 * @brief Spziby the background color (basicly transparent)
				 * @param[in] newColor the display background color
				 */
				void setColor(etk::Color<> _newColor) {
					m_color = _newColor;
					markToRedraw();
				};
			public: // Derived function
				virtual std::shared_ptr<ewol::Widget> getWidgetAtPos(const vec2& _pos) { return nullptr; };
				virtual void onRegenerateDisplay();
				virtual void onDraw();
				virtual void onParameterChangeValue(const ewol::object::ParameterRef& _paramPointer);
		};
	};
};

#endif
