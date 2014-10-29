/**
 * @author Edouard DUPIN
 * 
 * @copyright 2011, Edouard DUPIN, all right reserved
 * 
 * @license APACHE v2.0 (see license file)
 */

#ifndef __EWOL_LABEL_H__
#define __EWOL_LABEL_H__

#include <etk/types.h>
#include <ewol/debug.h>
#include <ewol/compositing/Text.h>
#include <ewol/widget/Widget.h>
#include <ewol/widget/Manager.h>
#include <ewol/resource/ColorFile.h>
#include <ewol/signal/Signal.h>

namespace ewol {
	namespace widget {
		/**
		 * @ingroup ewolWidgetGroup
		 */
		class Label : public ewol::Widget {
			public:
				// Event list of properties
				ewol::Signal<void> signalPressed;
			private:
				ewol::compositing::Text m_text; //!< Compositing text element.
				ewol::parameter::Value<std::u32string> m_label; //!< decorated text to display.
				std::shared_ptr<ewol::resource::ColorFile> m_colorProperty; //!< theme color property
				int32_t m_colorDefaultFgText; //!< Default color of the text
				int32_t m_colorDefaultBgText; //!< Default Background color of the text
			protected:
				/**
				 * @brief Constructor
				 * @param[in] _newLabel The displayed decorated text.
				 */
				Label();
				void init(std::string _newLabel="---");
			public:
				DECLARE_WIDGET_FACTORY(Label, "Label");
				/**
				 * @brief destructor
				 */
				virtual ~Label();
				/**
				 * @brief change the label displayed
				 * @param[in] _newLabel The displayed decorated text.
				 */
				void setLabel(const std::string& _newLabel) {
					m_label.set(etk::to_u32string(_newLabel));
				}
				//! @previous
				inline void setValue(const std::string& _newLabel) {
					m_label.set(etk::to_u32string(_newLabel));
				};
				/**
				 * @brief get the current displayed label
				 * @return The displayed decorated text.
				 */
				std::string getLabel() const {
					return etk::to_string(m_label);
				}
				//! @previous
				inline std::string getValue() const {
					return etk::to_string(m_label);
				};
			protected: // Derived function
				virtual void onDraw();
				virtual void onParameterChangeValue(const ewol::parameter::Ref& _paramPointer);
			public: // Derived function
				virtual void calculateMinMaxSize();
				virtual void onRegenerateDisplay();
				virtual bool onEventInput(const ewol::event::Input& _event);
				virtual bool loadXML(exml::Element* _node);
		};
	};
};

#endif
