/**
 * @author Edouard DUPIN
 * 
 * @copyright 2011, Edouard DUPIN, all right reserved
 * 
 * @license BSD v3 (see license file)
 */

#include <etk/types.h>
#include <ewol/debug.h>
#include <ewol/resources/Resource.h>
#include <ewol/resources/ResourceManager.h>
#include <ewol/ewol.h>
#include <ewol/renderer/openGL.h>
#include <ewol/renderer/eContext.h>


void ewol::resource::Resource::updateContext(void) {
	EWOL_DEBUG("Not set for : [" << getId() << "]" << getName() << " loaded " << m_counter << " time(s)");
}

void ewol::resource::Resource::removeContext(void) {
	EWOL_DEBUG("Not set for : [" << getId() << "]" << getName() << " loaded " << m_counter << " time(s)");
}

void ewol::resource::Resource::removeContextToLate(void) {
	EWOL_DEBUG("Not set for : [" << getId() << "]" << getName() << " loaded " << m_counter << " time(s)");
}

void ewol::resource::Resource::reload(void) {
	EWOL_DEBUG("Not set for : [" << getId() << "]" << getName() << " loaded " << m_counter << " time(s)");
}

ewol::resource::Manager& ewol::resource::Resource::getManager(void) {
	return ewol::getContext().getResourcesManager();
}