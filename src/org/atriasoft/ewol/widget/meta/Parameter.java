/** @file
 * @author Edouard DUPIN
 * @copyright 2011, Edouard DUPIN, all right reserved
 * @license MPL v2.0 (see license file)
 */
#pragma once

#include <etk/types.hpp>
#include <etk/types.hpp>
#include <ewol/debug.hpp>
#include <ewol/widget/PopUp.hpp>
#include <ewol/widget/Button.hpp>
#include <ewol/widget/Entry.hpp>
#include <ewol/widget/Label.hpp>
#include <ewol/widget/CheckBox.hpp>
#include <ewol/widget/WSlider.hpp>
#include <ewol/widget/meta/ParameterList.hpp>


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
				esignal::Signal<> signalClose;
			public: // properties
				eproperty::Value<etk::String> propertyLabelTitle;
			protected:
				Parameter();
				void init();
			public:
				DECLARE_WIDGET_FACTORY(Parameter, "Parameter");
				virtual ~Parameter();
			public:
				void menuAdd(etk::String _label, etk::String _image, ewol::WidgetShared _associateWidget);
				void menuAddGroup(etk::String _label);
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

