/** @file
 * @author Edouard DUPIN
 * @copyright 2015, Edouard DUPIN, all right reserved
 * @license MPL v2.0 (see license file)
 */

#include <test-debug/debug.hpp>
#include <ewol/context/Context.hpp>
#include <gtest/gtest.h>
#define NAME "Application"

TEST(TestEwolApplication, Creation) {
	ewol::context::Application* tmpAppl = new ewol::context::Application();
	EXPECT_NE(tmpAppl, nullptr);
	delete tmpAppl;
}

