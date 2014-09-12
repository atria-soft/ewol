/**
 * @author Edouard DUPIN
 * 
 * @copyright 2011, Edouard DUPIN, all right reserved
 * 
 * @license APACHE v2.0 (see license file)
 */

#include <ewol/debug.h>
#include <ewol/object/Worker.h>
#include <ewol/object/Manager.h>

ewol::object::Worker::Worker() {
	addObjectType("ewol::Worker");
}

void ewol::object::Worker::init() {
	ewol::Object::init();
	getObjectManager().workerAdd(shared_from_this());
}
//! @previous
void ewol::object::Worker::init(const std::string& _name) {
	ewol::Object::init(_name);
	getObjectManager().workerAdd(shared_from_this());
}

ewol::object::Worker::~Worker() {
	// nothing to do ...
}

void ewol::object::Worker::destroy() {
	ewol::Object::destroy();
	getObjectManager().workerRemove(shared_from_this());
}
