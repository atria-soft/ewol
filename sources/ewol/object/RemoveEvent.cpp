/**
 * @author Edouard DUPIN
 * 
 * @copyright 2011, Edouard DUPIN, all right reserved
 * 
 * @license BSD 3 clauses (see license file)
 */

#include <ewol/object/RemoveEvent.h>
#include <ewol/context/Context.h>

ewol::object::RemoveEvent::RemoveEvent() {
	EWOL_TODO("ewol::object::RemoveEvent::RemoveEvent()");
	/*
	Context& tmp = ewol::getContext();
	ewol::object::Manager& manager = tmp.getEObjectManager();
	manager.add(this);
	*/
}

ewol::object::RemoveEvent::~RemoveEvent() {
	EWOL_TODO("ewol::object::RemoveEvent::~RemoveEvent()");
	/*
	Context& tmp = ewol::getContext();
	ewol::object::Manager& manager = tmp.getEObjectManager();
	manager.rm(this);
	*/
}
