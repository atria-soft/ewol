/** @file
 * @author Edouard DUPIN
 * @copyright 2015, Edouard DUPIN, all right reserved
 * @license APACHE v2.0 (see license file)
 */

#include <test-debug/debug.h>
#include <ewol/context/Context.h>
#include <gtest/gtest.h>
#define NAME "Application"

TEST(TestEwolApplication, Creation) {
	ewol::context::Application* tmpAppl = new ewol::context::Application();
	EXPECT_NE(tmpAppl, nullptr);
	delete tmpAppl;
}

