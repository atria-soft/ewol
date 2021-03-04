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
		using Parameter = ememory::Ptr<ewol::widget::Parameter>;
		using ParameterWeak = ememory::WeakPtr<ewol::widget::Parameter>;
		/**
		 * @ingroup ewolWidgetGroup
		 */
		class Parameter : public ewol::widget::PopUp {
			public: // signals
				esignal::Signal<> signalClose;
			public: // properties
				eproperty::Value<String> propertyLabelTitle;
			protected:
				Parameter();
				void init();
			public:
				DECLARE_WIDGET_FACTORY(Parameter, "Parameter");
				 ~Parameter();
			public:
				void menuAdd(String _label, String _image, Widget _associateWidget);
				void menuAddGroup(String _label);
				void menuClear();
				void menuSeparator();
			private:
				int this.currentIdList;
				ewol::widget::Label this.widgetTitle;
				ewol::widget::ParameterList this.paramList;
				ewol::widget::WSlider this.wSlider;
			private:
				void onCallbackMenuclosed();
				void onCallbackParameterSave();
				void onCallbackMenuSelected( int _value);
			protected:
				 void onChangePropertyLabelTitle();
		};
	};
};

