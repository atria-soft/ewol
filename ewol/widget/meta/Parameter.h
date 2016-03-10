/**
 * @author Edouard DUPIN
 * 
 * @copyright 2011, Edouard DUPIN, all right reserved
 * 
 * @license APACHE v2.0 (see license file)
 */
#pragma once

#include <etk/types.h>
#include <etk/types.h>
#include <ewol/debug.h>
#include <ewol/widget/PopUp.h>
#include <ewol/widget/Button.h>
#include <ewol/widget/Entry.h>
#include <ewol/widget/Label.h>
#include <ewol/widget/CheckBox.h>
#include <ewol/widget/WSlider.h>
#include <ewol/widget/meta/ParameterList.h>


namespace ewol {
	namespace widget {
		class Parameter;
		using ParameterShared = ememory::SharedPtr<ewol::widget::Parameter>;
		using ParameterWeak = ememory::WeakPtr<ewol::widget::Parameter>;
		/**
		 * @ingroup ewolWidgetGroup
		 */
		class Parameter : public ewol::widget::PopUp {
			public: // signals
				esignal::ISignal<> signalClose;
			public: // properties
				eproperty::Value<std::string> propertyLabelTitle;
			protected:
				Parameter();
				void init();
			public:
				DECLARE_WIDGET_FACTORY(Parameter, "Parameter");
				virtual ~Parameter();
			public:
				void menuAdd(std::string _label, std::string _image, ewol::WidgetShared _associateWidget);
				void menuAddGroup(std::string _label);
				void menuClear();
				void menuSeparator();
			private:
				int32_t m_currentIdList;
				ewol::widget::LabelShared m_widgetTitle;
				ewol::widget::ParameterListShared m_paramList;
				ewol::widget::WSliderShared m_wSlider;
			private:
				void onCallbackMenuclosed();
				void onCallbackParameterSave();
				void onCallbackMenuSelected(const int32_t& _value);
			protected:
				virtual void onChangePropertyLabelTitle();
		};
	};
};

