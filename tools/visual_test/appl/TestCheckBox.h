/**
 * @author Edouard DUPIN
 * 
 * @copyright 2010, Edouard DUPIN, all right reserved
 * 
 * @license BSD v3 (see license file)
 */
#pragma once

#include <appl/debug.h>
#include <ewol/widget/CheckBox.h>
#include <ewol/widget/Composer.h>

namespace appl {
	class TestCheckBox : public ewol::widget::Composer {
		private:
			std::shared_ptr<ewol::widget::CheckBox> m_testWidget;
		public:
			// Constructeur
			TestCheckBox();
			void init();
		public:
			DECLARE_FACTORY(TestCheckBox);
		protected:
			void onCallbackExpendX(const bool& _value);
			void onCallbackExpendY(const bool& _value);
			void onCallbackFillX(const bool& _value);
			void onCallbackFillY(const bool& _value);
			void onCallbackTextToggle(const bool& _value);
			void onCallbackChangeText();
	};
}


