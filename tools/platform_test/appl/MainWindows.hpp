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
			int32_t m_idWidget;
			
		public:
			// Constructeur
			MainWindows();
			void init() override;
		public:
			DECLARE_FACTORY(MainWindows);
			~MainWindows() {};
		protected:
			void onCallbackPast();
			void onCallbackCopy();
			void onCallbackSetTitle();
			void onCallbackClose();
			void onCallbackSize();
			void onCallbackMove();
			void onCallbackFullScreen(const bool& _value);
			bool onEventInput(const ewol::event::Input& _event) override;
			bool onEventEntry(const ewol::event::Entry& _event) override;
			void forceFocusCall(const ewol::event::Time& _event);
			virtual ewol::WidgetShared getWidgetAtPos(const vec2& _pos) {
				ewol::WidgetShared wid = ewol::widget::Windows::getWidgetAtPos(_pos);
				if (wid != nullptr) {
					return wid;
				}
				return ememory::dynamicPointerCast<ewol::Widget>(sharedFromThis());
			};
	};
};


