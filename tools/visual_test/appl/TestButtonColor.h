/**
 * @author Edouard DUPIN
 * 
 * @copyright 2010, Edouard DUPIN, all right reserved
 * 
 * @license BSD v3 (see license file)
 */
#pragma once

#include <appl/debug.h>
#include <ewol/widget/Widget.h>
#include <ewol/widget/Button.h>
#include <ewol/widget/ButtonColor.h>
#include <ewol/widget/Spacer.h>
#include <ewol/widget/Composer.h>

namespace appl {
	class TestButtonColor : public ewol::widget::Composer {
		private:
			std::shared_ptr<ewol::widget::ButtonColor> m_testWidget;
		public:
			// Constructeur
			TestButtonColor();
			void init();
		public:
			DECLARE_FACTORY(TestButtonColor);
			virtual ~TestButtonColor() {};
		protected:
			void onCallbackExpendX(const bool& _value);
			void onCallbackExpendY(const bool& _value);
			void onCallbackFillX(const bool& _value);
			void onCallbackFillY(const bool& _value);
			void onCallbackChangeColor(const etk::Color<>& _value);
	};
}

