/** @file
 * @author Edouard DUPIN
 * @copyright 2011, Edouard DUPIN, all right reserved
 * @license MPL v2.0 (see license file)
 */

#include <etk/types.hpp>
#include <test-debug/debug.hpp>
#include <etk/Vector.hpp>
#include <etk/String.hpp>
#include <etk/Hash.hpp>
#include <etk/os/FSNode.hpp>
#include <etk/archive/Archive.hpp>
#include <etk/etk.hpp>
#include <etk/Color.hpp>
#include <etk/stdTools.hpp>
#include <etk/String.hpp>

#define NAME "Empty"
#include <etest/etest.hpp>

int main(int argc, const char *argv[]) {
	// init Google test :
	::testing::InitGoogleTest(&argc, const_cast<char **>(argv));
	etk::init(argc, argv);
	etk::initDefaultFolder("ewol-test");
	return RUN_ALL_TESTS();
}

