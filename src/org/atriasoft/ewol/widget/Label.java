/** @file
 * @author Edouard DUPIN
 * @copyright 2011, Edouard DUPIN, all right reserved
 * @license MPL v2.0 (see license file)
 */
#pragma once

#include <etk/types.hpp>
#include <ewol/debug.hpp>
#include <ewol/compositing/Text.hpp>
#include <ewol/widget/Widget.hpp>
#include <ewol/widget/Manager.hpp>
#include <ewol/resource/ColorFile.hpp>
#include <esignal/Signal.hpp>

namespace ewol {
	namespace widget {
		class Label;
		using Label = ememory::Ptr<ewol::widget::Label>;
		using LabelWeak = ememory::WeakPtr<ewol::widget::Label>;
		/**
		 * @ingroup ewolWidgetGroup
		 */
		class Label : public Widget {
			public: // signals
				esignal::Signal<> signalPressed;
			public: // properties
				eproperty::Value<bool> propertyAutoTranslate; //!< if at true the data is translate automaticaly translate.
				eproperty::Value<String> propertyValue; //!< decorated text to display.
				eproperty::Value<int> propertyFontSize; //!< default size of the font.
			private:
				ewol::compositing::Text this.text; //!< Compositing text element.
				etk::UString this.value;
				ememory::Ptr<ewol::resource::ColorFile> this.colorProperty; //!< theme color property
				int this.colorDefaultFgText; //!< Default color of the text
				int this.colorDefaultBgText; //!< Default Background color of the text
			protected:
				/**
				 * @brief Constructor
				 * @param[in] _newLabel The displayed decorated text.
				 */
				Label();
				void init() ;
			public:
				DECLARE_WIDGET_FACTORY(Label, "Label");
				/**
				 * @brief destructor
				 */
				 ~Label();
			protected:
				void onDraw() ;
			public:
				void calculateMinMaxSize() ;
				void onRegenerateDisplay() ;
				boolean onEventInput( ewol::event::Input _event) ;
				boolean loadXML( exml::Element _node) ;
			protected:
				 void onChangePropertyValue();
				 void onChangePropertyAutoTranslate();
				 void onChangePropertyFontSize();
		};
	};
};

