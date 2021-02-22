/** @file
 * @author Edouard DUPIN
 * @copyright 2011, Edouard DUPIN, all right reserved
 * @license MPL v2.0 (see license file)
 */
#pragma once

#include <etk/types.hpp>
#include <ewol/debug.hpp>
#include <ewol/widget/ContainerN.hpp>
#include <ewol/widget/Manager.hpp>

namespace ewol {
	namespace widget {
		class Layer;
		using LayerShared = ememory::SharedPtr<ewol::widget::Layer>;
		using LayerWeak = ememory::WeakPtr<ewol::widget::Layer>;
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
			public:
				ewol::WidgetShared getWidgetAtPos(const vec2& _pos) override;
		};
	};
};

