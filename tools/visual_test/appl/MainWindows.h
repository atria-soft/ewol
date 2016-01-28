/**
 * @author Edouard DUPIN
 * 
 * @copyright 2010, Edouard DUPIN, all right reserved
 * 
 * @license BSD v3 (see license file)
 */

#ifndef __MAIN_WINDOWS_H__
#define __MAIN_WINDOWS_H__

#include <appl/debug.h>
#include <ewol/widget/Widget.h>
#include <ewol/widget/Windows.h>
#include <ewol/widget/Sizer.h>
#include <ewol/widget/Button.h>
#include <ewol/widget/Label.h>
#include <ewol/widget/Spacer.h>
namespace appl {
	class MainWindows : public ewol::widget::Windows {
		private:
			std::shared_ptr<ewol::widget::Sizer> m_sizerVert;
			std::shared_ptr<ewol::widget::Sizer> m_sizerDynamic;
			std::shared_ptr<ewol::Widget> m_subWidget;
			std::shared_ptr<ewol::widget::Label> m_testName;
			int32_t m_idWidget;
		public:
			// Constructeur
			MainWindows();
			void init();
		public:
			DECLARE_FACTORY(MainWindows);
			~MainWindows() {};
		protected:
			void onCallbackThemeChange(const bool& _value);
			void onCallbackWidgetChange(int32_t _increment);
			void updateProperty();
	};
};
#endif


