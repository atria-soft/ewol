/** @file
 * @author Edouard DUPIN
 * 
 * @copyright 2010, Edouard DUPIN, all right reserved
 * 
 * @license BSD v3 (see license file)
 */
#pragma once

#include <appl/debug.h>
#include <ewol/widget/Widget.h>
#include <ewol/widget/Windows.h>
#include <ewol/widget/Sizer.h>
#include <ewol/widget/Button.h>
#include <ewol/widget/Label.h>
#include <ewol/widget/Spacer.h>
#include <ewol/widget/Composer.h>
namespace appl {
	class MainWindows : public ewol::widget::Windows {
		private:
			std::vector<esignal::Connection> m_listConnection;
			ewol::widget::ComposerShared m_composer;
			ewol::widget::SizerShared m_sizerVert;
			ewol::widget::SizerShared m_sizerDynamic;
			ewol::WidgetShared m_subWidget;
			ewol::gravity m_gravity;
			int32_t m_idWidget;
		public:
			// Constructeur
			MainWindows();
			void init();
		public:
			DECLARE_FACTORY(MainWindows);
			~MainWindows() {};
		protected:
			void onCallbackGravityChange();
			void onCallbackThemeChange(const bool& _value);
			void onCallbackWidgetChange(int32_t _increment);
			void updateProperty();
	};
};


