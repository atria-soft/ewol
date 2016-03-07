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
#include <ewol/compositing/Text.h>
#include <ewol/widget/Widget.h>
#include <ewol/widget/Manager.h>
#include <ewol/resource/ColorFile.h>
#include <esignal/ISignal.h>

namespace ewol {
	namespace widget {
		/**
		 * @ingroup ewolWidgetGroup
		 */
		class Label : public ewol::Widget {
			public: // signals
				esignal::ISignal<> signalPressed;
			public: // properties
				eproperty::Value<std::string> propertyValue; //!< decorated text to display.
			private:
				ewol::compositing::Text m_text; //!< Compositing text element.
				std::u32string m_value;
				std::shared_ptr<ewol::resource::ColorFile> m_colorProperty; //!< theme color property
				int32_t m_colorDefaultFgText; //!< Default color of the text
				int32_t m_colorDefaultBgText; //!< Default Background color of the text
			protected:
				/**
				 * @brief Constructor
				 * @param[in] _newLabel The displayed decorated text.
				 */
				Label();
			public:
				DECLARE_WIDGET_FACTORY(Label, "Label");
				/**
				 * @brief destructor
				 */
				virtual ~Label();
			protected: // Derived function
				virtual void onDraw();
			public: // Derived function
				virtual void calculateMinMaxSize();
				virtual void onRegenerateDisplay();
				virtual bool onEventInput(const ewol::event::Input& _event);
				virtual bool loadXML(const std::shared_ptr<const exml::Element>& _node);
			protected:
				virtual void onChangePropertyValue();
		};
	};
};

