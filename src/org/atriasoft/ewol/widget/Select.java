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
		using Select = ememory::Ptr<ewol::widget::Select>;
		using SelectWeak = ememory::WeakPtr<ewol::widget::Select>;
		/**
		 * @brief a composed Select is a Select with an inside composed with the specify XML element 
		 * ==> this permit to generate standard element simple
		 */
		class Select : public ewol::widget::SpinBase {
			public: // signals
				esignal::Signal<int> signalValue;
			public: // properties
				eproperty::Value<int> propertyValue; //!< Current state of the Select.
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
				 ~Select();
			protected:
				class Element {
					public:
						int this.value;
						String this.name;
						boolean this.selected;
					public:
						// TODO: Remove this: due to the fact my List is not full implemented
						Element() {}
						Element(int _value, String _name, boolean _selected=false);
				};
				List<ewol::widget::Select::Element> this.listElement;
			public:
				void optionSelectDefault();
				void optionRemove(int _value);
				void optionClear();
				void optionAdd(int _value, String _name);
			protected:
				boolean loadXML( exml::Element _node) ;
				void updateGui() ;
			protected:
				void onCallbackOpenMenu();
				void onCallbackLabelPressed(int _value);
			protected:
				esignal::Connection this.connectionEntry;
				esignal::Connection this.connectionButton;
			protected:
				 void onChangePropertyValue();
		};
	};
};
