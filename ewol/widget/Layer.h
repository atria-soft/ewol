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
#include <ewol/widget/ContainerN.h>
#include <ewol/widget/Manager.h>

namespace ewol {
	namespace widget {
		/**
		 * @ingroup ewolWidgetGroup
		 */
		class Layer : public ewol::widget::ContainerN {
			protected:
				/**
				 * @brief Constructor
				 */
				Layer();
			public:
				DECLARE_WIDGET_FACTORY(Layer, "Layer");
				/**
				 * @brief Desstructor
				 */
				virtual ~Layer();
			public: // Derived function
				virtual std::shared_ptr<ewol::Widget> getWidgetAtPos(const vec2& _pos);
		};
	};
};

