/** @file
 * @author Edouard DUPIN
 * @copyright 2011, Edouard DUPIN, all right reserved
 * @license APACHE v2.0 (see license file)
 */
#pragma once

#include <etk/types.h>
#include <ewol/debug.h>
#include <ewol/compositing/Text.h>
#include <ewol/widget/Widget.h>
#include <ewol/widget/Manager.h>
#include <ewol/resource/ColorFile.h>
#include <esignal/Signal.h>

namespace ewol {
	namespace widget {
		class Label;
		using LabelShared = ememory::SharedPtr<ewol::widget::Label>;
		using LabelWeak = ememory::WeakPtr<ewol::widget::Label>;
		/**
		 * @ingroup ewolWidgetGroup
		 */
		class Label : public ewol::Widget {
			public: // signals
				esignal::Signal<> signalPressed;
			public: // properties
				eproperty::Value<bool> propertyAutoTranslate; //!< if at true the data is translate automaticaly translate.
				eproperty::Value<std::string> propertyValue; //!< decorated text to display.
			private:
				ewol::compositing::Text m_text; //!< Compositing text element.
				std::u32string m_value;
				ememory::SharedPtr<ewol::resource::ColorFile> m_colorProperty; //!< theme color property
				int32_t m_colorDefaultFgText; //!< Default color of the text
				int32_t m_colorDefaultBgText; //!< Default Background color of the text
			protected:
				/**
				 * @brief Constructor
				 * @param[in] _newLabel The displayed decorated text.
				 */
				Label();
				void init() override;
			public:
				DECLARE_WIDGET_FACTORY(Label, "Label");
				/**
				 * @brief destructor
				 */
				virtual ~Label();
			protected:
				void onDraw() override;
			public:
				void calculateMinMaxSize() override;
				void onRegenerateDisplay() override;
				bool onEventInput(const ewol::event::Input& _event) override;
				bool loadXML(const exml::Element& _node) override;
			protected:
				virtual void onChangePropertyValue();
				virtual void onChangePropertyAutoTranslate();
		};
	};
};

