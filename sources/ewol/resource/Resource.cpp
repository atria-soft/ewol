/**
 * @author Edouard DUPIN
 * 
 * @copyright 2011, Edouard DUPIN, all right reserved
 * 
 * @license BSD v3 (see license file)
 */

#include <etk/types.h>
#include <ewol/debug.h>
#include <ewol/resource/Resource.h>
#include <ewol/resource/Manager.h>
#include <ewol/ewol.h>
#include <ewol/context/Context.h>


void ewol::Resource::updateContext() {
	EWOL_DEBUG("Not set for : [" << getId() << "]" << getName() << " loaded " << m_counter << " time(s)");
}

void ewol::Resource::removeContext() {
	EWOL_DEBUG("Not set for : [" << getId() << "]" << getName() << " loaded " << m_counter << " time(s)");
}

void ewol::Resource::removeContextToLate() {
	EWOL_DEBUG("Not set for : [" << getId() << "]" << getName() << " loaded " << m_counter << " time(s)");
}

void ewol::Resource::reload() {
	EWOL_DEBUG("Not set for : [" << getId() << "]" << getName() << " loaded " << m_counter << " time(s)");
}

ewol::resource::Manager& ewol::Resource::getManager() {
	return ewol::getContext().getResourcesManager();
}