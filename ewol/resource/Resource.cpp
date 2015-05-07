/**
 * @author Edouard DUPIN
 * 
 * @copyright 2011, Edouard DUPIN, all right reserved
 * 
 * @license APACHE v2.0 (see license file)
 */

#include <etk/types.h>
#include <ewol/debug.h>
#include <ewol/resource/Resource.h>
#include <ewol/resource/Manager.h>
#include <ewol/ewol.h>
#include <ewol/context/Context.h>

void ewol::Resource::init() {
	ewol::Object::init();
}

void ewol::Resource::init(const std::string& _name) {
	ewol::Object::init(_name);
}

void ewol::Resource::updateContext() {
	EWOL_DEBUG("Not set for : [" << getId() << "]" << getName() << " loaded " << shared_from_this().use_count() << " time(s)");
}

void ewol::Resource::removeContext() {
	EWOL_DEBUG("Not set for : [" << getId() << "]" << getName() << " loaded " << shared_from_this().use_count() << " time(s)");
}

void ewol::Resource::removeContextToLate() {
	EWOL_DEBUG("Not set for : [" << getId() << "]" << getName() << " loaded " << shared_from_this().use_count() << " time(s)");
}

void ewol::Resource::reload() {
	EWOL_DEBUG("Not set for : [" << getId() << "]" << getName() << " loaded " << shared_from_this().use_count() << " time(s)");
}

ewol::resource::Manager& ewol::Resource::getManager() {
	return ewol::getContext().getResourcesManager();
}