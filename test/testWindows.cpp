/** @file
 * @author Edouard DUPIN
 * @copyright 2015, Edouard DUPIN, all right reserved
 * @license APACHE v2.0 (see license file)
 */

#include <test-debug/debug.hpp>
#include <ewol/widget/Windows.hpp>
#include <gtest/gtest.h>
#define NAME "Windows"

namespace appl {
	class Windows : public ewol::widget::Windows {
		protected:
			Windows() {
				addObjectType("appl::Windows");
				propertyTitle.setDirectCheck("test set");
			}
			void init() {
				ewol::widget::Windows::init();
			}
		public:
			DECLARE_FACTORY(Windows);
	};
}


TEST(TestEwolWindows, Creation) {
	/*
	ememory::SharedPtr<appl::Windows> tmpWindows = appl::Windows::create();
	EXPECT_NE(tmpWindows, nullptr);
	tmpWindows.reset();
	*/
}

