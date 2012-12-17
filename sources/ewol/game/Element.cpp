/**
 * @author Edouard DUPIN
 * 
 * @copyright 2011, Edouard DUPIN, all right reserved
 * 
 * @license BSD v3 (see license file)
 */



#include <ewol/game/Element.h>
#include <ewol/renderer/ResourceManager.h>

static int32_t uniqueId = 0;


game::Element::Element(etk::UString meshResource) :
	m_resource(NULL),
	m_uniqueId(uniqueId),
	m_groupId(0),
	m_type(0),
	m_visible(true),
	m_mass(0)
{
	ewol::MeshObj* tmpObject = NULL;
	// get a resources :
	if (meshResource != "") {
		ewol::resource::Keep(meshResource, tmpObject);
		m_resource = tmpObject;
	}
	uniqueId++;
	
	m_property.Scale(vec3(100,100,100) );
	//m_property.Rotate(m_property.m_angle, rotx);
	m_property.Translate(vec3(0.01,0.0,0.0));
}

game::Element::~Element(void)
{
	if (NULL != m_resource) {
		ewol::MeshObj* tmpObject = static_cast<ewol::MeshObj*>(m_resource);
		ewol::resource::Release(tmpObject);
		m_resource = NULL;
	}
}

void game::Element::Draw(void)
{
	if (NULL != m_resource) {
		m_resource->Draw(m_property.GetMatrix());
	}
}


bool game::Element::ArtificialIntelligence(int32_t deltaMicroSecond)
{
	return false;
}

