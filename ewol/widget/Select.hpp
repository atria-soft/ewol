/** @file
 * @author Edouard DUPIN
 * @copyright 2011, Edouard DUPIN, all right reserved
 * @license MPL v2.0 (see license file)
 */
#pragma once

#include <etk/Vector.hpp>
#include <etk/types.hpp>
#include <ewol/widget/meta/SpinBase.hpp>

namespace ewol {
	namespace widget {
		class Select;
		using SelectShared = ememory::SharedPtr<ewol::widget::Select>;
		using SelectWeak = ememory::WeakPtr<ewol::widget::Select>;
		/**
		 * @brief a composed Select is a Select with an inside composed with the specify XML element 
		 * ==> this permit to generate standard element simple
		 */
		class Select : public ewol::widget::SpinBase {
			public: // signals
				esignal::Signal<int32_t> signalValue;
			public: // properties
				eproperty::Value<int32_t> propertyValue; //!< Current state of the Select.
			protected:
				/**
				 * @brief Constructor
				 * @param[in] _shaperName Shaper file properties
				 */
				Select();
			public:
				DECLARE_WIDGET_FACTORY(Select, "Select");
				/**
				 * @brief Destructor
				 */
				virtual ~Select();
			protected:
				class Element {
					public:
						int32_t m_value;
						etk::String m_name;
						bool m_selected;
					public:
						// TODO: Remove this: due to the fact my etk::Vector is not full implemented
						Element() {}
						Element(int32_t _value, etk::String _name, bool _selected=false);
				};
				etk::Vector<ewol::widget::Select::Element> m_listElement;
			public:
				void optionSelectDefault();
				void optionRemove(int32_t _value);
				void optionClear();
				void optionAdd(int32_t _value, etk::String _name);
			protected:
				bool loadXML(const exml::Element& _node) override;
				void updateGui() override;
			protected:
				void onCallbackOpenMenu();
				void onCallbackLabelPressed(int32_t _value);
			protected:
				esignal::Connection m_connectionEntry;
				esignal::Connection m_connectionButton;
			protected:
				virtual void onChangePropertyValue();
		};
	};
};
