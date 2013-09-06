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


// Specific for the resource : 
uint32_t ewol::Resource::m_valBase=0;


void ewol::Resource::UpdateContext(void)
{
	EWOL_DEBUG("Not Set for : [" << m_uniqueId << "]" << m_name << " loaded " << m_counter << " time(s)");
}

void ewol::Resource::RemoveContext(void)
{
	EWOL_DEBUG("Not Set for : [" << m_uniqueId << "]" << m_name << " loaded " << m_counter << " time(s)");
}

void ewol::Resource::RemoveContextToLate(void)
{
	EWOL_DEBUG("Not Set for : [" << m_uniqueId << "]" << m_name << " loaded " << m_counter << " time(s)");
}

void ewol::Resource::Reload(void)
{
	EWOL_DEBUG("Not Set for : [" << m_uniqueId << "]" << m_name << " loaded " << m_counter << " time(s)");
}

ewol::ResourceManager ewol::Resource::GetManager(void)
{
	return ewol::GetContext().GetResourcesManager();
}