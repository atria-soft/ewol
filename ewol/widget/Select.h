/**
 * @author Edouard DUPIN
 * 
 * @copyright 2011, Edouard DUPIN, all right reserved
 * 
 * @license APACHE v2.0 (see license file)
 */
#pragma once

#include <vector>
#include <etk/types.h>
#include <ewol/widget/meta/SpinBase.h>

namespace ewol {
	namespace widget {
		/**
		 * @brief a composed Select is a Select with an inside composed with the specify XML element 
		 * ==> this permit to generate standard element simple
		 */
		class Select : public ewol::widget::SpinBase {
			public:
				// Event list of properties
				ewol::Signal<int32_t> signalValue;
			protected:
				/**
				 * @brief Constructor
				 * @param[in] _shaperName Shaper file properties
				 */
				Select();
				void init(const std::string& _shaperName="{ewol}THEME:GUI:Select.json");
			public:
				DECLARE_WIDGET_FACTORY(Select, "Select");
				/**
				 * @brief Destructor
				 */
				virtual ~Select();
				/**
				 * @brief set the shaper name (use the contructer one this permit to not noad unused shaper)
				 * @param[in] _shaperName The new shaper filename
				 */
				void setShaperName(const std::string& _shaperName) {
					//m_shaper.setString(_shaperName);
				}
			protected:
				class Element {
					public:
						int32_t m_value;
						std::string m_name;
						bool m_selected;
					public:
						Element(int32_t _value, std::string _name, bool _selected=false);
				};
				std::vector<ewol::widget::Select::Element> m_listElement;
			public:
				void optionSelectDefault();
				void optionRemove(int32_t _value);
				void optionClear();
				void optionAdd(int32_t _value, std::string _name);
			protected:
				ewol::parameter::Value<int32_t> m_value; //!< Current state of the Select.
			public:
				/**
				 * @brief set the currentValue of the Select (pressed or not)
				 * @note Work only in toggle mode
				 * @param[in] _val New value of the Select
				 */
				void setValue(int32_t _val);
				/**
				 * @brief get the current Select value.
				 * @return True : The Select is pressed.
				 * @return false : The Select is released.
				 */
				bool getValue() const;
			protected:
				virtual void onParameterChangeValue(const ewol::parameter::Ref& _paramPointer);
				virtual bool loadXML(const std::shared_ptr<const exml::Element>& _node);
				virtual void updateGui();
			protected:
				void onCallbackOpenMenu();
				void onCallbackLabelPressed(int32_t _value);
		};
	};
};
