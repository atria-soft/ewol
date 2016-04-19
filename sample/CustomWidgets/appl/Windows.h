/** @file
 * @author Edouard DUPIN
 * 
 * @copyright 2010, Edouard DUPIN, all right reserved
 * 
 * @license BSD 3 clauses (see license file)
 */
#pragma once

#include <ewol/widget/Windows.h>
#include <ewol/widget/Composer.h>

namespace appl {
	class Windows;
	using WindowsShared = ememory::SharedPtr<appl::Windows>;
	using WindowsWeak = ememory::WeakPtr<appl::Windows>;
	class Windows : public ewol::widget::Windows {
		private:
			ewol::widget::ComposerShared m_composer;
		protected:
			Windows();
			void init();
		public:
			DECLARE_FACTORY(Windows);
		public: // callback functions
			void onCallbackChangeValues();
			void onCallbackAutoMode();
	};
}
