/** @file
 * @author Edouard DUPIN
 * @copyright 2010, Edouard DUPIN, all right reserved
 * @license GPL v3 (see license file)
 */
#pragma once

#include <ewol/widget/Windows.hpp>

namespace appl {
	class Windows;
	using WindowsShared = ememory::SharedPtr<appl::Windows>;
	using WindowsWeak = ememory::WeakPtr<appl::Windows>;
	class Windows : public ewol::widget::Windows {
		protected:
			Windows();
			void init();
		public:
			DECLARE_FACTORY(Windows);
	};
}

