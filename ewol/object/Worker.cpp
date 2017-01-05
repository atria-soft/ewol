/** @file
 * @author Edouard DUPIN
 * @copyright 2011, Edouard DUPIN, all right reserved
 * @license MPL v2.0 (see license file)
 */

#include <ewol/debug.hpp>
#include <ewol/object/Worker.hpp>
#include <ewol/object/Manager.hpp>

ewol::object::Worker::Worker() {
	addObjectType("ewol::Worker");
}

void ewol::object::Worker::init() {
	ewol::Object::init();
	getObjectManager().workerAdd(sharedFromThis());
}

ewol::object::Worker::~Worker() {
	// nothing to do ...
}

void ewol::object::Worker::destroy() {
	ewol::Object::destroy();
	getObjectManager().workerRemove(sharedFromThis());
}
