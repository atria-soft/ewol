/**
 * @author Edouard DUPIN
 * 
 * @copyright 2011, Edouard DUPIN, all right reserved
 * 
 * @license APACHE v2.0 (see license file)
 */

#include <etk/types.h>
#include <test-debug/debug.h>
#include <vector>
#include <string>
#include <etk/Hash.h>
#include <etk/os/FSNode.h>
#include <etk/archive/Archive.h>
#include <etk/log.h>
#include <etk/Color.h>
#include <etk/stdTools.h>
#include <string>

#define NAME "Empty"
#include <gtest/gtest.h>

#undef __class__
#define __class__	"ewol-test"


int main(int argc, const char *argv[]) {
	// init Google test :
	::testing::InitGoogleTest(&argc, const_cast<char **>(argv));
	// the only one init for etk:
	//etk::log::setLevel(etk::log::logLevelVerbose);
	etk::log::setLevel(etk::log::logLevelInfo);
	etk::setArgZero(argv[0]);
	etk::initDefaultFolder("ewol-test");
	return RUN_ALL_TESTS();
}

