/** @file
 * @author Edouard DUPIN
 * @copyright 2010, Edouard DUPIN, all right reserved
 * @license BSD v3 (see license file)
 */
#pragma once

#include <appl/debug.hpp>
#include <ewol/widget/Widget.hpp>
#include <ewol/widget/Windows.hpp>
#include <ewol/widget/Sizer.hpp>
#include <ewol/widget/Button.hpp>
#include <ewol/widget/Label.hpp>
#include <ewol/widget/Spacer.hpp>
#include <ewol/widget/Composer.hpp>
namespace appl {
	class MainWindows : public ewol::widget::Windows {
		private:
			etk::Vector<esignal::Connection> m_listConnection;
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
			void onCallbackShortCut(const etk::String& _value);
	};
};


