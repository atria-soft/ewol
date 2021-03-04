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
#include <etk/Color.hpp>
#include <ewol/compositing/Drawing.hpp>

namespace ewol {
	namespace widget {
		class Sizer;
		using Sizer = ememory::Ptr<ewol::widget::Sizer>;
		using SizerWeak = ememory::WeakPtr<ewol::widget::Sizer>;
		/**
		 * @ingroup ewolWidgetGroup
		 */
		class Sizer : public ewol::widget::ContainerN {
			public:
				enum displayMode {
					modeVert, //!< Vertical mode
					modeHori, //!< Horizontal mode
				};
				enum animation {
					animationNone, //!< No annimation
					animationTop, //!< element came from the top
					animationbuttom, //!< element came from the buttom
					animationLeft, //!< element came from the Left
					animationRight //!< element came from the right
					//animationZoom //!< element came from zooming
				};
			public: // properties:
				eproperty::List<enum displayMode> propertyMode; //!< Methode to display the widget list (vert/hory ...)
				eproperty::Value<gale::Dimension> propertyBorderSize; //!< Border size needed for all the display
				eproperty::List<enum animation> propertyAnimation; //!< Methode add and remove element (animation)
				eproperty::Value<float> propertyAnimationTime; //!< Time in second to generate animation
			protected:
				/**
				 * @brief Constructor
				 * @param[in] _mode The mode to display the elements
				 */
				Sizer();
			public:
				DECLARE_WIDGET_FACTORY(Sizer, "Sizer");
				/**
				 * @brief Destructor
				 */
				 ~Sizer();
			public:
				void onChangeSize() ;
				void calculateMinMaxSize() ;
				// overwrite the set fuction to start annimations ...
				int subWidgetAdd(Widget _newWidget) ;
				int subWidgetAddStart(Widget _newWidget) ;
				void subWidgetRemove(Widget _newWidget) ;
				void subWidgetUnLink(Widget _newWidget) ;
			protected:
				 void onChangePropertyMode();
				 void onChangePropertyBorderSize();
		};
	}
}

