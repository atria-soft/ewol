/**
 * @author Edouard DUPIN
 * 
 * @copyright 2010, Edouard DUPIN, all right reserved
 * 
 * @license BSD v3 (see license file)
 */

#ifndef __TEST_BUTTON_H__
#define __TEST_BUTTON_H__

#include <appl/debug.h>
#include <ewol/widget/Button.h>
#include <ewol/widget/Composer.h>
namespace appl {
	class TestButton : public ewol::widget::Composer {
		private:
			std::shared_ptr<ewol::widget::Button> m_testWidget;
		public:
			// Constructeur
			TestButton();
			void init();
		public:
			DECLARE_FACTORY(TestButton);
		protected:
			void onCallbackExpendX(const bool& _value);
			void onCallbackExpendY(const bool& _value);
			void onCallbackFillX(const bool& _value);
			void onCallbackFillY(const bool& _value);
			void onCallbackToggle(const bool& _value);
			void onCallbackTextToggle(const bool& _value);
			void onCallbackChangeText();
	};
};

#endif
