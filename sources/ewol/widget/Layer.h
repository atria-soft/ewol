/**
 * @author Edouard DUPIN
 * 
 * @copyright 2011, Edouard DUPIN, all right reserved
 * 
 * @license BSD v3 (see license file)
 */

#ifndef __EWOL_LAYER_H__
#define __EWOL_LAYER_H__

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
			public:
				/**
				 * @brief Main call of recording the widget on the List of "widget named creator"
				 */
				static void init(ewol::widget::Manager& _widgetManager);
			public:
				/**
				 * @brief Constructor
				 */
				Layer();
				/**
				 * @brief Desstructor
				 */
				virtual ~Layer();
			public: // Derived function
				virtual ewol::object::Shared<ewol::Widget> getWidgetAtPos(const vec2& _pos);
		};
	};
};

#endif
